/*
*
* @Projeto integrador 4° semestre Ciência da Computação
* @author: Heliton Leno
* @email: ciencia_@live.com
*
*/


#include <SoftwareSerial.h>  
#include <AFMotor.h>
#include <Servo.h>

#define trigPin A2        //Pino TRIG do sensor no pino analógico A2
#define echoPin 9         //Pino ECHO do sensor no pino analógico 9
#define buzzer A3         // Define o pino do buzzer no pino ANALÓGICO A3
#define pot    A4
 
int TempoGirar = 1;           // esse é o tempo para o robô girar em 45º
int distanciaObstaculo = 30;  // distância para o robô parar e recalcular o melhor caminho
int potVelocidade = 0;        // velocidade motores

SoftwareSerial bluetooth(A0, A1); // RX, TX

AF_DCMotor motor1(1); //Seleção do Motor 1
AF_DCMotor motor2(2); //Seleção do Motor 2

Servo servo;

//============== variáveis  para o sensor ultrassonico ================
long duracao;
long distancia_cm = 0;
int minimumRange = 5;      //tempo de resposta do sensor
int maximumRange= 200;
char buf;
boolean alone = false;

//============= Declaracao das funcoes ==================
void roboFrente(int v);
void roboDireita(int v);
void roboEsquerda(int v);
void roboTras(int v);
void roboParado();

void setup() {
  //Serial.begin(9600);
  bluetooth.begin(38400);
  servo.attach(10);
  servo.write(85);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buzzer,OUTPUT); 

}

void loop() {
  potVelocidade = analogRead(pot) /4;

  if(!bluetooth.available()){
   roboParado();
  }
  
  if(bluetooth.available()){
    buf = bluetooth.read();
  }

  if ( buf == 'X' ){
    alone = true;
    roboFrente(potVelocidade);
    int dist_cm = measureDistance();

    if(dist_cm < 30) autonomo(potVelocidade-30);
  }
  if ( buf == 'x' ){
    alone = false;
    roboParado();
  }
  if ( buf == 'F' && !alone ) roboFrente(potVelocidade);
  if ( buf == 'B' && !alone ) roboTras(potVelocidade);
  if ( buf == 'L' && !alone ) roboEsquerda(potVelocidade-20);
  if ( buf == 'R' && !alone ) roboDireita(potVelocidade-20);
  if ( buf == 'S' && !alone ) roboParado();

  //Serial.println(buf);
  delay(50);
}

float measureDistance()                       //Função que retorna a distância em centímetros
{
  float pulse;                                //Armazena o valor de tempo em µs que o pino echo fica em nível alto
        
  trigPulse();                                //Envia pulso de 10µs para o pino de trigger do sensor
  
  pulse = pulseIn(echoPin, HIGH);                //Mede o tempo em que echo fica em nível alto e armazena na variável pulse
   
  return (pulse/58.82);                      //Calcula distância em centímetros e retorna o valor
  
  
} //end measureDistante


void trigPulse()                             //Função para gerar o pulso de trigger para o sensor HC-SR04
{
   digitalWrite(trigPin,HIGH);                  //Saída de trigger em nível alto
   delayMicroseconds(10);                    //Por 10µs ...
   digitalWrite(trigPin,LOW);                   //Saída de trigger volta a nível baixo

}

void autonomo(int vel)                              //Compara as distâncias e decide qual melhor caminho a seguir
{
   roboParado();                              //Para o robô
   delay(500);                               //Aguarda 500ms
   servo.write(0);                          //Move sensor para direita através do servo
   delay(500);                               //Aguarda 500ms
   int dist_right = measureDistance();           //Mede distância e armazena em dist_right
   delay(2000);                              //Aguarda 2000ms
   servo.write(175);                        //Move sensor para esquerda através do servo
   delay(500);                               //Aguarda 500ms
   int dist_left = measureDistance();            //Mede distância e armazena em dis_left
   delay(2000);                               //Aguarda 2000ms
   servo.write(80);                         //Centraliza servo
   delay(500);
   
   if(dist_right > dist_left)                //Distância da direita maior que da esquerda?
   {                                         //Sim...
   
      roboTras(vel);                           //Move o robô para trás
      delay(300);                            //Por 600ms
      roboDireita(vel);                          //Move o robô para direita
      delay(300);                           //Por 2000ms
      roboFrente(vel);                          //Move o robô para frente
   
   } //end if
   
   else                                      //Não...
   {
   
      roboTras(vel);                           //Move o robô para trás
      delay(300);                            //Por 600ms
      roboEsquerda(vel);                           //Move o robô para esquerda
      delay(300);                            //Por 2000ms
      roboFrente(vel);                          //Move o robô para frente
   
   } //end else
   

}

void roboFrente(int v)
{
  motor1.setSpeed(v);
  motor1.run(FORWARD);
  motor2.setSpeed(v-10);
  motor2.run(FORWARD);
}

void roboDireita(int v)
{
  motor1.setSpeed(v);
  motor1.run(FORWARD);
  motor2.setSpeed(v);
  motor2.run(BACKWARD);
}

void roboEsquerda(int v)
{
  motor1.setSpeed(v);
  motor1.run(BACKWARD);
  motor2.setSpeed(v);
  motor2.run(FORWARD);
}

void roboTras(int v)
{
  motor1.setSpeed(v);
  motor1.run(BACKWARD);
  motor2.setSpeed(v-10);
  motor2.run(BACKWARD);
}

void roboParado()
{
  motor1.setSpeed(0);
  motor1.run(RELEASE);
  motor2.setSpeed(0);
  motor2.run(RELEASE);
}
