#pragma once
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <EEPROM.h>
#include "diagnostico_registro.h"
#include "version_airbit.h"

namespace AirbitDebug {
Registro huella={}, anterior={};
bool anteriorValido=false, iniciado=false, detallado=false, avisoRtc=false;
Etapa etapa=Etapa::Inicio, ultimaTerminada=Etapa::Inicio;
uint32_t minimoHeap=UINT32_MAX, maximoHttpMs=0, maximoLoopMs=0;
uint32_t ultimoLoopMs=0, ultimoInformeMs=0, bytesRespuesta=0;
int codigoRespuesta=0;
volatile uint32_t desconexiones=0;
volatile uint8_t motivoDesconexion=0;
void (*comandoRed)(char) = nullptr;

inline void memoria(uint32_t& libre,uint32_t& bloque,uint8_t& fragmentacion) {
  ESP.getHeapStats(&libre,&bloque,&fragmentacion);
  if(libre<minimoHeap)minimoHeap=libre;
}
inline void marcar(Etapa siguiente) {
  etapa=siguiente;
  if(!iniciado)return;
  uint32_t libre,bloque;uint8_t frag;
  memoria(libre,bloque,frag);
  huella.ms=millis();huella.etapa=static_cast<uint32_t>(etapa);
  huella.heap=libre;huella.bloque=bloque;huella.minimo=minimoHeap;
  sellar(huella);
  if(!ESP.rtcUserMemoryWrite(palabraRtc,reinterpret_cast<uint32_t*>(&huella),sizeof(huella))&&!avisoRtc){
    avisoRtc=true;Serial.println(F("[DBG] No se pudo escribir la huella RTC."));
  }
}
inline void imprimirHuella() {
  if(!anteriorValido){Serial.println(F("[DBG ANTERIOR] Sin huella RTC valida; no hay historial anterior disponible."));return;}
  Serial.printf_P(PSTR("[DBG ANTERIOR] arranque=%lu uptime=%lu ms etapa=%s req=%lu %s heap=%lu bloque=%lu min=%lu peticiones=%lu\n"),
    static_cast<unsigned long>(anterior.arranque),static_cast<unsigned long>(anterior.ms),
    nombre(static_cast<Etapa>(anterior.etapa)),static_cast<unsigned long>(anterior.solicitud),
    anterior.post?"POST":"GET",static_cast<unsigned long>(anterior.heap),static_cast<unsigned long>(anterior.bloque),
    static_cast<unsigned long>(anterior.minimo),static_cast<unsigned long>(anterior.peticiones));
}
inline void informar() {
  uint32_t libre,bloque;uint8_t frag;
  memoria(libre,bloque,frag);
  Serial.printf_P(PSTR("[DBG %10lu] ESTADO heap=%lu min=%lu bloque=%lu frag=%u%% pila=%lu wifi=%d rssi=%d APclientes=%u canal=%d HTTP=%lu maxHTTP=%lu ms maxLoop=%lu ms desc=%lu motivo=%u\n"),
    millis(),static_cast<unsigned long>(libre),static_cast<unsigned long>(minimoHeap),
    static_cast<unsigned long>(bloque),frag,static_cast<unsigned long>(ESP.getFreeContStack()),
    static_cast<int>(WiFi.status()),WiFi.status()==WL_CONNECTED?WiFi.RSSI():0,
    WiFi.softAPgetStationNum(),WiFi.channel(),static_cast<unsigned long>(huella.peticiones),
    static_cast<unsigned long>(maximoHttpMs),static_cast<unsigned long>(maximoLoopMs),
    static_cast<unsigned long>(desconexiones),motivoDesconexion);
}
inline void ayuda() {
  Serial.println(F("[DBG] Comandos serie: d = diagnostico; v = detalle HTTP; a = solo AP temporal; w = reanudar WiFi; l = pausar/reanudar LED; n = sockets/red; h = ayuda. Enviar sin comillas."));
}
inline void iniciar() {
  anteriorValido=ESP.rtcUserMemoryRead(palabraRtc,reinterpret_cast<uint32_t*>(&anterior),sizeof(anterior))&&valido(anterior);
  huella={};huella.arranque=anteriorValido?anterior.arranque+1:1;
  if(!huella.arranque)huella.arranque=1;
  iniciado=true;
  Serial.println(F("\n[DBG BOOT] Airbit diagnostico serie 1; firmware v" AIRBIT_FIRMWARE_VERSION "; compilado " __DATE__ " " __TIME__));
  const rst_info* reset=ESP.getResetInfoPtr();
  Serial.printf_P(PSTR("[DBG BOOT] razon=%s codigo=%lu arranqueRTC=%lu\n"),ESP.getResetReason().c_str(),
    static_cast<unsigned long>(reset?reset->reason:0),static_cast<unsigned long>(huella.arranque));
  Serial.printf_P(PSTR("[DBG BOOT] %s\n"),ESP.getResetInfo().c_str());
  imprimirHuella();marcar(Etapa::Inicio);ayuda();
}
inline void respuesta(int codigo,size_t bytes) {
  codigoRespuesta=codigo;bytesRespuesta=bytes;
  marcar(etapa);
  if(detallado||etapa!=Etapa::Estado||codigo>=400)
    Serial.printf_P(PSTR("[DBG %10lu] ENVIO #%lu codigo=%d bytes=%u heap=%lu\n"),millis(),
      static_cast<unsigned long>(huella.solicitud),codigo,static_cast<unsigned>(bytes),static_cast<unsigned long>(huella.heap));
}
class Ambito {
 public:
  explicit Ambito(Etapa nueva,bool post=false):anterior_(etapa),actual_(nueva),inicio_(millis()),
    heapInicio_(ESP.getFreeHeap()),visible_(detallado||nueva!=Etapa::Estado) {
    if(esWeb(nueva)){
      ++huella.peticiones;huella.solicitud=huella.peticiones;huella.post=post;
      codigoRespuesta=0;bytesRespuesta=0;
    }
    marcar(nueva);
    if(visible_)Serial.printf_P(PSTR("[DBG %10lu] INICIO %s #%lu %s heap=%lu bloque=%lu\n"),millis(),
      nombre(nueva),static_cast<unsigned long>(huella.solicitud),esWeb(nueva)?(post?"POST":"GET"):"TAREA",
      static_cast<unsigned long>(huella.heap),static_cast<unsigned long>(huella.bloque));
  }
  ~Ambito() {
    const uint32_t duracion=millis()-inicio_;
    const uint32_t libre=ESP.getFreeHeap();
    if(libre<minimoHeap)minimoHeap=libre;
    if(esWeb(actual_)&&duracion>maximoHttpMs)maximoHttpMs=duracion;
    if(visible_||duracion>=250||(esWeb(actual_)&&codigoRespuesta>=400))
      Serial.printf_P(PSTR("[DBG %10lu] FIN %s #%lu codigo=%d bytes=%lu dt=%lu ms heap=%lu delta=%ld\n"),millis(),
        nombre(actual_),static_cast<unsigned long>(huella.solicitud),esWeb(actual_)?codigoRespuesta:0,
        static_cast<unsigned long>(esWeb(actual_)?bytesRespuesta:0),static_cast<unsigned long>(duracion),
        static_cast<unsigned long>(libre),static_cast<long>(libre)-static_cast<long>(heapInicio_));
    ultimaTerminada=actual_;marcar(anterior_);
  }
  Ambito(const Ambito&)=delete;
  Ambito& operator=(const Ambito&)=delete;
 private:
  Etapa anterior_,actual_;
  uint32_t inicio_,heapInicio_;
  bool visible_;
};
inline bool guardarFlash() {
  Ambito escritura(Etapa::Flash);
  const bool ok=EEPROM.commit();
  if(!ok)Serial.println(F("[DBG] ERROR EEPROM.commit: no se guardo."));
  return ok;
}
inline void registrarDesconexion(uint8_t motivo) { ++desconexiones;motivoDesconexion=motivo; }
inline void procesar() {
  const uint32_t ahora=millis();
  if(ultimoLoopMs){
    const uint32_t pausa=ahora-ultimoLoopMs;
    if(pausa>maximoLoopMs)maximoLoopMs=pausa;
    if(pausa>=500)Serial.printf_P(PSTR("[DBG %10lu] LOOP_PAUSA %lu ms ultima=%s\n"),millis(),
      static_cast<unsigned long>(pausa),nombre(ultimaTerminada));
  }
  ultimoLoopMs=ahora;
  for(uint8_t n=0;n<16&&Serial.available();++n){
    const char c=Serial.read();
    if(c=='d'||c=='D'){
      Serial.printf_P(PSTR("[DBG RESET ANTERIOR] razon=%s; %s\n"),ESP.getResetReason().c_str(),ESP.getResetInfo().c_str());
      imprimirHuella();informar();
    }else if(c=='v'||c=='V'){
      detallado=!detallado;Serial.printf_P(PSTR("[DBG] Detalle HTTP=%s\n"),detallado?"ON":"OFF");
    }else if(c=='a'||c=='A'||c=='w'||c=='W'||c=='l'||c=='L'||c=='n'||c=='N'){
      if(comandoRed)comandoRed(c);
    }else if(c=='h'||c=='H'||c=='?')ayuda();
  }
  if(ahora-ultimoInformeMs>=5000){ultimoInformeMs=ahora;marcar(etapa);informar();}
}
}
