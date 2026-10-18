# Olas de LED desde las esquinas

Mapa indicado por el usuario. Cada lado contiene cinco LED. Se interpreta el tramo final como LED 16..20, porque LED 15 ya pertenece al lado 11..15.

```text
               LED 1 --> 5
       B2 --------------------- B1
       |                         |
 LED 20..16                 LED 6..10
       |                         |
       B3 --------------------- B4
               LED 15 <-- 11
```

Cada pulsación genera dos frentes que salen de la esquina y avanzan por sus lados vecinos:

| Botón | Color | Primer recorrido | Segundo recorrido |
| --- | --- | --- | --- |
| 1 | Rojo | 5 -> 4 -> 3 -> 2 -> 1 | 6 -> 7 -> 8 -> 9 -> 10 |
| 2 | Verde | 1 -> 2 -> 3 -> 4 -> 5 | 20 -> 19 -> 18 -> 17 -> 16 |
| 3 | Azul | 15 -> 14 -> 13 -> 12 -> 11 | 16 -> 17 -> 18 -> 19 -> 20 |
| 4 | Blanco | 10 -> 9 -> 8 -> 7 -> 6 | 11 -> 12 -> 13 -> 14 -> 15 |

La numeración de esta tabla es física (1..20). Los índices de `MemLeds` son 0..19.

## Comportamiento

- El frente avanza un LED cada 100 ms.
- Cada LED se desvanece durante 300 ms: estela de aproximadamente tres LED.
- La animación termina a los 700 ms y envía negro explícito a la tira. En reposo se reenvía negro cada 100 ms para recuperar un posible envío interrumpido.
- Se actualiza como máximo cada 20 ms usando `millis()`, sin esperas propias de la animación.
- Se pueden activar las cuatro olas a la vez. Si se cruzan, se suman sus colores con saturación.
- Una nueva pulsación del mismo botón reinicia su ola.
- Mantener un botón pulsado genera una sola ola; hay que soltar y volver a pulsar para repetir.
- Antirrebote de 30 ms. Un botón pulsado al arrancar debe soltarse antes de activar una ola.
- Se mantienen las acciones del cooler: botón 1 baja cinco unidades; botón 4 sube cinco unidades, con límites 0..255.
- Se conserva la detección que el usuario confirmó funcional: botones 1 y 2 activos en LOW, botones 3 y 4 activos en HIGH. También se conservan sus modos de entrada y GPIO.

Las funciones de lectura BMP y petición HTTPS son síncronas. Ahora solo se inician cuando no hay olas activas, para evitar que interrumpan una animación en curso. Si se pulsa mientras una operación síncrona ya está ejecutándose, la detección debe esperar a que termine. El efecto de ola no añade pausas bloqueantes; se retiró el `delay(500)` del loop.

## Ajustes en src/main.cpp

- `recorridoOlas`: orden de los LED de cada lado, por botón.
- `coloresOlas`: color por botón.
- `pasoOlaMs`: velocidad; aumentar el valor ralentiza el avance.
- `estelaOlaMs`: duración del desvanecimiento.
- `intervaloFrameOlaMs`: intervalo de refresco.
- `intervaloApagadoLedsMs`: intervalo de reenvío de negro en reposo.
- `antirreboteMs`: estabilidad mínima de la entrada.

## Validación

- Compilación PlatformIO correcta para `airbit_esp8266`.
- Mapa verificado: cada esquina comienza en los dos LED vecinos, cada recorrido es contiguo y contiene cinco LED, todos los índices están entre 0 y 19 y los dos recorridos de un botón contienen diez LED distintos.
- Se verificó que el bloque de supervisión Wi-Fi y el bloque de mediciones/envío no cambiaron.
- RAM: 30.884 / 81.920 bytes (37,7 %). Programa: 406.983 / 1.044.464 bytes (38,9 %).
- El usuario confirmó que los cuatro botones detectan correctamente sus pulsaciones antes de este cambio.
- Pendiente: cargar el nuevo firmware y validar dirección, color, estela, repetición y pulsaciones simultáneas en la placa.

Cargar con Upload de PlatformIO en COM9. En el monitor a 115200 baudios seguirá apareciendo `ON S 1` .. `ON S 4` con cada pulsación validada.

Copia local del código previo a las olas: `.pio/backups/main.antes_olas.cpp`. La carpeta `.pio` está ignorada y puede eliminarse al limpiar archivos del proyecto; esa copia incluye la configuración local existente.

## Apagado residual: corrección del 12/10/2026

El usuario confirmó que las olas funcionan en la placa, pero observó que a veces los últimos LED quedan tenues. El código anterior sí generaba negro al finalizar, aunque solo lo enviaba una vez. La transmisión FastLED 3.6.0 en ESP8266 puede abortar un frame ante alteraciones de temporización; no se ha demostrado que ese sea el motivo físico de este caso.

Se añade `FastLED.clear(true)` al terminar la última ola y se repite cada 100 ms mientras no hay ninguna ola activa. La limpieza en reposo no se ejecuta durante otras olas activas. Se sustituye `nscale8_video` (que conserva componentes no nulos al atenuar) por `nscale8`, de escalado lineal. No se modifica el mapa, los colores, los tiempos de recorrido ni la detección de botones.

Compilación correcta y verificación de que el resto del firmware se conserva. Pendiente cargar esta revisión y comprobar que desaparecen los restos tras olas individuales y simultáneas.

Referencia del controlador: https://github.com/FastLED/FastLED/blob/3.6.0/src/platforms/esp/8266/clockless_esp8266.h

## Fluidez: planificación de tareas — 12/10/2026

El usuario observó tirones intermitentes. El refresco de la ola está programado cada 20 ms (50 frames/s). El bloque de medición ejecuta cinco lecturas de presión y una de temperatura cada cinco segundos. Con el modo ULTRAHIGHRES predeterminado, la biblioteca espera 5 ms por temperatura y 26 ms por presión: el bloque suma 5 * (5 + 26) + 5 = 160 ms de esperas explícitas, además de I2C y salida serie. Este cálculo procede del código, no de una medición en la placa.

Se añade una condición para medir e iniciar la petición HTTPS únicamente cuando no queda ninguna ola activa. Los periodos quedan pendientes hasta que haya reposo. El cooler mantiene máxima potencia desde el minuto 9 y se apaga al minuto 10, aunque el envío de datos deba esperar a que termine una ola.

Se conserva el refresco de 20 ms, el efecto, los niveles de los botones, las fórmulas y las peticiones existentes. Compilación SUCCESS y comprobación del alcance de los cambios.

Limitaciones: si se generan olas de forma continua sin reposo, las mediciones y el envío permanecen pospuestos. Una petición HTTPS o lectura BMP ya iniciada sigue siendo síncrona y puede retrasar una nueva pulsación. Se requiere una prueba física para comprobar la mejora de fluidez; no se ha demostrado que estos bloqueos expliquen todos los tirones.

Referencia de las conversiones: https://github.com/adafruit/Adafruit-BMP085-Library/blob/master/Adafruit_BMP085.cpp

## Indicadores de arranque y conexión — 12/10/2026

- LED físico 16 (`MemLeds[15]`) rojo durante 5.000 ms desde su primer encendido. Tiene prioridad para conservar rojo puro durante ese tiempo.
- Frente azul con estela de tres LED que recorre 1 -> 20, vuelve a 1 y sigue girando mientras no hay conexión Wi-Fi. Avanza un LED cada 100 ms: una vuelta completa cada 2 s.
- Cuando se detecta `WL_CONNECTED`, el giro continúa 3.000 ms y se apaga. El rojo tiene su temporizador independiente: si esos 3 s terminan antes de los 5 s del arranque, el rojo sigue hasta completar sus 5 s.
- Si se pierde la conexión, el giro azul vuelve a aparecer mientras se reintenta, sin repetir el rojo de arranque.
- Las olas de botones se componen sobre el indicador azul. Se conserva el apagado reforzado en reposo.
- Un único compositor `actualizarLeds()` controla la tira; se retiran los apagados antiguos del setup que anulaban el rojo.

Parámetros: `duracionRojoArranqueMs = 5000` y `duracionGiroTrasConexionMs = 3000`. El refresco se mantiene cada 20 ms; los temporizadores utilizan `millis()` y no añaden esperas bloqueantes.

Las tareas lentas esperan durante las olas de botones y durante los 3 s de giro posteriores a conectarse. Si no hay conexión, las mediciones continúan para conservar el funcionamiento sin Wi-Fi; sus lecturas síncronas todavía pueden producir una pausa breve del indicador azul cada cinco segundos.

Compilación SUCCESS. Se comprobaron los temporizadores, el compositor único y la conservación de los botones, controles del cooler, credenciales y petición al formulario. La confirmación visual de los colores y tiempos en la placa requiere observar los LED.

## Pulsaciones repetidas ? 12/10/2026

La versi?n anterior reiniciaba el tiempo de una ?nica ola por bot?n en cada pulsaci?n. Al repetir m?s r?pido que sus 700 ms, el recorrido pod?a permanecer al principio. Esto explica una limitaci?n visual del c?digo; no demuestra un cuelgue del ESP8266.

Se a?ade un contador fijo de efectos pendientes por bot?n, sin memoria din?mica ni esperas. La ola activa termina y empieza la siguiente completa. Los cuatro botones conservan colas independientes y pueden animarse a la vez. El cooler cambia al detectar la pulsaci?n, no cuando se reproduce el efecto pendiente. Con 16 pendientes y una activa se pueden acumular aproximadamente 11,9 s de animaci?n por bot?n; durante las olas siguen pospuestas las tareas lentas conforme a la planificaci?n existente.

Cada pulsaci?n y cada liberaci?n deben estabilizarse al menos 30 ms. Los toques m?s breves pueden descartarse como rebote. Al alcanzar el l?mite se muestra `Olas S n: cola llena (16 pendientes)` y se conserva la respuesta del loop. Una lectura BMP o petici?n HTTPS ya iniciada puede seguir retrasando la detecci?n; no se han modificado en esta correcci?n.

Pendiente: comprobar en la placa una r?faga de pulsaciones, la finalizaci?n de todas las olas y la respuesta de otro bot?n durante la cola. Si deja de emitir `ON S n` o vuelve a aparecer `Inicio`, se requiere diagnosticar el bloqueo o reinicio real por separado.

Esta revisi?n compil? y se carg? por COM9 con hash verificado. En una observaci?n de 20 s continuaron las mediciones; no se capturaron pulsaciones. La prueba f?sica de r?fagas queda pendiente. Resultado actual en [VALIDACION.md](VALIDACION.md).

## Apagado residual persistente: prueba de transmisi?n ? 12/10/2026

El usuario vuelve a observar el ?ltimo LED tenue, especialmente en rojo, pese al apagado expl?cito y al reenv?o de negro cada 100 ms. El usuario identific? el LED f?sico 10; todav?a no ha indicado cu?nto dura el resto. El compositor ya limpia `MemLeds` antes de cada frame y no conserva por software el color del ?ltimo LED tras terminar una ola.

Se configura `FASTLED_ALLOW_INTERRUPTS = 0` antes de incluir FastLED para evitar la reapertura de interrupciones entre p?xeles. El env?o nominal de 20 LED de 24 bits a 800 kHz ocupa unos 600 us. `FASTLED_INTERRUPT_RETRY_COUNT = 5` ampl?a los reintentos acotados; el controlador todav?a puede abortar por NMI, que esta opci?n no deshabilita. No se garantiza que este sea el origen del problema observado. Fuente revisada: `.pio/libdeps/airbit_esp8266/FastLED/src/platforms/esp/8266/clockless_esp8266.h` y `led_sysdefs_esp8266.h`, versi?n 3.6.0.

La interrupci?n de la transmisi?n es una hip?tesis. Esta prueba no modifica colores, tiempos, mapa, cola de olas, indicadores, niveles de botones ni planificaci?n de las mediciones. Respaldo: `.pio/backups/main.antes_transmision_led.cpp`. Compilaci?n correcta. La primera carga fall? al abrir COM9 por acceso denegado; se detectaron procesos de monitor PlatformIO. No se lleg? a escribir flash en ese intento.

Despu?s de que el usuario cerrara el monitor, la carga termin? con SUCCESS y hash verificado. La placa arranc?, ley? los sensores y conect? al Wi-Fi; el monitor se cerr?.

Pendiente: comprobar visualmente el LED 10 tras varias olas rojas una vez conectada la placa y terminados los indicadores. Si persiste, comprobar se?al de datos y alimentaci?n antes de atribuir el fallo al software.

## Prioridad del modo calibraci?n ? 13/10/2026

Mantener los cuatro botones durante 10 s inicia la calibraci?n MQ135 de cinco minutos. Las olas pendientes se vac?an; los 20 LED respiran en verde y las acciones individuales quedan suspendidas hasta salir y liberar el gesto. Despu?s se recuperan las animaciones normales. Procedimiento y persistencia: [CALIBRACION_MQ135.md](CALIBRACION_MQ135.md).

## Fondo continuo de señal MQ135 — 13/10/2026

El usuario confirmó los 20 LED como fondo de calidad y las olas sobre él. Con referencia válida, el final de una ola recupera el fondo; ya no se envía negro a toda la tira en reposo. Sin referencia se mantiene el apagado anterior. El frente conserva su color y la estela revela el fondo gradualmente; los cruces de olas siguen sumando colores. El fondo no mantiene activa la bandera de olas ni pospone por sí solo las mediciones. Calibración y rojo de arranque conservan prioridad. [Escala y suavizado](ESCALA_CALIDAD_AIRE.md).
