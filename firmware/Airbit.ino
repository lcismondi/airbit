/*
   Versión base para definición de pin out

   Leds direccionables
   4 pulsadores
   Cooler
   Medición de calidad del aire
   Presión y temperatura
   Comunicación wifi con form

*/

#include <Adafruit_BMP085.h>
#include <FastLED.h>                                    //https://github.com/FastLED/FastLED/wiki/Basic-usage

#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoOTA.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#ifndef STASSID
#define STASSID "SSID"
#define STAPSK  "PASS"
#endif

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

const char* ssid = STASSID;
const char* password = STAPSK;
const String EstadoWiFi[] = {"Idle", "Error SSID", "Scan completo", "Conectado", "Contraseña incorrecta", "Conección perdida", "Desconectado"};

Adafruit_BMP085 bmp;
CRGB MemLeds[NumLeds];                                   //Espacio en memoria para trabajar

bool btnState1, btnFlag1 = 0;
bool btnState2, btnFlag2 = 0;
bool btnState3, btnFlag3 = 0;
bool btnState4, btnFlag4 = 0;

byte PWMcooler = 125;
byte brillo = 125;

float presion = 0;
float temperatura = 0;
float altitud = 0;
float presionMar = 0;
float alturaReal = 0;
float particulas = 0;


unsigned long tiempoMedicion, tiempoDB = 0;

char dbAirbit[109] = "ID_DB"; //99 caracteres

//ID de comunicación a los campos de la base de datos
struct identificadorAirbit
{
  const String PSN = "&entry.630942306=";     //presion
  const String TMP = "&entry.1972241633=";    //Temperatura
  const String ATT = "&entry.318527870=";     //Altitud
  const String PSM = "&entry.1217225494=";    //Presion a nivel del mar
  const String ALR = "&entry.1036674240=";    //Altura real
  const String PRT = "&entry.283096273=";     //Particulas
};
identificadorAirbit idAirbit;


void setup() {

  pinMode(Particulas, INPUT);
  pinMode(Cooler, OUTPUT);
  pinMode(BTN1, INPUT);
  pinMode(BTN2, INPUT);
  pinMode(BTN3, INPUT_PULLUP);
  pinMode(BTN4, INPUT);

  Serial.begin(115200);
  bmp.begin();

  //Apaga todas las luces
  FastLED.clear();
  fill_solid( &(MemLeds[0]), 20 /*number of leds*/, CRGB( 0, 0, 0) );
  FastLED.show();

  Serial.println("");
  Serial.println("");
  Serial.println("Inicio");
  Serial.println("");

  //******************* Perifericos **********************

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
  FastLED.setBrightness(brillo);
  Serial.println(" Configuradas");

  Serial.print("Cooler: ");
  analogWrite(Cooler, PWMcooler);
  Serial.println("Encendido");


  //******************* WIFI **********************
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  Serial.println("Rojo power");
  //Con el segundo parámetro se define cuantos leds toman el mismo color, a partir del inicial
  FastLED.clear();
  //fill_solid( &(MemLeds[15]), 1 /*number of leds*/, CRGB( 255, 0, 0) );
  MemLeds[15].setRGB( 255, 0, 0);
  FastLED.show();

  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }
  /*
    ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
      type = "sketch";
    } else { // U_FS
      type = "filesystem";
    }

    // NOTE: if updating FS this would be the place to unmount FS using FS.end()
    Serial.println("Start updating " + type);
    });
    ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
    });
    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    });
    ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) {
      Serial.println("Auth Failed");
    } else if (error == OTA_BEGIN_ERROR) {
      Serial.println("Begin Failed");
    } else if (error == OTA_CONNECT_ERROR) {
      Serial.println("Connect Failed");
    } else if (error == OTA_RECEIVE_ERROR) {
      Serial.println("Receive Failed");
    } else if (error == OTA_END_ERROR) {
      Serial.println("End Failed");
    }
    });
    ArduinoOTA.begin();
  */
  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());



  //******************* BOOT ERROR **********************
  btnFlag4 = 1;

  tiempoMedicion = millis();

  FastLED.clear();
  fill_solid( &(MemLeds[0]), 20 /*number of leds*/, CRGB( 0, 0, 0) );
  FastLED.show();

}

void loop() {

  //ArduinoOTA.handle();

  btnState1 = digitalRead(BTN1);
  btnState2 = digitalRead(BTN2);
  btnState3 = !digitalRead(BTN3);                               //Lógica negativa por hardware se corrige por software
  btnState4 = !digitalRead(BTN4);

  // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
  if (btnState1 == LOW && btnFlag1 == 0) {

    Serial.println("ON S 1");
    //MemLeds[4].setRGB( 255, 0, 0);
    //MemLeds[5].setRGB( 255, 0, 0);
    //FastLED.show();

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
    //MemLeds[0].setRGB( 0, 255, 0);
    //MemLeds[19].setRGB( 0, 255, 0);
    //FastLED.show();

    btnFlag2 = 1;
  }
  else if (btnState2 == HIGH && btnFlag2 == 1) {

    btnFlag2 = 0;

  }

  if (btnState3 == LOW  && btnFlag3 == 0) {

    Serial.println("ON S 3");     //FALLA LECTURA DE ESTE PULSADOR!!
    //MemLeds[14].setRGB( 0, 0, 255);
    //MemLeds[15].setRGB( 0, 0, 255);
    //FastLED.show();

    btnFlag3 = 1;
  }
  else if (btnState3 == HIGH && btnFlag3 == 1) {

    btnFlag3 = 0;

  }

  if (btnState4 == LOW && btnFlag4 == 0) {

    Serial.println("ON S 4");
    //MemLeds[9].setRGB( 255, 255, 255);
    //MemLeds[10].setRGB( 255, 255, 255);
    //FastLED.show();

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
  fill_solid( &(MemLeds[0]), 20 /*number of leds*/, CRGB( 0, 0, 0) );
  FastLED.show();

  // Periodo de medición
  //5min = 1000*60*5 = 300000
  if (millis() - tiempoMedicion >=  5000) {

    Serial.println("");

    Serial.print("Presión: ");
    presion = bmp.readPressure();
    Serial.print(presion);
    Serial.println(" Pa");
    Serial.print("Temperatura: ");
    temperatura = bmp.readTemperature()-3;
    Serial.print(temperatura);
    Serial.println(" °C");
    Serial.print("Altura: ");
    altitud = bmp.readAltitude();
    Serial.print(altitud);
    Serial.println(" m");
    Serial.print("Presión a nivel del mar: ");
    presionMar = bmp.readSealevelPressure();
    Serial.print(presionMar);
    Serial.println(" Pa");
    Serial.print("Altura real: ");
    alturaReal = bmp.readAltitude(bmp.readSealevelPressure());
    Serial.print(alturaReal);
    Serial.println(" m");

    Serial.print("Particulas: ");
    particulas = analogRead(Particulas);
    Serial.print(particulas);
    Serial.println(" (lectura del adc)");

    Serial.print("Cooler: ");
    Serial.println(PWMcooler);

    tiempoMedicion = millis();
  }

  // 1000 son un segundo
  // 60000 son un minuto
  // 300000 son 5 minutos
  // 600000 son 10 minutos
  // 1500000 son 25 minutos
  // 1800000 son 30 minutos
  if (millis() - tiempoDB >=  (600000-60000)) {
    PWMcooler = 255;
    analogWrite(Cooler, PWMcooler);
  }
  if (millis() - tiempoDB >=  600000) {
    PWMcooler = 0;
    analogWrite(Cooler, PWMcooler);

    tiempoDB = millis();

    String direccion =  String(dbAirbit) +
                        "?usp=pp_url"    +
                        idAirbit.PSN  + presion +
                        idAirbit.TMP  + temperatura +
                        idAirbit.ATT  + altitud +
                        idAirbit.PSM  + presionMar +
                        idAirbit.ALR  + alturaReal +
                        idAirbit.PRT  + particulas +
                        "&submit=Submit";

    Serial.println(direccion);

    //Inicia transferencia de información
    std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);
    client->setInsecure();
    HTTPClient https;

    if (https.begin(*client, direccion)) {  // HTTPS
      // start connection and send HTTP header
      int httpCode = https.GET();
      // httpCode will be negative on error
      if (httpCode > 0) {
        // HTTP header has been send and Server response header has been handled
        //Serial.printf("[HTTPS] GET... code: %d\n", httpCode);
        //Serial.println(" ");
        // file found at server
        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
          //String payload = https.getString();
          //Serial.println(payload);
          Serial.println("Verde");
          //FastLED.clear();
          //Con el segundo parámetro se define cuantos leds toman el mismo color, a partir del inicial
          //fill_solid( &(MemLeds[0]), 20 /*number of leds*/, CRGB( 0, 255, 0) );
          //FastLED.show();
          //delay(1000);
        }
      } else {
        //Serial.printf("[HTTPS] GET... failed, error: %s\n", https.errorToString(httpCode).c_str());
        Serial.println("Rojo");
        //FastLED.clear();
        //Con el segundo parámetro se define cuantos leds toman el mismo color, a partir del inicial
        //fill_solid( &(MemLeds[0]), 20 /*number of leds*/, CRGB( 255, 0, 0) );
        //FastLED.show();
      }

      https.end();

    } else {
      //Serial.printf("[HTTPS] Unable to connect\n");
      Serial.println("Amarillo");
      //FastLED.clear();
      //Con el segundo parámetro se define cuantos leds toman el mismo color, a partir del inicial
      //fill_solid( &(MemLeds[0]), 20 /*number of leds*/, CRGB( 255, 255, 0) );
      //FastLED.show();
    }

  }

}
