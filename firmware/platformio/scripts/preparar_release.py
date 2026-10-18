"""Empaqueta exclusivamente el firmware publico y comprueba que no incluya claves locales."""
from pathlib import Path
import argparse, datetime as dt, hashlib, json, re, subprocess

root = Path(__file__).resolve().parent.parent
parser = argparse.ArgumentParser(description=__doc__)
parser.add_argument('--commit', required=True, help='SHA del commit que se publicara')
args = parser.parse_args()
if not re.fullmatch(r'[0-9a-f]{40}', args.commit):
    parser.error('--commit debe ser un SHA Git completo')
head = subprocess.run(['git', '-C', str(root), 'rev-parse', 'HEAD'], check=True, capture_output=True, text=True).stdout.strip()
if head != args.commit:
    raise SystemExit('El commit indicado no coincide con HEAD. Empaquetar desde el clon Git revisado.')
status = subprocess.run(['git', '-C', str(root), 'status', '--porcelain', '--untracked-files=all'], check=True, capture_output=True, text=True).stdout.strip()
if status:
    raise SystemExit('La copia Git tiene cambios pendientes. Revisar y crear el commit antes de empaquetar.')
version = re.search(r'^#define AIRBIT_FIRMWARE_VERSION "([0-9]+\.[0-9]+\.[0-9]+)"', (root/'include/version_airbit.h').read_text(), re.M).group(1)
source = root/'.pio/build/airbit_release'
local = root/'include/configuracion_local.h'
private = {}
if local.exists():
    private = {field: json.loads('"'+encoded+'"') for field, encoded in re.findall(r'^#define (STASSID|STAPSK|AIRBIT_FORM_URL) "([^\n]*)"$', local.read_text(), re.M)}
files = {name: (source/name).read_bytes() for name in ['firmware.bin', 'firmware.elf']}
for name, data in files.items():
    for field, value in private.items():
        if value and value != '1234567890' and value.encode() in data:
            raise SystemExit('Se detecto un valor local privado en '+name+' ('+field+'). No se empaqueta.')
if version.encode() not in files['firmware.bin']:
    raise SystemExit('El binario no contiene la version declarada. Compilar airbit_release antes de empaquetar.')
output = root/'dist'/('v'+version)
output.mkdir(parents=True, exist_ok=True)
prefix = 'airbit-v'+version+'-esp8266'
for name, data in files.items():
    (output/(prefix+Path(name).suffix)).write_bytes(data)
manifest = {'version': version, 'commit': args.commit, 'entorno': 'airbit_release', 'chip': 'ESP8266EX', 'placa': 'nodemcuv2', 'flash_mb': 4, 'flash_mode': 'dio', 'direccion_carga': '0x0', 'configuracion_privada_incluida': False, 'binario': prefix+'.bin', 'sha256': hashlib.sha256(files['firmware.bin']).hexdigest(), 'bytes': len(files['firmware.bin']), 'generado_utc': dt.datetime.now(dt.timezone.utc).isoformat(timespec='seconds')}
(output/(prefix+'.json')).write_text(json.dumps(manifest, indent=2)+'\n', encoding='utf-8')
checks = []
for suffix in ['.bin', '.elf', '.json']:
    path = output/(prefix+suffix)
    checks.append(hashlib.sha256(path.read_bytes()).hexdigest()+'  '+path.name)
(output/'SHA256SUMS.txt').write_text('\n'.join(checks)+'\n', encoding='ascii')
print(json.dumps({'version': version, 'directorio': str(output), 'bin_sha256': manifest['sha256'], 'archivos': [p.name for p in output.iterdir()]}))
