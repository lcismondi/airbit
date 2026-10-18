#include "iluminacion_airbit.h"
using namespace AirbitLuz;
constexpr bool limites() { auto r=predeterminada(); r.manual=1;r.intensidad=100;sellar(r);if(!valido(r))return false;r.intensidad=101;sellar(r);if(valido(r))return false;r=predeterminada();r.manual=2;sellar(r);return !valido(r); }
constexpr bool corrupcion() { auto r=predeterminada();r.g=0;if(valido(r))return false;r=predeterminada();r.reservado[2]=1;sellar(r);if(valido(r))return false;r=predeterminada();r.version=2;r.crc=crcRegistro(r);return !valido(r); }
constexpr bool recuperarColor() { auto r=predeterminada();r.manual=1;r.r=12;r.g=34;r.b=56;r.intensidad=78;sellar(r);if(!valido(r))return false;r.manual=0;sellar(r);return valido(r)&&r.r==12&&r.g==34&&r.b==56&&r.intensidad==78; }
static_assert(valido(predeterminada()) && predeterminada().manual==0 && predeterminada().intensidad==50,"Arranque automatico con preferencias validas");
static_assert(limites(),"Modo e intensidad fuera de rango rechazados");
static_assert(corrupcion(),"CRC, reservas y version invalidos rechazados");
static_assert(canal(255,0)==0 && canal(255,100)==255 && canal(200,50)==100,"Escalado exacto y apagado completo");
static_assert(recuperarColor(),"Volver a calidad conserva color e intensidad manuales");
static_assert(direccion>=AirbitConfig::direccion+sizeof(AirbitConfig::Registro)&&direccion+sizeof(Registro)<=AirbitConfig::bytesEeprom,"Luz no solapa WiFi ni calibracion");

#include "intensidad_calidad.h"
namespace CalidadTest {
using namespace AirbitIntensidad;
constexpr bool limitesCalidad() { auto r=AirbitIntensidad::predeterminada();r.intensidad=0;sellar(r);if(!valido(r)||brillo(r)!=0)return false;r.intensidad=100;sellar(r);if(!valido(r)||brillo(r)!=255)return false;r.intensidad=101;sellar(r);return !valido(r); }
constexpr bool corrupcionCalidad() { auto r=AirbitIntensidad::predeterminada();r.intensidad=30;if(valido(r))return false;r=AirbitIntensidad::predeterminada();r.reservado[0]=1;sellar(r);if(valido(r))return false;r=AirbitIntensidad::predeterminada();r.version=2;r.crc=crcRegistro(r);if(valido(r))return false;r=AirbitIntensidad::predeterminada();r.bytes=0;r.crc=crcRegistro(r);return !valido(r); }
static_assert(valido(AirbitIntensidad::predeterminada())&&AirbitIntensidad::predeterminada().intensidad==25&&brillo(AirbitIntensidad::predeterminada())==64,"Calidad conserva brillo anterior");
static_assert(limitesCalidad(),"Calidad 0 apaga y 100 maximiza; rechaza 101");
static_assert(corrupcionCalidad(),"Calidad rechaza CRC, reserva, version o longitud invalidos");
}
