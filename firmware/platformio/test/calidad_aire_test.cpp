#include "calidad_aire.h"
using namespace AirbitCalidad;
constexpr bool igual(Color a, Color b) { return a.r == b.r && a.g == b.g && a.b == b.b; }
constexpr int diferencia(uint8_t a, uint8_t b) { return a > b ? a - b : b - a; }
constexpr bool paletaContinua() {
  for (uint16_t i = 1; i <= indiceMaximo; ++i) {
    const Color anterior = colorDesdeIndice(i - 1), actual = colorDesdeIndice(i);
    if (diferencia(anterior.r, actual.r) > 2 || diferencia(anterior.g, actual.g) > 2 ||
        diferencia(anterior.b, actual.b) > 2) return false;
  }
  return true;
}
constexpr bool puntos() {
  for (uint8_t i = 0; i < cantidadNiveles; ++i)
    if (!igual(colorDesdeIndice(i * 100), colores[i])) return false;
  return true;
}
constexpr bool lecturaRelativa() {
  return indiceDesdeAdc(0, 100000) == 0 && indiceDesdeAdc(100, 100000) == 0 &&
         indiceDesdeAdc(125, 100000) == 250 && indiceDesdeAdc(150, 100000) == 500 &&
         indiceDesdeAdc(175, 100000) == 750 && indiceDesdeAdc(200, 100000) == 1000 &&
         indiceDesdeAdc(500, 100000) == 1000;
}
constexpr bool limites() {
  return indiceDesdeAdc(1023, 1) == 1000 && indiceDesdeAdc(65535, 1) == 1000 &&
         indiceDesdeAdc(1023, 0) == 0 && igual(colorDesdeIndice(65535), colores[10]);
}
constexpr bool primeraLectura() {
  Indicador q;
  if (q.listo()) return false;
  q.recibir(500);
  return q.listo() && q.indice() == 500 && igual(q.color(), colores[5]);
}
constexpr bool escalonSuave() {
  Indicador q;
  q.recibir(0);
  q.recibir(1000);
  if (q.indice() != 0) return false;
  q.avanzar(20);
  return q.indice() > 0 && q.indice() < 10;
}
constexpr bool pausaAcotada() {
  Indicador a, b;
  a.recibir(0); a.recibir(1000);
  b.recibir(0); b.recibir(1000);
  a.avanzar(20000); b.avanzar(250);
  return a.indice() == b.indice() && a.indice() < 250;
}
constexpr bool convergencia() {
  Indicador q;
  q.recibir(0);
  uint16_t anterior = 0;
  for (uint16_t segundo = 0; segundo < 100; ++segundo) {
    q.recibir(1000);
    for (uint8_t frame = 0; frame < 50; ++frame) {
      q.avanzar(20);
      if (q.indice() < anterior) return false;
      anterior = q.indice();
    }
  }
  if (q.indice() != 1000 || !igual(q.color(), colores[10])) return false;
  for (uint16_t segundo = 0; segundo < 100; ++segundo) {
    q.recibir(0);
    for (uint8_t frame = 0; frame < 50; ++frame) {
      q.avanzar(20);
      if (q.indice() > anterior) return false;
      anterior = q.indice();
    }
  }
  return q.indice() == 0 && igual(q.color(), colores[0]);
}
constexpr bool nuevaReferencia() {
  Indicador q;
  q.recibir(1000);
  q.reiniciar();
  if (q.listo()) return false;
  q.recibir(0);
  return q.listo() && q.indice() == 0;
}
constexpr bool frameSinTiempo() {
  Indicador q;
  q.recibir(0); q.recibir(1000); q.avanzar(0);
  return q.indice() == 0;
}
static_assert(puntos(), "Los once colores exactos y su orden");
static_assert(paletaContinua(), "Sin saltos en los 1001 puntos del gradiente");
static_assert(lecturaRelativa(), "Umbrales relativos a la referencia de aire limpio");
static_assert(limites(), "Limites y referencia pequena sin overflow ni division por cero");
static_assert(primeraLectura(), "No hay color disponible antes de recibir una lectura");
static_assert(escalonSuave(), "Un salto del ADC se filtra y se muestra gradualmente");
static_assert(pausaAcotada(), "Una tarea lenta no provoca un salto completo de color");
static_assert(convergencia(), "Alcanza rojo/azul exactamente y sin sobrepasar el objetivo");
static_assert(nuevaReferencia(), "Recalibrar elimina la escala anterior");
static_assert(frameSinTiempo(), "No avanzar cuando no transcurrio tiempo");
static_assert(igual(colorDesdeIndice(125), Color{39,183,193}), "Color intermedio turquesa-esmeralda");
static_assert(igual(colorDesdeIndice(750), Color{243,128,49}), "Naranja intermedio exacto");

constexpr bool niveles() {
  return nivelDesdeIndice(0)==0 && nivelDesdeIndice(49)==0 && nivelDesdeIndice(50)==1 &&
    nivelDesdeIndice(500)==5 && nivelDesdeIndice(949)==9 && nivelDesdeIndice(950)==10 &&
    nivelDesdeIndice(1000)==10 && nivelDesdeIndice(65535)==10;
}
constexpr bool tablaSolicitada() {
  const Color esperados[11] = {{33,150,243},{38,182,212},{45,189,133},{101,196,80},{172,212,66},{242,212,58},{246,184,50},{247,147,50},{239,108,47},{230,73,50},{217,45,53}};
  for (uint8_t i=0;i<11;++i) if (!igual(colorDesdeIndice(i*100),esperados[i])) return false;
  return true;
}
static_assert(niveles(),"Estados por cercania con limites acotados");
static_assert(tablaSolicitada(),"Los once HEX solicitados exactos en cada punto");
