# GitHub y releases

Repositorio: https://github.com/lcismondi/airbit

El proyecto actual se publica dentro de `firmware/platformio`. El firmware Arduino historico y las carpetas de hardware, gabinete e imagenes conservan su historial.

## Sincronizar desde la carpeta de trabajo actual

```powershell
python scripts/sincronizar_github.py
git -C repositorio/airbit status --short
git -C repositorio/airbit diff --stat
git -C repositorio/airbit add firmware/platformio
git -C repositorio/airbit commit -m "Actualizar firmware Airbit"
git -C repositorio/airbit push origin main
```

El sincronizador prepara una copia Git en `repositorio/airbit`, excluyendo configuracion local, logs, binarios, cache y el `.ino` local. No hace commit ni push por si solo. Abrir `repositorio/airbit` en VS Code para ver el historial y los cambios con el panel de Git. Si se trabaja directamente en ese clon, editar `firmware/platformio` y utilizar Git normalmente, sin ejecutar el sincronizador desde alli.

## Crear una version

1. Cambiar `AIRBIT_FIRMWARE_VERSION` en `include/version_airbit.h` y actualizar `CHANGELOG.md`.
2. Sincronizar, revisar el diff y crear el commit. Anotar su SHA completo.
3. Compilar y empaquetar desde la copia Git, que debe estar limpia y en ese commit:

```powershell
cd repositorio/airbit/firmware/platformio
pio run -e airbit_release
python scripts/preparar_release.py --commit SHA_COMPLETO
```

4. Crear la etiqueta `vX.Y.Z` y subirla junto con el commit al repositorio (sin forzar ni reemplazar etiquetas):

```powershell
git tag -a vX.Y.Z SHA_COMPLETO -m "Airbit vX.Y.Z"
git push --atomic origin main vX.Y.Z
```

El tag debe identificar el mismo commit del manifiesto. Si ya fue creado al preparar el release, omitir `git tag`.
5. Preparar las notas en un archivo UTF-8 y publicar los cuatro adjuntos comprobados:

```powershell
python scripts/publicar_release.py --repo lcismondi/airbit --commit SHA_COMPLETO --notas docs/RELEASE_vX.Y.Z.md --assets dist/vX.Y.Z
```

El publicador verifica commit y checksums, crea un borrador, adjunta BIN/ELF/manifiesto/SHA256SUMS y publica al terminar. Usa la autenticacion Git ya disponible o GH_TOKEN/GITHUB_TOKEN, sin imprimir ni guardar el token. No reemplaza una release publicada. Las herramientas deben ejecutarse con Python que tenga pyserial para pruebas de hardware y PlatformIO para compilar.

El manifiesto registra commit, version, SHA256, placa, modo DIO y direccion de carga 0x0. El ELF se conserva para decodificar trazas de ese binario concreto. No publicar el entorno local `airbit_esp8266`: puede contener configuracion privada.

La version/MAC de la API es informacion del dispositivo en ejecucion. La MAC no se fija en el codigo ni en el binario descargable.
