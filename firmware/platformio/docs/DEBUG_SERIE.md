# Diagnostico por serie de Airbit

Abrir el monitor de PlatformIO en **COM9, 115200 baudios**, desde la carpeta de este proyecto. Se han configurado marcas de hora, decodificador de excepciones y guardado automatico en `logs/device-monitor-*.log`. DTR y RTS quedan desactivados al abrir; el monitor no ordena un reset.

```powershell
& 'C:\Users\siste\.platformio\penv\Scripts\platformio.exe' device monitor
```

Enviar cualquiera de estos caracteres (con o sin salto de linea):

| Comando | Accion |
| --- | --- |
| `d` | Motivo del ultimo reinicio, huella anterior y estado actual de memoria/red |
| `v` | Activar/desactivar detalle de todas las peticiones, incluido el polling de estado |
| `a` | Pausar STA y probar solo el AP, temporalmente y sin borrar preferencias |
| `w` | Reanudar un intento con la red WiFi guardada |
| `l` | Pausar/reanudar solo la transmision LED para una comparacion temporal |
| `n` | Estado de sockets HTTP, SDK, modo y ahorro WiFi |
| `h` o `?` | Ayuda |

El detalle HTTP comienza desactivado. Pagina, acciones, errores y operaciones lentas ya se registran; `v` agrega las consultas normales a /api/estado. El resumen ESTADO sale cada 5 segundos. Los comandos no reinician ni escriben preferencias. `a` interrumpe la conexion al router hasta enviar `w`, guardar WiFi desde la app o reiniciar.

## Captura alternativa

Permite guardar un registro con fecha y hora completas, por un tiempo acotado o hasta Ctrl+C. Cerrar antes cualquier otro monitor que tenga COM9 abierto.

```powershell
& 'C:\Users\siste\.platformio\penv\Scripts\python.exe' scripts/monitor_debug.py --command d
# Captura de cinco minutos:
& 'C:\Users\siste\.platformio\penv\Scripts\python.exe' scripts/monitor_debug.py --seconds 300 --command v
```

El capturador alternativo conserva las direcciones de la traza, pero no las decodifica en directo. Para resolverlas se necesita el firmware.elf exacto del binario que genero el registro. El monitor de PlatformIO usa ese ELF automaticamente. Se compila en modo debug con simbolos -g3 y se conserva la optimizacion -Os, para obtener lineas de nuestro codigo sin pasar a una compilacion sin optimizar. Las bibliotecas cerradas del SDK pueden mostrar solo el nombre de la funcion. Antes de cambiar de firmware conviene conservar juntos log, ELF y BIN.

## Que mirar

- **BOOT**: razon de arranque y datos de excepcion, cuando existen. Los codigos de esta linea corresponden al SDK: 0 encendido, 1 watchdog de hardware, 2 excepcion, 3 watchdog de software, 4 reinicio de software, 5 despertar, 6 reset externo. No son la misma numeracion que el `rst cause` del mensaje ROM.
- **ANTERIOR**: ultima huella RTC valida del arranque previo: etapa, tiempo encendido, ultima solicitud y memoria. Si la etapa es LOOP, no quedo una operacion instrumentada abierta en ese punto. Si indica una ruta web, flash, BMP o HTTPS, permite acotar donde estaba el firmware.
- **ESTADO**: `heap` son bytes libres, `min` el minimo observado, `bloque` el mayor bloque contiguo libre y `frag` la fragmentacion. `pila` informa el espacio libre observado de la pila del loop. Incluye estado Wi-Fi, RSSI, clientes AP, canal y contadores de peticiones/desconexiones.
- **INICIO / ENVIO / FIN**: etapa, solicitud, metodo, codigo HTTP, bytes previstos y duracion. No se registra el cuerpo ni parametros de las peticiones. FIN significa que retorno el manejador, no confirma que el telefono haya recibido la respuesta.
- **LOOP_PAUSA / ATENCION_WEB**: una vuelta o atencion web tardo al menos 500 ms. Las operaciones sincronas existentes pueden producir pausas; comparar la etapa y los tiempos.
- **EEPROM.commit**: escritura real de una preferencia o calibracion, con inicio y fin. El debug no agrega commits para guardar sus propias huellas.
- **ENVIO_HTTPS / HTTPS.GET**: envio periodico existente, su codigo de retorno y duracion.
- **Exception / Soft WDT / Hardware Watchdog / last failed alloc**: conservar toda la traza, incluidos los delimitadores CUT HERE. Se habilito DEBUG_ESP_OOM para recoger informacion de asignaciones fallidas en los informes de fallo.

Un delta de memoria negativo dentro de una peticion no demuestra una fuga: algunos objetos del servidor se liberan despues del retorno del manejador. Observar la tendencia del resumen cuando el equipo vuelve a reposo.

## Limites de la huella

Ocupa 48 bytes de RAM RTC del usuario, desde el byte 128 (offset 32 en palabras), fuera de la reserva inicial del core para OTA. Tiene version, limites y CRC32. Puede conservarse entre determinados reinicios, pero no es un historial duradero: puede perderse al cortar alimentacion. El contador es de arranques con huella conservada. Una huella invalida se informa como no disponible.

Se actualiza al entrar y salir de las operaciones instrumentadas y cada 5 segundos; no es una traza de cada instruccion ni cubre todos los callbacks internos del SDK. Una etapa anterior es una pista, no demuestra por si sola que esa funcion cause el reset. Un reset externo o de encendido tampoco identifica por si solo un defecto de alimentacion.

Las claves no se imprimen. El registro contiene datos operativos como lecturas y nombres/direcciones de red. No se agregaron reinicios automaticos ni se desactivo el watchdog.

Fuentes primarias: [diagnostico de fallos ESP8266 3.1.2](https://arduino-esp8266.readthedocs.io/en/3.1.2/faq/a02-my-esp-crashes.html), [APIs de memoria, RTC y reinicio](https://arduino-esp8266.readthedocs.io/en/3.1.2/libraries.html). Las firmas y filtros se contrastaron ademas con los archivos de las versiones instaladas.


## Primeros hallazgos en la placa - 16/10/2026

Durante la prueba de tres minutos con peticiones repetidas y el navegador abierto se capturaron dos reinicios reales por Software Watchdog (codigo SDK 3), aparte del reset externo intencional usado para verificar RTC. En ambos EPC1 era 0x402564d2 y el ELF de esa prueba lo resuelve como pm_send_nullfunc, una rutina interna del SDK Wi-Fi. El contexto de la traza era sys y la huella previa indicaba LOOP. No quedo un manejador web instrumentado abierto en esa huella.

La ultima muestra antes de cada fallo tenia 25.256 y 19.240 bytes libres, con bloque maximo 25.160 y 19.128 bytes. Son muestras previas, no la medida exacta del instante de fallo. No se concluye que una fuga, la alimentacion, los LED o el portal por si solos sean la causa. La pista inmediata es el bloqueo en la rutina Wi-Fi; confirmar el desencadenante exige una comparacion controlada.

Registro original: logs/airbit-validacion-debug-20261016-195741.log. Decodificacion: .pio/validacion/reinicio_watchdog_decodificado.txt. ELF/BIN de esa captura en .pio/backups/diagnostico-serie-20261016/. La version final con simbolos completos tiene su propia pareja ELF/BIN en .pio/backups/diagnostico-serie-20261016-simbolos/: usar siempre el par correspondiente al registro.

## Recuperacion del portal - 17/10/2026

Registro: logs/device-monitor-261017-084405.log; copia previa a los cambios en .pio/validacion/ap-fallo-20261017-antes.log. La captura comienza con el programa ya iniciado. `d` recupero Exception 29, EPC1 0x4000e1c3, direccion 0x18 y una huella anterior de 5808 bytes libres; no hay traza completa de ese fallo en este archivo y no se atribuye a una funcion concreta. Las consultas `d` repetidas no son nuevos arranques. Ahora se identifican con RESET ANTERIOR, reservando BOOT para el arranque real.

En el incidente observado el uptime continuaba, los sensores seguian midiendo y el contador HTTP quedo en 210. Aparecieron fallos STA 15/202 y cambios de canal durante reintentos. El PC tampoco pudo asociarse al AP durante la prueba de solo lectura; su perfil sic5 fue restaurado. Resultado: .pio/validacion/ap_estado_actual.json. Esto no demuestra una clave incorrecta ni explica por si solo la excepcion anterior.

Tambien se midio una pausa de 10018 ms en ENVIO_HTTPS. La transferencia antigua es sincrona y durante esa espera el loop no atiende HTTP/DNS.

Correccion preparada: reabrir HTTP/DNS desde el loop tras desconexion STA o nueva IP, agrupando eventos durante 250 ms. No se recrean rutas ni tokens. Si entra un cliente AP durante un intento automatico, cancelar STA y dar prioridad al portal; esperar 15 s desde la salida del ultimo cliente antes de nuevos reintentos. Los intentos solicitados al guardar WiFi o enviar `w` siguen permitidos. Registrar altas/bajas AP y recuperacion de servicios.

El envio HTTPS se pospone mientras haya clientes AP o actividad web en los ultimos 15 s. Se conserva un unico envio pendiente y el reloj del cooler avanza independientemente; las esperas de cliente/HTTP se ajustan a 1500 ms. Esto no convierte HTTPS en asincrono ni garantiza un limite total de 1500 ms para DNS/TLS. Las pulsaciones y calibracion conservan su funcionamiento.

Compilacion y siete comprobaciones RTC PASS; RAM 35500 / 81920, flash 515459 / 1044464. ELF/BIN en .pio/backups/recuperacion-ap-20261017/. Carga COM9 con hash verificado y prueba real PASS: 29 respuestas HTTP 200, cero errores transitorios y cero reinicios durante la prueba. PC con IP 192.168.4.100 por DHCP, DNS cautivo correcto antes y despues de reanudar STA, pagina exacta al HTML local. Router red_de_prueba conectado con IP 192.168.1.146 y AP Airbit-66FF61 activo simultaneamente. Preferencias de iluminacion/intensidad sin cambios; calibracion valida 42.04 ADC / 300 muestras. Red original sic5 del PC restaurada y COM9 cerrado al terminar. Esta prueba corta no demuestra resueltos todos los reinicios intermitentes. Los watchdog previos y Exception 29 siguen sin causa raiz demostrada.

## Nuevo watchdog de hardware - 17/10/2026 10:00:58

La recuperacion del portal no elimino el fallo de fondo. logs/device-monitor-261017-095701.log contiene un arranque real por Hardware Watchdog (SDK 1, ROM rst cause 4), EPC1=EPC3=0x40104da6. Decodificado con el ELF exacto de recuperacion-ap-20261017: lmacProcessTxSuccess. El desensamblado en esa direccion es un salto a si misma: el SDK queda en un bucle sin salida hasta que vence el watchdog. Esto localiza el bloqueo, pero no demuestra que lo haya provocado la web, FastLED, memoria o una averia de placa.

Huella anterior: arranque 3, 528622 ms, LOOP, solicitud POST 140 terminada, 34384 bytes libres, bloque 32984 y minimo observado 30944. La ultima peticion visible fue /api/calidad, respuesta 200 y duracion 4 ms; no habia un envio HTTPS instrumentado activo. No hay stack completo de este watchdog. Copia de captura, resumen y localizacion en .pio/validacion/watchdog-hardware-20261017/.

Siguiente comparacion: enviar a en serie para mantener AP y pausar STA sin regrabar ni borrar ajustes; observar el portal con STA apagada. w vuelve a intentar la red guardada. Una prueba corta estable no permite declarar resuelto el reinicio intermitente.

## Comparacion LED pausado/activo y prueba de IRQ - 17/10/2026

La prueba solo AP inicial se interrumpio por timeout antes de diez minutos. No se registro un reinicio dentro de esa captura corta; despues de restaurar STA se recupero con d una Exception 29 con huella de 1792 bytes libres. El descenso ya era visible con STA pausada (34864 -> 29536 -> 22984), por lo que no basta atribuirlo a la autenticacion del router. Informes: solo_ap_prueba.json y logs/estado-tras-prueba-solo-ap-20261017.log. No se conservaba el estado de asociacion del PC en ese primer timeout.

Se agregaron comandos l (pausar/reanudar la transmision LED, manteniendo el ultimo frame fisico) y n (sockets/SDK/modo/sleep) para una comparacion con mejor captura. Log: logs/aislamiento-led-20261017.log; informe: .pio/validacion/aislamiento_led_prueba.json. Con STA pausada y LED pausado: 182.8 s y 183 peticiones correctas. Al reactivar LED sin cambiar AP ni carga HTTP, se produjo timeout y una Exception 29. El PC seguia asociado a Airbit, RSSI -39 dBm; sockets sin cliente actual ni pendientes mientras caia el heap. SDK observado 2.2.2-dev(38a443e), modo AP, sleep 2. Esto aporta evidencia de una interaccion con la transmision LED, sin demostrar el mecanismo interno del SDK. ELF/BIN de esta captura: .pio/backups/aislamiento-led-20261017/.

Cambio puntual para comparar: FASTLED_ALLOW_INTERRUPTS=1 e INTERRUPT_THRESHOLD=0 (umbral predeterminado de ESP8266 en FastLED 3.6), manteniendo cinco reintentos, GPIO0, los 20 LED y los tiempos de efectos. Antes se mantenian bloqueadas las IRQ durante el frame completo; ahora se cede entre pixeles. No se cambia SDK ni sleep para esta prueba. Compilado: RAM 35516 / 81920, programa 516127 / 1044464. Carga COM9 con hash verificado. SHA256 e28b8372932d493637eb6289aac5b580a86f652cd43e86c0f012f4ac98f373f6. Copia ELF/BIN/fuentes: .pio/backups/irq-led-20261017/.

Resultado de la primera prueba solo IRQ: sin reinicios ni descenso continuo de heap, pero se produjo IncompleteRead (4824 bytes recibidos de 55842) durante una pagina, con receptor aun asociado al AP. Informe original preservado en irq_led_prueba_antes_timeout.json. La correccion adicional y su validacion se describen abajo. Los comandos l/a son temporales y se restauran al reiniciar o mediante l/w respectivamente; no escriben EEPROM.

## Estado final: IRQ entre pixeles y timeout del flujo HTML - 17/10/2026

En el core 3.1.2, send_P crea un StreamConstPtr con timeout propio de 1000 ms; StreamSend.cpp usa el timeout del flujo fuente tanto para lectura como para escritura, independientemente del timeout del WiFiClient. El corte registrado duro unos 1101 ms. La ruta raiz ahora proporciona un StreamConstPtr con 5000 ms de timeout y longitud explicita, sin cargar el HTML completo en RAM ni cambiar la pagina. Se conserva el informe de transferencia y bytes leidos del flujo. streamRemaining() devuelve el tamano original en esta clase, por eso el contador usa available(). bytes leidos no es prueba de recepcion; un error de escritura puede haber consumido bytes del flujo que no se entregaron.

Validacion sostenida de IRQ+timeout: 400 respuestas HTTP 200, pagina identica al archivo editable, descarga con receptor TCP de ventana pequena detenido 2 s completa (55842 bytes en 2.53 s), 180.56 s AP solo y 377.3 s AP+STA, sin reinicios ni agotamiento continuo del heap. Muestras: AP solo 37904 -> 36656 (menor 34448); AP+STA 35200 -> 35200 (menor 31528). Durante el minuto posterior el minimo observado fue 30344 y el heap se mantuvo en torno a 35-37 KB. El informe original irq_led_prueba.json figura FAIL porque la comprobacion DNS posterior dio WinError 10051; no se transforma ese error en PASS. Hubo tambien desconexion STA 202. La captura completa esta en logs/irq-led-timeout-20261017.log.

La prueba independiente posterior recupero IP 192.168.4.100 y verifico asociacion AP, DNS cautivo, pagina completa y API 200, con router conectado de nuevo. Informe ap_estado_tras_irq.json. Red sic5 del PC restaurada. Calibracion 42.04 / 300 conservada; no se escribieron preferencias durante estas pruebas. No dar por resueltos todos los cortes de radio: hubo un corte al final, aunque la reentrada al AP funciono y no aparecio reinicio.

La ultima compilacion solo corrige el contador del debug, sin cambiar la transferencia probada. RAM 35516 / 81920, programa 516175 / 1044464; BIN SHA256 02e8cc800e4ac1b75d6a130b9414c77646f83bc394dcc9b57a7dfde967c81b22. Carga verificada y comprobacion LAN final PASS: pagina exacta, leidosDelFlujo=55842/55842 informe=0, router conectado, AP y DNS activos, calibracion conservada. Fuentes/ELF/BIN/config: .pio/backups/irq-led-timeout-final-20261017/. COM9 cerrado al terminar. Informes consolidados: irq_led_resumen_final.json y irq_led_verificacion_final.json. Sigue pendiente aceptar en uso prolongado y comprobar visualmente las olas/posibles pixeles residuales con las interrupciones habilitadas.
