  #include <Servo.h>
  #include <AFMotor.h>
    
  #define trigPin A2        //Pino TRIG do sensor no pino analógico A2
  #define echoPin 9         //Pino ECHO do sensor no pino analógico 9
  #define buzzer A3         // Define o pino do buzzer no pino ANALÓGICO A3

  AF_DCMotor motor1(1);     // Define o motor1 ligado ao M1 
  AF_DCMotor motor2(4);     // Define o motor2 ligado ao M4

  unsigned char velocidade1 = 0x00; //Armazena a velocidade do motor (8 bits)
 
  int TempoGirar = 1;           // esse é o tempo para o robô girar em 45º
  int distanciaObstaculo = 30;  // distância para o robô parar e recalcular o melhor caminho
  int velocidadeMotores = 80;   // velocidade dos motores
  Servo servo; 
 
  //----- variáveis  para o sensor ultrassonico -----
  long duracao;
  long distancia_cm = 0;
  int minimumRange = 5;      //tempo de resposta do sensor
  int maximumRange= 200;

  void rotacao_Parado();
  void autonomo();
  void roboParado();
  
  void setup(){      
    Serial.begin(9600);

    servo.attach(10);  // Define o servo motor ligado no pino digital 10
    
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);
    pinMode(buzzer,OUTPUT); 
    
    motor1.setSpeed(velocidadeMotores);
    motor2.setSpeed(velocidadeMotores);
    
    servo.write(85);                        // O servo do sensor se inicia a 85 graus
    rotacao_Parado();                       //inica com os motores parados    
  }   
     
  void loop(){   
    autonomo();
  }   

  // =============================================================================================
  //==== Função principal, função robô autônomo ==== 
  void autonomo(){   
    reposicionaServoSonar(); //Coloca o servo para olhar a frente   
    int distancia = lerSonar(); // Ler o sensor de distância 
    Serial.print("distancia em cm: ");
    Serial.println(distancia);   // Exibe no serial monitor
  
    if (distancia > distanciaObstaculo) {  // Se a distância for maior que 20 cm 
     rotacao_Frente(); //robô anda para frente  
    }else{  
     rotacao_Parado();  //para o robô 
     posicionaCarroMelhorCaminho(); //calcula o melhor caminho   
     autonomo();   
   }  
  } 

  // =============================================================================================
  // ==== Ler e calcular a distância do sensor ultrassônico ====
  int lerSonar(){   
    digitalWrite(trigPin, LOW); //não envia som
    delayMicroseconds(2);
    digitalWrite(trigPin,HIGH); //envia som
    delayMicroseconds(10);
    digitalWrite(trigPin,LOW);       //não envia o som e espera o retorno do som enviado
    duracao = pulseIn(echoPin,HIGH); //Captura a duração em tempo do retorno do som.
    distancia_cm = duracao/56;       //Calcula a distância
    delay(30);                       //delay(30ms)
    return distancia_cm;             // Retorna a distância 
  }  

  // =============================================================================================
  // ==== Calcular a distância do centro ====
  int calcularDistanciaCentro(){   
   servo.write(90);   
   delay(20);  
   int leituraDoSonar = lerSonar();  // Ler sensor de distância 
   delay(500);  
   leituraDoSonar = lerSonar();  
   delay(500);  
   Serial.print("Distancia do Centro: "); // Exibe no serial 
   Serial.println(leituraDoSonar);  
   return leituraDoSonar;       // Retorna a distância 
  }   

  // =============================================================================================
  // ==== Calcular a distância da direita ====   
  int calcularDistanciaDireita(){   
   servo.write(0);  
   delay(200); 
   int leituraDoSonar = lerSonar();  
   delay(500);  
   leituraDoSonar = lerSonar();  
   delay(500);  
   Serial.print("Distancia da Direita: "); 
   Serial.println(leituraDoSonar);  
   return leituraDoSonar;   
  }   

  // ===============================================================================================================
  // ==== Calcula a distância da esquerda ====   
  int calcularDistanciaEsquerda(){   
   servo.write(180);  
   delay(200); 
   int leituraDoSonar = lerSonar();  
   delay(500);  
   leituraDoSonar = lerSonar();  
   delay(500);  
   Serial.print("Distancia Esquerda: "); 
   Serial.println(leituraDoSonar);  
   return leituraDoSonar;   
  }   

  // ===============================================================================================================
  // ==== Pegar as distâncias lidas e calcular a melhor distância ====
  char calculaMelhorDistancia(){   
   int esquerda = calcularDistanciaEsquerda();   
   int centro = calcularDistanciaCentro();   
   int direita = calcularDistanciaDireita();   
   
   reposicionaServoSonar();   
   
   int maiorDistancia = 0;  
   char melhorDistancia = '0';    
      
   if (centro > direita && centro > esquerda){   
     melhorDistancia = 'c';   
     maiorDistancia = centro;   
   }else  
   if (direita > centro && direita > esquerda){   
     melhorDistancia = 'd';   
     maiorDistancia = direita;   
   }else 
   if (esquerda > centro && esquerda > direita){   
     melhorDistancia = 'e';   
     maiorDistancia = esquerda;   
   }   
   if (maiorDistancia <= distanciaObstaculo) { //distância limite para parar o robô  
     rotacao_Re();   
     posicionaCarroMelhorCaminho();   
   }   
   
   reposicionaServoSonar(); 
   
   return melhorDistancia;   
  }   
  
  // ===============================================================================================================   
  // ==== Função robô na melhor distância ====
  void posicionaCarroMelhorCaminho(){   
   char melhorDist = calculaMelhorDistancia();    
   Serial.print("melhor Distancia em cm: "); 
   Serial.println(melhorDist); 
   if (melhorDist == 'c'){   
     autonomo();   
   }else if (melhorDist == 'd'){   
     rotacao_Direita();   
   }else if (melhorDist == 'e'){   
     rotacao_Esquerda();    
   }else{   
     rotacao_Re();   
   }   
   reposicionaServoSonar();   
  }   

  // ===============================================================================================================
  // ==== Função sensor do robô no centro ====
  void reposicionaServoSonar(){   
   servo.write(90);  
   delay(200);  
  }   

  // ===============================================================================================================
  // ==== Função Robo parar ====
  void rotacao_Parado(){   
   Serial.println(" Motor: Parar ");
   motor1.run(RELEASE); // Motor para 
   motor2.run(RELEASE); 
  }   
     
  // ===============================================================================================================
  // ==== Função robô andar para frente ====
  void rotacao_Frente(){   
   Serial.println("Motor: Frente ");  
   motor1.run(FORWARD); // Roda vai para frente 
   motor2.run(FORWARD);  
   delay(50);   
  } 
  
  // ===============================================================================================================
  // ==== Função robô parado ====
  void roboParado(){
    velocidade1 = 0x00;           // Velocidade recebe o valor mínimo
    motor1.setSpeed(velocidade1); // Seleciona velocidade atual
    motor1.run(RELEASE);          // Motor parado
    motor2.setSpeed(velocidade1); // Seleciona velocidade atual
    motor2.run(RELEASE);          // Motor parado
  }
  
  // ===============================================================================================================
  // ==== Função robô andar para trás e emite som quando ele dá ré ====
  void rotacao_Re(){   
   Serial.println("Motor: ré "); 
   for (int i=0; i <= 3; i++){
      digitalWrite(buzzer, HIGH); // Liga o som
      delay(100);
      motor1.run(BACKWARD);    // Roda vai para trás 
      motor2.run(BACKWARD);    // Roda vai para trás 
      delay(100); 
      digitalWrite(buzzer, LOW); // Desliga o som 
      delay(100);
   }
   rotacao_Parado();   
  }   

  // ===============================================================================================================
  // ==== Função Robô virar à direita ====
  void rotacao_Direita(){   
   digitalWrite(buzzer, HIGH); // Liga o som
   delay(100);
   motor1.run(BACKWARD);    //o robô dá uma ré para não colidir ao girar
   motor2.run(BACKWARD);     
   delay(50); 
   digitalWrite(buzzer, LOW); // Desliga o som 
   delay(100);
   Serial.println("Para a direita"); 
   motor1.run(FORWARD); // Roda vai para frente 
   motor2.run(BACKWARD); // Roda vai para trás  
   delay(TempoGirar);   
  }   

  // ===============================================================================================================
  // ==== Função robô virar à esquerda ====  
  void rotacao_Esquerda(){   
   digitalWrite(buzzer, HIGH); // Liga o som
   delay(100);
   motor1.run(BACKWARD);    // // O robô dá uma ré para não colidir ao girar
   motor2.run(BACKWARD);  
   delay(50); 
   digitalWrite(buzzer, LOW); // Desliga o som 
   delay(100);
   Serial.println(" Para a esquerda "); 
   motor1.run(BACKWARD); // Roda vai para trás 
   motor2.run(FORWARD); // Roda vai para frente 
   delay(TempoGirar);   
  }



