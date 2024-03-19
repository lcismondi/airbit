/*
   Versión base para definición de pin out

   Leds direccionables
   4 pulsadores
   Cooler
   Medición de calidad del aire
   Presión y temperatura

*/

#include <Adafruit_BMP085.h>
#include <FastLED.h>                                    //https://github.com/FastLED/FastLED/wiki/Basic-usage


//****************************************************
//******************** Pinout ************************
//****************************************************

#define Particulas  A0 //
#define D0          16 //D0 Led PCB 
#define D1           5 //D1 SCL(I2C)
#define D2           4 //D2 SDA(I2C)
#define Leds         0 //D3 
#define D4           2 //D4
#define Cooler      14 //D5
#define BTN1        12 //D6
#define BTN2        13 //D7
#define BTN3        15 //D8
#define TX           1 //TX
#define RX           3 //RX
#define CLK          6 //CLK
#define SD0          7 //SD0
#define SD1          8 //SD1
#define SD2          9 //SD2
#define BTN4        10 //SD3
#define CMD         11 //CMD

#define NumLeds  20

//****************************************************
//******************* Variables **********************
//****************************************************

Adafruit_BMP085 bmp;
CRGB MemLeds[NumLeds];                                   //Espacio en memoria para trabajar

bool btnState1, btnFlag1 = 0;
bool btnState2, btnFlag2 = 0;
bool btnState3, btnFlag3 = 0;
bool btnState4, btnFlag4 = 0;

byte PWMcooler = 125;


unsigned long tiempoMedicion = 0;

void setup() {

  pinMode(Particulas, INPUT);
  pinMode(Cooler, OUTPUT);
  pinMode(BTN1, INPUT);
  pinMode(BTN2, INPUT);
  pinMode(BTN3, INPUT_PULLUP);
  pinMode(BTN4, INPUT);

  Serial.begin(115200);
  bmp.begin();

  Serial.println("");
  Serial.println("");
  Serial.println("Inicio");
  Serial.println("");

  Serial.print("Presión: ");
  Serial.print(bmp.readPressure());
  Serial.println(" Pa");
  Serial.print("Temperatura: ");
  Serial.print(bmp.readTemperature());
  Serial.println(" °C");
  Serial.print("Altura: ");
  Serial.print(bmp.readAltitude());
  Serial.println(" m");
  Serial.print("Presión a nivel del mar: ");
  Serial.print(bmp.readSealevelPressure());
  Serial.println(" Pa");
  Serial.print("Altura real: ");
  Serial.print(bmp.readAltitude(bmp.readSealevelPressure()));
  Serial.println(" m");

  Serial.print("Particulas: ");
  Serial.print(analogRead(Particulas));
  Serial.println(" (lectura del adc)");

  Serial.print("Luces: ");
  FastLED.addLeds<WS2812B, Leds>(MemLeds, NumLeds);
  Serial.println(" Configuradas");

  Serial.print("Cooler: ");
  analogWrite(Cooler, PWMcooler);
  Serial.println("Encendido");

  //Por error de boot
  btnFlag4 = 1;

  tiempoMedicion = millis();

}

void loop() {

  btnState1 = digitalRead(BTN1);
  btnState2 = digitalRead(BTN2);
  btnState3 = !digitalRead(BTN3);                               //Lógica negativa por hardware se corrige por software
  btnState4 = !digitalRead(BTN4);

  // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
  if (btnState1 == LOW && btnFlag1 == 0) {

    Serial.println("ON S 1");
    MemLeds[4].setRGB( 255, 0, 0);
    MemLeds[5].setRGB( 255, 0, 0);
    FastLED.show();

    if (PWMcooler > 0)
    {
      PWMcooler = PWMcooler - 5;
      analogWrite(Cooler, PWMcooler);
    }

    btnFlag1 = 1;

  }
  else if (btnState1 == HIGH && btnFlag1 == 1) {

    btnFlag1 = 0;

  }


  if (btnState2 == LOW && btnFlag2 == 0) {

    Serial.println("ON S 2");
    MemLeds[0].setRGB( 0, 255, 0);
    MemLeds[19].setRGB( 0, 255, 0);

    FastLED.show();

    btnFlag2 = 1;
  }
  else if (btnState2 == HIGH && btnFlag2 == 1) {

    btnFlag2 = 0;

  }

  if (btnState3 == LOW  && btnFlag3 == 0) {

    Serial.println("ON S 3");     //FALLA LECTURA DE ESTE PULSADOR!!
    MemLeds[14].setRGB( 0, 0, 255);
    MemLeds[15].setRGB( 0, 0, 255);

    FastLED.show();

    btnFlag3 = 1;
  }
  else if (btnState3 == HIGH && btnFlag3 == 1) {

    btnFlag3 = 0;

  }

  if (btnState4 == LOW && btnFlag4 == 0) {

    Serial.println("ON S 4");
    MemLeds[9].setRGB( 255, 255, 255);
    MemLeds[10].setRGB( 255, 255, 255);

    FastLED.show();

    btnFlag4 = 1;

    if (PWMcooler < 255)
    {
      PWMcooler = PWMcooler + 5;
      analogWrite(Cooler, PWMcooler);
    }
  }
  else if (btnState4 == HIGH && btnFlag4 == 1) {

    btnFlag4 = 0;

  }

  delay(500);
  FastLED.clear();
  FastLED.show();

  // Periodo de medición
  //5min = 1000*60*5 = 300000
  if (millis() >= tiempoMedicion + 5000) {

  Serial.println("");

  Serial.print("Presión: ");
  Serial.print(bmp.readPressure());
  Serial.println(" Pa");
  Serial.print("Temperatura: ");
  Serial.print(bmp.readTemperature());
  Serial.println(" °C");
  Serial.print("Altura: ");
  Serial.print(bmp.readAltitude());
  Serial.println(" m");
  Serial.print("Presión a nivel del mar: ");
  Serial.print(bmp.readSealevelPressure());
  Serial.println(" Pa");
  Serial.print("Altura real: ");
  Serial.print(bmp.readAltitude(bmp.readSealevelPressure()));
  Serial.println(" m");

  Serial.print("Particulas: ");
  Serial.print(analogRead(Particulas));
  Serial.println(" (lectura del adc)");

  Serial.print("Luces: ");
  FastLED.addLeds<WS2812B, Leds>(MemLeds, NumLeds);
  Serial.println(" Configuradas");

  Serial.print("Cooler: ");
  Serial.println(PWMcooler);

  tiempoMedicion = millis();
  }


}
