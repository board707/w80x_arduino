#define INTERNAL_STORAGE_ADDR 0x80FF7FF

struct data_storage
{
uint32_t offset;
uint8_t data1;
uint16_t data2;
float data3;
};


data_storage ds = {
             .offset = 200,
             .data1 = 15,
             .data2 = 0xE5E5,
             .data3 = 3.14f
};
data_storage ds1;

void setup() {
Serial.begin(115200);

// Storing structure ds
HAL_FLASH_Write(INTERNAL_STORAGE_ADDR, (uint8_t *) &ds, sizeof(data_storage));

// Restore data to ds1;
HAL_FLASH_Read(INTERNAL_STORAGE_ADDR,  (uint8_t *) &ds1, sizeof(data_storage));
Serial.printf("DS1 offset = %d, data1 = %d, data2 = %d, data3 = %f\n", ds1.offset, ds1.data1, ds1.data2, ds1.data3);
}

void loop() {

}