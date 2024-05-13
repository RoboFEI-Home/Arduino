#include <Sabertooth.h>
#include <Encoder.h>

#include <stdio.h>

#define ODOM_PERIOD 50

#define LED_PIN 13

Sabertooth STFront(128);        //Definição do sabertooth em packetized serial mode adress 128 (controla os 2 motores individualm,ente) - botoes sabertooth (baixo, baixo, baixo, alto, alto, alto)
Sabertooth STRear(129);        //Definição do sabertooth em packetized serial mode adress 129 (controla os 2 motores individualmente) - botoes sabertooth (baixo, baixo, baixo, baixo, alto, alto)

long odom_timer;

//float V1, V2, V3;

float enc1, enc2, enc3;

Encoder Encoder1(33, 35);
Encoder Encoder2(37, 39);
Encoder Encoder3(41, 43);

float map(float x, float
in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

int convertToMotor(float value) {
  float r = map(value, -10.0, 10.0, -127.0, 127.0);
  r = constrain(r, -127.0, 127.0);
  return (int) round(r);
}

void moveMotors(int Power1, int Power2, int Power3) {
  STFront.motor(1, Power1);
  STFront.motor(2, Power2);
  STRear.motor(1, Power3);
}

void stopMotors() {
  STFront.motor(1, 0);
  STFront.motor(2, 0);
  STRear.motor(1, 0);
}

void clearEncoders() {
  Encoder1.write(0);
  Encoder2.write(0);
  Encoder3.write(0);
}

void setup() {

  pinMode(12, INPUT_PULLUP);
  pinMode(13, INPUT_PULLUP);
  
  SabertoothTXPinSerial.begin(9600);

  Serial.begin(9600);

  stopMotors();

  clearEncoders();

  odom_timer = millis();

}

void loop() {
  
  if (millis() - odom_timer >= ODOM_PERIOD) {

    enc1 = Encoder1.read();
    enc2 = Encoder2.read();
    enc3 = Encoder3.read();

    Serial.println(enc1 + enc2 + enc3);
    
    if (Serial.available() > 0) {
      String cmd = Serial.readString();
      int cmd_len = cmd.length() + 1;
      char cmd_array[cmd_len];
      cmd.toCharArray(cmd_array, cmd_len);
      Serial.println(cmd);

      char* cmd1 = strtok(cmd_array, " ");
      Serial.println(cmd1);

      char* cmd2 = strtok(NULL, " ");
      Serial.println(cmd2);

      char* cmd3 = strtok(NULL, " ");
      Serial.println(cmd3);
      
      float cmd1f = atof(cmd1);
      float cmd2f = atof(cmd2);
      float cmd3f = atof(cmd3);
      Serial.println(cmd1f);
      Serial.println(cmd2f);
      Serial.println(cmd3f);
      Serial.println(convertToMotor(cmd1f));
      Serial.println(convertToMotor(cmd2f));
      Serial.println(convertToMotor(cmd3f));
      moveMotors(convertToMotor(cmd3f), convertToMotor(cmd2f), convertToMotor(cmd1f));
    }
   
    
    odom_timer = millis();

    clearEncoders();


  }

}
