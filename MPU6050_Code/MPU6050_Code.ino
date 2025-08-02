#include <Wire.h> // Required for I2C communication with MPU6050

// MPU6050
const int MPU = 0x68; // I2C address of the MPU-6050
// Variables to store raw data from MPU6050
int16_t AcX, AcY, AcZ, Tmp, GyX, GyY, GyZ;

/***********************************************************************************************************************************/
void setup()
{
  Wire.begin(); // Initialize I2C bus

  Serial.begin(9600); // Initialize serial port for data output
  while (!Serial); // Wait for serial monitor to connect (for some Arduino boards)

  // MPU6050 Initialization
  // Send command to wake up MPU-6050
  Wire.beginTransmission(MPU);
  Wire.write(0x6B); // PWR_MGMT_1 register
  Wire.write(0);    // Set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);

  Serial.println("MPU6050 initialized. Reading data for Serial Plotter...");
  Serial.println("AcX\tAcY\tAcZ\tTemp\tGyX\tGyY\tGyZ"); // Header for Serial Plotter
}

/***********************************************************************************************************************************/
void loop()
{
  // Read data from MPU6050
  Wire.beginTransmission(MPU);
  Wire.write(0x3B); // Starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false); // Do not stop transmission to continue reading

  Wire.requestFrom(MPU, 14, true); // Request a total of 14 registers (accelerometer, temperature, gyroscope)

  // Read accelerometer data
  AcX = Wire.read() << 8 | Wire.read(); // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)
  AcY = Wire.read() << 8 | Wire.read(); // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  AcZ = Wire.read() << 8 | Wire.read(); // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)

  // Read temperature data
  Tmp = Wire.read() << 8 | Wire.read(); // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)

  // Read gyroscope data
  GyX = Wire.read() << 8 | Wire.read(); // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
  GyY = Wire.read() << 8 | Wire.read(); // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
  GyZ = Wire.read() << 8 | Wire.read(); // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)

  // Output data to serial monitor and plotter
  // For Serial Plotter, it's important that all values are on one line, separated by tabs.
  // This will allow the plotter to display each parameter as a separate line.
  Serial.print(AcX); Serial.print("\t");
  Serial.print(AcY); Serial.print("\t");
  Serial.print(AcZ); Serial.print("\t");
  Serial.print(Tmp / 340.00 + 36.53); Serial.print("\t"); // Convert raw temperature data to Celsius
  Serial.print(GyX); Serial.print("\t");
  Serial.print(GyY); Serial.print("\t");
  Serial.println(GyZ); // println adds a newline, completing the output for the plotter

  delay(100); // Small delay for stable data output
}
