#pragma once
#include <stdint.h>
#include <stddef.h>
#include "configuracion_airbit.h"
namespace AirbitLuz {
constexpr uint16_t direccion = 196;
constexpr uint32_t marca = 0x41424c5a;
struct Registro {
  uint32_t identificador;
  uint16_t version, bytes;
  uint8_t manual, r, g, b, intensidad;
  uint8_t reservado[3];
  uint32_t crc;
};
struct Color { uint8_t r, g, b; };
static_assert(sizeof(Registro) == 20 && offsetof(Registro, crc) == 16, "Registro luz estable");
static_assert(direccion >= AirbitConfig::direccion + sizeof(AirbitConfig::Registro), "No pisar WiFi");
static_assert(direccion + sizeof(Registro) <= AirbitConfig::bytesEeprom, "Luz dentro de EEPROM");
constexpr uint32_t crcRegistro(const Registro& r) {
  uint32_t crc = 0xffffffffUL;
  const uint32_t palabras[4] = {r.identificador,
    static_cast<uint32_t>(r.version) | (static_cast<uint32_t>(r.bytes) << 16),
    static_cast<uint32_t>(r.manual) | (static_cast<uint32_t>(r.r) << 8) | (static_cast<uint32_t>(r.g) << 16) | (static_cast<uint32_t>(r.b) << 24),
    static_cast<uint32_t>(r.intensidad) | (static_cast<uint32_t>(r.reservado[0]) << 8) | (static_cast<uint32_t>(r.reservado[1]) << 16) | (static_cast<uint32_t>(r.reservado[2]) << 24)};
  for (uint32_t palabra : palabras) for (uint8_t b = 0; b < 4; ++b)
    crc = AirbitCal::crcByte(crc, static_cast<uint8_t>(palabra >> (b * 8)));
  return crc ^ 0xffffffffUL;
}
constexpr void sellar(Registro& r) { r.identificador = marca; r.version = 1; r.bytes = sizeof(Registro); r.crc = crcRegistro(r); }
constexpr Registro predeterminada() { Registro r = {}; r.r = r.g = r.b = 255; r.intensidad = 50; sellar(r); return r; }
constexpr bool valido(const Registro& r) {
  if (r.identificador != marca || r.version != 1 || r.bytes != sizeof(Registro) || r.manual > 1 || r.intensidad > 100) return false;
  for (uint8_t c : r.reservado) if (c) return false;
  return r.crc == crcRegistro(r);
}
constexpr uint8_t canal(uint8_t valor, uint8_t intensidad) { return static_cast<uint16_t>(valor) * intensidad / 100; }
constexpr Color color(const Registro& r) { return {canal(r.r,r.intensidad),canal(r.g,r.intensidad),canal(r.b,r.intensidad)}; }
}
