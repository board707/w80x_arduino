//------------------------------------------------------------------------------------------------------------------------
// 
// Title: SD Card Wav Player
// Note: Original code was written by XTronical for ESP32 based boards
//
// Description:
//    Simple example to demonstrate the basic usage of W80x i2s interface for playing WAV files. 
//    Plays WAV file from SD card using asynchronically DMA transfer. To keep this simple the WAV must be 16bit samples. 
//    The Mono/Stereo and Samples Per second can be anything. On the SD Card the wav file must be in root.
//    
// Connections:   
//
//     W80x board             Module
//   --------------------------------
//                 SD_Card
//        PB15        |         SCK  
//        PB16        |         MISO
//        PB17        |         MOSI
//        PB14        |         CS
//
//                MAX98357A
//        PB11        |         DIN
//        PB8         |         BCLK
//        PB9         |         LRC
//
//
// 
//    Original copyright/usage information:
//    (c) XTronical, www.xtronical.com
//    Use as you wish for personal or monatary gain, or to rule the world (if that sort of thing spins your bottle)
//    However you use it, no warrenty is provided etc. etc. It is not listed as fit for any purpose you perceive
//    It may damage your house, steal your lover, drink your beers and more.
//    http://www.xtronical.com/i2s-ep3
//
//    Ported to W80x boards by Dmitry E. Dmitriev (c) 2023
//------------------------------------------------------------------------------------------------------------------------
//
// Includes

#include "SD.h"   
#include <W_DMA.h>
//------------------------------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------------------------------------
// Defines
#define DEBUG_PRINT 0
#define SYS_I2S_CLK        (160000000)

// Used pins:
//    SD Card
          #define SD_CS         PB14          // SD Card chip select
   
//    I2S
          #define I2S_DOUT      PB11         // i2S Data out oin
          #define I2S_BCLK      PB8          // Bit clock
          #define I2S_WS        PB9          // Word select 
        
// Wav File reading
#define NUM_BYTES_TO_READ_FROM_FILE 4096    // How many bytes to read from wav file at a time

//------------------------------------------------------------------------------------------------------------------------
// structures and also variables

struct WavHeader_Struct
      {
          //   RIFF Section    
          char RIFFSectionID[4];      // Letters "RIFF"
          uint32_t Size;              // Size of entire file less 8
          char RiffFormat[4];         // Letters "WAVE"
          
          //   Format Section    
          char FormatSectionID[4];    // letters "fmt"
          uint32_t FormatSize;        // Size of format section less 8
          uint16_t FormatID;          // 1=uncompressed PCM
          uint16_t NumChannels;       // 1=mono,2=stereo
          uint32_t SampleRate;        // 44100, 16000, 8000 etc.
          uint32_t ByteRate;          // =SampleRate * Channels * (BitsPerSample/8)
          uint16_t BlockAlign;        // =Channels * (BitsPerSample/8)
          uint16_t BitsPerSample;     // 8,16,24 or 32
        
          // Data Section
          char DataSectionID[4];      // The letters "data"
          uint32_t DataSize;          // Size of the data that follows
      }WavHeader;

//  I2S configuration

 void i2s_config(uint8_t BitsPerSample, uint8_t NumChannels, uint32_t AudioFreq) {

    // congigure I2s instance
    hi2s.Instance  = I2S;
    hi2s.Init.Mode = I2S_MODE_MASTER;
    hi2s.Init.Standard = I2S_STANDARD_PHILIPS;
    hi2s.Init.MCLKOutput = I2S_MCLKOUTPUT_ENABLE;
    
    if (BitsPerSample == 8) hi2s.Init.DataFormat = I2S_DATAFORMAT_8B;
    else hi2s.Init.DataFormat = I2S_DATAFORMAT_16B;
   
    //hi2s.Init.AudioFreq = I2S_AUDIOFREQ_22K;
    hi2s.Init.AudioFreq = AudioFreq;
    
    if (NumChannels == 1) hi2s.Init.Channel = I2S_CHANNEL_MONO;
    else hi2s.Init.Channel = I2S_CHANNEL_STEREO;
    
    hi2s.Init.ChannelSel = I2S_CHANNELSEL_LEFT;
    if (HAL_I2S_Init(&hi2s) != HAL_OK)
    {
       // Error_Handler();
    }
    
    // enable I2S clocking
     __HAL_RCC_I2S_CLK_ENABLE();
     __HAL_RCC_GPIO_CLK_ENABLE();
        
    // setup DMA    
    uint8_t dma_ch = claim_unused_dma_channel();
    if (dma_ch != -1) {
          DMA_HandleTypeDef* hdma = dma_simple_config(dma_ch, DMA_REQUEST_SOURCE_I2S_TX);
          __HAL_LINKDMA(&hi2s, hdmatx, *hdma);
          dma_init(hdma);
      }
    
    //enable I2S interrupt
    HAL_NVIC_SetPriority(I2S_IRQn, 1);
    HAL_NVIC_EnableIRQ(I2S_IRQn);
};

void i2s_pins_config(uint8_t CK_pin, uint8_t WS_pin, uint8_t MOSI_pin) {
   //__HAL_AFIO_REMAP_I2S_MCK(GPIOA, GPIO_PIN_7);
   //__HAL_AFIO_REMAP_I2S_WS(GPIOB, GPIO_PIN_9);
   //__HAL_AFIO_REMAP_I2S_CK(GPIOB, GPIO_PIN_8);
   //__HAL_AFIO_REMAP_I2S_MOSI(GPIOB, GPIO_PIN_11);
   //__HAL_AFIO_REMAP_I2S_MISO(GPIOB, GPIO_PIN_10);
   
   __HAL_AFIO_REMAP_I2S_CK(digitalPinToPort(CK_pin), digitalPinToBitMask(CK_pin));
   __HAL_AFIO_REMAP_I2S_WS(digitalPinToPort(WS_pin), digitalPinToBitMask(WS_pin));
   __HAL_AFIO_REMAP_I2S_MOSI(digitalPinToPort(MOSI_pin), digitalPinToBitMask(MOSI_pin));
};     
//------------------------------------------------------------------------------------------------------------------------

//  Global Variables/objects    
    volatile bool dma_busy = false;
    volatile byte buf_index = 0;
    volatile byte* buf_ptr = NULL;
    bool pause_flag = false;
    
    File WavFile;                                 // Object for root of SD card directory
      

//------------------------------------------------------------------------------------------------------------------------


void setup() {    
    Serial.begin(115200); 
    
    // Used for info/debug
    SDCardInit();
    
    // get the wav file from the SD card
    WavFile = SD.open("/myfile.wav");                   // Open the wav file
    if(WavFile==false)
      Serial.println("Could not open 'myfile.wav'");
    else
    {
      WavFile.read((byte *) &WavHeader,44);               // Read in the WAV header, which is first 44 bytes of the file. 
                                                          // We have to typecast to bytes for the "read" function
      DumpWAVHeader(&WavHeader);                          // Dump the header data to serial, optional!
      
      if(ValidWavData(&WavHeader)) {

            // configure I2s interface, using audio parameters, readed from the file
        i2s_config(WavHeader.BitsPerSample, WavHeader.NumChannels, WavHeader.SampleRate);
            // setup i2s pins
        i2s_pins_config(I2S_BCLK, I2S_WS, I2S_DOUT);
      }
         
    }
}


void loop()
{    
    PlayWav();                                            // Have to keep calling this to keep the wav file playing
   
    // Your normal code to do your task can go here
}

void PlayWav()
{
  static bool ReadingFile=true;                       // True if reading file from SD. false if filling I2S buffer
  static byte sampl[2 * NUM_BYTES_TO_READ_FROM_FILE];   // Dual buffer allocated to store the data read in from the wav file
  static uint16_t BytesRead;   // Num bytes actually read from the wav file which will either be
  

  if(ReadingFile)                                     // Read next chunk of data in from file if needed
  {
    
    buf_index = 1 - buf_index;         // Swap the buffers, fill the one with the data from the file
                                       // while the other transfers by DMA to I2s bus
    buf_ptr = sampl + buf_index * NUM_BYTES_TO_READ_FROM_FILE;  // pointer to the current buffer
    
    BytesRead=ReadFile((byte*) buf_ptr);   // Read data into our memory buffer, return num bytes read in
    ReadingFile=false;                     // Switch to sending the buffer to the I2S
    
  }
  else {
    
    
    ReadingFile=FillI2SBuffer((byte *)buf_ptr,BytesRead); // We keep calling this routine until it returns true
    
    if (pause_flag) {              // This flag became true when playing the whole file is finished.
      
      delay(5000); 
      pause_flag = false; 
      }
  }             // this will swap us back to Reading the next block of data from the file.
                // Reading true means it has managed to push all the data to the I2S 
                // Handler, false means there still more to do and you should call this
                // routine again and again until it returns true.
}

uint16_t ReadFile(byte* Ptr)
{
    static uint32_t BytesReadSoFar=0;                   // Number of bytes read from file so far
    uint16_t BytesToRead;                               // Number of bytes to read from the file
    
    if(BytesReadSoFar+NUM_BYTES_TO_READ_FROM_FILE>WavHeader.DataSize)   // If next read will go past the end then adjust the 
      BytesToRead=WavHeader.DataSize-BytesReadSoFar;                    // amount to read to whatever is remaining to read
    else
      BytesToRead=NUM_BYTES_TO_READ_FROM_FILE;                          // Default to max to read
      
    WavFile.read(Ptr,BytesToRead);                  // Read in the bytes from the file
    BytesReadSoFar+=BytesToRead;                    // Update the total bytes red in so far

    if(BytesReadSoFar>=WavHeader.DataSize)              // Have we read in all the data?
    {
      WavFile.seek(44);                                 // Reset to start of wav data  
      BytesReadSoFar=0;                                 // Clear to no bytes read in so far     
      pause_flag = true;                  
    }
    return BytesToRead;                                 // return the number of bytes read into buffer
}

bool FillI2SBuffer(byte* Data,uint16_t BytesInBuffer)
{
    // Start the DMA transfer the data to I2s, wait if DMA is busy with previous data. Returns true if data sent else false,
    // so just keep calling this routine until returns true to know they've all been written, then
    // you can re-fill the buffer.
    
    
    uint16_t BytesToSend = BytesInBuffer;        // Number of bytes to send to I2S
    
    if (! dma_busy) {   // Test either the DMA finished previous transfer
      
      uint32_t res = HAL_I2S_Transmit_DMA(&hi2s, (uint32_t*) Data, BytesToSend/2);
      if ( res == 0 ) { // 0 means the data sent, no error
#if (DEBUG_PRINT)        
        Serial.printf("Send %d bytes, error code = %d\n", BytesToSend, res);
#endif        
        dma_busy = true;
        return true;
      }
      
      else Serial.printf("Not send, error code = %d\n",res);
    }
    
    return false;       // Still not send data to I2S so return false to indicate this
}

void SDCardInit()
{        
    pinMode(SD_CS, OUTPUT); 
    digitalWrite(SD_CS, HIGH); // SD card chips select
    if(!SD.begin(SD_CS))
    {
        Serial.println("Error talking to SD card!");
        while(true);                  // end program
    }
}

bool ValidWavData(WavHeader_Struct* Wav)
{
  
  if(memcmp(Wav->RIFFSectionID,"RIFF",4)!=0) 
  {    
    Serial.print("Invalid data - Not RIFF format");
    return false;        
  }
  if(memcmp(Wav->RiffFormat,"WAVE",4)!=0)
  {
    Serial.print("Invalid data - Not Wave file");
    return false;           
  }
  if(memcmp(Wav->FormatSectionID,"fmt",3)!=0) 
  {
    Serial.print("Invalid data - No format section found");
    return false;       
  }
  if(memcmp(Wav->DataSectionID,"data",4)!=0) 
  {
    Serial.print("Invalid data - data section not found");
    return false;      
  }
  if(Wav->FormatID!=1) 
  {
    Serial.print("Invalid data - format Id must be 1");
    return false;                          
  }
  if(Wav->FormatSize!=16) 
  {
    Serial.print("Invalid data - format section size must be 16.");
    return false;                          
  }
  if((Wav->NumChannels!=1)&(Wav->NumChannels!=2))
  {
    Serial.print("Invalid data - only mono or stereo permitted.");
    return false;   
  }
  if(Wav->SampleRate>48000) 
  {
    Serial.print("Invalid data - Sample rate cannot be greater than 48000");
    return false;                       
  }
  if((Wav->BitsPerSample!=8)& (Wav->BitsPerSample!=16)) 
  {
    Serial.print("Invalid data - Only 8 or 16 bits per sample permitted.");
    return false;                        
  }
  return true;
}


void DumpWAVHeader(WavHeader_Struct* Wav)
{
  if(memcmp(Wav->RIFFSectionID,"RIFF",4)!=0)
  {
    Serial.print("Not a RIFF format file - ");    
    PrintData(Wav->RIFFSectionID,4);
    return;
  } 
  if(memcmp(Wav->RiffFormat,"WAVE",4)!=0)
  {
    Serial.print("Not a WAVE file - ");  
    PrintData(Wav->RiffFormat,4);  
    return;
  }  
  if(memcmp(Wav->FormatSectionID,"fmt",3)!=0)
  {
    Serial.print("fmt ID not present - ");
    PrintData(Wav->FormatSectionID,3);      
    return;
  } 
  if(memcmp(Wav->DataSectionID,"data",4)!=0)
  {
    Serial.print("data ID not present - "); 
    PrintData(Wav->DataSectionID,4);
    return;
  }  
  // All looks good, dump the data
  Serial.print("Total size :");Serial.println(Wav->Size);
  Serial.print("Format section size :");Serial.println(Wav->FormatSize);
  Serial.print("Wave format :");Serial.println(Wav->FormatID);
  Serial.print("Channels :");Serial.println(Wav->NumChannels);
  Serial.print("Sample Rate :");Serial.println(Wav->SampleRate);
  Serial.print("Byte Rate :");Serial.println(Wav->ByteRate);
  Serial.print("Block Align :");Serial.println(Wav->BlockAlign);
  Serial.print("Bits Per Sample :");Serial.println(Wav->BitsPerSample);
  Serial.print("Data Size :");Serial.println(Wav->DataSize);
}

void PrintData(const char* Data,uint8_t NumBytes)
{
    for(uint8_t i=0;i<NumBytes;i++)
      Serial.print(Data[i]); 
      Serial.println();  
}

void HAL_I2S_TxCpltCallback(I2S_HandleTypeDef *hi2s)
{
    //Serial.printf("tx cplt\r\n");
    dma_busy = false;
}