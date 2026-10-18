#pragma once

#include <stdint.h>
#include <stddef.h>

namespace AirbitCal {
constexpr uint32_t pulsacionMs = 10000;
constexpr uint32_t duracionMs = 300000;
constexpr uint32_t intervaloMuestraMs = 1000;
constexpr uint32_t muestrasEsperadas = duracionMs / intervaloMuestraMs;
constexpr uint32_t muestrasMinimas = muestrasEsperadas * 9 / 10;
constexpr uint16_t adcMaximo = 1023;

// Solo genera un evento por gesto; requiere soltar todos para rearmarse.
class Gesto {
 public:
  constexpr void inicializar(uint8_t mascara) {
    esperandoSoltar_ = mascara != 0;
    contando_ = false;
  }
  constexpr bool actualizar(uint8_t mascara, uint32_t ahora) {
    if (mascara == 0) {
      esperandoSoltar_ = false;
      contando_ = false;
      return false;
    }
    if (esperandoSoltar_) return false;
    if (mascara != 0x0f) {
      if (contando_) esperandoSoltar_ = true;
      contando_ = false;
      return false;
    }
    if (!contando_) {
      contando_ = true;
      inicioMs_ = ahora;
      return false;
    }
    if (static_cast<uint32_t>(ahora - inicioMs_) < pulsacionMs) return false;
    contando_ = false;
    esperandoSoltar_ = true;
    return true;
  }
  constexpr bool contando() const { return contando_; }
  constexpr bool bloqueaAcciones() const { return contando_ || esperandoSoltar_; }
 private:
  bool esperandoSoltar_ = false;
  bool contando_ = false;
  uint32_t inicioMs_ = 0;
};

struct Registro {
  uint32_t marca;
  uint16_t version;
  uint16_t bytes;
  uint32_t promedioMilliAdc;
  uint32_t muestras;
  uint32_t suma;
  uint16_t minimo;
  uint16_t maximo;
  uint32_t duracion;
  uint32_t crc;
};
static_assert(sizeof(Registro) == 32, "Formato EEPROM de 32 bytes");
static_assert(offsetof(Registro, crc) == 28, "CRC al final del registro");
constexpr uint32_t marcaRegistro = 0x41495242; // AIRB

constexpr uint32_t crcByte(uint32_t crc, uint8_t dato) {
  crc ^= dato;
  for (uint8_t bit = 0; bit < 8; ++bit)
    crc = (crc >> 1) ^ ((crc & 1) ? 0xedb88320UL : 0);
  return crc;
}
constexpr uint32_t crcRegistro(const Registro& r) {
  const uint32_t palabras[7] = {
    r.marca, static_cast<uint32_t>(r.version) | (static_cast<uint32_t>(r.bytes) << 16),
    r.promedioMilliAdc, r.muestras, r.suma,
    static_cast<uint32_t>(r.minimo) | (static_cast<uint32_t>(r.maximo) << 16), r.duracion
  };
  uint32_t crc = 0xffffffffUL;
  for (uint8_t i = 0; i < 7; ++i)
    for (uint8_t b = 0; b < 4; ++b)
      crc = crcByte(crc, static_cast<uint8_t>(palabras[i] >> (8 * b)));
  return crc ^ 0xffffffffUL;
}
constexpr bool registroValido(const Registro& r) {
  if (r.marca != marcaRegistro || r.version != 1 || r.bytes != sizeof(Registro) ||
      r.duracion != duracionMs || r.muestras < muestrasMinimas ||
      r.muestras > muestrasEsperadas || r.minimo > r.maximo ||
      r.maximo > adcMaximo || r.suma < r.muestras * r.minimo ||
      r.suma > r.muestras * r.maximo || r.promedioMilliAdc == 0 ||
      r.promedioMilliAdc >= static_cast<uint32_t>(adcMaximo) * 1000) return false;
  if (r.promedioMilliAdc != (r.suma * 1000 + r.muestras / 2) / r.muestras) return false;
  return r.crc == crcRegistro(r);
}

class Ventana {
 public:
  constexpr void iniciar(uint32_t ahora) {
    activa_ = true;
    inicioMs_ = ahora;
    ultimaMuestraMs_ = ahora - intervaloMuestraMs; // Primera lectura inmediata.
    suma_ = 0;
    muestras_ = 0;
    minimo_ = adcMaximo;
    maximo_ = 0;
  }
  constexpr bool activa() const { return activa_; }
  constexpr bool termino(uint32_t ahora) const {
    return activa_ && static_cast<uint32_t>(ahora - inicioMs_) >= duracionMs;
  }
  constexpr bool tocaMuestra(uint32_t ahora) const {
    return activa_ && !termino(ahora) &&
           static_cast<uint32_t>(ahora - ultimaMuestraMs_) >= intervaloMuestraMs;
  }
  constexpr bool agregar(uint32_t ahora, uint16_t lectura) {
    if (!tocaMuestra(ahora) || lectura > adcMaximo || muestras_ >= muestrasEsperadas)
      return false;
    ultimaMuestraMs_ = ahora;
    suma_ += lectura;
    ++muestras_;
    if (lectura < minimo_) minimo_ = lectura;
    if (lectura > maximo_) maximo_ = lectura;
    return true;
  }
  constexpr void detener() { activa_ = false; }
  constexpr uint32_t muestras() const { return muestras_; }
  constexpr uint32_t inicioMs() const { return inicioMs_; }
  constexpr uint32_t restanteMs(uint32_t ahora) const {
    const uint32_t transcurrido = ahora - inicioMs_;
    return transcurrido >= duracionMs ? 0 : duracionMs - transcurrido;
  }
  constexpr Registro registro() const {
    Registro r = {marcaRegistro, 1, sizeof(Registro),
                  muestras_ ? (suma_ * 1000 + muestras_ / 2) / muestras_ : 0,
                  muestras_, suma_, minimo_, maximo_, duracionMs, 0};
    r.crc = crcRegistro(r);
    return r;
  }
 private:
  bool activa_ = false;
  uint32_t inicioMs_ = 0;
  uint32_t ultimaMuestraMs_ = 0;
  uint32_t suma_ = 0;
  uint32_t muestras_ = 0;
  uint16_t minimo_ = adcMaximo;
  uint16_t maximo_ = 0;
};
} // namespace AirbitCal
