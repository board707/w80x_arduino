// Theory of this code described in article  YMFC-32_document_1.pdf
// in folder MPU_QMC_Test
// Tilt compensated compass. Simple example.
// Based on MPU6500 and QMC5883L

#include "Arduino.h"
#include "Wire.h"

// Registers of MPU&QMC
#define mpu_adr 0x68
#define qmc_adr 0x0D
#define qmc_period 0x0B
#define qmc_cntrl_1 0x09
#define qmc_cntrl_2 0x0A
#define qmc_status 0x06
#define pwr_mgmt_1 0x6B
#define mpu_status 0x3A
#define int_en 0x38
#define gyro_out 0x43
#define accel_out 0x3B
#define qmc_out 0x00

// Array depth
#define depth 64

// Calibration data. You must did it! Many ways described on the net
int gyro_offs[] = { 0, 0, 0 };
int accel_offs[] = { 0, 0, 0 };
int qmc_matrix[] = { -1, 1, -1, 1, -1, 1 };

// Calibration data on a flat surface to the horizon
float pitch_offs = 0, roll_offs = 0;

//Integration time
long lastTime=0;

//Arrays of averages
float aver_pitch[depth], aver_roll[depth], aver_mag[depth];

//RAW data
float qmc_data[3];
float accel_data[3];
float gyro_data[3];

// Prototypes
void writeByte(byte, byte, byte);
void readMagData(float*);
bool qmc_isReady(void);
byte readByte(byte, byte);
void readBytes(uint8_t, uint8_t, uint8_t, uint8_t*);
void readAccelData(float* );
void readGyroData(float*);

void setup()
{
  //Wire.begin();               // Hardware I2C  (PA4 = SDA, PA1 = SCL by default)
  Wire.begin(PB5,PB2);        // Software I2C  (SDA, SCL) - any pins
  Serial.begin(115200);         // Arduino Serial communication. Recommended speed for w80x
  Serial.println("Compass init...");
  writeByte(mpu_adr, pwr_mgmt_1, 0x00);
  writeByte(mpu_adr, pwr_mgmt_1, 0x01);
  writeByte(qmc_adr, qmc_cntrl_2, 0x80);
  writeByte(qmc_adr, qmc_period, 0x01);
  writeByte(qmc_adr, qmc_cntrl_1, 0x0D);
  lastTime = millis();
}


void loop()
{
  static float g_roll, g_pitch, pitch, roll;
  long newTime = millis();
  float elapsedTime = (float)(newTime - lastTime) / 1000.0f;

  // Read all data
  readMagData(qmc_data);
  readAccelData(accel_data);
  readGyroData(gyro_data);

  // Calculation vector and pitch/roll
  float r = sqrtf(pow(accel_data[0],2)+pow(accel_data[1],2)+pow(accel_data[2],2));
  roll = (-asin(accel_data[1]/r)* 180 / PI) - roll_offs;
  pitch = (asin(accel_data[0]/r)* 180 / PI) - pitch_offs;

  // Time integration of gyroscope data
  g_roll += gyro_data[0] * elapsedTime;
  g_pitch += gyro_data[1] * elapsedTime;

  // Complementary filter
  pitch = 0.90 * g_pitch + 0.10 * pitch;
  roll = 0.90 * g_roll + 0.10 * roll;

  // Recording current data
  float sum_pitch = 0, sum_roll = 0;
  for (byte i = depth - 1; i > 0; i--) {
    aver_pitch[i] = aver_pitch[i - 1];
    sum_pitch += aver_pitch[i - 1];
    aver_roll[i] = aver_roll[i - 1];
    sum_roll += aver_roll[i - 1];
  }
  aver_pitch[0] = pitch;
  aver_roll[0] = roll;

  // Calculating Averages
  roll = (sum_roll + roll) / depth;
  pitch = (sum_pitch + pitch) / depth;
  g_roll = roll;
  g_pitch = pitch;

  // Computing Azimuth and Compensation
  float pitch_rad = PI / 180 * pitch ;
  float roll_rad = PI / 180 * roll;

  float cx = qmc_data[0] * cos(pitch_rad) + qmc_data[1] * sin(roll_rad) * sin(pitch_rad) - qmc_data[2] * cos(roll_rad) * sin(pitch_rad);
  float cy = qmc_data[1] * cos(roll_rad) + qmc_data[2] * sin(roll_rad);

  int az_rad = 180 / PI * atan2(cy, cx);
  int azimuth = round((int)(az_rad + 360.0) % 360);

  // Calculation of the average azimuth value
  int sum_mag = 0;
  for (byte i = depth - 1; i > 0; i--) {
    aver_mag[i] = aver_mag[i - 1];
    sum_mag += aver_mag[i - 1];
  }
  aver_mag[0] = azimuth;
  azimuth = (azimuth + sum_mag) / depth;
  // Output data
  Serial.print("Roll = ");Serial.print(roll);Serial.print("  Pith = ");Serial.print(pitch);Serial.print("  Azimuth(Yawl) = ");Serial.println(azimuth);
  lastTime = newTime;
}

void readMagData(float* destination) {
  byte rawData[6];
  while (!qmc_isReady()){}
  readBytes(qmc_adr, qmc_out, 6, &rawData[0]);
  int mX = (int)rawData[0] | rawData[1] << 8;
  int mY = (int)rawData[2] | rawData[3] << 8;
  int mZ = (int)rawData[4] | rawData[5] << 8;
  float x_ofs = (float)(qmc_matrix[0] + qmc_matrix[1]) / 2.0;
  float y_ofs = (float)(qmc_matrix[2] + qmc_matrix[3]) / 2.0;
  float z_ofs = (float)(qmc_matrix[4] + qmc_matrix[5]) / 2.0;
  float x_avg_delta = (float)(qmc_matrix[1] - qmc_matrix[0]) / 2.0;
  float y_avg_delta = (float)(qmc_matrix[3] - qmc_matrix[2]) / 2.0;
  float z_avg_delta = (float)(qmc_matrix[5] - qmc_matrix[4]) / 2.0;
  float avg_delta = (x_avg_delta + y_avg_delta + z_avg_delta) / 3.0;
  float x_scale = avg_delta / x_avg_delta;
  float y_scale = avg_delta / y_avg_delta;
  float z_scale = avg_delta / z_avg_delta;
  destination[0] = (float)((mX - x_ofs) * x_scale) ;
  destination[1] = (float)((mY - y_ofs) * y_scale) ;
  destination[2] = (float)((mZ - z_ofs) * z_scale) ;
}

void readAccelData(float * destination)
{
  byte rawData[6];
  readBytes(mpu_adr, accel_out, 6, &rawData[0]);
  destination[0] = (float)(((int)rawData[0] << 8 | rawData[1]) - accel_offs[0] ) / 16384.0;
  destination[1] = (float)(((int)rawData[2] << 8 | rawData[3]) - accel_offs[1] ) / 16384.0;
  destination[2] = (float)(((int)rawData[4] << 8 | rawData[5]) - accel_offs[2] ) / 16384.0;
}

void readGyroData(float * destination)
{
  byte rawData[6];
  readBytes(mpu_adr, gyro_out, 6, &rawData[0]);
  destination[0] = (float)(((int)rawData[0] << 8 | rawData[1]) - gyro_offs[0]) /  131.0;
  destination[1] = (float)(((int)rawData[2] << 8 | rawData[3]) - gyro_offs[1]) /  131.0;
  destination[2] = (float)(((int)rawData[4] << 8 | rawData[5]) - gyro_offs[2]) /  131.0;
}

bool qmc_isReady(void) {
  byte status = readByte(qmc_adr, qmc_status);
  return bitRead(status, 0);
}

byte readByte(byte address, byte subAddress) {
  byte data;
  Wire.beginTransmission(address);
  Wire.write(subAddress);
  Wire.endTransmission(false);
  Wire.requestFrom(address, (byte)1);
  data = Wire.read();
  return data;
}

void writeByte(byte address, byte subAddress, byte data) {
  Wire.beginTransmission(address);
  Wire.write(subAddress);
  Wire.write(data);
  Wire.endTransmission();
}

void readBytes(uint8_t address, uint8_t subAddress, uint8_t count, uint8_t* dest) {
  Wire.beginTransmission(address);
  Wire.write(subAddress);
  Wire.endTransmission(false);
  uint8_t i = 0;
  Wire.requestFrom(address, count);
  while (Wire.available()) dest[i++] = Wire.read();
}
