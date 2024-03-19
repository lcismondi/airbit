# Diseño del PCB
El prototipo del PCB está centrado en la disposición eficiente de los componentes para garantizar un rendimiento óptimo y una fácil conexión. Los aspectos destacados del diseño incluyen:
* Compatibilidad: El PCB está diseñado para alojar tanto la versión MCU v2 como V3 del microcontrolador ESP8266, dando una mayor flexibilidad en la elección del hardware.
* Conexión de sensores: Los sensores BMP085, BMP180 y MQ135 están conectados al PCB de manera que se facilite su lectura por parte del microcontrolador.
* Level shifter: Incluye un adaptador bidireccional de 3.3 a 5V para garantizar la compatibilidad entre los sensores y el ESP8266.
* Refrigeración: Incorpora un cooler para mejorar la precisión de las mediciones del sensor MQ135, evitando así lecturas influenciadas por el microclima dentro del gabinete.
* Interfaz de usuario: Integra una base lumínica con WS2812 y cuatro pulsadores en la carcasa para proporcionar una interfaz fácil de usar y visualmente atractiva.

# Características del PCB
* Microcontrolador: ESP8266 (MCU v2 o V3).
## Sensores:
* BMP085 y BMP180 para la medición de presión.
* MQ135 para la detección de gases.
## Otros componentes:
* Level shifter de 3.3 a 5V.
* Cooler para mejorar la precisión de las mediciones.
* Base lumínica con WS2812.
* Cuatro pulsadores integrados en la carcasa.

# Contribuciones
¡Las contribuciones son bienvenidas! Si deseas mejorar el diseño del PCB Airbit, no dudes en enviar pull requests con tus cambios propuestos. También puedes abrir issues para informar sobre errores o sugerir mejoras en el diseño.
