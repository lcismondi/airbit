# Escala continua de señal MQ135 — 13/10/2026

Los 20 LED muestran un fondo cuyo color depende de la señal MQ135 respecto al promedio de aire limpio guardado. El usuario confirmó el fondo completo, con las olas de botones sobre él.

## Escala actual - 16/10/2026

Se conserva el indice relativo: 0 en la referencia o por debajo y 1000 cuando la lectura duplica la referencia. Se aplica la siguiente paleta compartida por el fondo LED y la interfaz:

| Nivel | Estado | Indice del color exacto | ADC respecto a referencia | HEX |
| --- | --- | --- | --- | --- |
| 1 | Aire de montaña | 0 | +0 % o inferior | #2196F3 |
| 2 | Brisa fresca | 100 | +10 % | #26B6D4 |
| 3 | Brisa limpia | 200 | +20 % | #2DBD85 |
| 4 | Un respiro | 300 | +30 % | #65C450 |
| 5 | Aire tranquilo | 400 | +40 % | #ACD442 |
| 6 | Brisa en pausa | 500 | +50 % | #F2D43A |
| 7 | Se va cargando | 600 | +60 % | #F6B832 |
| 8 | Ambiente cargadito | 700 | +70 % | #F79332 |
| 9 | Pide una ventana | 800 | +80 % | #EF6C2F |
| 10 | Toca renovar | 900 | +90 % | #E64932 |
| 11 | ¡Un respiro, por favor! | 1000 | +100 % o mas | #D92D35 |

Entre estos once puntos se interpolan los canales RGB, conservando el filtro y la transicion suave existentes. El texto se elige por el punto mas cercano: nivel 1 para 0..49, nivel 2 para 50..149, sucesivamente, y nivel 11 para 950..1000. Los nombres describen la escala relativa del equipo; no son categorias normativas.

La pantalla muestra el estado y nivel en el circulo, con el color de la escala a intensidad completa para su lectura. El texto central usa blanco suave con sombra para distinguirlo sobre la paleta. Las tarjetas Lectura actual (ADC) e Indice relativo (n / 1000) aparecen juntas. El circulo sigue representando calidad aunque se seleccione iluminacion manual; la intensidad fisica conserva su ajuste independiente. Sin escala disponible muestra Sin referencia, Esperando lectura o Calibrando sobre un fondo neutro.

GET /api/estado agrega aire.nivel (1..11, o 0 sin escala) y aire.estado (texto, o vacio sin escala). El color, el estado y el indice se calculan a partir del mismo valor suavizado en el firmware.

Se asume la salida analógica convencional del módulo MQ135: más señal ADC corresponde a mayor respuesta del sensor. El fabricante describe el aumento de conductividad con los gases objetivo y el circuito de carga que convierte esa respuesta en tensión. La escala representa la variación relativa de esa señal. Los puntos +25/+50/+75/+100 % son una configuración de visualización elegida para este firmware, no concentraciones, un porcentaje de contaminación, ppm, CO₂ ni AQI. La interpretación depende de una referencia estable y del circuito conectado. [Manual oficial MQ135](https://www.winsen-sensor.com/manual/mq135.html).

## Suavizado y brillo

- Muestreo normal de A0 cada 1.000 ms, independiente de las olas y del bloque BMP.
- Filtro exponencial del índice: cada muestra aporta un cuarto de la diferencia respecto al objetivo.
- Transición de la visualización con constante de tiempo de 2.000 ms, actualizada con los frames de 20 ms. Esa constante no es un plazo fijo para completar un cambio.
- Después de una tarea lenta se limita el avance temporal a 250 ms por frame para evitar un salto grande al destino.
- Fondo con intensidad configurable 0..100 %, inicialmente 25 % (factor 64 anterior), antes del brillo global existente de 125. Las olas conservan su intensidad. Ver [INTENSIDAD_CALIDAD.md](INTENSIDAD_CALIDAD.md).
- Al comenzar una ola, el frente sustituye el fondo en sus LED; la estela recupera el fondo gradualmente. Los cruces de varias olas conservan la suma de colores existente.

Una mayor suavidad implica una respuesta más lenta al cambio real de la señal. El primer dato disponible inicializa el color; las lecturas posteriores cambian gradualmente.

## Prioridades

1. Calibración: los 20 LED respiran exclusivamente en verde durante cinco minutos.
2. LED 16 rojo durante los primeros cinco segundos de arranque.
3. Giro azul de conexión y olas de botones, visibles sobre el fondo.
4. Fondo de señal relativo a la calibración.

Al terminar una ola, los LED recuperan el color de fondo, en lugar de apagarse. El giro azul deja de moverse tres segundos después de conectar; el fondo permanece. Sin referencia EEPROM válida se conserva el comportamiento anterior y no se muestra un fondo que aparente una calidad conocida. Se debe completar primero la calibración.

Al salir de la calibración se reinicia el filtro y se toma una lectura nueva frente a la referencia que corresponda. No cambia el formato EEPROM ni se guarda cada lectura. La carga normal por USB no ejecuta un borrado total de flash ni una nueva calibración.

El refresco del fondo no mantiene activa la bandera de olas: BMP y HTTPS conservan su planificación existente. Las conversiones BMP o HTTPS síncronos pueden todavía retrasar frames; este cambio no elimina esos bloqueos. El ventilador conserva exactamente su ciclo y sus controles.

## Ajustes

En `include/calidad_aire.h`:

- `aumentoParaRojoPct = 100`: exceso relativo que alcanza rojo. Si se cambia, los once puntos se reparten uniformemente en ese rango.
- `colores` y `estados`: once puntos RGB y nombres, con intervalos de 100 puntos y texto por cercania.
- `factorFiltroSensor = 4`: divisor del filtro por muestra.
- `tiempoTransicionMs = 2000`: constante temporal del color.

En `src/main.cpp`:

- `intervaloLecturaAireMs = 1000`.
- `intensidadCalidadActual`: preferencia independiente de intensidad.

El rango disponible depende de A0: con referencias superiores a aproximadamente 511 ADC, el doble de la referencia queda por encima del máximo 1023. En ese caso este ajuste no puede alcanzar rojo únicamente con el exceso de +100 %; el rango se puede reducir después de comprobar el circuito y la referencia.

## Verificación

```powershell
& 'C:\Users\siste\.platformio\penv\Scripts\python.exe' scripts/test_calidad.py
& 'C:\Users\siste\.platformio\penv\Scripts\platformio.exe' run
```

Las 14 pruebas constexpr del módulo real comprueban los once puntos, los 1.001 niveles del gradiente sin saltos grandes, los umbrales relativos, límites/referencias pequeñas sin desbordamiento, primera lectura, filtrado, pausas largas acotadas, convergencia exacta sin sobrepasar el destino, reinicio del indicador y colores intermedios.

El monitor mantiene el ADC, referencia y variación, y añade `Escala relativa MQ135: n/1000 (0=azul, 1000=rojo)`. El índice mostrado incluye el filtrado; no tiene unidades de concentración.

Respaldo local previo: `.pio/backups/main.antes_escala_calidad.cpp`. Resultado de carga y pruebas físicas pendientes: [VALIDACION.md](VALIDACION.md).
