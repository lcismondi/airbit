# Interfaz Airbit: aire, luz y calma - 14/10/2026

Referencia visual: `app/award-winner-design-image-ID182133-N3.png`. Se interpreta su fondo azul grisaceo, superficies suaves, halos turquesa/menta, bordes redondeados e indicadores circulares. La interfaz conserva los apartados Wi-Fi, calibracion, iluminacion y calidad de aire.

## Estilo

- Fondo `#edf4f7`, con luz ambiental turquesa/menta mediante gradientes CSS.
- Texto principal `#183540`; texto secundario `#586f7b`.
- Acentos `#66d4b9` y `#62cfe0`; accion principal en azul oscuro `#173642`.
- Tarjetas de 25..27 px de radio, lineas claras y sombras discretas.
- Iconos SVG dentro del HTML; tipografia del sistema. No se requieren imagenes, fuentes ni servicios externos.
- Navegacion lateral en escritorio; barra flotante inferior en movil, con margen para la zona segura.
- Indicadores circulares para la referencia ADC y el indice relativo; arcos decorativos sin porcentaje inventado.
- Campos con etiquetas visibles, estado de conexion, respuesta al guardado, foco de teclado y colores suaves. La transicion del color existente respeta `prefers-reduced-motion`.

## Funcionamiento

En el cambio inicial de estilo se conservo el JavaScript identico. Posteriormente se agrego el boton de inicio de calibracion web, siguiendo la misma estetica; ver [CALIBRACION_MQ135.md](CALIBRACION_MQ135.md). Los valores siguen llegando de `/api/estado`, y el guardado mantiene `/api/wifi`, token, validacion y confirmacion. La pagina conserva la edicion durante el refresco de estado y permite iniciar la calibracion desde la tarjeta Controles web. El gesto de cuatro botones fisicos tambien sigue disponible. Iluminacion y escala no tienen nuevas acciones.

Archivos editables: `src/configuracion.html` y, tras ejecutar `scripts/generar_pagina.py`, `include/pagina_configuracion.h`. Respaldo del HTML anterior: `.pio/backups/configuracion.antes_estilo_suave.html`.

## Comprobaciones

- Test del JavaScript real y sincronizacion HTML/PROGMEM: PASS.
- Chrome con anchos reales de 320, 390 y 1400 px: sin desbordamiento horizontal; cuatro apartados visibles mediante navegacion; barra movil dentro de pantalla; sin recursos externos.
- Capturas revisadas de Wi-Fi, calibracion, iluminacion y calidad. Se guardan en `app/previews/` y usan una lectura real tomada antes de cargar. La etiqueta Vista previa distingue estas capturas del portal vivo.
- Compilacion y carga COM9: SUCCESS, con verificacion de hash por esptool.
- La placa devuelve HTTP 200 con HTML identico al archivo final: 28.993 bytes. AP, portal cautivo y Wi-Fi siguen activos. Referencia actual 71,757 ADC / 300 muestras, igual que antes de la carga.

No se afirma una prueba visual en telefono fisico; la revision movil se hizo por emulacion de viewport en Chrome. Informes locales: `.pio/preview/revision-visual.json` y `.pio/validacion/interfaz_estilo_suave.json`.

## Capturas

Las capturas de revision y la referencia de app se conservan localmente; no se incluyen en el repositorio.


## Orden del menu - 16/10/2026

Calidad de aire, Iluminacion, Calibracion y Wi-Fi, tanto en la barra inferior movil como en la navegacion lateral. Calidad de aire es la pantalla inicial y el boton seleccionado al abrir o recargar el portal. El cambio solo afecta a la navegacion; conserva el estado y los ajustes del equipo.


## Marca original Airbit - 16/10/2026

La cabecera usa el logotipo original completo en lugar del simbolo de brisa y el texto tipografico anterior. Se mantiene Tu aire. Tu luz. Tu espacio. debajo. El JPG indicado tiene una variante vectorial airbit.svg en la misma carpeta de Logos; se uso ese original, con copia en src/assets/airbit.svg e integrado en el HTML para evitar descargas adicionales. No se modificaron las formas del logotipo. Ancho 174 px en escritorio y 150 px en movil, conservando la proporcion del viewBox. Se revisaron capturas a 320, 390 y 1400 px, sin texto duplicado, solapes ni desbordamiento.


### Transparencia del centro de la a

El SVG original superponia un circulo blanco para simular el hueco. En la copia web se sustituyo por un recorte vectorial con fill-rule evenodd: el centro es transparente y deja ver cualquier fondo. Se mantiene el contorno original y no se cambia el archivo de la carpeta de Diseno/Logos. Verificado en navegador: alfa 0 en el centro y alfa 255 en el contorno, con revision movil y escritorio.


### Menos texto en Calidad de aire - 16/10/2026

Retirados el parrafo introductorio La lectura de tu MQ135..., la leyenda Nivel x de 11 dentro del circulo y Segun tu referencia de aire limpio debajo. El circulo conserva el estado y su color. Se mantienen las tarjetas ADC/indice y los controles. Se retiro tambien la actualizacion JavaScript del elemento de nivel eliminado.


### Extremos de la barra de colores

Las etiquetas inferiores son Aire de montaña a la izquierda y Toca renovar a la derecha, segun lo solicitado.


### Tarjeta Intensidad de luz

Retirada de la pantalla la aclaracion Esta escala es relativa a tu referencia. No indica ppm ni AQI. El titulo Opciones de la escala pasa a Intensidad de luz y se elimina el icono informativo a su izquierda. El slider conserva su comportamiento y preferencias.


En la tarjeta Intensidad de la luz se elimina la etiqueta inferior duplicada. El porcentaje mantiene su posicion derecha y el slider se identifica con el titulo mediante aria-labelledby.


### Barra gris clara y halo mas visible

Los sliders usan una pista gris clara #D4DFE2 con tramo activo y cursor verdes. El porcentaje controla el relleno visible. Estilos especificos WebKit y Firefox conservan el input range nativo y el teclado. El halo de calidad aumenta de 22 a 48 % de opacidad, manteniendo el color de la escala y el desenfoque; el borde del aro exterior pasa a blanco al 70 %. Capturas revisadas a 320, 390 y 1400 px en .pio/validacion/barra-halo-*.png. La vista previa 35 % fue simulada en el navegador y no cambio el valor de la placa.


### Aro uniforme en calidad de aire

El circulo de calidad desactiva el pseudoelemento decorativo ::after de .orb, que dibujaba bordes superior y derecho de 2 px y producia un semicirculo mas grueso. Conserva el halo al 48 % y el aro exterior completo. Revision visual movil completada.


### Texto central claro

El estado dentro del circulo usa blanco suave #F8FFFD en todos los colores, con sombra discreta para separarlo de los tonos claros. Se retiro la seleccion automatica de texto oscuro/claro. Se reviso la pantalla a 320/390/1400 px y una vista conjunta de los once colores en .pio/validacion/texto-claro-paleta.png.
