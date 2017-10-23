#include <SoftwareSerial.h>  
#include <Wire.h>
 
//Define os pinos para a serial   
SoftwareSerial mySerial(10, 11); // RX, TX 

 //Endereco I2C MPU6050
const int MPU=0x68;
//Variaveis para armazenar valores dos sensores
int AcX, AcY, AcZ, Tmp, GyX, GyY, GyZ;

void setup()
{
  //Inicializa a serial nas portas 10 e 11
  mySerial.begin(38400);

  //Inicializa o MPU-6050
  Wire.begin();
  Wire.beginTransmission(MPU);
  Wire.write(0x6B); 
   
  //Inicializa o MPU-6050
  Wire.write(0); 
  Wire.endTransmission(true);
}
 
void loop()
{
  char valorLido;
  
  Wire.beginTransmission(MPU);
  Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  
  //Solicita os dados do sensor
  Wire.requestFrom(MPU,14,true);
  
  AcX=Wire.read()<<8|Wire.read();
  AcY=Wire.read()<<8|Wire.read();

  if(AcX > 10000)  valorLido = 'L';
  if(AcX < -10000) valorLido = 'R';
  if(AcY > 10000)  valorLido = 'B';
  if(AcY < -10000) valorLido = 'F';
  if(AcX < 10000 && AcX > -10000 && AcY < 10000 && AcY > -10000) valorLido = 'S';

  //Envia pela serial o caracter L
  
    mySerial.print(valorLido);

  delay(600);
}
