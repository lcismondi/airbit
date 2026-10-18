# Iluminacion manual - 14/10/2026

En Iluminacion con color, el switch Usar como iluminacion selecciona el fondo de los 20 LED:

- Apagado: escala continua de calidad de aire, con la referencia MQ135 existente.
- Encendido: color e intensidad manuales; no hace falta una calibracion para iluminar.

La rueda cromatica elige el tono alrededor del circulo y su saturacion por la distancia al centro. El centro es blanco. El selector de color nativo, junto al hexadecimal, ofrece tambien acceso por teclado. El slider va de 0 a 100 %; cero apaga el fondo manual. Se conserva el brillo global existente de FastLED (125); el porcentaje es relativo a ese maximo.

Las olas de los botones aparecen sobre ambos fondos. Calibracion mantiene prioridad verde exclusiva; los indicadores de arranque y conexion siguen visibles. La lectura MQ135 y el indice se siguen actualizando aunque el fondo sea manual.

## Aplicacion y persistencia

POST `/api/iluminacion` con token `X-Airbit-Token` y campos manual 0/1, r/g/b 0..255, intensidad 0..100 y guardar 0/1. Rechaza valores incompletos, negativos, no numericos o fuera de rango. La respuesta indica aplicado y guardado de forma explicita.

Mientras se arrastra, se agrupan las actualizaciones y se aplican en RAM, sin commit. Al soltar el control, al cambiar el color nativo o al accionar el switch, se guarda la preferencia. Las peticiones se serializan y prevalece la ultima eleccion. El polling no pisa los controles editados ni aplica una lectura previa al guardado. Un error conserva la eleccion y ofrece Reintentar.

Registro independiente de 20 bytes, version 1 y CRC32 en EEPROM 196..215. No se modifica el formato de Wi-Fi/AP (64..195) ni de calibracion (0..31). Inicialmente se usa calidad de aire, color manual blanco e intensidad 50 %. Un registro invalido vuelve a esos valores. Se omiten commits si la preferencia coincide con lo ya guardado.

## Archivos

- `include/iluminacion_airbit.h`: registro y escalado de intensidad.
- `src/main.cpp`: compositor del fondo manual/automatico.
- `include/portal_configuracion.h`: estado, carga y accion HTTP.
- `src/configuracion.html`: switch, rueda y slider.
- `test/iluminacion_airbit_test.cpp`: limites, CRC, escalado y mapa.
- `test/pagina_configuracion_test.cjs`: edicion, cola, guardado, rueda y rechazo sin confirmacion, junto con las pruebas existentes.

El guardado comparte el sector EEPROM existente. No se guarda por cada muestra del sensor ni por cada frame LED.

## Resultado de la validacion

Compilado y cargado por COM9. Probados el control de puntero de la rueda, slider 37 %, retorno automatico, recuperacion tras reinicio y preview sin escritura. Referencia actual 42,040 ADC / 300 muestras conservada. Se dejaron los valores iniciales en modo automatico. Capturas y limite de comprobacion visual de los LED: [VALIDACION.md](VALIDACION.md).
