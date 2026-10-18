// Pruebas del modulo real mediante evaluacion constexpr del compilador C++.
#include "calibracion_aire.h"
using namespace AirbitCal;

constexpr bool gestoCompleto() {
  Gesto g;
  g.inicializar(0);
  if (g.actualizar(15, 100) || !g.bloqueaAcciones()) return false;
  if (g.actualizar(15, 10099)) return false;
  if (!g.actualizar(15, 10100)) return false;
  if (g.actualizar(15, 600000) || !g.bloqueaAcciones()) return false;
  if (g.actualizar(7, 600001) || !g.bloqueaAcciones()) return false;
  g.actualizar(0, 600002);
  if (g.bloqueaAcciones()) return false;
  g.actualizar(15, 600003);
  return g.actualizar(15, 610003);
}
constexpr bool gestoInterrumpido() {
  Gesto g;
  g.inicializar(0);
  g.actualizar(15, 100);
  if (g.actualizar(7, 9000)) return false;
  if (g.actualizar(15, 20000)) return false;
  if (g.actualizar(15, 40000)) return false;
  g.actualizar(0, 40001);
  g.actualizar(15, 40002);
  return !g.actualizar(15, 50001) && g.actualizar(15, 50002);
}
constexpr bool arranquePulsado() {
  Gesto g;
  g.inicializar(15);
  if (g.actualizar(15, 20000)) return false;
  g.actualizar(0, 20001);
  g.actualizar(15, 20002);
  return g.actualizar(15, 30002);
}
constexpr bool relojDesbordado() {
  Gesto g;
  g.inicializar(0);
  const uint32_t inicio = 0xfffffff0UL;
  g.actualizar(15, inicio);
  if (!g.actualizar(15, inicio + pulsacionMs)) return false;
  Ventana v;
  v.iniciar(inicio);
  for (uint32_t i = 0; i < muestrasEsperadas; ++i)
    if (!v.agregar(inicio + i * intervaloMuestraMs, 75)) return false;
  return v.termino(inicio + duracionMs) && registroValido(v.registro());
}
constexpr Registro crearReferencia(uint16_t primera = 60, uint16_t segunda = 80) {
  Ventana v;
  v.iniciar(500);
  for (uint32_t i = 0; i < muestrasEsperadas; ++i)
    v.agregar(500 + i * intervaloMuestraMs, i % 2 ? segunda : primera);
  return v.registro();
}
constexpr bool muestreoYTiempo() {
  Ventana v;
  v.iniciar(100);
  if (!v.tocaMuestra(100) || !v.agregar(100, 60)) return false;
  if (v.agregar(100, 80) || v.agregar(1099, 80)) return false;
  if (!v.agregar(1100, 80)) return false;
  // Un hueco no se rellena con multiples copias de una misma lectura.
  if (!v.agregar(100100, 70) || v.muestras() != 3) return false;
  if (v.termino(300099) || !v.termino(300100)) return false;
  if (v.agregar(300100, 70) || registroValido(v.registro())) return false;
  v.detener();
  return !v.activa() && !v.tocaMuestra(300101);
}
constexpr bool limiteLecturas() {
  Ventana v;
  v.iniciar(0);
  if (v.agregar(0, 1024)) return false;
  for (uint32_t i = 0; i < muestrasEsperadas; ++i)
    if (!v.agregar(i * intervaloMuestraMs, 1022)) return false;
  return v.muestras() == 300 && v.registro().promedioMilliAdc == 1022000 &&
         !v.agregar(300000, 1022) && registroValido(v.registro());
}
constexpr bool muestreoMinimo() {
  Ventana v;
  v.iniciar(0);
  for (uint32_t i = 0; i < muestrasMinimas - 1; ++i)
    v.agregar(i * intervaloMuestraMs, 50);
  if (registroValido(v.registro())) return false;
  v.agregar((muestrasMinimas - 1) * intervaloMuestraMs, 50);
  return registroValido(v.registro());
}
constexpr bool reiniciarVentana() {
  Ventana v;
  v.iniciar(0);
  v.agregar(0, 900);
  v.iniciar(10000);
  for (uint32_t i = 0; i < muestrasEsperadas; ++i)
    v.agregar(10000 + i * intervaloMuestraMs, 65);
  return v.registro().suma == 19500 && v.registro().minimo == 65 &&
         registroValido(v.registro());
}
constexpr bool detectarCorrupcion() {
  Registro r = crearReferencia();
  if (!registroValido(r)) return false;
  ++r.suma;
  if (registroValido(r)) return false;
  r = crearReferencia();
  r.crc ^= 1;
  if (registroValido(r)) return false;
  r = crearReferencia();
  ++r.promedioMilliAdc;
  r.crc = crcRegistro(r);
  if (registroValido(r)) return false;
  r = crearReferencia();
  r.version = 2;
  r.crc = crcRegistro(r);
  return !registroValido(r) && !registroValido(Registro{});
}
constexpr uint32_t vectorCrc() {
  uint32_t crc = 0xffffffffUL;
  const char texto[] = "123456789";
  for (uint8_t i = 0; i < 9; ++i) crc = crcByte(crc, texto[i]);
  return crc ^ 0xffffffffUL;
}
static_assert(gestoCompleto(), "Gesto exacto 10 s y un solo disparo hasta liberar todos");
static_assert(gestoInterrumpido(), "Soltar un boton cancela el gesto");
static_assert(arranquePulsado(), "Pulsados al arrancar requieren liberacion");
static_assert(relojDesbordado(), "Gesto y muestreo toleran rollover uint32");
static_assert(muestreoYTiempo(), "No duplicar muestras ni prolongar la ventana de 5 min");
static_assert(limiteLecturas(), "Rango ADC y acumulador al extremo valido");
static_assert(muestreoMinimo(), "Rechazar cobertura inferior al 90 por ciento");
static_assert(reiniciarVentana(), "Nueva calibracion sin muestras anteriores");
static_assert(crearReferencia().promedioMilliAdc == 70000, "Promedio 70.000 ADC");
static_assert(crearReferencia().muestras == 300 && crearReferencia().suma == 21000,
              "300 muestras independientes de 1 Hz");
static_assert(crearReferencia().minimo == 60 && crearReferencia().maximo == 80,
              "Guardar rango observado");
static_assert(!registroValido(crearReferencia(0, 0)), "Rechazar ADC clavado a cero");
static_assert(!registroValido(crearReferencia(1023, 1023)), "Rechazar ADC saturado");
static_assert(detectarCorrupcion(), "Rechazar registro vacio, corrupto o incompatible");
static_assert(vectorCrc() == 0xcbf43926UL, "CRC32 IEEE vector conocido");
