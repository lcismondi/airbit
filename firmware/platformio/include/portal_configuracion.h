#pragma once
#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include <StreamDev.h>
#include "pagina_configuracion.h"

// Este adaptador se incluye despues de los estados del firmware.
ESP8266WebServer servidorPortal(80);
DNSServer dnsPortal;
bool dnsPortalActivo = false;
bool serviciosPortalIniciados = false;
volatile bool recuperarPortalPendiente = false;
volatile unsigned long momentoRecuperacionPortalMs = 0;
unsigned long ultimaActividadPortalMs = 0;
bool huboActividadPortal = false;

void solicitarRecuperacionPortal() {
  momentoRecuperacionPortalMs = millis();
  recuperarPortalPendiente = true;
}
bool portalEnUso() {
  return WiFi.softAPgetStationNum() != 0 ||
      (huboActividadPortal && millis() - ultimaActividadPortalMs < 15000);
}
void registrarActividadPortal() {
  ultimaActividadPortalMs = millis(); huboActividadPortal = true;
}
AirbitConfig::Registro configuracionPortal = {};
AirbitLuz::Registro iluminacionGuardada = AirbitLuz::predeterminada();
AirbitIntensidad::Registro intensidadCalidadGuardada = AirbitIntensidad::predeterminada();
char nombreAP[24] = {};
char tokenPortal[33] = {};
bool apConfiguracionActivo = false;
bool cambioWiFiPendiente = false;
unsigned long momentoCambioWiFiMs = 0;

const char* ssidConfigurado() {
  return configuracionPortal.wifiPersonalizado ? configuracionPortal.ssid : ssid;
}
const char* claveWiFiConfigurada() {
  return configuracionPortal.wifiPersonalizado ? configuracionPortal.claveWiFi : password;
}
String jsonTexto(const String& texto) {
  String salida;
  salida.reserve(texto.length() + 8);
  salida += '"';
  for (unsigned int i = 0; i < texto.length(); ++i) {
    const uint8_t c = static_cast<uint8_t>(texto[i]);
    if (c == '"' || c == '\\') { salida += '\\'; salida += static_cast<char>(c); }
    else if (c < 32) {
      char escape[7]; snprintf(escape, sizeof(escape), "\\u%04x", c); salida += escape;
    } else salida += static_cast<char>(c);
  }
  salida += '"';
  return salida;
}
void cabecerasPortal() {
  servidorPortal.keepAlive(false);
  servidorPortal.sendHeader("Cache-Control", "no-store");
  servidorPortal.sendHeader("X-Content-Type-Options", "nosniff");
  servidorPortal.sendHeader("Content-Security-Policy",
      "default-src 'self'; script-src 'self' 'unsafe-inline'; style-src 'self' 'unsafe-inline'; connect-src 'self'; frame-ancestors 'none'; form-action 'self'");
}
void enviarRespuestaPortal(int codigo, const char* tipo="text/plain; charset=utf-8", const String& contenido=String()) {
  registrarActividadPortal();
  AirbitDebug::respuesta(codigo,contenido.length());
  servidorPortal.send(codigo,tipo,contenido);
}
void redirigirConfiguracion() {
  cabecerasPortal();
  // Usar la interfaz de la peticion: AP o IP domestica.
  servidorPortal.sendHeader("Location", "http://" + servidorPortal.client().localIP().toString() + "/");
  enviarRespuestaPortal(302, "text/plain; charset=utf-8", "Abrir la configuracion de Airbit.");
}
bool redirigirHostCautivo() {
  if (servidorPortal.client().localIP() != WiFi.softAPIP()) return false;
  const String ipAP = WiFi.softAPIP().toString();
  const String host = servidorPortal.hostHeader();
  if (host == ipAP || host == ipAP + ":80") return false;
  redirigirConfiguracion();
  return true;
}
void errorPortal(int codigo, const char* mensaje) {
  enviarRespuestaPortal(codigo, "application/json; charset=utf-8", "{\"error\":" + jsonTexto(mensaje) + "}");
}
String jsonIluminacion() {
  return "{\"manual\":" + String(iluminacionActual.manual ? "true" : "false") +
      ",\"color\":[" + String(iluminacionActual.r) + "," + String(iluminacionActual.g) + "," + String(iluminacionActual.b) +
      "],\"intensidad\":" + String(iluminacionActual.intensidad) + "}";
}
bool byteIluminacion(const char* nombre, uint16_t maximo, uint8_t& valor) {
  if (!servidorPortal.hasArg(nombre)) return false;
  const String texto = servidorPortal.arg(nombre);
  if (!texto.length() || texto.length() > 3) return false;
  uint16_t numero = 0;
  for (unsigned int i = 0; i < texto.length(); ++i) {
    if (texto[i] < '0' || texto[i] > '9') return false;
    numero = numero * 10 + texto[i] - '0';
  }
  if (numero > maximo) return false;
  valor = numero; return true;
}
void guardarIluminacionPortal() {
  AirbitDebug::Ambito diagnostico(AirbitDebug::Etapa::Iluminacion,servidorPortal.method()==HTTP_POST);
  cabecerasPortal();
  if (servidorPortal.header("X-Airbit-Token") != tokenPortal) {
    errorPortal(403, "La sesion ha cambiado. Actualiza la pagina e intentalo de nuevo."); return;
  }
  AirbitLuz::Registro nueva = iluminacionActual;
  uint8_t guardar = 0;
  if (!byteIluminacion("manual",1,nueva.manual) || !byteIluminacion("r",255,nueva.r) ||
      !byteIluminacion("g",255,nueva.g) || !byteIluminacion("b",255,nueva.b) ||
      !byteIluminacion("intensidad",100,nueva.intensidad) || !byteIluminacion("guardar",1,guardar)) {
    errorPortal(400, "Comprueba el modo, el color y la intensidad."); return;
  }
  AirbitLuz::sellar(nueva);
  if (guardar && nueva.crc != iluminacionGuardada.crc) {
    EEPROM.put(AirbitLuz::direccion,nueva);
    if (!AirbitDebug::guardarFlash()) {
      EEPROM.put(AirbitLuz::direccion,iluminacionGuardada);
      errorPortal(500, "No se pudo guardar la iluminacion. Intentalo de nuevo."); return;
    }
    iluminacionGuardada = nueva;
  }
  iluminacionActual = nueva;
  enviarRespuestaPortal(200,"application/json; charset=utf-8",String("{\"aplicado\":true,\"guardado\":") +
      (guardar ? "true" : "false") + ",\"iluminacion\":" + jsonIluminacion() + "}");
}
String jsonIntensidadCalidad() {
  return "{\"intensidad\":" + String(intensidadCalidadActual.intensidad) + "}";
}
void guardarIntensidadCalidadPortal() {
  AirbitDebug::Ambito diagnostico(AirbitDebug::Etapa::Calidad,servidorPortal.method()==HTTP_POST);
  cabecerasPortal();
  if (servidorPortal.header("X-Airbit-Token") != tokenPortal) {
    errorPortal(403, "La sesion ha cambiado. Actualiza la pagina e intentalo de nuevo."); return;
  }
  AirbitIntensidad::Registro nueva = intensidadCalidadActual;
  uint8_t guardar = 0;
  if (!byteIluminacion("intensidad",100,nueva.intensidad) || !byteIluminacion("guardar",1,guardar)) {
    errorPortal(400, "La intensidad debe estar entre 0 y 100."); return;
  }
  AirbitIntensidad::sellar(nueva);
  if (guardar && nueva.crc != intensidadCalidadGuardada.crc) {
    EEPROM.put(AirbitIntensidad::direccion,nueva);
    if (!AirbitDebug::guardarFlash()) {
      EEPROM.put(AirbitIntensidad::direccion,intensidadCalidadGuardada);
      errorPortal(500, "No se pudo guardar la intensidad. Intentalo de nuevo."); return;
    }
    intensidadCalidadGuardada = nueva;
  }
  intensidadCalidadActual = nueva;
  enviarRespuestaPortal(200,"application/json; charset=utf-8",String("{\"aplicado\":true,\"guardado\":") +
      (guardar ? "true" : "false") + ",\"calidad\":" + jsonIntensidadCalidad() + "}");
}
void enviarEstadoPortal() {
  AirbitDebug::Ambito diagnostico(AirbitDebug::Etapa::Estado,servidorPortal.method()==HTTP_POST);
  cabecerasPortal();
  const bool conectado = !cambioWiFiPendiente && WiFi.status() == WL_CONNECTED && WiFi.SSID() == ssidConfigurado();
  const bool escala = referenciaAireValida && indicadorCalidad.listo() && !calibracionAire.activa();
  const AirbitCalidad::Color color = indicadorCalidad.color();
  String json;
  json.reserve(1000);
  json = "{\"dispositivo\":{\"version\":" + jsonTexto(AIRBIT_FIRMWARE_VERSION);
  json += ",\"mac\":" + jsonTexto(WiFi.macAddress()) + ",\"macAP\":" + jsonTexto(WiFi.softAPmacAddress());
  json += "},\"csrf\":" + jsonTexto(tokenPortal) + ",\"ap\":{\"ssid\":" + jsonTexto(nombreAP);
  json += ",\"ip\":" + jsonTexto(WiFi.softAPIP().toString()) + ",\"activa\":" + (apConfiguracionActivo ? String("true") : String("false"));
  json += ",\"cautivo\":" + String(dnsPortalActivo ? "true" : "false");
  json += ",\"clientes\":" + String(WiFi.softAPgetStationNum()) + ",\"canal\":" + String(WiFi.channel());
  json += "},\"wifi\":{\"ssid\":" + jsonTexto(ssidConfigurado());
  json += ",\"conectado\":" + String(conectado ? "true" : "false");
  json += ",\"redAbierta\":" + String(claveWiFiConfigurada()[0] ? "false" : "true");
  json += ",\"estado\":" + jsonTexto(cambioWiFiPendiente ? "Iniciando conexion" : descripcionEstadoWiFi(WiFi.status()));
  json += ",\"ip\":" + jsonTexto(WiFi.localIP().toString());
  json += "},\"calibracion\":{\"valida\":" + String(referenciaAireValida ? "true" : "false");
  json += ",\"activa\":" + String(calibracionAire.activa() ? "true" : "false");
  json += ",\"referencia\":" + String(referenciaAireValida ? referenciaAire.promedioMilliAdc / 1000.0f : 0.0f, 3);
  json += ",\"muestras\":" + String(referenciaAireValida ? referenciaAire.muestras : 0);
  json += ",\"restante\":" + String(calibracionAire.activa() ? (calibracionAire.restanteMs(millis()) + 999) / 1000 : 0);
  json += "},\"aire\":{\"disponible\":" + String(lecturaAireDisponible ? "true" : "false");
  json += ",\"adc\":" + String(particulas, 2) + ",\"escalaDisponible\":" + String(escala ? "true" : "false");
  json += ",\"indice\":" + String(escala ? indicadorCalidad.indice() : 0);
  json += ",\"nivel\":" + String(escala ? AirbitCalidad::nivelDesdeIndice(indicadorCalidad.indice()) + 1 : 0);
  json += ",\"estado\":" + jsonTexto(escala ? AirbitCalidad::estadoDesdeIndice(indicadorCalidad.indice()) : "");
  json += ",\"color\":[" + String(color.r) + "," + String(color.g) + "," + String(color.b) + "]},\"iluminacion\":" + jsonIluminacion() + ",\"calidad\":" + jsonIntensidadCalidad() + "}";
  enviarRespuestaPortal(200, "application/json; charset=utf-8", json);
}
void iniciarCalibracionPortal() {
  AirbitDebug::Ambito diagnostico(AirbitDebug::Etapa::Calibracion,servidorPortal.method()==HTTP_POST);
  cabecerasPortal();
  if (servidorPortal.header("X-Airbit-Token") != tokenPortal) {
    errorPortal(403, "La sesion ha cambiado. Actualiza la pagina e intentalo de nuevo."); return;
  }
  if (calibracionAire.activa()) {
    errorPortal(409, "Ya hay una calibracion en curso."); return;
  }
  iniciarCalibracionAire();
  enviarRespuestaPortal(202, "application/json; charset=utf-8", "{\"iniciada\":true,\"duracion\":300}");
  Serial.println("Portal: calibracion iniciada desde la app.");
}
void guardarWiFiPortal() {
  AirbitDebug::Ambito diagnostico(AirbitDebug::Etapa::WiFi,servidorPortal.method()==HTTP_POST);
  cabecerasPortal();
  if (servidorPortal.header("X-Airbit-Token") != tokenPortal) {
    errorPortal(403, "La sesion ha cambiado. Actualiza la pagina e intentalo de nuevo."); return;
  }
  if (!servidorPortal.hasArg("ssid") || !servidorPortal.hasArg("password") || !servidorPortal.hasArg("redAbierta")) {
    errorPortal(400, "Faltan datos de la red."); return;
  }
  const String red = servidorPortal.arg("ssid");
  const String claveIntroducida = servidorPortal.arg("password");
  const String abierta = servidorPortal.arg("redAbierta");
  if (red.length() == 0 || red.length() > 32 || strlen(red.c_str()) != red.length()) {
    errorPortal(400, "El nombre de red debe tener entre 1 y 32 bytes."); return;
  }
  if (claveIntroducida.length() > 64 || strlen(claveIntroducida.c_str()) != claveIntroducida.length() ||
      (abierta != "0" && abierta != "1")) {
    errorPortal(400, "Comprueba la contrasena y el tipo de red."); return;
  }
  const String clave = abierta == "1" ? String("") :
      (claveIntroducida.length() ? claveIntroducida : String(claveWiFiConfigurada()));
  if ((abierta == "0" && clave.length() == 0) || !AirbitConfig::claveWiFiValida(clave.c_str())) {
    errorPortal(400, "La clave debe tener 8 a 63 bytes, o 64 caracteres hexadecimales."); return;
  }
  AirbitConfig::Registro nueva = configuracionPortal;
  nueva.wifiPersonalizado = 1;
  if (!AirbitConfig::copiar(nueva.ssid, sizeof(nueva.ssid), red.c_str()) ||
      !AirbitConfig::copiar(nueva.claveWiFi, sizeof(nueva.claveWiFi), clave.c_str())) {
    errorPortal(400, "Datos de red demasiado largos."); return;
  }
  AirbitConfig::sellar(nueva);
  if (!AirbitConfig::valido(nueva)) { errorPortal(400, "Configuracion no valida."); return; }
  EEPROM.put(AirbitConfig::direccion, nueva);
  if (!AirbitDebug::guardarFlash()) {
    EEPROM.put(AirbitConfig::direccion, configuracionPortal);
    errorPortal(500, "No se pudo guardar la red. Intentalo de nuevo."); return;
  }
  configuracionPortal = nueva;
  cambioWiFiPendiente = true;
  momentoCambioWiFiMs = millis();
  enviarRespuestaPortal(200, "application/json; charset=utf-8", "{\"guardado\":true}");
  Serial.println("Portal: WiFi guardado. Conexion pendiente, sin reiniciar el equipo.");
}
void iniciarPortalConfiguracion() {
  EEPROM.get(AirbitIntensidad::direccion,intensidadCalidadActual);
  if (!AirbitIntensidad::valido(intensidadCalidadActual)) intensidadCalidadActual = AirbitIntensidad::predeterminada();
  intensidadCalidadGuardada = intensidadCalidadActual;
  EEPROM.get(AirbitLuz::direccion,iluminacionActual);
  if (!AirbitLuz::valido(iluminacionActual)) iluminacionActual = AirbitLuz::predeterminada();
  iluminacionGuardada = iluminacionActual;
  EEPROM.get(AirbitConfig::direccion, configuracionPortal);
  if (!AirbitConfig::valido(configuracionPortal)) {
    configuracionPortal = {};
  }
  // Aplicar la clave solicitada sin borrar las credenciales WiFi guardadas.
  if (strcmp(configuracionPortal.claveAP, AirbitConfig::claveApPredeterminada) != 0) {
    AirbitConfig::copiar(configuracionPortal.claveAP, sizeof(configuracionPortal.claveAP),
                        AirbitConfig::claveApPredeterminada);
    AirbitConfig::sellar(configuracionPortal);
    EEPROM.put(AirbitConfig::direccion, configuracionPortal);
    if (!AirbitDebug::guardarFlash()) Serial.println("Portal: error al guardar la clave AP; disponible solo en RAM.");
  }
  snprintf(nombreAP, sizeof(nombreAP), "Airbit-%06X", static_cast<unsigned int>(ESP.getChipId()));
  snprintf(tokenPortal, sizeof(tokenPortal), "%08X%08X%08X%08X",
           static_cast<unsigned int>(ESP.random()), static_cast<unsigned int>(ESP.random()),
           static_cast<unsigned int>(ESP.random()), static_cast<unsigned int>(ESP.random()));
  WiFi.softAPConfig(IPAddress(192, 168, 4, 1), IPAddress(192, 168, 4, 1), IPAddress(255, 255, 255, 0));
  apConfiguracionActivo = WiFi.softAP(nombreAP, configuracionPortal.claveAP, 1, false, 4);
  dnsPortal.setTTL(0);
  dnsPortal.setErrorReplyCode(DNSReplyCode::NoError);
  dnsPortalActivo = apConfiguracionActivo && dnsPortal.start(53, "*", WiFi.softAPIP());
  servidorPortal.collectHeaders("X-Airbit-Token");
  servidorPortal.on("/", HTTP_GET, []() {
    registrarActividadPortal();
    AirbitDebug::Ambito diagnostico(AirbitDebug::Etapa::Pagina);
    if (redirigirHostCautivo()) return;
    cabecerasPortal();
    AirbitDebug::respuesta(200,sizeof(paginaConfiguracion)-1);
    // send_P crea un StreamConstPtr con timeout de 1 s independiente del
    // timeout TCP. Dar margen al flujo y conservar el informe de transferencia.
    StreamConstPtr pagina(paginaConfiguracion, sizeof(paginaConfiguracion) - 1);
    pagina.setTimeout(5000);
    servidorPortal.send(200, "text/html; charset=utf-8", &pagina, sizeof(paginaConfiguracion) - 1);
    const size_t pendientes = static_cast<size_t>(pagina.available());
    Serial.printf_P(PSTR("[DBG PAGINA] leidosDelFlujo=%u/%u informe=%u\n"),
        static_cast<unsigned int>(sizeof(paginaConfiguracion) - 1 - pendientes),
        static_cast<unsigned int>(sizeof(paginaConfiguracion) - 1),
        static_cast<unsigned int>(pagina.getLastSendReport()));
  });
  servidorPortal.on("/api/estado", HTTP_GET, enviarEstadoPortal);
  servidorPortal.on("/api/wifi", HTTP_POST, guardarWiFiPortal);
  servidorPortal.on("/api/calibracion", HTTP_POST, iniciarCalibracionPortal);
  servidorPortal.on("/api/iluminacion", HTTP_POST, guardarIluminacionPortal);
  servidorPortal.on("/api/calidad", HTTP_POST, guardarIntensidadCalidadPortal);
  servidorPortal.on("/favicon.ico", HTTP_GET, []() { AirbitDebug::Ambito diagnostico(AirbitDebug::Etapa::Favicon); enviarRespuestaPortal(204); });
  // Sondas HTTP de Android, Apple y Windows: indicar que hay un portal.
  const char* sondas[] = {"/generate_204", "/gen_204", "/hotspot-detect.html",
      "/library/test/success.html", "/ncsi.txt", "/connecttest.txt", "/redirect",
      "/fwlink", "/success.txt", "/canonical.html"};
  for (const char* ruta : sondas) servidorPortal.on(ruta, HTTP_GET, []() {
    AirbitDebug::Ambito diagnostico(AirbitDebug::Etapa::Redireccion);
    redirigirConfiguracion();
  });
  servidorPortal.onNotFound([]() {
    AirbitDebug::Ambito diagnostico(AirbitDebug::Etapa::NoEncontrado,servidorPortal.method()==HTTP_POST);
    if (servidorPortal.method() == HTTP_GET && servidorPortal.client().localIP() == WiFi.softAPIP()) {
      redirigirConfiguracion(); return;
    }
    cabecerasPortal(); errorPortal(404, "Pagina no encontrada.");
  });
  servidorPortal.begin();
  serviciosPortalIniciados = true;
  recuperarPortalPendiente = false;
  Serial.printf("AP configuracion: %s (%s)\n", nombreAP, apConfiguracionActivo ? "activo" : "error");
  Serial.print("AP direccion: http://"); Serial.println(WiFi.softAPIP());
  // Clave del AP local, no la contrasena del router.
  Serial.println("AP: clave configurada (omitida del registro)");
  Serial.println("Portal: acceso sin usuario ni contrasena");
  Serial.printf("Portal cautivo DNS: %s\n", dnsPortalActivo ? "activo" : "error");
}
void informarSocketsPortal() {
  Serial.printf_P(PSTR("[DBG SOCKETS] escucha=%u cliente=%u recibidos=%d pendientes=%u datosPendientes=%u DNS=%u\n"),
      servidorPortal.getServer().status(), servidorPortal.client().connected(), servidorPortal.client().available(),
      servidorPortal.getServer().hasClient(), static_cast<unsigned int>(servidorPortal.getServer().hasClientData()), dnsPortalActivo);
}
void procesarPortalConfiguracion() {
  // Los callbacks marcan la recuperacion; reabrir sockets desde el loop cuando
  // se estabilice la interfaz, sin recrear rutas, tokens ni preferencias.
  if (serviciosPortalIniciados && recuperarPortalPendiente &&
      millis() - momentoRecuperacionPortalMs >= 250) {
    recuperarPortalPendiente = false;
    servidorPortal.client().stop();
    servidorPortal.close();
    dnsPortal.stop();
    dnsPortalActivo = (WiFi.getMode() & WIFI_AP) && dnsPortal.start(53, "*", WiFi.softAPIP());
    servidorPortal.begin();
    Serial.printf_P(PSTR("[DBG PORTAL] Servicios recuperados HTTP=%u DNS=%s modo=%u APclientes=%u canal=%u\n"),
        servidorPortal.getServer().status(), dnsPortalActivo ? "OK" : "ERROR",
        static_cast<unsigned int>(WiFi.getMode()), WiFi.softAPgetStationNum(), WiFi.channel());
  }
  if (cambioWiFiPendiente && millis() - momentoCambioWiFiMs >= 500) {
    cambioWiFiPendiente = false;
    WiFi.disconnect(false, false);
    iniciarConexionWiFi(true);
    ultimoEstadoWiFi = -1;
  }
  if (dnsPortalActivo) dnsPortal.processNextRequest();
  const unsigned long inicioWeb=millis();
  servidorPortal.handleClient();
  if (millis()-inicioWeb>=500) Serial.printf_P(PSTR("[DBG %10lu] ATENCION_WEB dt=%lu ms\n"),millis(),millis()-inicioWeb);
}
