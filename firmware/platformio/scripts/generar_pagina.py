"""Sincroniza el HTML editable con la pagina PROGMEM servida por Airbit."""
from pathlib import Path

project = Path(__file__).resolve().parent.parent
html = (project / "src" / "configuracion.html").read_text(encoding="utf-8")
if ')AIRBITHTML"' in html:
    raise SystemExit("El HTML contiene el delimitador C++ reservado")
header = '#pragma once\n#include <pgmspace.h>\nstatic const char paginaConfiguracion[] PROGMEM = R"AIRBITHTML(' + html + ')AIRBITHTML";\n'
(project / "include" / "pagina_configuracion.h").write_text(header, encoding="utf-8", newline="\n")
print("Pagina PROGMEM sincronizada")
