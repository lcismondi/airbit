# Airbit

**Tu aire. Tu luz. Tu espacio.**

Airbit es un monitor ambiental y una luz para espacios interiores. Combina un sensor MQ135, medición de presión y temperatura, un perímetro de 20 LED y cuatro esquinas que funcionan como pulsadores. El color cambia suavemente según la señal del aire; también podés elegir un color fijo para iluminar el ambiente.

![Airbit: vista del conjunto](imagenes/airbitrender.png)

## Una experiencia de aire y luz

Airbit está pensado para acompañar un escritorio, una habitación o un espacio de trabajo. La información se expresa con una transición de colores y palabras cercanas, sin tener que mirar números todo el tiempo.

| El aire se ve | La luz se elige | Las esquinas responden |
| --- | --- | --- |
| El perímetro cambia de color según la señal relativa del MQ135. En el portal podés consultar la lectura y el estado. | Un color fijo y una intensidad a tu gusto convierten el mismo objeto en una luz ambiental. | Cada pulsación envía una ola por dos lados del cuadrado. La interacción física forma parte del objeto. |

El gabinete, la electrónica y el firmware se desarrollan como partes de la misma experiencia: las cuatro esquinas permiten tocar la luz y la rejilla integra el intercambio de aire en el diseño.

## Qué hace

- Muestra una escala relativa del MQ135 con once estados, desde **Aire de montaña** hasta **¡Un respiro, por favor!**.
- Permite usar los 20 LED como iluminación, con rueda de color y control de intensidad.
- Genera olas de luz desde cada esquina al pulsarla.
- Guarda una referencia de aire limpio mediante una calibración de cinco minutos.
- Ofrece un portal web para configurar WiFi, iluminación y calibración, sin instalar una app.
- Mantiene su punto de acceso disponible mientras intenta conectarse al router.
- Informa la versión del firmware y la MAC del dispositivo en la interfaz y en su API local.

El MQ135 entrega una señal analógica sensible al ambiente. La escala compara esa señal con la referencia guardada: **no mide partículas, no informa ppm ni un AQI oficial**. El BMP085/BMP180 aporta presión y temperatura; este montaje no mide humedad.

## Primer uso

1. Alimentá el equipo y buscá la red **Airbit-XXXXXX**.
2. Conectate con la clave **1234567890**. Si el teléfono avisa que no hay Internet, mantené la conexión.
3. Abrí el aviso de configuración del teléfono o entrá en **http://192.168.4.1**.
4. En **Wi-Fi**, guardá los datos de tu red de 2,4 GHz. El portal abre directamente; la clave se exige para entrar al AP.
5. Con el sensor estabilizado y en aire limpio, iniciá la calibración desde la pestaña **Calibración**. También podés mantener los cuatro botones durante diez segundos.
6. Esperá los cinco minutos de muestreo. Los LED respiran en verde y la referencia se guarda al finalizar una ventana válida.

El orden del menú es **Calidad de aire → Iluminación → Calibración → Wi-Fi**. La primera pantalla muestra el estado del aire, la lectura ADC, el índice relativo y la intensidad del fondo.

## La interfaz

El portal acompaña la luz del objeto con una interfaz clara, tarjetas suaves y controles de color. Se abre desde el navegador del teléfono o del ordenador.

| Calidad de aire | Iluminación con color |
| --- | --- |
| ![Portal de Airbit: calidad de aire](imagenes/portal-calidad-aire.png) | ![Portal de Airbit: iluminación con color](imagenes/portal-iluminacion.png) |

*Capturas de la interfaz v0.2.0 con datos de ejemplo. No representan una medición real ni la configuración de un dispositivo.*

## Recorrido por el repositorio

| Sección | Qué vas a encontrar |
| --- | --- |
| [Firmware](firmware/Readme.md) | Uso, controles, API, diagnóstico y relación entre el código actual y el histórico. |
| [Proyecto PlatformIO](firmware/platformio/README.md) | Código ESP8266, conexiones, compilación, pruebas y publicación. |
| [Hardware](hardware/Readme.md) | Esquema, PCB, alimentación, mapa de pines y comprobaciones del montaje. |
| [Gabinete](gabinete/Readme.md) | Concepto mecánico, piezas, disposición de los botones y vistas del conjunto. |
| [Imágenes](imagenes/README.md) | Render general y captura histórica de variables. |

## Firmware actual y descargas

La versión preparada es **v0.2.0**, desarrollada con PlatformIO y Arduino para ESP8266. Recupera el firmware original y suma el portal de configuración y los modos actuales de luz. Las conexiones y la placa probada se detallan en [Hardware](hardware/Readme.md).

Los binarios publicados estarán en [Releases](https://github.com/lcismondi/airbit/releases). Cada release reúne una etiqueta de versión, notas, BIN, ELF para diagnóstico, manifiesto y checksums. Las instrucciones están en [Publicación](firmware/platformio/docs/PUBLICACION.md).

La versión pública funciona localmente: no requiere una cuenta ni un servicio externo para controlar la luz o calibrar. El binario omite las credenciales locales y el destino privado del formulario histórico. El proyecto Arduino original se conserva en [firmware/Airbit.ino](firmware/Airbit.ino).

## Estado del proyecto

Airbit es un prototipo funcional en evolución. El portal, la calibración persistente y los controles de luz se probaron en el equipo de desarrollo. Se incorporaron correcciones de convivencia LED/WiFi y recuperación del portal; todavía queda por confirmar la estabilidad prolongada y la respuesta visual de todas las olas en uso continuado. Los resultados y los cortes de radio observados están documentados en la validación.

[Historial de firmware](firmware/Releases.md) · [Cambios actuales](firmware/platformio/CHANGELOG.md) · [Validación y límites conocidos](firmware/platformio/docs/VALIDACION.md)

En hardware y gabinete se publican imágenes de referencia. Todavía faltan los archivos de fabricación y los modelos editables para que el repositorio permita reproducir el conjunto completo.

## Origen y colaboración

Airbit nació como un monitor de ambiente vinculado al proyecto [Fmart / Heladera inteligente](https://github.com/lcismondi/HeladeraSmart). Esta etapa recupera ese trabajo y suma configuración y controles locales desde el navegador.

Para informar un problema, abrí un issue con la versión del firmware, la revisión de PCB si la conocés, los pasos para reproducirlo y el registro serie. Para proponer cambios, indicá qué parte afecta y cómo la verificaste. Los cambios de hardware deben acompañarse de esquema y mapa de conexiones; los de firmware, de su compilación y pruebas relevantes.

[Presentación original](https://www.lucianocismondi.com.ar/2023/04/airbit-calidad-de-aire-interior.html) · [Instagram](https://www.instagram.com/stories/highlights/17906926166257568/)
