## Example of using W80x I2s interface 

 In the `example` folder there is a simple example to demonstrate the basic usage of W80x i2s interface for outputs digital audio data. The code plays WAV file from SD card using asynchronically DMA transfer. To keep this simple the WAV must be 16bit samples. The Mono/Stereo and Samples Per second reads from the file and can be anything. To test place the wav file in the root folder of the SD card. The name of the file must match the "8.3" scheme.

 The `wav_files` folder contains a few example WAV files, that can used with **Wav_SD_Player** code:

 * one_tone.wav - single music tone, 44 KHz, Mono, 16 bits, about 2.5 sec
 * music.wav - CantinaBand music sample, 22 KHz, Mono, 16 bits, about 3 secs

  