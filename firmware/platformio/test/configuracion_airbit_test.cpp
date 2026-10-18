#include "configuracion_airbit.h"
using namespace AirbitConfig;
constexpr Registro base() {
  Registro r = {};
  copiar(r.claveAP, sizeof(r.claveAP), "0123456789ABCDEF");
  sellar(r);
  return r;
}
constexpr bool guardarRed() {
  Registro r = base();
  r.wifiPersonalizado = 1;
  if (!copiar(r.ssid, sizeof(r.ssid), "Mi red 2.4G") ||
      !copiar(r.claveWiFi, sizeof(r.claveWiFi), "clave-test-123")) return false;
  sellar(r);
  return valido(r);
}
constexpr bool longitudSSID() {
  Registro r = base();
  r.wifiPersonalizado = 1;
  if (!copiar(r.ssid, sizeof(r.ssid), "12345678901234567890123456789012")) return false;
  sellar(r);
  if (!valido(r)) return false;
  return !copiar(r.ssid, sizeof(r.ssid), "123456789012345678901234567890123");
}
constexpr bool detectarCorrupcion() {
  Registro r = base();
  if (!valido(r)) return false;
  r.claveAP[0] = 'F';
  if (valido(r)) return false;
  r = base(); r.version = 2; r.crc = crcRegistro(r);
  if (valido(r)) return false;
  r = base(); r.wifiPersonalizado = 2; r.crc = crcRegistro(r);
  if (valido(r)) return false;
  r = base(); r.reservado[0] = 1; r.crc = crcRegistro(r);
  return !valido(r) && !valido(Registro{});
}
constexpr bool cadenasInvalidas() {
  Registro r = base();
  for (char& c : r.ssid) c = 'x';
  sellar(r);
  if (valido(r)) return false;
  r = base(); r.wifiPersonalizado = 1; sellar(r);
  if (valido(r)) return false;
  r = base(); r.claveAP[0] = '\x01'; sellar(r);
  return !valido(r);
}
constexpr bool cambioSinRestos() {
  Registro r = base();
  copiar(r.ssid, sizeof(r.ssid), "Nombre anterior mas largo");
  copiar(r.ssid, sizeof(r.ssid), "Nueva");
  for (unsigned int i = 5; i < sizeof(r.ssid); ++i) if (r.ssid[i] != 0) return false;
  r.wifiPersonalizado = 1; sellar(r);
  return valido(r);
}
static_assert(valido(base()), "Primer AP sin cambiar la red de origen");
static_assert(guardarRed(), "Credenciales personalizadas persistibles");
static_assert(longitudSSID(), "SSID de 32 bytes y rechazo de 33");
static_assert(claveWiFiValida("12345678") && !claveWiFiValida("1234567"), "Minimo WPA de 8 bytes");
static_assert(claveWiFiValida(""), "Red abierta explicita");
static_assert(claveWiFiValida("0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF"), "PSK de 64 hex");
static_assert(!claveWiFiValida("Z123456789ABCDEF0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF"), "PSK 64 no hexadecimal rechazada");
static_assert(detectarCorrupcion(), "CRC, versiones y banderas corruptas rechazadas");
static_assert(cadenasInvalidas(), "Cadenas sin terminador, SSID vacio y AP invalido rechazados");
static_assert(cambioSinRestos(), "Reducir SSID limpia el resto del buffer");
static_assert(direccion >= sizeof(AirbitCal::Registro) && direccion + sizeof(Registro) <= bytesEeprom,
              "Configuracion y calibracion no se solapan");

constexpr bool actualizarClaveAP() {
  Registro r = base();
  r.wifiPersonalizado = 1;
  copiar(r.ssid, sizeof(r.ssid), "Red existente");
  copiar(r.claveWiFi, sizeof(r.claveWiFi), "clave-test-123");
  sellar(r);
  if (!valido(r)) return false;
  const Registro anterior = r;
  copiar(r.claveAP, sizeof(r.claveAP), claveApPredeterminada);
  sellar(r);
  if (!valido(r) || longitud(r.claveAP, sizeof(r.claveAP)) != 10 || r.wifiPersonalizado != 1) return false;
  for (size_t i = 0; i < sizeof(r.ssid); ++i) if (r.ssid[i] != anterior.ssid[i]) return false;
  for (size_t i = 0; i < sizeof(r.claveWiFi); ++i) if (r.claveWiFi[i] != anterior.claveWiFi[i]) return false;
  for (size_t i = 10; i < sizeof(r.claveAP); ++i) if (r.claveAP[i] != 0) return false;
  return true;
}
static_assert(actualizarClaveAP(), "Clave AP solicitada valida y WiFi anterior conservado");
static_assert(!claveAPValida("1234567") && !claveAPValida("12345678901234567"), "Limites de clave AP almacenada");
