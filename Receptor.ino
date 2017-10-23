#include <SoftwareSerial.h>  
#include <AFMotor.h>
#include <Servo.h>

Servo servo;

SoftwareSerial bluetooth(A0, A1); // RX, TX

AF_DCMotor motor1(1); //Seleção do Motor 1
AF_DCMotor motor2(4); //Seleção do Motor 2

char buf;
unsigned char velocidade1 = 0x00;
//============= Declaracao das funcoes ==================
void roboFrente();
void roboDireita();
void roboEsquerda();
void roboTras();
void roboParado();

void setup() {
  Serial.begin(9600);
  bluetooth.begin(38400);
  servo.attach(10);
  servo.write(85);

}

void loop() {
  if(bluetooth.available()){
    buf = bluetooth.read();
  }

  if (buf == 'F'){
    roboFrente();
  }
    
  if (buf == 'R'){
    roboDireita();
  }

  if (buf == 'L'){
    roboEsquerda();
  }

  if (buf == 'B'){
    roboTras();
  }

  if (buf == 'S'){
    roboParado();
  }

    Serial.println(buf);

  delay(100);
}

void roboFrente()
{
  velocidade1 = 100;           //Velocidade recebe o valor máximo
  motor1.setSpeed(velocidade1); //Seleciona velocidade atual
  motor1.run(FORWARD);          //Movimenta motor no sentido horário
  motor2.setSpeed(velocidade1); //Seleciona velocidade atual
  motor2.run(FORWARD);          //Movimenta motor no sentido horário  
  delay(50);
}

void roboDireita()
{
  velocidade1 = 100;          //Velocidade recebe o valor mínimo
  motor1.setSpeed(velocidade1); //Seleciona velocidade atual
  motor1.run(FORWARD);          //Motor parado
  motor2.setSpeed(velocidade1); //Seleciona velocidade atual
  motor2.run(BACKWARD);          //Motor parado
}

void roboEsquerda()
{
  velocidade1 = 100;           //Velocidade recebe o valor mínimo
  motor1.setSpeed(velocidade1); //Seleciona velocidade atual
  motor1.run(BACKWARD);          //Motor parado
  motor2.setSpeed(velocidade1); //Seleciona velocidade atual
  motor2.run(FORWARD);          //Motor parado
}

void roboTras()
{
  velocidade1 = 100;           //Velocidade recebe o valor mínimo
  motor1.setSpeed(velocidade1); //Seleciona velocidade atual
  motor1.run(BACKWARD);          //Motor parado
  motor2.setSpeed(velocidade1); //Seleciona velocidade atual
  motor2.run(BACKWARD);          //Motor parado
}

void roboParado()
{
  velocidade1 = 0x00;           //Velocidade recebe o valor mínimo
  motor1.setSpeed(velocidade1); //Seleciona velocidade atual
  motor1.run(RELEASE);          //Motor parado
  motor2.setSpeed(velocidade1); //Seleciona velocidade atual
  motor2.run(RELEASE);          //Motor parado
 
}
