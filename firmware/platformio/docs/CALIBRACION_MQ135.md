# Calibración de referencia MQ135 — 13/10/2026

El usuario confirmó MQ135 conectado a A0 y calibración en aire limpio.

## Uso

1. Con el sensor estabilizado, colocar el equipo en aire limpio y soltar los cuatro botones.
2. Mantener los cuatro botones durante 10 segundos continuos. El tiempo comienza cuando las cuatro entradas superan el antirrebote de 30 ms.
3. Al iniciar, todos los LED respiran en verde. Se pueden soltar los botones.
4. Esperar cinco minutos completos. Se toma una lectura por segundo, normalmente unas 300 muestras. El monitor informa del tiempo restante y del promedio cada 10 segundos.
5. Al terminar, buscar `Calibracion guardada: promedio=... ADC`. Se guarda la referencia en flash y se vuelve al funcionamiento normal, con el fondo de la [escala continua](ESCALA_CALIDAD_AIRE.md).
6. Al reiniciar, debe aparecer `MQ135: referencia guardada ... ADC (... muestras)`. Comprobar el mismo promedio confirma su recuperación persistente.

Soltar un botón antes de los 10 s cancela la entrada. Hay que soltar todos antes de repetir. Mantenerlos no vuelve a iniciar una calibración al terminar. Si algún botón está pulsado al arrancar, se deben soltar todos para rearmar el gesto. Durante la calibración se ignoran las acciones individuales y se vacían las olas anteriores.

La respiración utiliza `sin8`, periodo de tres segundos y el brillo global de 125. Los LED mantienen verde con intensidad variable 16..255 antes del brillo global. Tiene prioridad sobre las otras animaciones y el fondo de calidad. Wi-Fi sigue supervisándose. Durante la calibración y la cuenta del gesto se posponen BMP y el inicio de HTTPS; el ciclo automático del ventilador se conserva.

## Inicio desde la app - 14/10/2026

Abrir Calibracion y, en la tarjeta Controles web, pulsar **Iniciar calibracion** con Airbit en aire limpio y el sensor estabilizado. La app no exige mantener los botones: inicia directamente la misma ventana de cinco minutos, a 1 Hz, con los LED verdes respirando. La referencia se guarda solo al completar una ventana valida.

El boton muestra Iniciando mientras espera la confirmacion, y Calibracion en curso mientras el proceso esta activo. La cuenta restante de la tarjeta Estado de la calibracion se actualiza automaticamente. Al terminar se habilita otra vez. Una pulsacion duplicada no reinicia el contador. Se conserva el gesto fisico de cuatro botones durante 10 s.

La accion usa POST `/api/calibracion` y el token `X-Airbit-Token`; responde 202 con `iniciada:true` cuando la ventana arranca, 409 si ya hay una calibracion activa y 403 si falta la sesion. El GET no inicia el proceso. No se presenta exito si falta la confirmacion del equipo; tras un error se consulta de nuevo su estado. Una lectura anterior al POST se descarta para que no habilite indebidamente el boton.

## Referencia

Media aritmética de muestras independientes. No se rellenan huecos con copias. Se guarda el promedio en milésimas de ADC, la suma, cantidad de muestras y extremos.

El monitor muestra el ADC actual, referencia, diferencia y variación porcentual. Referencia 70 y lectura 84 ADC significan +14 ADC y +20 % de señal; es un ejemplo, no una medición ni un porcentaje de contaminación. La [escala de colores](ESCALA_CALIDAD_AIRE.md) usa ahora esa referencia.

No se convierte a ppm ni AQI. Para una conversión cuantitativa se necesitan circuito, resistencia de carga, alimentación/divisor de A0, curva del gas objetivo y condiciones ambientales. El formulario histórico conserva su campo Partículas con ADC bruto.

## Memoria persistente

EEPROM emulada del núcleo ESP8266, sin bibliotecas externas nuevas. Registro de 32 bytes en dirección lógica 0:

| Offset | Bytes | Campo |
| --- | --- | --- |
| 0 | 4 | Marca AIRB (0x41495242) |
| 4 | 2 | Versión 1 |
| 6 | 2 | Tamaño 32 |
| 8 | 4 | Promedio en milésimas de ADC |
| 12 | 4 | Cantidad de muestras |
| 16 | 4 | Suma |
| 20 | 2 | ADC mínimo |
| 22 | 2 | ADC máximo |
| 24 | 4 | Duración 300.000 ms |
| 28 | 4 | CRC32 IEEE de los primeros 28 bytes |

El registro de calibración sigue ocupando 32 bytes en 0..31. La memoria compartida ahora se carga con `EEPROM.begin(256)` y también contiene la configuración AP/Wi-Fi en 64..195; ver [AP_CONFIGURACION.md](AP_CONFIGURACION.md). La calibración se guarda con `EEPROM.commit()` solo después de una ventana válida. Marca, versión, tamaño, duración, media/rango, cobertura y CRC se comprueban al arrancar.

Se exigen 270..300 muestras, al menos el 90 % nominal. Se rechazan referencias cero, saturadas a 1023 o incoherentes. Una calibración incompleta no escribe flash, por lo que apagar durante el muestreo conserva la referencia anterior.

Si falla el commit se informa y se conserva la referencia anterior en RAM. La EEPROM ocupa un único sector; cortar alimentación durante el borrado/escritura puede invalidar la referencia persistente. El CRC permite detectarlo y solicitar otra calibración. No hay un registro doble de respaldo.

## Estabilización

Los cinco minutos promedian la señal y no sustituyen el calentamiento. El fabricante especifica más de 48 horas para el precalentamiento de sus condiciones de prueba/acondicionamiento inicial. El usuario inicia el proceso cuando el sensor está listo; no se impone esa espera por firmware.

Fuentes: [manual oficial MQ135](https://www.winsen-sensor.com/manual/mq135.html), [EEPROM ESP8266 3.1.2](https://arduino-esp8266.readthedocs.io/en/3.1.2/libraries.html#eeprom).

## Pruebas

```powershell
& 'C:\Users\siste\.platformio\penv\Scripts\python.exe' scripts/test_calibracion.py
```

Las 15 comprobaciones constexpr usan el módulo real de `include/calibracion_aire.h`: gesto/cancelación/rearme, arranque pulsado, desbordamiento temporal, espaciado y fin exacto de cinco minutos, promedio/extremos, reinicio de acumulación, cobertura, saturación y CRC.

Respaldo previo: `.pio/backups/main.antes_calibracion_mq135.cpp`. Las pruebas de compilador no verifican el ADC, gesto físico, LED ni persistencia real. Resultados y pruebas físicas pendientes en [VALIDACION.md](VALIDACION.md).

Prueba de inicio web del 14/10/2026: click real en la app, confirmacion 202, bloqueo de doble click y segundo inicio 409. La prueba se interrumpio por reinicio antes de guardar para mantener la referencia anterior. Se conserva el registro del resultado en VALIDACION.md; no equivale a completar una nueva calibracion de cinco minutos.
