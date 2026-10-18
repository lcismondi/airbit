# Airbit - firmware ESP8266

Firmware **v0.2.0** para el monitor Airbit, con MQ135, BMP085/BMP180, cuatro pulsadores y 20 LED WS2812B. Proyecto PlatformIO; el mapa de conexiones y las preferencias guardadas se conservan.

## Configurar el equipo

1. Conectarse a `Airbit-XXXXXX` con la clave AP `1234567890`.
2. Abrir el portal cautivo o `http://192.168.4.1` en el navegador.
3. En WiFi, guardar la red de 2,4 GHz. El AP permanece disponible.

Calidad de aire es la pantalla inicial. Iluminacion permite color fijo e intensidad. Calibracion toma muestras durante cinco minutos en aire limpio, desde la web o manteniendo los cuatro botones durante diez segundos. La referencia del MQ135 es relativa: no representa ppm ni un AQI oficial. El BMP085/BMP180 mide presion y temperatura, no humedad.

## Version e identidad

El pie de todas las pantallas muestra version y MAC WiFi. `GET /api/estado` incluye:

```json
{"dispositivo":{"version":"0.2.0","mac":"AA:BB:CC:11:22:33","macAP":"AA:BB:CC:44:55:66"}}
```

`mac` identifica la interfaz STA del equipo y `macAP` la del punto de acceso. Son valores leidos de cada placa. La version se define una sola vez en `include/version_airbit.h` y aparece tambien en el arranque serie.

## Compilar y cargar

Instalar PlatformIO y ejecutar desde esta carpeta:

```powershell
pio run -e airbit_release
pio run -e airbit_release -t upload
pio device monitor -e airbit_release
```

El entorno publico usa `nodemcuv2`, ESP8266 de 4 MB, flash DIO, COM9 y 115200 baudios. Ajustar el puerto si el sistema asigna otro. La carga normal no borra calibracion ni preferencias EEPROM. Cerrar el monitor antes de cargar.

Para desarrollo local, `pio run -e airbit_esp8266` admite el archivo opcional `include/configuracion_local.h`, creado a partir de `configuracion_local.example.h`. Esta excluido de Git. **El entorno `airbit_release` ignora ese archivo**, por lo que el binario publico no contiene claves WiFi ni la URL privada del formulario historico. El envio al formulario queda desactivado cuando no hay URL local; sensores, portal y LED siguen funcionando.

## Hardware

| Funcion | GPIO |
| --- | --- |
| MQ135 | A0 |
| BMP085/BMP180: SCL / SDA | 5 / 4 |
| 20 WS2812B | 0 |
| Cooler PWM | 14 |
| Botones 1 / 2 / 3 / 4 | 12 / 13 / 15 / 10 |
| Serie TX / RX | 1 / 3 |

Los LED usan una fuente aparte en la placa validada. Se mantiene flash DIO y el mapa de pulsadores que funciona en este equipo. El modelo comercial exacto de la placa no esta confirmado; no asumir compatibilidad de otros montajes solo por compilar.

## Diagnostico y pruebas

Monitor serie: 115200 baudios, log automatico en `logs/`. `d` consulta el ultimo reset, `v` alterna detalle HTTP, `a` pausa STA, `w` reanuda el router, `l` pausa/reanuda la transmision LED, `n` informa sockets y `h` muestra ayuda. No escriben EEPROM. Guia: [DEBUG_SERIE.md](docs/DEBUG_SERIE.md).

```powershell
node test/pagina_configuracion_test.cjs
python scripts/test_calibracion.py
python scripts/test_calidad.py
python scripts/test_configuracion.py
python scripts/test_iluminacion.py
python scripts/test_diagnostico.py
```

[Validacion y limites conocidos](docs/VALIDACION.md) - [Cambios](CHANGELOG.md) - [Publicar y sincronizar GitHub](docs/PUBLICACION.md).

El archivo Arduino original de esta carpeta se mantiene solo localmente porque contenia configuracion privada. En GitHub se conserva la referencia historica en `firmware/Airbit.ino`, con marcadores genericos de configuracion.
