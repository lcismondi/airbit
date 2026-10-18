#pragma once
#include "iluminacion_airbit.h"
namespace AirbitIntensidad {
constexpr uint16_t direccion = 216;
constexpr uint32_t marca = 0x41424943;
struct Registro {
  uint32_t identificador;
  uint16_t version, bytes;
  uint8_t intensidad, reservado[3];
  uint32_t crc;
};
static_assert(sizeof(Registro) == 16 && offsetof(Registro, crc) == 12, "Registro intensidad estable");
static_assert(direccion >= AirbitLuz::direccion + sizeof(AirbitLuz::Registro), "No pisar luz manual");
static_assert(direccion + sizeof(Registro) <= AirbitConfig::bytesEeprom, "Intensidad dentro de EEPROM");
constexpr uint32_t crcRegistro(const Registro& r) {
  uint32_t crc = 0xffffffffUL;
  const uint32_t palabras[3] = {r.identificador,
    static_cast<uint32_t>(r.version) | (static_cast<uint32_t>(r.bytes) << 16),
    static_cast<uint32_t>(r.intensidad) | (static_cast<uint32_t>(r.reservado[0]) << 8) |
      (static_cast<uint32_t>(r.reservado[1]) << 16) | (static_cast<uint32_t>(r.reservado[2]) << 24)};
  for (uint32_t palabra : palabras) for (uint8_t b = 0; b < 4; ++b)
    crc = AirbitCal::crcByte(crc, static_cast<uint8_t>(palabra >> (b * 8)));
  return crc ^ 0xffffffffUL;
}
constexpr void sellar(Registro& r) { r.identificador = marca; r.version = 1; r.bytes = sizeof(Registro); r.crc = crcRegistro(r); }
constexpr Registro predeterminada() { Registro r = {}; r.intensidad = 25; sellar(r); return r; }
constexpr bool valido(const Registro& r) {
  if (r.identificador != marca || r.version != 1 || r.bytes != sizeof(Registro) || r.intensidad > 100) return false;
  for (uint8_t c : r.reservado) if (c) return false;
  return r.crc == crcRegistro(r);
}
// 25 % conserva el factor 64 del fondo anterior.
constexpr uint8_t brillo(const Registro& r) { return (static_cast<uint16_t>(r.intensidad) * 255 + 50) / 100; }
}
