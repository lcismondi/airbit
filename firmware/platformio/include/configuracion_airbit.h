#pragma once
#include <stdint.h>
#include <stddef.h>
#include "calibracion_aire.h"

namespace AirbitConfig {
constexpr uint16_t direccion = 64;
constexpr uint16_t bytesEeprom = 256;
constexpr uint32_t marca = 0x41424647;
constexpr char claveApPredeterminada[] = "1234567890";
struct Registro {
  uint32_t identificador;
  uint16_t version;
  uint16_t bytes;
  uint8_t wifiPersonalizado;
  uint8_t reservado[3];
  char ssid[33];
  char claveWiFi[65];
  char claveAP[17];
  uint8_t relleno;
  uint32_t crc;
};
static_assert(sizeof(Registro) == 132, "Registro de configuracion estable");
static_assert(offsetof(Registro, crc) == 128, "CRC tras los datos sin padding");
static_assert(direccion >= sizeof(AirbitCal::Registro), "No pisar la calibracion");
static_assert(direccion + sizeof(Registro) <= bytesEeprom, "Configuracion dentro de EEPROM");

constexpr size_t longitud(const char* s, size_t limite) {
  size_t n = 0;
  while (n < limite && s[n]) ++n;
  return n;
}
constexpr bool hexadecimal(char c) {
  return (c >= '0' && c <= '9') || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f');
}
constexpr bool claveWiFiValida(const char* clave) {
  const size_t n = longitud(clave, 65);
  if (n == 0) return true;
  if (n >= 8 && n <= 63) return true;
  if (n != 64) return false;
  for (size_t i = 0; i < n; ++i) if (!hexadecimal(clave[i])) return false;
  return true;
}
constexpr bool claveAPValida(const char* clave) {
  const size_t n = longitud(clave, 17);
  if (n < 8 || n > 16) return false;
  for (size_t i = 0; i < n; ++i)
    if (clave[i] < 32 || clave[i] > 126) return false;
  return true;
}
constexpr bool copiar(char* destino, size_t capacidad, const char* fuente) {
  const size_t n = longitud(fuente, capacidad);
  if (n >= capacidad) return false;
  for (size_t i = 0; i < capacidad; ++i) destino[i] = i < n ? fuente[i] : 0;
  return true;
}
constexpr uint32_t crcRegistro(const Registro& r) {
  uint32_t crc = 0xffffffffUL;
  const uint32_t cabecera[3] = {
    r.identificador, static_cast<uint32_t>(r.version) | (static_cast<uint32_t>(r.bytes) << 16),
    static_cast<uint32_t>(r.wifiPersonalizado) | (static_cast<uint32_t>(r.reservado[0]) << 8) |
    (static_cast<uint32_t>(r.reservado[1]) << 16) | (static_cast<uint32_t>(r.reservado[2]) << 24)
  };
  for (uint8_t i = 0; i < 3; ++i)
    for (uint8_t b = 0; b < 4; ++b)
      crc = AirbitCal::crcByte(crc, static_cast<uint8_t>(cabecera[i] >> (b * 8)));
  for (char c : r.ssid) crc = AirbitCal::crcByte(crc, static_cast<uint8_t>(c));
  for (char c : r.claveWiFi) crc = AirbitCal::crcByte(crc, static_cast<uint8_t>(c));
  for (char c : r.claveAP) crc = AirbitCal::crcByte(crc, static_cast<uint8_t>(c));
  crc = AirbitCal::crcByte(crc, r.relleno);
  return crc ^ 0xffffffffUL;
}
constexpr void sellar(Registro& r) {
  r.identificador = marca;
  r.version = 1;
  r.bytes = sizeof(Registro);
  r.crc = crcRegistro(r);
}
constexpr bool valido(const Registro& r) {
  if (r.identificador != marca || r.version != 1 || r.bytes != sizeof(Registro) ||
      r.wifiPersonalizado > 1 || r.relleno != 0) return false;
  for (uint8_t c : r.reservado) if (c != 0) return false;
  const size_t ssidLen = longitud(r.ssid, sizeof(r.ssid));
  if (ssidLen >= sizeof(r.ssid) || (r.wifiPersonalizado && ssidLen == 0)) return false;
  if (!claveWiFiValida(r.claveWiFi) || !claveAPValida(r.claveAP)) return false;
  return r.crc == crcRegistro(r);
}
} // namespace AirbitConfig
