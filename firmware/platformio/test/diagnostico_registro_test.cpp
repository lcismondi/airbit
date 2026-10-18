#include "diagnostico_registro.h"
using namespace AirbitDebug;
constexpr Registro muestra() {
  Registro r={};r.arranque=2;r.ms=12345;r.etapa=static_cast<uint32_t>(Etapa::Iluminacion);
  r.solicitud=19;r.post=1;r.heap=24000;r.bloque=18000;r.minimo=23000;r.peticiones=19;sellar(r);return r;
}
constexpr bool corrupcion() {
  const Registro original=muestra();
  for(uint8_t i=0;i<12;++i){
    Registro r=original;
    switch(i){
      case 0:r.marca^=1;break;case 1:r.version^=1;break;case 2:r.arranque^=1;break;
      case 3:r.ms^=1;break;case 4:r.etapa^=1;break;case 5:r.solicitud^=1;break;
      case 6:r.post^=1;break;case 7:r.heap^=1;break;case 8:r.bloque^=1;break;
      case 9:r.minimo^=1;break;case 10:r.peticiones^=1;break;case 11:r.crc^=1;break;
    }
    if(valido(r))return false;
  }
  return true;
}
constexpr bool limites() {
  Registro r=muestra();r.arranque=0;sellar(r);if(valido(r))return false;
  r=muestra();r.post=2;sellar(r);if(valido(r))return false;
  r=muestra();r.etapa=99;sellar(r);if(valido(r))return false;
  r=muestra();r.version=2;r.crc=crcRegistro(r);return !valido(r);
}
constexpr bool aritmeticaTiempo() { return static_cast<uint32_t>(20U-0xfffffff0U)==36U; }
static_assert(valido(muestra()),"Huella valida tras sellar");
static_assert(!valido(Registro{}),"RTC vacia no es un historial valido");
static_assert(corrupcion(),"Se detecta corrupcion en todos los campos");
static_assert(limites(),"Rechazar formatos, etapas y metodos invalidos aunque su CRC coincida");
static_assert(esWeb(Etapa::Pagina)&&esWeb(Etapa::Estado)&&esWeb(Etapa::NoEncontrado)&&esWeb(Etapa::Favicon)&&!esWeb(Etapa::Flash)&&!esWeb(Etapa::HTTPSGet),"Clasificacion de operaciones");
static_assert(palabraRtc>=32&&palabraRtc*4+sizeof(Registro)<=512,"No solapar reserva OTA");
static_assert(aritmeticaTiempo(),"Duraciones correctas al desbordar millis");
