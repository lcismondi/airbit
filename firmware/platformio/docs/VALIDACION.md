# Version e identidad - 18/10/2026

Firmware v0.2.0 compilado y cargado en COM9 con hash de grabacion verificado. RAM 35600 / 81920; programa 517359 / 1044464 bytes. SHA256 del binario local: 254fd6e81b0fffb0bb5bceb0b3d6eeb3db2ae39c402fda754fe0be93a3b81328. HTTP real confirma version 0.2.0, MAC STA/AP leidas de la placa y pagina exacta de 56461 bytes con el pie de identidad. La calibracion 42.04 ADC / 300 muestras se conserva y WiFi vuelve a conectar. Prueba JavaScript de identidad y compatibilidad con respuestas antiguas PASS.

Esta carga usa el entorno local airbit_esp8266. El binario distribuible se genera por separado con airbit_release, sin configuracion privada; su hash y commit quedan en el manifiesto del release. No confundir estos dos binarios al decodificar excepciones.

# Reinicios: comparacion LED y correccion de IRQ/HTML - 17/10/2026

Reproducido un watchdog de hardware en lmacProcessTxSuccess y posteriores excepciones con heap muy bajo. Con AP solo y LED pausado: 183 respuestas durante 182.8 s, heap estable. Al reactivar LED con igual trafico: timeout, heap hasta 3504 y Exception 29, PC aun asociado. Esto apunta a la interaccion del envio WS2812B con WiFi. Se habilitan interrupciones entre pixeles (FASTLED_ALLOW_INTERRUPTS=1, INTERRUPT_THRESHOLD=0), conservando pin, efectos y cinco reintentos. No se cambia SDK ni sleep.

Se detecto ademas una pagina truncada con el flujo PROGMEM predeterminado de 1000 ms. La ruta ahora usa un flujo con 5000 ms y longitud explicita. Validacion funcional: 400 HTTP 200, 180.56 s AP solo + 377.3 s AP/STA, cero reinicios, memoria sin caida continua; descarga lenta con pausa de receptor 2 s completa. El arnes original termina FAIL por WinError 10051 en DNS al final, con una desconexion STA registrada. La comprobacion independiente posterior de reentrada AP/DHCP/DNS/HTTP paso y el router volvio a conectar. No se declara curado todo corte de radio.

Ultimo cambio solo de contador debug: streamRemaining devuelve tamano original, available refleja bytes pendientes de leer. Compilado/cargado con hash verificado, RAM 35516 / 81920, programa 516175 / 1044464. SHA256 02e8cc800e4ac1b75d6a130b9414c77646f83bc394dcc9b57a7dfde967c81b22. Verificacion final por LAN PASS: HTML exacto, contador correcto, AP/DNS activos, router conectado, calibracion 42.04 / 300 conservada. COM9 libre y PC en sic5. Pendiente uso prolongado y aceptacion visual de olas/pixeles residuales.

Informe consolidado .pio/validacion/irq_led_resumen_final.json; verificacion de ultima carga irq_led_verificacion_final.json; guia detallada DEBUG_SERIE.md. Cada variante conserva su ELF/BIN/fuentes para decodificar sus propios fallos; la ultima esta en .pio/backups/irq-led-timeout-final-20261017/.

# Recuperacion AP/STA y portal - 17/10/2026

Recuperar HTTP/DNS desde el loop tras desconexion STA o nueva IP, con agrupacion de eventos de 250 ms. Prioridad a clientes AP durante reintentos automaticos, margen de 15 s despues de salir el ultimo cliente; guardar WiFi o comando w permite un intento expreso. Comandos serie a (solo AP temporal) y w (reanudar red guardada), sin escribir preferencias. Las consultas d muestran RESET ANTERIOR para no confundirlas con un nuevo arranque. HTTPS pospuesto mientras se usa el portal y reloj del cooler independiente del envio pendiente.

Compilacion SUCCESS: RAM 35500 / 81920; programa 515459 / 1044464. Siete comprobaciones RTC PASS. SHA256 a12a852330092f265f25da73583d0f4979b23e68e9e6234267831c0001fcc373. ELF/BIN/config y fuentes archivados en .pio/backups/recuperacion-ap-20261017/.

Carga COM9 con hash verificado y prueba real PASS: 29 respuestas HTTP 200, cero errores transitorios y cero reinicios durante la prueba. PC con IP 192.168.4.100 por DHCP, DNS cautivo correcto antes y despues de reanudar STA, pagina exacta al HTML local. Router red_de_prueba conectado con IP 192.168.1.146 y AP Airbit-66FF61 activo simultaneamente. Preferencias de iluminacion/intensidad sin cambios; calibracion valida 42.04 ADC / 300 muestras. Red original sic5 del PC restaurada y COM9 cerrado al terminar. Esta prueba corta no demuestra resueltos todos los reinicios intermitentes.

Informe: .pio/validacion/recuperacion_ap_prueba.json; registro serie: logs/recuperacion-ap-20261017.log. Antes del ajuste el PC no logro asociarse al AP mientras el firmware seguia midiendo y repitiendo errores STA 15/202; informe .pio/validacion/ap_estado_actual.json. El registro previo conserva una Exception 29 historica recuperada con d, sin stack completo, y una pausa HTTPS de 10018 ms. No atribuir una causa raiz unica a esos fallos.

# Diagnostico serie y watchdog capturados - 16/10/2026

Incorporado diagnostico por serie a 115200: motivo de arranque y registros de excepcion, huella RTC anterior con CRC, resumen de heap/bloque/fragmentacion/pila/red cada 5 s, comandos d/v/h y trazas de web, escrituras EEPROM, BMP y HTTPS. Las consultas normales de estado se detallan al activar v; errores y operaciones lentas quedan visibles. Se omiten claves, cuerpos/parametros HTTP y URL completa del formulario. No se desactivo el watchdog ni se agregaron reinicios automaticos. Huella de 48 B en RTC desde byte 128, sin escrituras propias de diagnostico en EEPROM. La captura alternativa esta en scripts/monitor_debug.py.

PlatformIO configura DTR/RTS inactivos, filtros esp8266_exception_decoder/time/log2file, DEBUG_ESP_OOM y build_type debug con -g3 -Os. Final compilado: RAM 35.444 / 81.920 y programa 512.467 / 1.044.464 bytes. Carga COM9 por esptool con hash verificado; SHA256 4a7b99f17ddb9bd1c753d092cd1a517c56f0940d476a0cbd68d87295628931e2. ELF/BIN/config finales conservados en .pio/backups/diagnostico-serie-20261016-simbolos/. Addr2line resuelve loop a src/main.cpp:707. Monitor real abierto con filtros, log automatico y sin advertencia de simbolos; cerrado al terminar para dejar COM9 disponible.

Pruebas: siete comprobaciones constexpr del registro RTC PASS; capturador Python compilado y ejecutado; comandos d/v/h probados en hardware; HTTP 200/403 y commits trazados; RTC valida tras reset externo intencional. Prueba final corta: 22 solicitudes, cero errores HTTP, cero reinicios y calibracion conservada 42,040 ADC / 300 muestras. API existente PASS (pagina/estado 200, sin token 403 y datos invalidos 400). La comprobacion corta no demuestra que el fallo intermitente este resuelto.

En la prueba anterior de tres minutos, con recargas y navegador real abierto, se registraron dos Software Watchdog reales (codigo SDK 3). EPC1 0x402564d2 en ambos, resuelto con el ELF correspondiente como pm_send_nullfunc del SDK Wi-Fi; contexto sys y huella anterior LOOP, con 25.256 y 19.240 B libres en las ultimas muestras. No se ha demostrado el desencadenante exacto ni aplicado una correccion al Wi-Fi por esa hipotesis. Guia: DEBUG_SERIE.md.

Registro real de los dos fallos: logs/airbit-validacion-debug-20261016-195741.log. Decodificacion: .pio/validacion/reinicio_watchdog_decodificado.txt. ELF de esos fallos: .pio/backups/diagnostico-serie-20261016/. La prueba inicial termino con una asercion del arnes al esperar verbose OFF, porque un reinicio habia restablecido el modo; se corrigio la suposicion del arnes. Informes conservados separados de la prueba final: diagnostico_serie_primera_prueba.json, watchdogs_web_resumen.json y diagnostico_serie.json, en .pio/validacion. El navegador real completo 30 cambios de pesta?a y recargas sin acciones de escritura ni excepciones JavaScript.

Durante la captura previa a la carga la IP LAN no respondia, pero por serie seguian mediciones normales, sin calibracion ni un arranque nuevo en esos 12 s. El primer build fallo por carpeta de objetos ausente tras una limpieza de PlatformIO en G:; al recrearla y repetir se compilo. La carga nobuild de PlatformIO no encontro el LD script; se uso esptool con los parametros de su builder y el binario ya validado. No se borraron la flash completa ni los ajustes.

# Texto central blanco suave - 16/10/2026

El estado del circulo usa #F8FFFD fijo con sombra, en lugar de alternar texto oscuro/claro. JavaScript existente PASS. Capturas locales a 320/390/1400 px y vista conjunta de once colores revisadas: .pio/validacion/texto-claro-*.png. Vista previa con datos simulados, sin escritura en placa. Compilacion y carga COM9 SUCCESS con hash verificado: RAM 34.980 / 81.920, programa 505.295 / 1.044.464 bytes. HTTP real confirma pagina actual con texto claro y sombra.

Calibracion conservada 42,040 ADC / 300 muestras y calidad al 90 %. Se observo iluminacion manual activa con intensidad 2 % antes y 26 % despues; el color [0,25,255] se mantuvo. No se escribieron preferencias desde estas pruebas ni se revirtio el valor observado. No se atribuye su cambio al ajuste visual sin evidencia. Estado final en .pio/validacion/texto_claro_estado_final.json. COM9 cerrado.

# Aro de calidad uniforme - 16/10/2026

La causa era .orb::after: bordes superior/derecho de 2 px, con el resto transparente. Se desactiva solo en quality-orb; se conserva el halo al 48 % y el aro completo. Prueba JavaScript existente PASS. Capturas locales a 320/390/1400 px, movil revisada. Compilado y cargado por COM9 con hash verificado: RAM 34.980 / 81.920, programa 505.535 / 1.044.464 bytes. HTTP real confirma HTML actual con el borde parcial desactivado y el halo conservado. Ajustes y calibracion iguales a la lectura previa. COM9 cerrado.

# Pista gris clara y halo reforzado - 16/10/2026

Pista de sliders #D4DFE2, relleno/cursor verdes, estilos WebKit y Firefox. Halo de calidad 48 % de opacidad y borde exterior blanco 70 %. JavaScript existente PASS, con fixture adaptado al metodo DOM style.setProperty. Navegador Chrome local: capturas a 320/390/1400 px revisadas, sin desbordamiento; halo calculado rgba(33,150,243,0.48). La lectura de estilo del pseudoelemento de pista no expuso el gradiente en Chrome: la pista se verifico visualmente en las capturas. Vista previa a 35 % solo en navegador, sin escritura en placa.

Compilado y cargado por COM9, hash verificado: RAM 34.980 / 81.920, programa 505.503 / 1.044.464 bytes. HTTP real confirma pagina actual y estilos presentes; preferencias y calibracion iguales a la lectura previa. COM9 cerrado. Capturas: .pio/validacion/barra-halo-*.png.

# Titulo final Intensidad de la luz - 16/10/2026

Titulo actualizado a Intensidad de la luz, manteniendo retirada la etiqueta inferior duplicada. JavaScript existente PASS. Compilado y cargado por COM9, hash verificado: RAM 34.980 / 81.920, programa 504.463 / 1.044.464 bytes. HTTP real confirma pagina actual y titulo exacto; preferencias y calibracion conservadas. COM9 cerrado.

# Etiqueta duplicada de intensidad retirada - 16/10/2026

Retirado el texto inferior Intensidad de la luz. Se conserva el titulo Intensidad de luz y el porcentaje a la derecha; el slider usa el titulo como nombre accesible. Prueba JavaScript existente PASS. Compilacion y carga COM9 SUCCESS, hash verificado: RAM 34.980 / 81.920, programa 504.463 / 1.044.464 bytes. HTTP real confirma pagina actual y etiqueta retirada, con ajustes de luz y calibracion conservados. COM9 cerrado.

# Tarjeta Intensidad de luz y retirada de aclaracion - 16/10/2026

Eliminada la aclaracion Esta escala es relativa a tu referencia. No indica ppm ni AQI. Titulo actualizado a Intensidad de luz e icono izquierdo retirado. Prueba JavaScript existente PASS. Compilacion/carga COM9 SUCCESS, hash verificado: RAM 34.980 / 81.920, programa 504.415 / 1.044.464 bytes. HTTP real confirma HTML actual, titulo y ausencia de aclaracion/icono. Preferencias de luz y calibracion iguales a la lectura previa. COM9 cerrado.

# Etiquetas bajo la barra de colores - 16/10/2026

Izquierda: Aire de montaña. Derecha: Toca renovar. Prueba JavaScript existente PASS. Compilado y cargado por COM9 con hash verificado: RAM 34.980 / 81.920, programa 504.687 / 1.044.464 bytes. HTTP real confirma HTML actual y las dos etiquetas exactas. Preferencias y calibracion iguales a la lectura previa. COM9 cerrado.

# Limpieza de textos de calidad de aire - 16/10/2026

Retirados el parrafo introductorio La lectura de tu MQ135..., Nivel x de 11 dentro del circulo y Segun tu referencia de aire limpio debajo. Se retiro la referencia JavaScript al elemento eliminado. Prueba de pagina existente PASS. Compilado y cargado por COM9 con hash verificado. RAM 34.980 / 81.920; programa 504.703 / 1.044.464 bytes. HTTP real confirma pagina identica al HTML editable, textos ausentes y elemento de estado conservado; AP activo y Wi-Fi conectado. La consulta previa a la carga tuvo timeout y no genero una captura valida; el estado final se leyo en .pio/validacion/limpieza_textos_estado_final.json. Cambio limitado a interfaz. COM9 cerrado.

# Centro transparente en la a del logo - 16/10/2026

Sustituido el circulo blanco por un hueco vectorial evenodd en la copia web del SVG. Prueba de navegador: alfa 0 en centro y 255 en contorno. Capturas a 320/390/1400 px revisadas; JavaScript existente PASS. Compilacion y carga COM9 SUCCESS, hash verificado. RAM 34.980 / 81.920; programa 505.087 / 1.044.464 bytes. HTTP real confirma HTML actual con el recorte; ajustes de luz y calibracion iguales a la lectura previa, AP activo y Wi-Fi conectado. COM9 cerrado.

# Cabecera con logo original Airbit - 16/10/2026

Sustituido el simbolo de brisa y el nombre tipografico por el logotipo original vectorial de la misma carpeta que airbit.jpg. Se conserva la frase debajo. SVG integrado, sin solicitudes de imagen adicionales. Prueba JavaScript existente PASS; navegador a 320/390/1400 px, sin duplicado, solapes ni desbordamiento. Capturas movil y escritorio revisadas: .pio/validacion/logo-airbit-*.png.

Compilado y cargado por COM9 con hash verificado. RAM 34.980 / 81.920; programa 506.623 / 1.044.464 bytes. HTTP real confirma pagina identica al HTML editable, logo y frase presentes; preferencias y calibracion iguales a la lectura previa, AP activo y Wi-Fi conectado. La primera peticion tras el reinicio tuvo timeout; el reintento respondio y completo la validacion. COM9 cerrado.

# Orden del menu y pantalla inicial - 16/10/2026

Menu: Calidad de aire, Iluminacion, Calibracion, Wi-Fi. Inicio en Calidad de aire, visible y seleccionada desde el HTML inicial. Prueba JavaScript existente PASS. Compilado y cargado por COM9, hash verificado. HTTP real confirma pagina actual, orden y unica seccion inicial; calibracion y preferencias conservadas, AP activo y Wi-Fi conectado. Informe: .pio/validacion/orden_menu.json.

# Estados de calidad de aire y paleta de once colores - 16/10/2026

Indice numerico n / 1000 trasladado a una tarjeta junto a Lectura actual (ADC). El circulo muestra el texto y nivel de la tabla solicitada, con el color de calidad. La paleta del firmware/LED y el gradiente de la pagina usan los once HEX indicados. Puntos 0,100,...,1000, interpolacion RGB continua y estado por cercania (cortes 50,150,...,950). Se conserva la formula relativa, la calibracion, el suavizado y el control independiente de intensidad.

Pruebas: 14 comprobaciones C++ del modulo real (incluidos los once HEX, limites, continuidad y filtro) y pagina JavaScript PASS. Navegador con datos simulados: 44 vistas, once estados a 320/390/850/1400 px; tarjetas juntas, textos dentro del circulo y contraste >=4,5 en los once puntos. Capturas movil y escritorio revisadas, incluidos Aire de montana y el texto mas largo. Informes en .pio/validacion/aire_estados_visual.json y aire-estados-*.png. La simulacion visual no modifica el sensor ni el equipo.

Compilado y cargado por COM9, hash verificado: RAM 34.980 / 81.920; programa 496.783 / 1.044.464 bytes. Verificacion HTTP real: pagina servida identica al HTML editable; indice 0, nivel 1, estado Aire de montana, RGB [33,150,243]. Comprobada coherencia entre indice/nivel/texto/color. Estado final en .pio/validacion/estados_aire_final.json.

Conservados los ajustes previos a la carga: intensidad de calidad 91 %, modo automatico, color manual [0,25,255], intensidad manual 2 %, referencia 42,040 ADC / 300 muestras y Wi-Fi conectado/AP activo. No se cambiaron credenciales ni se iniciaron calibraciones. COM9 cerrado. La reproduccion fisica de los nuevos tonos en los LED queda para confirmar por el usuario.

# Intensidad del fondo de calidad de aire - 16/10/2026

Agregado slider 0..100 % en la tarjeta Opciones de la escala, independiente de iluminacion manual. Valor inicial/final 25 %, equivalente al factor 64 anterior. Vista previa en RAM al arrastrar y guardado al soltar. Registro CRC independiente en EEPROM 216..231; detalles en [INTENSIDAD_CALIDAD.md](INTENSIDAD_CALIDAD.md).

Compilacion y carga COM9: SUCCESS, hash verificado. ESP8266EX MAC f4:cf:a2:66:ff:61. RAM 34.724 / 81.920; programa 494.503 / 1.044.464 bytes. Pruebas locales: 9 comprobaciones C++ de iluminacion/calidad y pagina JavaScript, incluyendo edicion preservada, polling anterior, cola entre los dos modos y reintento sin falsa confirmacion.

Prueba real HTTP: token ausente 403, valores invalidos 400, 37 % guardado y recuperado tras reinicio, preview 0 % aplicado sin persistencia y 100 % aceptado. Navegador real: slider 48 % confirmado por la placa. Capturas revisadas en movil 390 y escritorio; sin desbordamiento a 320, 390 y 1400 px. Informe y capturas: .pio/validacion/intensidad_calidad.json y calidad-intensidad-*.png.

Restaurado 25 %. Iluminacion del usuario conservada: automatico, RGB manual [0,25,255], intensidad manual 2 %. Referencia conservada 42,040 ADC / 300 muestras. Wi-Fi conectado y AP activo; no se iniciaron calibraciones ni se cambiaron credenciales. COM9 cerrado. La apariencia y respuesta fisica de los LED queda para confirmar en el equipo; las pruebas demuestran estado, control y persistencia.

# Recuperacion de comunicacion con la app - 14/10/2026

El usuario informo Failed to fetch, cierre de la ventana y perdida de acceso al AP tras activar iluminacion. Observacion inicial por COM9 sin reset: sensores seguian activos y no se vio un reinicio; STA reintentaba y termino en timeout del handshake WPA (SDK 15). No prueba una contrasena del router cambiada.

Lectura del sector EEPROM por esptool: CRC validos de Wi-Fi/AP, iluminacion y calibracion. AP conserva 1234567890. Ajuste encontrado: modo manual blanco 50 %. Referencia 42,040 ADC / 300 muestras. El reinicio que hizo esptool recupero STA sin cambiar ajustes. El usuario confirmo fuente separada para los LED; no se atribuye el fallo a alimentacion sin medicion.

Correccion: autoreconexion SDK desactivada; intentos STA acotados a 15 s y suspendidos al fallar. AP se mantiene al desconectar STA, sin borrar credenciales. Los nuevos intentos automaticos solo empiezan sin clientes AP. Guardar red inicia un intento explicito. AP admite cuatro clientes en lugar de dos. Estado JSON incluye clientes/canal. La app traduce TypeError de fetch y conserva los ajustes para Reintentar.

Compilacion/carga COM9: SUCCESS, hash verificado. RAM 34.564 / 81.920; programa 489.679 / 1.044.464 bytes. Prueba JavaScript: PASS, incluido error Failed to fetch y reintento con eleccion conservada. El error se represento en la misma clase TypeError del contexto de navegador.

Prueba real por AP: el PC recibio 192.168.4.100; raiz y estado HTTP 200; modificar/guardar intensidad y restaurarla HTTP 200. Se simulo una red de router inexistente conservando su clave; el AP siguio respondiendo durante el fallo y despues de 15 s, sin errores transitorios en las peticiones. Se restauro el SSID original, la iluminacion y la red del PC. Estado final por LAN: Wi-Fi conectado, AP/cautivo activos, blanco manual 50 %, calibracion conservada. COM9 cerrado.

La primera prueba AP habia intentado HTTP antes de obtener DHCP; no se considera prueba de servidor caido. La prueba definitiva espero direccion IPv4 y completo lectura/escritura y fallo de STA. Informe: `.pio/validacion/ap_recuperacion.json` y estado final `.pio/validacion/recuperacion_estado_final.json`.

No se afirma haber demostrado el desencadenante exacto del primer corte ni la recuperacion en el telefono del usuario. Se verifico el AP real, el guardado de luz y la proteccion con STA fallida; falta repetir el uso desde el telefono.

# Iluminacion manual con rueda e intensidad - 14/10/2026

Switch Usar como iluminacion, rueda cromatica con blanco central, selector nativo accesible y slider 0..100 %. Modo apagado recupera calidad de aire; encendido sustituye el fondo con RGB/intensidad manuales. Olas y prioridades de calibracion/arranque se conservan. Detalle: [ILUMINACION_MANUAL.md](ILUMINACION_MANUAL.md).

Registro de 20 bytes con CRC en EEPROM 196..215, independiente de configuracion Wi-Fi/AP y calibracion. Preview durante el gesto solo RAM; guardar al soltar/cambiar, evitando commits por frame o por evento de arrastre. Cola de peticiones con ultima eleccion y proteccion ante polling atrasado; errores conservan la seleccion y ofrecen Reintentar.

6 pruebas constexpr de registro, limites, CRC, escalado y no solapamiento: PASS. JavaScript real: PASS, incluido modo manual, color, centro blanco de rueda, intensidad, cambios en cola, guardado al soltar, polling anterior, confirmacion y reintento; siguen pasando Wi-Fi y calibracion.

Compilacion y carga COM9: SUCCESS; esptool verifico el hash. RAM 34.444 / 81.920; programa 489.175 / 1.044.464 bytes (46,8 %).

Pruebas en la placa: peticion sin token 403; intensidad fuera de rango/negativa/no numerica/vacia 400. Preferencia manual recuperada tras reinicio. Un preview distinto con guardar=0 desaparecio al reiniciar, recuperando el ultimo ajuste guardado. En Chrome contra el portal real: rueda mediante eventos de puntero cambio a rojo; slider guardo 37 %; switch regreso a modo de calidad conservando RGB/intensidad. Vista 390 px revisada, 320 px sin desbordamiento y escritorio 1400 px revisado.

Un intento de prueba tuvo timeout despues de reiniciar la placa con conexiones HTTP reutilizadas. El verificador se ajusto para abrir conexiones nuevas; la comprobacion completa posterior paso. La asercion inicial de escritorio tambien se ajusto para admitir el ancho reservado por la barra de scroll. Son incidencias del verificador; no se afirma una modificacion de firmware para esos casos.

Se restauraron las preferencias iniciales: modo automatico, blanco manual, intensidad 50 %. Referencia actual 42,040 ADC / 300 muestras conservada; AP, cautivo y Wi-Fi activos. COM9 cerrado. No se afirma una observacion visual de la tira LED; se verificaron el compositor por codigo/escalado y los controles/estado reales.

Informe: `.pio/validacion/iluminacion_manual.json`. Capturas: `app/previews/iluminacion-manual-movil.png` y `app/previews/iluminacion-controles-escritorio.png`.

# Inicio de calibracion desde la app - 14/10/2026

Nuevo boton Iniciar calibracion en la tarjeta Controles web. Inicia la funcion existente `iniciarCalibracionAire()` mediante POST `/api/calibracion`: cinco minutos, 1 Hz, verde respirando y guardado al completar la ventana valida. No se reimplementa el promedio ni el registro EEPROM.

JavaScript real y PROGMEM: PASS. Se verificaron doble click, proceso fisico activo, confirmacion explicita, errores, falta de sesion y lectura de estado anterior a la peticion. El boton se bloquea durante el inicio/proceso y se rehabilita cuando termina.

Compilacion y carga COM9: SUCCESS; hash verificado por esptool. RAM 34.192 / 81.920 bytes; programa 477.139 / 1.044.464 bytes.

Prueba en el portal real, con Chrome a 390 px: tarjeta y boton sin desbordamiento; click en el boton HTTP 202, doble click produjo una unica peticion, estado activo y cuenta de 298 segundos, boton Calibracion en curso deshabilitado. Un segundo POST valido devolvio 409 sin reiniciar la ventana; tiempo restante observado 297 segundos. POST sin token y con token incorrecto devolvieron 403; GET devolvio 404 sin iniciar el proceso.

La prueba de inicio duro unos segundos. El script reinicio la placa antes del fin de cinco minutos para conservar la referencia: 71,757 ADC / 300 muestras, confirmada antes y despues. Estado final: sin calibracion activa, Wi-Fi/AP/cautivo disponibles y COM9 cerrado. No se hizo una recalibracion completa ni una comprobacion visual de los LED en este paso.

Informe local: `.pio/validacion/calibracion_web.json`. Capturas del portal real: `app/previews/calibracion-web-lista.png` y `app/previews/calibracion-web-activa.png`. Procedimiento: [CALIBRACION_MQ135.md](CALIBRACION_MQ135.md).

# Interfaz inspirada en la referencia de app - 14/10/2026

Estilo claro en turquesa y menta, halos circulares, tarjetas suaves y navegacion flotante movil. Se conserva el JavaScript anterior identico, confirmado por comparacion de su texto. Descripcion y capturas: [DISENO_INTERFAZ.md](DISENO_INTERFAZ.md).

Pagina/PROGMEM y prueba JavaScript: PASS. Chrome: 320, 390 y 1400 px, los cuatro apartados sin desbordamiento ni recursos externos. Se corrigieron el contraste secundario y el fondo completo despues de las capturas iniciales, y se reviso la version final.

Compilacion y carga COM9: SUCCESS; esptool verifico el hash. RAM 34.068 / 81.920 bytes; programa 474.263 / 1.044.464 bytes (45,4 %). HTTP real tras cargar: 200, HTML final identico (28.993 bytes), sin autenticacion web. AP y cautivo activos, Wi-Fi conectado. Calibracion actual 71,757 ADC / 300 muestras conservada, verificada contra el estado leido antes de cargar. No se inicia ni modifica una calibracion en este cambio.

La herramienta de navegador interactivo no inicio por un error del sandbox de su kernel. La revision se completo con un Chrome aislado y oculto, mediante emulacion real de los anchos anteriores. La primera captura CLI tenia un viewport minimo de 500 px; se detecto y se reemplazo por las capturas de emulacion de 390/320 px. No se confunde ese recorte del renderer con un fallo del CSS. Vista visual en telefono fisico pendiente del usuario.

# Portal cautivo cargado - 13/10/2026

Se incorpora DNSServer UDP/53 con comodin, IP AP 192.168.4.1 y TTL 0; las diez sondas HTTP registradas de Android/Apple/Windows/Firefox redirigen con 302 a la raiz de la interfaz utilizada. En AP, el host ajeno en raiz y los GET desconocidos tambien redirigen. Se conserva AP+STA, clave AP 1234567890 y portal sin autenticacion. El loop atiende DNS y HTTP, sin nuevas pausas.

Compilacion y carga COM9: SUCCESS; esptool verifico el hash del ESP8266EX. RAM 34.068 / 81.920; programa 459.767 / 1.044.464 bytes.

Prueba real `scripts/verificar_cautivo.py` por la IP domestica 192.168.1.146: cuatro nombres DNS, incluidos Android, Apple, Microsoft y un nombre inexistente, resolvieron a 192.168.4.1; las diez sondas devolvieron 302 con Location correcto y no-store. Pagina y estado HTTP 200 sin contrasena, estado cautivo activo, Wi-Fi conectado y calibracion 46,867 ADC / 300 muestras conservada. Informe `.pio/validacion/portal_cautivo.json`.

El primer intento concurrente de los dos verificadores tuvo tiempos de espera HTTP de 7 s. El verificador cautivo completo paso al repetirlo secuencialmente; las comprobaciones finales se ejecutan una por una. No se atribuye ese primer timeout a una causa no demostrada.

Pendiente de validacion en telefono: reconectar a Airbit y comprobar la notificacion/ventana automatica. No se ha cambiado el Wi-Fi del PC al AP; las ramas de host/ruta genericos de esa interfaz y la ventana cautiva no se dan por probadas. El acceso manual sigue en http://192.168.4.1.

# Historico: portal sin contrasena cargado - 13/10/2026

El usuario confirmo que pudo conectar al AP y solicito eliminar el acceso con usuario/contrasena del portal. Se retiro HTTP Basic en pagina, estado, guardado Wi-Fi y rutas desconocidas. La clave de la red `Airbit-66FF61` sigue siendo `1234567890`. El portal tambien abre directamente desde la red del router. Se mantienen el token de sesion para guardar y las cabeceras existentes.

Compilacion y carga COM9: SUCCESS; esptool verifico el hash. RAM 33.684 / 81.920; programa 454.251 / 1.044.464 bytes. Verificacion HTTP real sin cabecera Authorization: pagina y estado HTTP 200, sin WWW-Authenticate. Guardado sin token HTTP 403; SSID vacio y clave corta HTTP 400, sin modificar la red. Referencia MQ135 46,867 ADC / 300 muestras conservada. COM9 cerrado. Informe: `.pio/validacion/portal_http.json`.

# Historico: clave AP simplificada y cargada - 13/10/2026

Por solicitud del usuario, red `Airbit-66FF61` y portal usan `1234567890`; usuario web `airbit`. La constante `claveApPredeterminada` en `include/configuracion_airbit.h` permite cambiarla en el firmware. Al arrancar se migra solo la clave AP y se conserva la red Wi-Fi guardada. No se escribe en cada arranque si ya coincide.

13 pruebas constexpr: PASS, incluida clave de 10 caracteres, limites y conservacion de SSID/clave Wi-Fi. Compilacion y carga COM9: SUCCESS; esptool verifico el hash. RAM 33.748 / 81.920; programa 458.051 / 1.044.464 bytes.

Verificado tras reiniciar: AP activo, clave solicitada recuperada, Wi-Fi conectado. Endpoint real de estado: clave nueva HTTP 200; clave anterior HTTP 401. Referencia MQ135 46,867 ADC / 300 muestras conservada. Informe local: `.pio/validacion/cambio_clave_ap.json`. COM9 cerrado. Asociacion del telefono al AP pendiente de comprobar por el usuario.

# Histórico: AP y configuración Wi-Fi cargados — 13/10/2026

El usuario pidió un AP para Wi-Fi y los apartados de calibración, iluminación con color y calidad de aire, dejando la definición de las funciones para otro paso. Confirmó AP siempre disponible. Implementado AP+STA, portal local autenticado y guardado Wi-Fi en EEPROM; las funciones actuales del equipo se conservan y los apartados muestran su estado.

Respaldo de la versión confirmada por el usuario: fuente `.pio/backups/main.antes_ap_config.cpp` y binario `.pio/backups/firmware.antes_ap_config.bin`, SHA256 `596DF552C499F57C66A7085BBA1C1051C592476C6AC2F0EEF94C41F49EC857D0`.

Pruebas previas:

- 11 comprobaciones constexpr del registro AP/Wi-Fi: PASS, salida 0.
- JavaScript real del portal con respuestas de prueba: PASS, salida 0. Verificados HTML/PROGMEM sincronizados, estado, navegación, edición no sobrescrita por polling, token, contraseña no prellenada y confirmación explícita del guardado.
- Revisión del cambio de `src/main.cpp`: solo incluye del portal/configuración, ampliación EEPROM, AP+STA, selección de credenciales y atención del servidor en loop. Sensor, gradiente, olas, calibración, ventilador y formulario existentes no se reescriben.

Compilación y carga: SUCCESS, salida 0. RAM 33.744 / 81.920 bytes (41,2 %); programa 458.023 / 1.044.464 bytes (43,9 %). Esptool identificó el ESP8266EX MAC f4:cf:a2:66:ff:61, escribió 462.176 bytes y verificó el hash.

Comprobación física por serie:

- `AP configuracion: Airbit-66FF61 (activo)`.
- AP en `http://192.168.4.1`.
- Clave AP generada y guardada; no se registró la contraseña del router en las salidas de prueba.
- Wi-Fi doméstico conectado; IP observada `192.168.1.146` (dirección DHCP que puede cambiar).
- Referencia MQ135 **46,867 ADC / 300 muestras** conservada.

Prueba HTTP real en esa IP, sin proxy:

| Prueba | Resultado |
| --- | --- |
| Estado sin autenticación | 401 |
| Página autenticada | 200 |
| Estado autenticado, JSON válido | 200 |
| Guardado sin token | 403 |
| SSID vacío | 400 |
| Clave de 3 bytes | 400 |
| Guardado de la misma red vigente, conservando su clave | 200 y confirmación de flash |
| Reconexión posterior | Confirmada; AP y calibración conservados |

Se probaron las mismas credenciales vigentes, sin seleccionar otra red. El formulario en navegador no se usó para esa prueba: se comprobó el endpoint HTTP real y, por separado, el JavaScript real con respuestas de prueba.

Después de guardar se reinició la placa mediante el control serie. Se recuperó la misma clave AP, volvió a conectar y respondió HTTP 200 con la referencia **46,867 ADC / 300 muestras**. El puerto COM9 quedó cerrado. La metadata de acceso queda localmente en `.pio/validacion/acceso_portal.json`, ignorada; incluye solo la clave del AP y direcciones, no la contraseña del router. El informe HTTP sin claves queda en `.pio/validacion/portal_http.json`.

La herramienta de navegador falló dos veces al iniciar con `trusted Node process exited unexpectedly; kernel reset`. No se obtuvieron capturas ni se afirma una validación visual. Pendiente: abrir la página en PC/teléfono, comprobar distribución y navegación, y asociar un cliente al AP. No se han definido aún acciones web de calibración/iluminación/calidad, según el alcance pedido.

El primer lector serie también falló por la codificación CP1252 ante bytes de arranque; se corrigió la salida a UTF-8 y se completaron las comprobaciones. No se interpreta ese error del script como un fallo de firmware.

SHA256 del binario cargado: `80F30C1485DCEE48CA499B99C45B2F26BB421FF243D928662EB49A6ADFF100B9`.

Guía de acceso, memoria y respaldo: [AP_CONFIGURACION.md](AP_CONFIGURACION.md).

---

# Histórico: escala continua MQ135 cargada — 13/10/2026

Implementada la escala de fondo en los 20 LED, confirmada por el usuario: azul → verde → amarillo → naranja → rojo. Los puntos relativos iniciales son referencia, +25 %, +50 %, +75 % y +100 % de ADC. Entre puntos se interpola RGB; el índice se filtra por muestras y cambia gradualmente en los frames de 20 ms. Es una escala relativa de señal, con umbrales elegidos para visualización; no se convierte a ppm ni AQI.

La lectura normal A0 ahora se toma a 1 Hz, independientemente de las olas. El fondo atenuado se compone bajo las olas; al terminar recupera su color en vez de apagarse. Calibración, rojo de arranque y giro azul conservan sus prioridades. Se conserva el formato EEPROM, Wi-Fi, GPIO/niveles, ciclo y botones del ventilador, y formulario histórico.

Pruebas: 12 comprobaciones constexpr PASS del módulo real (paleta, 1.001 niveles continuos, umbrales, límites numéricos, filtro, pausas acotadas, convergencia y reinicio). Compilación y carga: SUCCESS, salida 0. RAM 31.832 / 81.920 bytes (38,9 %); programa 411.243 / 1.044.464 bytes (39,4 %).

La primera compilación mostró un aviso de directorio temporal de build no vacío (WinError 145); continuó, recompiló el fuente nuevo y terminó correctamente. La carga también recompiló y terminó correctamente. No se borraron manualmente directorios ni se cambió el lugar del build.

El usuario cerró el monitor de PlatformIO antes de cargar. Esptool identificó el mismo ESP8266EX MAC f4:cf:a2:66:ff:61, escribió 415.392 bytes y confirmó el hash. No se ejecutó un borrado total de flash ni una calibración de prueba.

Observación serie durante 20 s, con DTR/RTS desactivados antes de abrir:

- Se recuperó una referencia válida de **46,867 ADC**, guardada previamente por la calibración. Esta lectura tras la actualización y el reinicio confirma la recuperación persistente.
- ADC actual observado: 38, 40, 46 y 48.
- Índice mostrado: 5, 8, 6 y 4 sobre 1000, muy próximo al extremo azul. El índice incorpora las muestras intermedias y los filtros; no representa solo la última lectura impresa.
- No se capturaron pulsaciones ni excepciones en esa ventana. El puerto se cerró al terminar.

Pendiente: confirmar visualmente el fondo, la suavidad y los cinco colores en la placa; comprobar la ola sobre el fondo y que lo recupera al terminar. La prueba de compilador no sustituye esa observación física. Se ha verificado la recuperación de la referencia, pero no se ha observado en herramientas el gesto y la respiración de la calibración completa.

SHA256 del binario cargado: 596DF552C499F57C66A7085BBA1C1051C592476C6AC2F0EEF94C41F49EC857D0

Guía y parámetros: [ESCALA_CALIDAD_AIRE.md](ESCALA_CALIDAD_AIRE.md).

---

# Histórico: calibración MQ135 implementada y cargada - 13/10/2026

El usuario confirm? MQ135 y calibraci?n en aire limpio. Se implement? el gesto de cuatro botones mantenidos 10 s continuos, muestreo A0 a 1 Hz durante 300.000 ms, promedio/rango, guardado EEPROM de un registro de 32 bytes con CRC32 y recuperaci?n al arrancar. Durante la calibraci?n los 20 LED respiran en verde con periodo de 3 s; las otras animaciones y tareas lentas quedan pospuestas. Se conserva Wi-Fi, GPIO/niveles, brillo global, ciclo autom?tico del cooler y formulario hist?rico. La referencia y variaci?n ADC preparan una futura escala; no se generan ppm ni AQI.

Pruebas del m?dulo real: 15 comprobaciones constexpr PASS usando el compilador Xtensa instalado. Se verific? tambi?n el ejecutor `scripts/test_calibracion.py`, salida 0. Cubren gesto exacto/cancelaci?n/rearme, arranque pulsado, rollover, espaciado y fin temporal, acumulaci?n y promedio, extremos, cobertura m?nima y corrupci?n/CRC.

Compilaci?n PlatformIO: SUCCESS, salida 0. RAM 31.732 / 81.920 bytes (38,7 %); programa 410.375 / 1.044.464 bytes (39,3 %).

Carga por COM9: SUCCESS, salida 0. Se identific? el ESP8266EX MAC f4:cf:a2:66:ff:61. Esptool escribi? 414.528 bytes y confirm? `Hash of data verified`, seguido del reinicio de la placa.

Observaci?n serie durante 20 s con DTR/RTS desactivados antes de abrir: ADC MQ135 42..49 y `MQ135: sin calibrar` aproximadamente cada cinco segundos. Esto confirma ejecuci?n del nuevo firmware y ausencia de una referencia v?lida en esa lectura; no se inici? una calibraci?n por software ni se escribieron valores de referencia de prueba. No se capturaron pulsaciones o eventos de calibraci?n durante la ventana. COM9 qued? cerrado.

Pendiente en la placa: comprobar el gesto f?sico de 10 s, la respiraci?n verde, las cinco min completas, mensaje `Calibracion guardada`, y reiniciar despu?s para comprobar `MQ135: referencia guardada` con el mismo promedio. Tambi?n comprobar cancelaci?n antes de 10 s, que no se repita con los botones todav?a mantenidos y recuperaci?n de las olas normales. No se presenta la prueba de l?gica/compilaci?n como validaci?n f?sica del ADC o de la persistencia.

SHA256 del binario cargado: 7E5885B23A1185AD8EE535FCA759FEE6BFECFABACFF17D38C5A06575418DCD98

Gu?a, mapa de EEPROM y l?mites: [CALIBRACION_MQ135.md](CALIBRACION_MQ135.md).

---

# Hist?rico: apagado residual, ajuste de transmisi?n cargado ? 12/10/2026

El usuario informa restos en el ?ltimo LED, especialmente rojo. Se protege la transmisi?n FastLED frente a interrupciones entre p?xeles y se aumentan sus reintentos acotados a cinco. Es una prueba de una posible causa, no una correcci?n f?sica confirmada. Se verific? que el resto del fuente es id?ntico al respaldo `.pio/backups/main.antes_transmision_led.cpp`.

Compilaci?n SUCCESS: RAM 30.932 / 81.920 bytes (37,8 %); programa 407.007 / 1.044.464 bytes (39,0 %).

Carga por COM9 FAILED: `PermissionError(13, 'Acceso denegado.', None, 5)` al abrir el puerto, antes de escribir flash. Se detectaron tres procesos de monitor PlatformIO (PID 18628, 10360 y 37900). El usuario cerr? el monitor y confirm? que el resto aparece en el LED f?sico 10, uno de los finales de la ola roja. No se finalizaron sus procesos.

Segundo intento de carga: SUCCESS, salida 0. Esptool escribi? 411.152 bytes, verific? el hash y reinici? la placa. Observaci?n serie durante 12 s: arranque normal, mediciones activas, temperatura corregida 24,40 ?C y ADC 56; WiFi conectado (estado=3) a los 6,52 s desde la apertura. COM9 qued? cerrado. No se capturaron pulsaciones en esta ventana.

SHA256 del binario cargado: 9A6E87E837F354420B6F57637DE8B44CDACD8B1F3EA4482668F1B39756A31F64

Pendiente: comprobar visualmente el apagado del LED 10 tras varias olas rojas. La duraci?n del resto no est? confirmada. Si persiste, comprobar la se?al de datos y alimentaci?n; la observaci?n serie no demuestra que esta prueba de transmisi?n resuelva el problema f?sico.

Detalles: [OLAS_LED.md](OLAS_LED.md).

---

# Hist?rico: pulsaciones repetidas, cola de olas cargada ? 12/10/2026

Se corrige la limitaci?n que reiniciaba la ola del mismo bot?n en cada pulsaci?n. Ahora se completa la activa y se reproducen las pendientes, con una cola independiente de hasta 16 por bot?n. El l?mite genera un aviso serie y descarta ?nicamente el efecto adicional. Se conservan el mapa, colores, tiempos, antirrebote, controles del cooler, indicadores de arranque y planificaci?n de tareas lentas. Copia previa: `.pio/backups/main.antes_cola_olas.cpp`.

Compilaci?n y carga por COM9: SUCCESS, salida 0. RAM: 30.932 / 81.920 bytes (37,8 %). Programa: 407.087 / 1.044.464 bytes (39,0 %). Esptool escribi? 411.232 bytes y verific? el hash.

Observaci?n serie durante 20 s, con DTR/RTS desactivados antes de abrir: contin?an las mediciones aproximadamente cada 5 s, temperatura corregida 24,70 ?C y ADC 58..66. No se capturaron eventos `ON S`, mensajes de excepci?n ni reinicios en esa ventana. El puerto qued? cerrado. Esto confirma ejecuci?n tras la carga; no valida todav?a la respuesta a una r?faga real de pulsaciones ni el comportamiento visual.

El usuario no hab?a comprobado el monitor cuando observ? el supuesto cuelgue. Pendiente: pulsar cinco veces un bot?n y comprobar cinco `ON S n` y cinco olas completas; probar otro bot?n mientras se vac?a la cola. Si la placa deja de responder o vuelve a imprimir `Inicio`, investigar ese bloqueo/reinicio por separado. El antirrebote requiere 30 ms de estabilidad al pulsar y al soltar; las operaciones s?ncronas BMP/HTTPS ya iniciadas pueden retrasar la detecci?n.

SHA256 del binario cargado: DFADAAC9D61AD067086D5A726DEEFAEF3EAF7ECEE012BBDFD04D8A88FBFC3684

Detalles: [OLAS_LED.md](OLAS_LED.md).

---

# Hist?rico: indicadores de arranque y Wi-Fi cargados — 12/10/2026

Implementados en `src/main.cpp` y cargados por COM9:

- LED 16 rojo durante 5.000 ms, con prioridad sobre otros colores durante ese tiempo.
- Ola azul por el perímetro durante la conexión. Se apaga a los 3.000 ms de detectar conexión Wi-Fi.
- Los temporizadores son independientes; si el azul termina antes, el rojo completa sus 5 s.
- Se conserva el efecto de los botones, el ciclo del cooler, las credenciales y el envío al formulario.

Compilación y carga: SUCCESS, salida 0. RAM: 30.884 bytes (37,7 %). Programa: 406.983 bytes (39,0 %). Esptool escribió 411.136 bytes y confirmó `Hash of data verified`.

Comprobación serie tras reiniciar: aparece el mensaje de los nuevos indicadores y `WiFi: Conectado (estado=3)` a los 6,48 s desde la apertura del monitor. COM9 quedó cerrado. Esta comprobación confirma el arranque y la conexión, no una observación física de los colores o su duración.

Pendiente: confirmar visualmente la secuencia de LED y su convivencia con las olas de los botones.

Se propuso después dar prioridad inicial al indicador durante un máximo de 15 s, retrasando las mediciones. La revisión automática rechazó esa acción por exceder la autorización de la secuencia solicitada. La acción no llegó a ejecutarse y se comprobó que ese ajuste no está en el código. Se conserva el firmware cuya carga está verificada arriba.

SHA256 del binario cargado: DF1C1947F9B4E2FC3AA8E2EFB94DE1AA03741AEF0A417A2BF7F5DED746EE2B73

Detalles: [OLAS_LED.md](OLAS_LED.md).

---

# Histórico: grabación y arranque verificados — 12/10/2026

La apertura de COM9 falló con WinError 121 (tiempo de espera del semáforo), antes de escribir flash. Se reprodujo el fallo con una consulta chip_id. Windows enumeraba el CP210x como conectado y con estado OK, pero no podía abrir el puerto.

El usuario desconectó y reconectó el USB. Después se abrió COM9 correctamente y se identificó el mismo ESP8266EX de Airbit (chip ID 0x0066ff61).

Se ejecutó `platformio run --target upload`: SUCCESS, salida 0. Se escribieron 410.688 bytes en 0x00000000 y esptool informó `Hash of data verified`, seguido del reinicio por RTS.

Comprobación del arranque por serie a 115200 baudios:

- Aparecen `Inicio` y `Airbit listo`.
- Se imprimen lecturas de presión, temperatura y ADC. Ejemplo: presión 93.537 Pa, temperatura corregida 24,70 °C y ADC 63.
- Se observa `WiFi: Conectado (estado=3)` y RSSI -60 dBm.
- Se cerró COM9 al finalizar la consulta.

La reconexión USB recuperó la apertura del puerto; no se determinó una causa específica de cable o controlador. No se han verificado en esta carga la fluidez visual de las olas ni la recepción del formulario.

SHA256 del binario cargado: 0DDA84A5D589B3BAEDC56C9E3136866D143B4F51E499918C2F496DAC881778CD

---

# Histórico: prioridad de las olas — 12/10/2026

Se posponen las lecturas BMP y el inicio de HTTPS mientras haya olas activas. El cooler conserva su ciclo incluso si el envio debe esperar. Frecuencia programada: 50 frames/s. Compilacion SUCCESS (salida 0): RAM 30.820 bytes (37,6 %), programa 406.535 bytes (38,9 %). Verificado que efecto, botones, formulas de medicion y peticion no cambiaron. Pendiente cargar y comprobar la fluidez en la placa.

SHA256 del binario: 0DDA84A5D589B3BAEDC56C9E3136866D143B4F51E499918C2F496DAC881778CD

Detalle y limites: [OLAS_LED.md](OLAS_LED.md).

---

# Histórico: apagado reforzado de LED — 12/10/2026

El usuario confirma que las olas funcionan y reporta restos tenues intermitentes. Se envia negro explicito al terminar y se reenvia cada 100 ms en reposo. Se cambia el escalado de brillo a nscale8. Compilacion SUCCESS (salida 0): RAM 30.820 bytes (37,6 %), programa 406.503 bytes (38,9 %). Se verifico que el mapa, colores, velocidad, botones y resto del firmware se conservan. Pendiente cargar y comprobar el apagado fisico.

SHA256 del binario: 0DA7348124168AA0F9021AB4B2A7ED07BFB1BF53857948CF772CC73E1AC94511

Detalle: [OLAS_LED.md](OLAS_LED.md).

---

# Histórico: olas por pulsador — 12/10/2026

Implementadas en src/main.cpp. Compilacion SUCCESS (salida 0): RAM 30.820 bytes (37,6 %), programa 406.471 bytes (38,9 %). Mapa de cuatro esquinas verificado y bloques de WiFi, mediciones y envio conservados. Pendiente de cargar y validar visualmente en la placa.

SHA256 del firmware con olas: F8B139AD1E0FB2283075B137C56BB491BF8FD40399768D34E0D15AD2B8EFF258

Detalle: [OLAS_LED.md](OLAS_LED.md).

---

# Histórico: corrección de reinicios Wi-Fi — 12/10/2026

El usuario aportó un registro con lecturas de sensores seguido de `Connection Failed! Rebooting...` y un nuevo arranque. El código contenía la llamada explícita `ESP.restart()` después de fallar la conexión. Eso explica el reinicio observado; el registro todavía no identifica la causa de la conexión fallida.

Cambios actuales en `src/main.cpp`:

- Se elimina la espera bloqueante y el reinicio por fallo Wi-Fi.
- Se conecta en segundo plano y se solicita un reintento cada 30 s mientras no haya conexión.
- Se informa el estado de conexión y el motivo SDK de desconexión sin imprimir la contraseña.
- Al conectarse, se informa IP y RSSI.
- Sensores y pulsadores siguen ejecutándose sin Wi-Fi.
- Se omite el envío del ciclo de 10 min si no hay conexión; no se guarda para envío posterior.

Validación actual: `platformio run` terminó con SUCCESS, salida 0. RAM estática 30.700 / 81.920 bytes (37,5 %); programa 405.903 / 1.044.464 bytes (38,9 %).

El cambio corregido está compilado y pendiente de cargar y comprobar en la placa. Revisar localmente las credenciales antes de la carga. Tras cargar, capturar las líneas `WiFi:` y `WiFi desconectado:` para precisar el fallo. Eliminar el reinicio por software no garantiza por sí solo que la red se conecte ni descarta otros reinicios del hardware.

SHA256 del binario corregido: 4A157DA2697EBE658B5FC396E740B710927390A4780ABBB464D4BDF959853DC4

---

# Histórico: validación de la primera migración

Fecha: 12/10/2026.

## Resultado confirmado

- Acceso al proyecto y migración a `src/main.cpp` completados.
- COM9 identificado como Silicon Labs CP210x; esptool identifica ESP8266EX y flash de 4 MB.
- `platformio run`: SUCCESS; código de salida 0.
- Entorno: `airbit_esp8266`, placa `nodemcuv2`, modo flash DIO.
- Plataforma Espressif 8266 4.2.1, core Arduino ESP8266 3.1.2, compilador Xtensa 10.3.0.
- Bibliotecas: Adafruit BMP085 1.2.4, FastLED 3.6.0; dependencias resueltas Adafruit Unified Sensor 1.1.15 y Adafruit BusIO 1.17.4.
- RAM estática: 30.192 / 81.920 bytes (36,9 %).
- Programa: 403.907 / 1.044.464 bytes (38,7 % del espacio de aplicación).
- Artefactos: `.pio/build/airbit_esp8266/firmware.elf` y `firmware.bin`.
- Nota informativa de FastLED: no hay pines SPI hardware definidos. Los WS2812B del proyecto no utilizan SPI.
- Verificación automática: el contenido de `src/main.cpp` coincide con el `.ino` original salvo las inclusiones añadidas, las dos inclusiones sin uso retiradas y `analogWriteRange(255)`.
- SHA256 del original conservado: `734917ED1BFCAF8444A2DA44C9F9462A16B28EFE9F636F4C72D93B497E5E3A80`.

## Pendiente de prueba física

La migración se ha compilado; el firmware no se ha cargado en la flash de la placa. Las consultas esptool únicamente identificaron chip y memoria y reiniciaron la placa.

Antes de cargar, revisar las credenciales Wi-Fi conservadas en `src/main.cpp`. Después de la carga:

1. Abrir el monitor a 115200 baudios y comprobar inicio y conexión Wi-Fi sin reinicios.
2. Confirmar lecturas reales del BMP085/BMP180 y ADC de partículas.
3. Verificar la tira de 20 LED y el PWM del cooler.
4. Probar cada pulsador, en particular GPIO15 y GPIO10. La lógica histórica del pulsador 3 ya tenía una anotación de fallo.
5. Si se mantiene el envío histórico, comprobar el registro real en el formulario; la compilación no verifica la recepción de datos.

No se ha demostrado todavía el funcionamiento de sensores, pulsadores, LEDs, cooler, conexión Wi-Fi o recepción del formulario.

## Editor y artefacto

- Configuración VS Code regenerada con platformio project init --ide vscode --environment airbit_esp8266; resultado correcto.
- esptool image_info reconoce la imagen ESP8266 y valida su checksum.
- SHA256 del firmware.bin: 2C4D7FE2F11FA37DE08DAD71AF9ECC16205D06217A9159E37D9CDE32FCF06C45.
