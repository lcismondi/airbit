# Intensidad en calidad de aire - 16/10/2026

En Calidad de aire, la tarjeta Opciones de la escala permite ajustar la intensidad del fondo de los 20 LED entre 0 y 100 %. Cero apaga ese fondo; 100 % usa el maximo existente de FastLED (brillo global 125). El valor inicial de 25 % conserva exactamente el factor de escalado 64 anterior. El color sigue dependiendo del indice MQ135; las olas, el arranque y la calibracion conservan sus prioridades e intensidad.

La preferencia es independiente del color y la intensidad manuales. Se puede editar con iluminacion manual activa y se aplicara al apagar Usar como iluminacion. Arrastrar aplica una vista previa en RAM; soltar guarda. Se agrupan los cambios y se serializan los envios de ambos controles de luz. El polling conserva las elecciones en edicion y descarta lecturas anteriores al cambio. Ante un error se conserva la eleccion y aparece Reintentar.

POST /api/calidad requiere X-Airbit-Token, intensidad (0..100) y guardar (0/1). La respuesta confirma aplicado y guardado, con calidad.intensidad. GET /api/estado incluye calidad.intensidad. Una peticion sin token es 403; un valor incompleto o fuera de rango es 400.

Registro independiente de 16 bytes en EEPROM 216..231: identificador, version 1, longitud, intensidad, tres bytes reservados a cero y CRC32. Un registro invalido usa 25 %. No cambia el formato de Wi-Fi/AP, calibracion o iluminacion manual. La zona 232..255 queda libre. Se omiten commits de valores ya guardados.
