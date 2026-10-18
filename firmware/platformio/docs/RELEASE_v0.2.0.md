Airbit v0.2.0 recupera el firmware ESP8266 en PlatformIO e incorpora el portal web de configuracion.

- AP permanente y portal cautivo para configurar WiFi.
- Calibracion MQ135 de cinco minutos, referencia persistente y escala continua de once estados.
- Iluminacion manual con rueda de color, intensidad y olas por pulsador.
- Version y MAC del equipo en el pie de la interfaz y en `GET /api/estado` (`dispositivo`).
- Diagnostico serie y ajustes de convivencia LED/WiFi, recuperacion del portal y descarga del HTML.
- Documentacion de presentacion, hardware y gabinete actualizada, con capturas del portal y limites de reproduccion del prototipo.

El binario adjunto no contiene claves WiFi ni la URL privada del formulario historico. En un equipo sin configuracion guardada, conectarse a `Airbit-XXXXXX`, clave `1234567890`, y abrir `http://192.168.4.1`. Configurar la red desde la pestana WiFi. La carga normal conserva preferencias y calibracion EEPROM.

Validado en ESP8266EX de 4 MB, modo DIO: 400 respuestas HTTP durante unos nueve minutos sin reinicios ni agotamiento continuo de memoria, descarga lenta completa y reentrada al AP. Todavia se observaron cortes de radio; queda pendiente la estabilidad prolongada y la aceptacion visual de las olas. La escala MQ135 es relativa, no ppm ni AQI oficial.

Adjuntos: firmware BIN para direccion 0x0, ELF para diagnostico, manifiesto y SHA256SUMS. Conservar el ELF correspondiente al binario utilizado.
