#include <SPI.h>      //SPI library for communicate with the nRF24L01+
#include "RF24.h"     //The main library of the nRF24L01+

// Define enable pins of the Motors
const int enbA = 3;
const int enbB = 5;

//Define control pins of the Motors
//If the motors rotate in the opposite direction, you can change the positions of the following pin numbers
const int IN1 = 2;    //Right Motor (-)
const int IN2 = 4;    //Right Motor (+)
const int IN3 = 7;    //Left Motor (+)
const int IN4 = 6;    //Right Motor (-)

// Velocidade dos motores para frente ou para trás
int RightSpd = 150;
int LeftSpd = 150;

// Velocidade nas curvas
int RightSpdV = 100;
int LeftSpdV = 100;

//Led - recebendo?
int led = 8;

// Define packet for the direction (X axis and Y axis)
int data[2];

// Define object from RF24 library - 9 and 10 are a digital pin numbers to which signals CE and CSN are connected
RF24 radio(9,10);

// Create a pipe addresses for the communicate
const uint64_t pipe = 0xE8E8F0F0E1LL;

void setup(){
  pinMode(enbA, OUTPUT);
  pinMode(enbB, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  pinMode(led,OUTPUT);
  
  Serial.begin(9600);
  radio.begin();                    // Start the nRF24 communicate            
  radio.setPALevel(RF24_PA_MIN);
  radio.openReadingPipe(1, pipe);   // Sets the address of the transmitter to which the program will receive data.
  radio.startListening();             
  }

void loop(){
  if (radio.available()){
    digitalWrite(led, HIGH);
    radio.read(data, sizeof(data));
    Serial.println("AVALIABLE");
    if(data[1] > 170){
      // frente
      analogWrite(enbA, RightSpd);
      analogWrite(enbB, LeftSpd);
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, HIGH);
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, HIGH);
      Serial.println("FRENTE");
    }

    if(data[1] < 140){
      // trás            
      analogWrite(enbA, RightSpd);
      analogWrite(enbB, LeftSpd);
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
      digitalWrite(IN3, HIGH);
      digitalWrite(IN4, LOW);
      Serial.println("TRÁS");
    }
    
    if(data[0] > 372){
      // direita
      analogWrite(enbA, RightSpdV);
      analogWrite(enbB, LeftSpdV);
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, HIGH);
      Serial.println("DIREITA");
    }
    
    if(data[0] < 315){
      // esquerda
      analogWrite(enbA, RightSpdV);
      analogWrite(enbB, LeftSpdV);
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, HIGH);
      digitalWrite(IN3, HIGH);
      digitalWrite(IN4, LOW);
      Serial.println("ESQUERDA");
    }

    if(data[0] > 330 && data[0] < 362 && data[1] > 140 && data[1] < 160){
      // parar
      analogWrite(enbA, 0);
      analogWrite(enbB, 0);
      Serial.println("STOP");
    }
  }
}
