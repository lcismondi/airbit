# Cambios

## 0.2.0 - 2026-10-18

- Migracion del firmware historico a PlatformIO para ESP8266 con 20 WS2812B, MQ135 y BMP085/BMP180.
- Portal cautivo con AP permanente, configuracion WiFi, calibracion web, rueda de color e intensidad independiente para luz y calidad de aire.
- Calibracion MQ135 de cinco minutos, referencia persistente y escala relativa continua con once estados de color.
- Olas por pulsador y efectos de arranque, conservando mapa fisico y controles del cooler.
- Recuperacion de HTTP/DNS tras cambios de red; prioridad al AP y aplazamiento del envio HTTPS mientras se usa el portal.
- Interrupciones entre pixeles para reducir la interferencia del envio LED con WiFi; timeout del flujo HTML ampliado para evitar respuestas truncadas.
- Diagnostico serie con memoria, causas de reinicio, huella RTC y captura automatica.
- Version y MAC principal/AP en `/api/estado`; version y MAC WiFi visibles al pie de la interfaz.
- Compilacion publica sin credenciales ni destino privado de telemetria incrustados.

Validacion: 400 respuestas HTTP correctas en unos nueve minutos de trafico sostenido sin reinicios ni descenso continuo de memoria, mas descarga lenta y reentrada al AP. Se observaron cortes de radio y un error de red en la comprobacion final del arnes; posteriormente AP/DNS/HTTP respondieron. Sigue pendiente confirmar estabilidad prolongada y aceptacion visual de las olas.

## 0.1

Firmware Arduino historico: adquisicion de sensores, pulsadores, LED y comunicacion WiFi.
