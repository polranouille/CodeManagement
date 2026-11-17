/*#include "HX711.h" //This library can be obtained here http://librarymanager/All#Avia_HX711

#define calibration_factor -7050.0 //This value is obtained using the SparkFun_HX711_Calibration sketch

bool startloop = false;
unsigned long tempsDebutRun;
const int pinJauge = A0;

//Etape 5

#define DOUT  13
#define CLK  12

HX711 scale;


void setup() {

  Serial.begin(115200, SERIAL_8N1); // 8bit, pas de parité, 1bit stop
  
  scale.begin(DOUT, CLK);
  scale.set_scale(420.093);
  scale.tare();


  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  
  delay(10);
  Serial.println("Arduino pret, attente de r");

while(startloop == false) {
    if(Serial.available() > 0) {
        int value = Serial.read();
        if(value == 114) {
            startloop = true;
            tempsDebutRun = micros();
        }
    }
}
}

void loop() {
  digitalWrite(LED_BUILTIN, HIGH);

  // Etape 3 : comptage du temps en microsecondes
  unsigned long tempsRun = micros() - tempsDebutRun;
  unsigned long tempsRunMs = tempsRun / 1000;

  
  
  Serial.println("before get_units");
  float valeurCapteur = scale.get_units(1);
  Serial.println("after get_units");

//char message[32];
//sprintf(message, "%lu;%ld", tempsRunMs, valeurCapteur);
Serial.println(valeurCapteur);

//Serial.println(message);
Serial.flush();

//Etape 2 
    if (Serial.available() > 0) {          // caractère reçu ?
    int value = Serial.read();
    if (value == 82) {                   // 'R' (majuscule) = caractère de reset == "82"
      NVIC_SystemReset();                       // redémarre l'Arduino

//Etape 4
      if (value == 82) {
    Serial.flush();          // AJOUTÉ
    NVIC_SystemReset();
}
    }
  }
}
*/

#include "HX711.h"

#define DOUT 13
#define CLK  12

HX711 scale;

bool startloop = false;
unsigned long tempsDebutRun;

void setup() {
  Serial.begin(115200, SERIAL_8N1);   // 8 bits, pas de parité, 1 bit de stop

  scale.begin(DOUT, CLK);
  scale.set_scale(420.0983);          // à calibrer pour ta cellule
  scale.tare();

  Serial.println("Arduino pret, attente de r");

  // Attente du caractère 'r' envoyé par le PC
  while (!startloop) {
    if (Serial.available() > 0) {
      if (Serial.read() == 'r') {
        startloop = true;
        tempsDebutRun = micros();
      }
    }
  }
}

void loop() {
  // Reset logiciel sur 'R'
  if (Serial.available() > 0) {
    if (Serial.read() == 'R') {
      Serial.flush();
      NVIC_SystemReset();
    }
  }

  unsigned long tempsMs = (micros() - tempsDebutRun) / 1000;
  float valeur = scale.get_units(1);

  Serial.print(tempsMs);
  Serial.print(";");
  Serial.println(valeur);
}


