#include <Arduino.h>
#include "version_airbit.h"
#include <memory>
#include <EEPROM.h>
#include "calibracion_aire.h"
#include "calidad_aire.h"
#include "configuracion_airbit.h"
#include "iluminacion_airbit.h"
AirbitLuz::Registro iluminacionActual = AirbitLuz::predeterminada();
#include "intensidad_calidad.h"
AirbitIntensidad::Registro intensidadCalidadActual = AirbitIntensidad::predeterminada();
#include <WiFiClientSecureBearSSL.h>

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
// Ceder al WiFi entre pixeles WS2812B; no bloquear IRQ durante los 20 LED.
// Si una interrupcion altera el tiempo de un pixel, FastLED reintenta el frame.
#define FASTLED_ALLOW_INTERRUPTS 1
#define INTERRUPT_THRESHOLD 0 // Mismo umbral que la opcion predeterminada ESP8266.
#define FASTLED_INTERRUPT_RETRY_COUNT 5
#include <FastLED.h>                                    //https://github.com/FastLED/FastLED/wiki/Basic-usage

#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoOTA.h>
#include "diagnostico_airbit.h"



#if !defined(AIRBIT_PUBLIC_RELEASE) && __has_include("configuracion_local.h")
#include "configuracion_local.h"
#endif
#ifndef STASSID
#define STASSID ""
#endif
#ifndef STAPSK
#define STAPSK ""
#endif
#ifndef AIRBIT_FORM_URL
#define AIRBIT_FORM_URL ""
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

#define inicio 0
#define errorWifi 1
#define espera 2

//****************************************************
//******************* Variables **********************
//****************************************************

const char* ssid = STASSID;
const char* password = STAPSK;

Adafruit_BMP085 bmp;
CRGB MemLeds[NumLeds];                                   //Espacio en memoria para trabajar


byte PWMcooler = 125;
byte brillo = 125;

unsigned int estado = 0;

float presion = 0;
float temperatura = 0;
float altitud = 0;
float presionMar = 0;
float alturaReal = 0;
float particulas = 0;


unsigned long tiempoMedicion, tiempoDB = 0;
bool envioDBPendiente = false;

/* https://docs.google.com/forms/d/e/FORMULARIO_LOCAL/viewform?
  usp=pp_url
  &entry.630942306=1
  &entry.1972241633=2
  &entry.318527870=3
  &entry.1217225494=4
  &entry.1036674240=5
  &entry.283096273=6
*/

const char dbAirbit[] = AIRBIT_FORM_URL; //99 caracteres

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


// La conexion Wi-Fi se supervisa sin detener sensores ni pulsadores.
WiFiEventHandler eventoDesconexionWiFi, eventoIPWiFi, eventoEntradaAP, eventoSalidaAP;
bool wifiPausadoDesdeSerie = false;
bool pausarEnvioLedsDiagnostico = false;
void informarSocketsPortal();
bool intentoWiFiDesdePortal = false;
unsigned long ultimoClienteAPMs = 0;
bool huboClienteAP = false;
const unsigned long margenSalidaAPMs = 15000;
void solicitarRecuperacionPortal();
bool portalEnUso();
unsigned long ultimoIntentoWiFi = 0;
bool intentoWiFiActivo = false;
const unsigned long limiteIntentoWiFiMs = 15000;
const char* ssidConfigurado();
const char* claveWiFiConfigurada();
void iniciarConexionWiFi(bool desdePortal = false) {
  if (!ssidConfigurado()[0]) {
    intentoWiFiActivo = false;
    Serial.println(F("WiFi: sin red configurada; configurar desde el AP"));
    return;
  }
  wifiPausadoDesdeSerie = false;
  intentoWiFiDesdePortal = desdePortal;
  WiFi.enableSTA(true);
  WiFi.begin(ssidConfigurado(), claveWiFiConfigurada());
  ultimoIntentoWiFi = millis();
  intentoWiFiActivo = true;
}
const unsigned long intervaloReintentoWiFi = 30000;
int ultimoEstadoWiFi = -1;

const unsigned long duracionRojoArranqueMs = 5000;
const unsigned long duracionGiroTrasConexionMs = 3000;
unsigned long inicioIndicadoresMs = 0;
unsigned long inicioGiroWiFiMs = 0;
unsigned long momentoConexionWiFiMs = 0;
bool wifiConectadoParaIndicador = false;


const char* descripcionEstadoWiFi(int estado) {
  switch (estado) {
    case WL_IDLE_STATUS: return "Conectando";
    case WL_NO_SSID_AVAIL: return "Red no encontrada: revisar SSID y cobertura";
    case WL_SCAN_COMPLETED: return "Escaneo completado";
    case WL_CONNECTED: return "Conectado";
    case WL_CONNECT_FAILED: return "Fallo de conexion: revisar autenticacion y router";
    case WL_CONNECTION_LOST: return "Conexion perdida";
    case WL_WRONG_PASSWORD: return "Contrasena incorrecta";
    case WL_DISCONNECTED: return "Desconectado";
    default: return "Estado desconocido";
  }
}

const char* descripcionMotivoWiFi(uint8_t motivo) {
  switch (motivo) {
    case 201: return "Punto de acceso no encontrado";
    case 202: return "Fallo de autenticacion: revisar clave y seguridad del router";
    case 15:
    case 204: return "Tiempo de autenticacion agotado: revisar clave, senal y router";
    default: return "Consultar motivo SDK";
  }
}

bool indicadorWiFiActivo(unsigned long ahora) {
  return !wifiConectadoParaIndicador ||
         ahora - momentoConexionWiFiMs < duracionGiroTrasConexionMs;
}

void gestionarWiFi() {
  const int estado = WiFi.status();
  if (estado == WL_CONNECTED && !wifiConectadoParaIndicador) {
    wifiConectadoParaIndicador = true;
    momentoConexionWiFiMs = millis();
  } else if (estado != WL_CONNECTED && wifiConectadoParaIndicador) {
    wifiConectadoParaIndicador = false;
    inicioGiroWiFiMs = millis();
  }
  if (estado != ultimoEstadoWiFi) {
    Serial.printf("WiFi: %s (estado=%d)\n", descripcionEstadoWiFi(estado), estado);
    ultimoEstadoWiFi = estado;
    if (estado == WL_CONNECTED) {
      Serial.print("IP address: ");
      Serial.println(WiFi.localIP());
      Serial.printf("WiFi RSSI: %d dBm\n", WiFi.RSSI());
    }
  }
  const uint8_t clientesAP = WiFi.softAPgetStationNum();
  if (clientesAP) { ultimoClienteAPMs = millis(); huboClienteAP = true; }
  if (estado == WL_CONNECTED) intentoWiFiActivo = false;
  // Priorizar al telefono frente a un intento automatico, sin cancelar una
  // prueba de red solicitada expresamente al guardar WiFi en la app.
  if (estado != WL_CONNECTED && intentoWiFiActivo && clientesAP && !intentoWiFiDesdePortal) {
    intentoWiFiActivo = false;
    ultimoIntentoWiFi = millis();
    WiFi.disconnect(true, false);
    solicitarRecuperacionPortal();
    Serial.println(F("WiFi: intento automatico pausado para atender al cliente AP"));
  }
  const bool fallo = estado == WL_NO_SSID_AVAIL || estado == WL_CONNECT_FAILED || estado == WL_WRONG_PASSWORD;
  if (estado != WL_CONNECTED && intentoWiFiActivo &&
      (fallo || millis() - ultimoIntentoWiFi >= limiteIntentoWiFiMs)) {
    // Detener escaneos de STA conserva el AP disponible para configurar.
    WiFi.disconnect(true, false); // No borrar credenciales; AP sigue encendido.
    intentoWiFiActivo = false;
    ultimoIntentoWiFi = millis();
    solicitarRecuperacionPortal();
    Serial.println("WiFi: intento terminado; AP disponible, sin reiniciar Airbit");
  }
  const bool margenAPCumplido = !huboClienteAP || millis() - ultimoClienteAPMs >= margenSalidaAPMs;
  if (estado != WL_CONNECTED && !intentoWiFiActivo && !wifiPausadoDesdeSerie && ssidConfigurado()[0] && clientesAP == 0 &&
      margenAPCumplido && millis() - ultimoIntentoWiFi >= intervaloReintentoWiFi) {
    Serial.println("WiFi: reintento con AP sin clientes, sin reiniciar Airbit");
    iniciarConexionWiFi();
  }
}

void controlarRedDesdeSerie(char comando) {
  if (comando == 'a' || comando == 'A') {
    wifiPausadoDesdeSerie = true;
    intentoWiFiActivo = false;
    WiFi.disconnect(true, false); // Mantener AP y credenciales.
    solicitarRecuperacionPortal();
    Serial.println(F("[DBG RED] Solo AP temporal: STA pausada. w reanuda la red guardada; no se cambia EEPROM."));
  } else if (comando == 'l' || comando == 'L') {
    pausarEnvioLedsDiagnostico = !pausarEnvioLedsDiagnostico;
    Serial.printf_P(PSTR("[DBG LED] Transmision=%s; pausa temporal para aislar el envio WS2812B\n"),
        pausarEnvioLedsDiagnostico ? "PAUSADA" : "ACTIVA");
  } else if (comando == 'n' || comando == 'N') {
    informarSocketsPortal();
    Serial.printf_P(PSTR("[DBG RED] SDK=%s modo=%u sleep=%u STApausada=%u LEDpausado=%u\n"),
        ESP.getSdkVersion(), static_cast<unsigned int>(WiFi.getMode()),
        static_cast<unsigned int>(WiFi.getSleepMode()), wifiPausadoDesdeSerie, pausarEnvioLedsDiagnostico);
  } else if (comando == 'w' || comando == 'W') {
    iniciarConexionWiFi(true);
    Serial.println(F("[DBG RED] Reanudando conexion al router con la configuracion guardada."));
  }
}

// Numeracion fisica 1..20; los indices de MemLeds empiezan en 0.
// Cada fila corresponde a botones 1, 2, 3 y 4. Dos lados por esquina.
const uint8_t recorridoOlas[4][2][5] = {
  {{4, 3, 2, 1, 0},     {5, 6, 7, 8, 9}},       // B1: LED 5->1 y 6->10
  {{0, 1, 2, 3, 4},     {19, 18, 17, 16, 15}},  // B2: LED 1->5 y 20->16
  {{14, 13, 12, 11, 10}, {15, 16, 17, 18, 19}},  // B3: LED 15->11 y 16->20
  {{9, 8, 7, 6, 5},     {10, 11, 12, 13, 14}}   // B4: LED 10->6 y 11->15
};
const CRGB coloresOlas[4] = {CRGB::Red, CRGB::Green, CRGB::Blue, CRGB::White};
const unsigned long pasoOlaMs = 100;    // Tiempo entre LED consecutivos.
const unsigned long estelaOlaMs = 300; // Cada LED se desvanece en 300 ms.
const unsigned long duracionOlaMs = 4 * pasoOlaMs + estelaOlaMs;
const unsigned long intervaloFrameOlaMs = 20;
const unsigned long intervaloApagadoLedsMs = 100; // Reenvio de negro en reposo.
const unsigned long antirreboteMs = 30;
const uint8_t maxOlasPendientes = 16; // Por boton, ademas de la ola activa.

struct Ola {
  bool activa;
  unsigned long inicioMs;
  uint8_t pendientes;
};
Ola olas[4] = {};
bool actualizarFrameOlas = false;
bool indicadoresMostrados = false;
unsigned long ultimoFrameOlas = 0;

struct Pulsador {
  uint8_t pin;
  uint8_t nivelPulsado;
  bool lectura;
  bool estable;
  bool listo;
  unsigned long ultimoCambio;
};
// Se conservan los niveles efectivos del codigo que ya funciona en la placa.
Pulsador pulsadores[4] = {
  {BTN1, LOW, false, false, false, 0},
  {BTN2, LOW, false, false, false, 0},
  {BTN3, HIGH, false, false, false, 0},
  {BTN4, HIGH, false, false, false, 0}
};

AirbitCal::Gesto gestoCalibracion;
AirbitCal::Ventana calibracionAire;
AirbitCal::Registro referenciaAire = {};
bool referenciaAireValida = false;
unsigned long ultimoInformeCalibracionMs = 0;
const unsigned long periodoRespiracionMs = 3000;
AirbitCalidad::Indicador indicadorCalidad;
const unsigned long intervaloLecturaAireMs = 1000;
unsigned long ultimaLecturaAireMs = 0;
bool lecturaAireDisponible = false;

void limpiarOlas() {
  for (Ola& ola : olas) ola = {};
  actualizarFrameOlas = false;
}

void cargarReferenciaAire() {
  EEPROM.begin(AirbitConfig::bytesEeprom);
  EEPROM.get(0, referenciaAire);
  referenciaAireValida = AirbitCal::registroValido(referenciaAire);
  if (referenciaAireValida) {
    Serial.printf("MQ135: referencia guardada %.3f ADC (%u muestras)\n",
                  referenciaAire.promedioMilliAdc / 1000.0,
                  static_cast<unsigned int>(referenciaAire.muestras));
  } else {
    Serial.println("MQ135: sin referencia valida. Mantener los 4 botones 10 s para calibrar.");
  }
}

void informarReferenciaAire(float lectura) {
  if (!referenciaAireValida) {
    Serial.println("MQ135: sin calibrar");
    return;
  }
  const float referencia = referenciaAire.promedioMilliAdc / 1000.0f;
  Serial.printf("MQ135: referencia=%.3f ADC; diferencia=%+.3f ADC; variacion=%+.1f %%\n",
                referencia, lectura - referencia, (lectura / referencia - 1.0f) * 100.0f);
  if (indicadorCalidad.listo())
    Serial.printf("Escala relativa MQ135: %u/1000 (0=azul, 1000=rojo)\n",
                  static_cast<unsigned int>(indicadorCalidad.indice()));
}

void actualizarCalidadAire() {
  if (calibracionAire.activa()) return; // La ventana toma sus propias muestras.
  const unsigned long ahora = millis();
  if (lecturaAireDisponible && ahora - ultimaLecturaAireMs < intervaloLecturaAireMs) return;
  ultimaLecturaAireMs = ahora;
  const int lectura = analogRead(Particulas);
  if (lectura < 0 || lectura > AirbitCal::adcMaximo) return;
  particulas = lectura;
  lecturaAireDisponible = true;
  if (referenciaAireValida) {
    indicadorCalidad.recibir(AirbitCalidad::indiceDesdeAdc(
        static_cast<uint16_t>(lectura), referenciaAire.promedioMilliAdc));
  } else {
    indicadorCalidad.reiniciar(); // Sin referencia, no mostrar una calidad inventada.
  }
}

void iniciarCalibracionAire() {
  limpiarOlas();
  calibracionAire.iniciar(millis());
  ultimoInformeCalibracionMs = millis();
  Serial.println("Calibracion MQ135 iniciada: 5 minutos en aire limpio, 1 lectura/s.");
  Serial.println("Puede soltar los botones. LED verdes respirando; guardar al terminar.");
}

void actualizarCalibracionAire() {
  if (!calibracionAire.activa()) return;
  const unsigned long ahora = millis();
  if (calibracionAire.termino(ahora)) {
    const AirbitCal::Registro nueva = calibracionAire.registro();
    if (!AirbitCal::registroValido(nueva)) {
      Serial.printf("Calibracion descartada: muestras=%u, min=%u, max=%u. Referencia anterior conservada.\n",
                    static_cast<unsigned int>(nueva.muestras),
                    static_cast<unsigned int>(nueva.minimo), static_cast<unsigned int>(nueva.maximo));
    } else {
      // Una escritura al completar la ventana; no guardar cada lectura.
      EEPROM.put(0, nueva);
      if (AirbitDebug::guardarFlash()) {
        referenciaAire = nueva;
        referenciaAireValida = true;
        Serial.printf("Calibracion guardada: promedio=%.3f ADC; muestras=%u; min=%u; max=%u\n",
                      nueva.promedioMilliAdc / 1000.0,
                      static_cast<unsigned int>(nueva.muestras),
                      static_cast<unsigned int>(nueva.minimo), static_cast<unsigned int>(nueva.maximo));
      } else {
        Serial.println("Error al guardar calibracion en flash. Referencia anterior conservada en RAM.");
      }
    }
    calibracionAire.detener();
    indicadorCalidad.reiniciar();
    lecturaAireDisponible = false;
    // El compositor recupera indicadores normales o envia negro al salir.
    indicadoresMostrados = true;
    return;
  }
  if (calibracionAire.tocaMuestra(ahora)) {
    const int lectura = analogRead(Particulas);
    if (lectura >= 0 && lectura <= AirbitCal::adcMaximo) {
      particulas = lectura;
      calibracionAire.agregar(ahora, static_cast<uint16_t>(lectura));
    }
  }
  if (ahora - ultimoInformeCalibracionMs >= 10000) {
    ultimoInformeCalibracionMs = ahora;
    Serial.printf("Calibrando MQ135: quedan %u s; muestras=%u; promedio=%.3f ADC\n",
                  static_cast<unsigned int>((calibracionAire.restanteMs(ahora) + 999) / 1000),
                  static_cast<unsigned int>(calibracionAire.muestras()),
                  calibracionAire.registro().promedioMilliAdc / 1000.0);
  }
}

void iniciarPulsadores() {
  const unsigned long ahora = millis();
  for (Pulsador& boton : pulsadores) {
    boton.lectura = digitalRead(boton.pin) == boton.nivelPulsado;
    boton.estable = boton.lectura;
    boton.listo = !boton.lectura; // Si arranca pulsado, esperar a soltarlo.
    boton.ultimoCambio = ahora;
  }
  uint8_t mascara = 0;
  for (uint8_t i = 0; i < 4; ++i)
    if (pulsadores[i].estable) mascara |= 1 << i;
  gestoCalibracion.inicializar(mascara);
}

void iniciarOla(uint8_t boton) {
  Ola& ola = olas[boton];
  if (!ola.activa) {
    ola.activa = true;
    ola.inicioMs = millis();
  } else if (ola.pendientes < maxOlasPendientes) {
    ++ola.pendientes; // Completar la actual antes de reproducir otra.
  } else {
    Serial.printf("Olas S %u: cola llena (%u pendientes)\n",
                  static_cast<unsigned int>(boton + 1),
                  static_cast<unsigned int>(maxOlasPendientes));
  }
  actualizarFrameOlas = true;
}

void procesarPulsacion(uint8_t boton) {
  Serial.printf("ON S %u\n", static_cast<unsigned int>(boton + 1));
  iniciarOla(boton);
  // Se conservan los controles del cooler en botones 1 y 4.
  if (boton == 0) {
    PWMcooler = PWMcooler >= 5 ? PWMcooler - 5 : 0;
    analogWrite(Cooler, PWMcooler);
  } else if (boton == 3) {
    PWMcooler = PWMcooler <= 250 ? PWMcooler + 5 : 255;
    analogWrite(Cooler, PWMcooler);
  }
}

void actualizarPulsadores() {
  const unsigned long ahora = millis();
  bool nuevasPulsaciones[4] = {};
  uint8_t mascara = 0;
  // Actualizar las cuatro entradas antes de decidir acciones o gesto conjunto.
  for (uint8_t i = 0; i < 4; ++i) {
    Pulsador& boton = pulsadores[i];
    const bool lectura = digitalRead(boton.pin) == boton.nivelPulsado;
    if (lectura != boton.lectura) {
      boton.lectura = lectura;
      boton.ultimoCambio = ahora;
    }
    if (ahora - boton.ultimoCambio >= antirreboteMs && boton.estable != boton.lectura) {
      boton.estable = boton.lectura;
      if (!boton.estable) {
        boton.listo = true;
      } else if (boton.listo) {
        boton.listo = false;
        nuevasPulsaciones[i] = true;
      }
    }
    if (boton.estable) mascara |= 1 << i;
  }
  const bool contandoAntes = gestoCalibracion.contando();
  const bool iniciar = gestoCalibracion.actualizar(mascara, ahora);
  if (!contandoAntes && gestoCalibracion.contando() && !calibracionAire.activa()) {
    limpiarOlas();
    Serial.println("Calibracion: 4 botones pulsados; mantener 10 s sin soltar.");
  }
  if (iniciar && !calibracionAire.activa()) iniciarCalibracionAire();
  if (calibracionAire.activa() || gestoCalibracion.bloqueaAcciones()) return;
  for (uint8_t i = 0; i < 4; ++i)
    if (nuevasPulsaciones[i]) procesarPulsacion(i);
}

void iniciarIndicadoresArranque() {
  inicioIndicadoresMs = millis();
  inicioGiroWiFiMs = inicioIndicadoresMs;
  wifiConectadoParaIndicador = false;
  indicadoresMostrados = true;
  FastLED.clear();
  MemLeds[15] = CRGB::Red; // LED fisico 16.
  FastLED.show();
}

void dibujarIndicadorWiFi(unsigned long ahora, uint8_t* cobertura) {
  const unsigned long transcurrido = ahora - inicioGiroWiFiMs;
  const unsigned long pasoActual = transcurrido / pasoOlaMs;
  const uint8_t cabeza = pasoActual % NumLeds;
  const unsigned long fraccionPaso = transcurrido % pasoOlaMs;
  // Un frente azul sigue el orden LED 1 -> 20 y vuelve a empezar.
  for (uint8_t distancia = 0; distancia < 3; ++distancia) {
    if (pasoActual < distancia) {
      continue; // No dibujar una estela anterior al primer paso del arranque.
    }
    const unsigned long edad = distancia * pasoOlaMs + fraccionPaso;
    if (edad >= estelaOlaMs) {
      continue;
    }
    CRGB color = CRGB::Blue;
    color.nscale8(255 - (edad * 255 / estelaOlaMs));
    const uint8_t led = (cabeza + NumLeds - distancia) % NumLeds;
    MemLeds[led] += color;
    const uint8_t intensidad = 255 - (edad * 255 / estelaOlaMs);
    if (intensidad > cobertura[led]) cobertura[led] = intensidad;
  }
}

void actualizarLeds() {
  if (pausarEnvioLedsDiagnostico) return; // Solo prueba serie: conservar el ultimo frame fisico.
  const unsigned long ahora = millis();
  if (calibracionAire.activa()) {
    if (ahora - ultimoFrameOlas < intervaloFrameOlaMs) return;
    ultimoFrameOlas = ahora;
    const uint8_t fase = static_cast<uint8_t>(
        ((ahora - calibracionAire.inicioMs()) % periodoRespiracionMs) * 256 / periodoRespiracionMs + 192);
    const uint8_t intensidad = 16 + static_cast<uint16_t>(sin8(fase)) * 239 / 255;
    fill_solid(MemLeds, NumLeds, CRGB(0, intensidad, 0));
    FastLED.show();
    indicadoresMostrados = true;
    return; // Verde exclusivo durante la calibracion.
  }
  const bool rojoArranque = ahora - inicioIndicadoresMs < duracionRojoArranqueMs;
  const bool giroWiFi = indicadorWiFiActivo(ahora);
  const bool fondoManual = iluminacionActual.manual;
  const bool fondoCalidad = !fondoManual && referenciaAireValida && indicadorCalidad.listo();
  const bool hayFondo = fondoManual || fondoCalidad;
  if (!actualizarFrameOlas && !rojoArranque && !giroWiFi && !hayFondo) {
    // Apagar al terminar los indicadores y reforzar negro cada 100 ms.
    if (indicadoresMostrados || ahora - ultimoFrameOlas >= intervaloApagadoLedsMs) {
      ultimoFrameOlas = ahora;
      FastLED.clear(true);
      indicadoresMostrados = false;
    }
    return;
  }
  if (ahora - ultimoFrameOlas < intervaloFrameOlaMs) {
    return;
  }
  const unsigned long transcurridoFrame = ahora - ultimoFrameOlas;
  ultimoFrameOlas = ahora;
  indicadorCalidad.avanzar(transcurridoFrame);
  FastLED.clear();
  uint8_t coberturaOlas[NumLeds] = {};
  if (giroWiFi) {
    dibujarIndicadorWiFi(ahora, coberturaOlas);
  }
  bool hayOlas = false;
  for (uint8_t boton = 0; boton < 4; ++boton) {
    Ola& ola = olas[boton];
    if (!ola.activa) {
      continue;
    }
    unsigned long transcurrido = ahora - ola.inicioMs;
    if (transcurrido >= duracionOlaMs) {
      if (ola.pendientes == 0) {
        ola.activa = false;
        continue;
      }
      --ola.pendientes;
      ola.inicioMs = ahora;
      transcurrido = 0; // Cada efecto pendiente empieza completo.
    }
    hayOlas = true;
    for (uint8_t paso = 0; paso < 5; ++paso) {
      const unsigned long comienzaLed = paso * pasoOlaMs;
      if (transcurrido < comienzaLed) {
        continue;
      }
      const unsigned long edad = transcurrido - comienzaLed;
      if (edad >= estelaOlaMs) {
        continue;
      }
      const uint8_t intensidad = 255 - (edad * 255 / estelaOlaMs);
      CRGB color = coloresOlas[boton];
      color.nscale8(intensidad); // Escalado lineal: permite llegar a negro.
      for (uint8_t lado = 0; lado < 2; ++lado) {
        const uint8_t led = recorridoOlas[boton][lado][paso];
        MemLeds[led] += color;
        if (intensidad > coberturaOlas[led]) coberturaOlas[led] = intensidad;
      }
    }
  }
  if (hayFondo) {
    CRGB fondo;
    if (fondoManual) {
      const AirbitLuz::Color rgb = AirbitLuz::color(iluminacionActual);
      fondo = CRGB(rgb.r, rgb.g, rgb.b);
    } else {
      const AirbitCalidad::Color rgb = indicadorCalidad.color();
      fondo = CRGB(rgb.r, rgb.g, rgb.b);
      fondo.nscale8(AirbitIntensidad::brillo(intensidadCalidadActual));
    }
    for (uint8_t led = 0; led < NumLeds; ++led) {
      CRGB aporte = fondo;
      // El frente reemplaza el fondo; su estela lo recupera gradualmente.
      aporte.nscale8(255 - coberturaOlas[led]);
      MemLeds[led] += aporte;
    }
  }
  // Durante los primeros 5 s, el LED 16 permanece rojo puro.
  if (rojoArranque) {
    MemLeds[15] = CRGB::Red;
  }
  if (hayOlas || rojoArranque || giroWiFi || hayFondo) {
    FastLED.show();
  } else {
    FastLED.clear(true);
  }
  actualizarFrameOlas = hayOlas; // Solo las olas de botones, no el giro Wi-Fi.
  indicadoresMostrados = rojoArranque || giroWiFi || hayFondo;
}

#include "portal_configuracion.h"

void setup() {

  pinMode(Particulas, INPUT);
  pinMode(Cooler, OUTPUT);
  analogWriteRange(255); // Rango explicito usado por PWMcooler (0..255).
  pinMode(BTN1, INPUT);
  pinMode(BTN2, INPUT);
  pinMode(BTN3, INPUT_PULLUP);
  pinMode(BTN4, INPUT);
  iniciarPulsadores();

  Serial.begin(115200);
  AirbitDebug::iniciar();
  cargarReferenciaAire();
  FastLED.addLeds<WS2812B, Leds>(MemLeds, NumLeds);
  FastLED.setBrightness(brillo);
  iniciarIndicadoresArranque();
  bmp.begin();

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

  Serial.print("MQ135: ");
  Serial.print(analogRead(Particulas));
  Serial.println(" (lectura del adc)");

  Serial.print("Luces: ");

  Serial.println(" Configuradas");

  Serial.print("Cooler: ");
  analogWrite(Cooler, PWMcooler);
  Serial.println("Encendido");


  //******************* WIFI **********************
  WiFi.persistent(false);
  WiFi.mode(WIFI_AP_STA);
  WiFi.hostname("Airbit");
  WiFi.setAutoReconnect(false); // Los reintentos los gestiona el loop, protegiendo el AP.
  AirbitDebug::comandoRed = controlarRedDesdeSerie;
  eventoIPWiFi = WiFi.onStationModeGotIP([](const WiFiEventStationModeGotIP&) {
    solicitarRecuperacionPortal();
  });
  eventoEntradaAP = WiFi.onSoftAPModeStationConnected([](const WiFiEventSoftAPModeStationConnected& e) {
    Serial.printf_P(PSTR("[DBG AP] Cliente asociado aid=%u canal=%u\n"), e.aid, WiFi.channel());
  });
  eventoSalidaAP = WiFi.onSoftAPModeStationDisconnected([](const WiFiEventSoftAPModeStationDisconnected& e) {
    ultimoClienteAPMs = millis(); huboClienteAP = true;
    Serial.printf_P(PSTR("[DBG AP] Cliente desconectado aid=%u canal=%u\n"), e.aid, WiFi.channel());
  });
  eventoDesconexionWiFi = WiFi.onStationModeDisconnected([](const WiFiEventStationModeDisconnected& evento) {
    AirbitDebug::registrarDesconexion(static_cast<uint8_t>(evento.reason));
    solicitarRecuperacionPortal();
    Serial.printf("WiFi desconectado: %s (motivo SDK=%u)\n",
                  descripcionMotivoWiFi(static_cast<uint8_t>(evento.reason)),
                  static_cast<unsigned int>(evento.reason));
  });
  iniciarPortalConfiguracion();
  iniciarConexionWiFi();
  Serial.println("WiFi: conexion en segundo plano; reintento solo con AP sin clientes");

  Serial.println("Arranque: LED 16 rojo durante 5 s; ola azul mientras conecta");

  // El loop supervisa la conexion; no se reinicia por un fallo Wi-Fi.
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
  Serial.println("Airbit listo; sensores y pulsadores activos aunque no haya WiFi");
  AirbitDebug::marcar(AirbitDebug::Etapa::Listo);



  //******************* BOOT ERROR **********************
  // Los pulsadores se arman tras soltar cualquier tecla pulsada al arrancar.

  tiempoMedicion = millis();
  gestionarWiFi();
  actualizarLeds();

}

void loop() {
  AirbitDebug::procesar();

  gestionarWiFi();

  //ArduinoOTA.handle();

  actualizarPulsadores();
  actualizarCalibracionAire();
  actualizarCalidadAire();
  actualizarLeds();
  delay(1); // Cede tiempo al ESP8266 sin frenar la animacion.
  procesarPortalConfiguracion();

  // El giro posterior a la conexion tampoco debe cortarse por tareas lentas.
  // Sin WiFi se mantienen las mediciones, aunque el indicador siga girando.
  const bool giroTrasConexion = wifiConectadoParaIndicador && indicadorWiFiActivo(millis());
  const bool tareasLentasPermitidas = !actualizarFrameOlas && !giroTrasConexion &&
                                    !calibracionAire.activa() && !gestoCalibracion.bloqueaAcciones();

  // Periodo de medición
  //5min = 1000*60*5 = 300000
  // Las conversiones BMP son sincronas: medir cuando haya terminado la ola.
  if (tareasLentasPermitidas && millis() - tiempoMedicion >= 5000) {
    AirbitDebug::Ambito diagnosticoBMP(AirbitDebug::Etapa::BMP);

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

    Serial.print("MQ135: ");
    // Usar la ultima muestra de 1 Hz; no volver a leer A0 en este bloque.
    Serial.print(particulas);
    Serial.println(" (lectura del adc)");
    informarReferenciaAire(particulas);

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
  const unsigned long transcurridoDB = millis() - tiempoDB;
  // El cooler cumple su ciclo aunque se posponga la peticion HTTPS.
  if (transcurridoDB >= 600000) {
    tiempoDB = millis();
    envioDBPendiente = true;
    PWMcooler = 0;
    analogWrite(Cooler, PWMcooler);
  } else if (transcurridoDB >= (600000 - 60000)) {
    PWMcooler = 255;
    analogWrite(Cooler, PWMcooler);
  }
  // GET es sincrono: no iniciar el envio mientras quede alguna ola activa.
  // Posponer TLS mientras se usa el portal; conserva el ciclo del cooler.
  if (tareasLentasPermitidas && envioDBPendiente && !portalEnUso()) {
    envioDBPendiente = false;

    if (WiFi.status() != WL_CONNECTED) {
      Serial.println("Sin WiFi: se omite el envio de este ciclo al formulario");
      return;
    }

    if (!dbAirbit[0]) {
      Serial.println(F("HTTPS: no hay destino configurado; se omite el envio"));
      return;
    }
    AirbitDebug::Ambito diagnosticoHTTPS(AirbitDebug::Etapa::HTTPS);
    String direccion =  String(dbAirbit) +
                        "?usp=pp_url"    +
                        idAirbit.PSN  + presion +
                        idAirbit.TMP  + temperatura +
                        idAirbit.ATT  + altitud +
                        idAirbit.PSM  + presionMar +
                        idAirbit.ALR  + alturaReal +
                        idAirbit.PRT  + particulas +
                        "&submit=Submit";

    Serial.println("HTTPS: envio de mediciones (URL omitida del registro)");

    //Inicia transferencia de información
    std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);
    client->setInsecure();
    client->setTimeout(1500);
    HTTPClient https;
    https.setTimeout(1500);

    if (https.begin(*client, direccion)) {  // HTTPS
      // start connection and send HTTP header
      int httpCode;
      {
        AirbitDebug::Ambito diagnosticoGET(AirbitDebug::Etapa::HTTPSGet);
        httpCode = https.GET();
        Serial.printf_P(PSTR("[DBG %10lu] HTTPS codigo=%d\n"), millis(), httpCode);
      }
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
