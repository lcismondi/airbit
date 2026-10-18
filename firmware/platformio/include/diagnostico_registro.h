#pragma once
#include <stddef.h>
#include <stdint.h>
#include "calibracion_aire.h"

namespace AirbitDebug {
enum class Etapa : uint32_t {
  Inicio, Listo, Pagina, Estado, WiFi, Calibracion, Iluminacion, Calidad,
  Redireccion, NoEncontrado, Flash, BMP, HTTPS, HTTPSGet, Favicon
};
constexpr uint32_t marcaRegistro = 0x41424447;
constexpr uint32_t palabraRtc = 32; // Los primeros 128 bytes los reserva el core para OTA.
struct Registro {
  uint32_t marca, version, arranque, ms, etapa, solicitud, post;
  uint32_t heap, bloque, minimo, peticiones, crc;
};
static_assert(sizeof(Registro) == 48 && offsetof(Registro, crc) == 44, "Huella RTC estable y alineada");
static_assert(palabraRtc * 4 + sizeof(Registro) <= 512, "Huella dentro de RTC de usuario");
constexpr uint32_t crcRegistro(const Registro& r) {
  uint32_t crc = 0xffffffffUL;
  const uint32_t palabras[11] = {r.marca,r.version,r.arranque,r.ms,r.etapa,r.solicitud,
    r.post,r.heap,r.bloque,r.minimo,r.peticiones};
  for (uint32_t palabra : palabras) for (uint8_t i = 0; i < 4; ++i)
    crc = AirbitCal::crcByte(crc, static_cast<uint8_t>(palabra >> (8 * i)));
  return crc ^ 0xffffffffUL;
}
constexpr void sellar(Registro& r) { r.marca=marcaRegistro;r.version=1;r.crc=crcRegistro(r); }
constexpr bool valido(const Registro& r) {
  return r.marca==marcaRegistro && r.version==1 && r.arranque>0 &&
    r.etapa<=static_cast<uint32_t>(Etapa::Favicon) && r.post<=1 && r.crc==crcRegistro(r);
}
constexpr bool esWeb(Etapa e) {
  return (e>=Etapa::Pagina && e<=Etapa::NoEncontrado) || e==Etapa::Favicon;
}
inline const char* nombre(Etapa e) {
  switch(e) {
    case Etapa::Inicio:return "ARRANQUE";
    case Etapa::Listo:return "LOOP";
    case Etapa::Pagina:return "/";
    case Etapa::Estado:return "/api/estado";
    case Etapa::WiFi:return "/api/wifi";
    case Etapa::Calibracion:return "/api/calibracion";
    case Etapa::Iluminacion:return "/api/iluminacion";
    case Etapa::Calidad:return "/api/calidad";
    case Etapa::Redireccion:return "PORTAL_CAUTIVO";
    case Etapa::NoEncontrado:return "RUTA_DESCONOCIDA";
    case Etapa::Flash:return "EEPROM.commit";
    case Etapa::BMP:return "SENSORES_BMP";
    case Etapa::HTTPS:return "ENVIO_HTTPS";
    case Etapa::HTTPSGet:return "HTTPS.GET";
    case Etapa::Favicon:return "/favicon.ico";
  }
  return "DESCONOCIDA";
}
}
