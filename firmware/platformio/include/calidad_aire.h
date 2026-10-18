#pragma once
#include <stdint.h>

namespace AirbitCalidad {
// Escala relativa: 0 = ADC de referencia o inferior; 1000 = +100% o mas.
// Porcentajes de visualizacion ajustables, sin equivalencia a ppm/AQI.
constexpr uint16_t aumentoParaRojoPct = 100;
constexpr uint16_t indiceMaximo = 1000;
constexpr uint32_t tiempoTransicionMs = 2000;
constexpr uint16_t factorFiltroSensor = 4;
constexpr int32_t precision = 1024;
static_assert(aumentoParaRojoPct > 0 && aumentoParaRojoPct <= 1000, "El rango relativo debe ser positivo");
static_assert(factorFiltroSensor > 0, "Filtro valido");

struct Color { uint8_t r, g, b; };
constexpr uint8_t cantidadNiveles = 11;
constexpr uint16_t pasoColor = 100;
constexpr Color colores[cantidadNiveles] = {
  {33,150,243},
  {38,182,212},
  {45,189,133},
  {101,196,80},
  {172,212,66},
  {242,212,58},
  {246,184,50},
  {247,147,50},
  {239,108,47},
  {230,73,50},
  {217,45,53}
};
constexpr const char* estados[cantidadNiveles] = {
  "Aire de montaña",
  "Brisa fresca",
  "Brisa limpia",
  "Un respiro",
  "Aire tranquilo",
  "Brisa en pausa",
  "Se va cargando",
  "Ambiente cargadito",
  "Pide una ventana",
  "Toca renovar",
  "¡Un respiro, por favor!"
};
// Estado mas cercano al indice; colores continuos entre los once puntos.
constexpr uint8_t nivelDesdeIndice(uint16_t indice) {
  return indice >= indiceMaximo ? cantidadNiveles - 1 : (indice + pasoColor / 2) / pasoColor;
}
constexpr const char* estadoDesdeIndice(uint16_t indice) { return estados[nivelDesdeIndice(indice)]; }
constexpr uint16_t indiceDesdeAdc(uint16_t adc, uint32_t referenciaMilliAdc) {
  if (adc > 1023) adc = 1023;
  const uint32_t lecturaMilliAdc = static_cast<uint32_t>(adc) * 1000;
  if (referenciaMilliAdc == 0 || lecturaMilliAdc <= referenciaMilliAdc) return 0;
  const uint32_t aumento = lecturaMilliAdc - referenciaMilliAdc;
  // Calcular primero el exceso relativo en milesimas evita productos grandes.
  const uint32_t relativo = aumento * indiceMaximo / referenciaMilliAdc;
  if (relativo >= indiceMaximo * aumentoParaRojoPct / 100) return indiceMaximo;
  const uint32_t indice = relativo * 100 / aumentoParaRojoPct;
  return indice >= indiceMaximo ? indiceMaximo : static_cast<uint16_t>(indice);
}
constexpr uint8_t interpolar(uint8_t desde, uint8_t hasta, uint16_t fraccion) {
  return static_cast<uint8_t>(static_cast<int32_t>(desde) +
      (static_cast<int32_t>(hasta) - desde) * fraccion / pasoColor);
}
constexpr Color colorDesdeIndice(uint16_t indice) {
  if (indice >= indiceMaximo) return colores[cantidadNiveles - 1];
  const uint8_t tramo = indice / pasoColor;
  const uint16_t fraccion = indice % pasoColor;
  return {interpolar(colores[tramo].r, colores[tramo + 1].r, fraccion),
          interpolar(colores[tramo].g, colores[tramo + 1].g, fraccion),
          interpolar(colores[tramo].b, colores[tramo + 1].b, fraccion)};
}

class Indicador {
 public:
  constexpr void reiniciar() {
    listo_ = false;
    filtrado_ = 0;
    mostrado_ = 0;
  }
  constexpr void recibir(uint16_t indice) {
    const int32_t objetivo = (indice > indiceMaximo ? indiceMaximo : indice) * precision;
    if (!listo_) {
      filtrado_ = objetivo;
      mostrado_ = objetivo;
      listo_ = true;
    } else {
      const int32_t paso = (objetivo - filtrado_) / factorFiltroSensor;
      if (paso == 0) filtrado_ = objetivo;
      else filtrado_ += paso;
    }
  }
  constexpr void avanzar(uint32_t transcurridoMs) {
    if (!listo_ || transcurridoMs == 0) return;
    // Tras una tarea lenta, evitar que el color salte al destino de golpe.
    const uint32_t pasoMs = transcurridoMs > 250 ? 250 : transcurridoMs;
    const int32_t diferencia = filtrado_ - mostrado_;
    const int32_t paso = diferencia * static_cast<int32_t>(pasoMs) /
                         static_cast<int32_t>(tiempoTransicionMs + pasoMs);
    if (paso == 0) mostrado_ = filtrado_;
    else mostrado_ += paso;
  }
  constexpr bool listo() const { return listo_; }
  constexpr uint16_t indice() const { return static_cast<uint16_t>(mostrado_ / precision); }
  constexpr Color color() const { return colorDesdeIndice(indice()); }
 private:
  bool listo_ = false;
  int32_t filtrado_ = 0;
  int32_t mostrado_ = 0;
};
} // namespace AirbitCalidad
