# AP y portal de configuración — 13/10/2026

El usuario pidió un AP para configurar Wi-Fi y preparar calibración, iluminación con color y calidad de aire. Confirmó que el AP debe estar siempre disponible mientras Airbit se conecta al router.

## Acceso

1. Buscar la red `Airbit-66FF61` de esta placa.
2. Usar la clave `1234567890`, también indicada como `AP clave:` en el monitor serie.
3. Si el teléfono indica que la red no tiene Internet, mantener la conexión: se utiliza para configurar Airbit.
4. El teléfono puede ofrecer **Iniciar sesión en la red** / **Abrir configuración**. Si no aparece, abrir `http://192.168.4.1`.
5. La página abre directamente, sin usuario ni contraseña.

La página también se puede abrir desde la red doméstica mediante la IP que Airbit imprime como `IP address:`. Ese acceso también abre sin usuario ni contraseña. No se devuelve al navegador la contraseña del router ni la clave del AP.

El AP permanece activo, con hasta cuatro clientes simultáneos. AP y estación comparten la radio/canal del ESP8266; al cambiar de router/canal puede ser necesario reconectar el teléfono al AP. Se implementa portal cautivo mediante DNS comodín y redirección HTTP. Airbit no proporciona acceso a Internet a sus clientes AP.

## Portal cautivo

Se usa la biblioteca incluida en el nucleo instalado, siguiendo el [ejemplo oficial ESP8266 3.1.2](https://github.com/esp8266/Arduino/blob/3.1.2/libraries/DNSServer/examples/CaptivePortal/CaptivePortal.ino).

El DNS UDP/53 resuelve los nombres IPv4 a `192.168.4.1`, con TTL 0. Se atiende en el loop junto con HTTP. Las sondas de Android (`/generate_204`, `/gen_204`), Apple (`/hotspot-detect.html`, `/library/test/success.html`, `/canonical.html`), Windows (`/connecttest.txt`, `/ncsi.txt`, `/redirect`, `/fwlink`) y Firefox (`/success.txt`) reciben HTTP 302 hacia la configuracion. La redireccion usa la IP de la interfaz por la que llego la peticion; el acceso directo desde el router sigue disponible. En el AP, nombres ajenos en la raiz y rutas GET desconocidas tambien redirigen.

La apertura de la ventana depende del sistema del telefono y debe comprobarse al reconectar al AP. Se conserva el acceso manual por HTTP; no se intercepta HTTPS. La clave solo se exige para entrar al Wi-Fi, no para abrir el portal.

Prueba reproducible: `scripts/verificar_cautivo.py`. Por defecto prueba DNS y sondas por la IP domestica sin cambiar de red. Para comprobar la interfaz AP, conectar el PC a Airbit y ejecutar con `--ip 192.168.4.1`; esa variante tambien verifica host y ruta desconocidos. La prueba no guarda configuracion.

## Wi-Fi

- Introducir SSID y contraseña y pulsar **Guardar Wi-Fi**.
- Dejar la contraseña vacía conserva la contraseña vigente.
- Marcar **Esta red no tiene contraseña** guarda una red abierta y elimina la contraseña de esa configuración.
- El nombre admite 1..32 bytes. La clave admite 8..63 bytes o una PSK de 64 caracteres hexadecimales.
- Guardar confirma la escritura en flash; no afirma que el router haya aceptado la conexión.
- Tras guardar, Airbit inicia la conexión en segundo plano y conserva el AP. El estado de la página confirma si se conectó.
- La respuesta HTTP se envía antes de aplicar el cambio de estación, con una espera de 500 ms gestionada por `millis()`, sin reiniciar la placa.
- Sin configuración personalizada válida se utilizan las credenciales existentes del firmware.

## Funcionalidad preparada

Los apartados muestran el estado actual. Calibración permite iniciar el proceso desde la web:

| Apartado | Información disponible | Pendiente |
| --- | --- | --- |
| Calibración | Referencia ADC, muestras guardadas, tiempo restante y botón para iniciar el proceso | Otras opciones por definir |
| Iluminación con color | Switch manual, rueda cromática y slider de intensidad | Otras opciones por definir |
| Calidad de aire | ADC, índice relativo y escala de colores | Definir opciones de la escala |

Se conserva el gesto físico de cuatro botones/10 s, los cinco minutos de calibración, el fondo de calidad, las olas y el ciclo del ventilador. La página permite iniciar la misma calibración de cinco minutos. La iluminación manual se controla desde la página; los controles de umbrales de calidad siguen por definir.

## Persistencia

Se amplía la EEPROM emulada de 32 a 256 bytes, leyendo primero el contenido existente. El registro de calibración conserva su formato y posición. Ambos registros tienen CRC.

| Dirección lógica | Tamaño | Uso |
| --- | --- | --- |
| 0..31 | 32 | Calibración MQ135 existente |
| 32..63 | 32 | Reservado |
| 64..67 | 4 | Marca de configuración |
| 68..69 | 2 | Versión 1 |
| 70..71 | 2 | Tamaño 132 |
| 72 | 1 | Configuración Wi-Fi personalizada: 0/1 |
| 73..75 | 3 | Reservado a cero |
| 76..108 | 33 | SSID con terminador |
| 109..173 | 65 | Clave Wi-Fi con terminador |
| 174..190 | 17 | Clave AP de 8..16 caracteres ASCII imprimibles y terminador |
| 191 | 1 | Relleno a cero |
| 192..195 | 4 | CRC32 IEEE de los 128 bytes anteriores del registro |
| 196..215 | 20 | Preferencias de iluminación con CRC; ver ILUMINACION_MANUAL.md |
| 216..231 | 16 | Intensidad de calidad con CRC; ver INTENSIDAD_CALIDAD.md |
| 232..255 | 24 | Reservado |

La clave AP se define en `include/configuracion_airbit.h`, constante `claveApPredeterminada`. Esta versión aplica `1234567890` al arrancar: actualiza la clave anterior conservando el SSID y la contraseña Wi-Fi; solo escribe si la clave cambia. La página no dispone todavía de un campo para cambiar la clave AP. Wi-Fi se escribe al guardar desde la página. La calibración sigue escribiéndose solo al completar una ventana válida. Los `EEPROM.put()` se realizan en sus respectivas direcciones y el commit escribe la memoria compartida.

Si se corta alimentación durante un commit, el único sector de EEPROM puede invalidarse; los CRC permiten detectarlo. Una configuración inválida vuelve a las credenciales de origen y aplica la clave AP definida en el firmware. La clave puede consultarse por USB.

## Código y pruebas

- `include/configuracion_airbit.h`: formato, validación, copia acotada y CRC.
- `include/portal_configuracion.h`: AP, DNS cautivo, estado, guardado Wi-Fi e inicio de calibración.
- `src/configuracion.html`: página editable.
- `include/pagina_configuracion.h`: copia PROGMEM servida.
- `scripts/generar_pagina.py`: sincronización del HTML.
- `test/configuracion_airbit_test.cpp`: 13 comprobaciones constexpr del registro.
- `test/pagina_configuracion_test.cjs`: JavaScript real del formulario y navegación con respuestas de prueba.

Después de editar el HTML:

```powershell
& 'C:\Users\siste\.platformio\penv\Scripts\python.exe' scripts/generar_pagina.py
node test/pagina_configuracion_test.cjs
& 'C:\Users\siste\.platformio\penv\Scripts\python.exe' scripts/test_configuracion.py
& 'C:\Users\siste\.platformio\penv\Scripts\platformio.exe' run
```

Los cambios de red y el inicio de calibración requieren POST y un token de sesión. Los textos recibidos se muestran como texto, no como HTML. La página no descarga fuentes, imágenes o bibliotecas externas, por lo que funciona sin Internet. La lectura de estado usa GET y no escribe EEPROM.

El servidor se atiende en el loop. Las tareas BMP/HTTPS síncronas existentes todavía pueden retrasar una respuesta web; no se modifican en este paso.

## Respaldo

Se conserva la versión previa que el usuario confirmó:

- `.pio/backups/main.antes_ap_config.cpp`.
- `.pio/backups/firmware.antes_ap_config.bin`.
- SHA256 del binario previo: `596DF552C499F57C66A7085BBA1C1051C592476C6AC2F0EEF94C41F49EC857D0`.

La carpeta `.pio` está ignorada y puede eliminarse al limpiar. El código previo contiene la configuración privada existente; no publicarlo. Al volver al firmware anterior, su calibración usa EEPROM de 32 bytes: si se recalibra con esa versión, su commit puede borrar los datos AP/Wi-Fi de la zona ampliada. La calibración principal sigue en 0..31.

Resultados de carga, pruebas HTTP y revisión visual: [VALIDACION.md](VALIDACION.md).

Fuentes de las APIs: [AP del núcleo ESP8266 3.1.2](https://arduino-esp8266.readthedocs.io/en/3.1.2/esp8266wifi/soft-access-point-class.html), [EEPROM ESP8266](https://arduino-esp8266.readthedocs.io/en/3.1.2/libraries.html#eeprom).

Prueba HTTP local reproducible: `scripts/verificar_portal.py --acceso RUTA_JSON`. Por defecto verifica acceso/validación y no guarda red. `--guardar-red-actual` prueba el guardado de la red vigente y su reconexión, manteniendo su contraseña; usarlo solo cuando se quiera esa comprobación. La metadata local usa `ap_password`, `ap_name` y `station_ip`; no poner la contraseña del router en ese archivo ni publicarlo.

Iluminación manual: [switch, color e intensidad](ILUMINACION_MANUAL.md). El registro AP/Wi-Fi mantiene su formato; cada proceso escribe su propia zona de memoria.

## Recuperacion ante fallo del router - 14/10/2026

Los intentos STA duran como maximo 15 s. Al fallar, STA se detiene conservando el AP y las credenciales. Los reintentos automaticos se realizan cada 30 s solo si el AP no tiene clientes; guardar una red desde el portal inicia un intento explicito. Asi la configuracion sigue accesible aunque falle el router. Al salir del AP pueden reanudarse los intentos. AP y STA siguen compartiendo el canal de radio del ESP8266.

Si se cierra la ventana cautiva tras una perdida de red, reconectar a Airbit y abrir http://192.168.4.1 en Chrome/Safari. Un error de fetch se explica en espanol y ofrece reintento conservando la seleccion de luz.
