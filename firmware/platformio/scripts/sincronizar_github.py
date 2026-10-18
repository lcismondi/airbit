"""Prepara la copia Git del firmware sin hacer commit, push ni publicar una release."""
from pathlib import Path
import argparse, json, shutil, subprocess

root = Path(__file__).resolve().parent.parent
parser = argparse.ArgumentParser(description=__doc__)
parser.add_argument('--checkout', type=Path, default=root/'repositorio/airbit')
args = parser.parse_args()
checkout = args.checkout.resolve()
expected = 'https://github.com/lcismondi/airbit.git'
if not (checkout/'.git').exists():
    subprocess.run(['git','clone',expected,str(checkout)],check=True)
remote = subprocess.run(['git','-C',str(checkout),'remote','get-url','origin'],check=True,capture_output=True,text=True).stdout.strip()
if remote.rstrip('/') != expected:
    raise SystemExit('El destino no corresponde al repositorio Airbit esperado.')
target = checkout/'firmware/platformio'
target.mkdir(parents=True,exist_ok=True)
files = [root/name for name in ['README.md','platformio.ini','.gitignore','.gitattributes','CHANGELOG.md','Airbit_v0.2.code-workspace']]
for directory in ['src','include','lib','test','scripts','docs']:
    files.extend(p for p in (root/directory).rglob('*') if p.is_file() and '__pycache__' not in p.parts and p.suffix!='.pyc' and p.name!='configuracion_local.h')
files.append(root/'.vscode/extensions.json')
owned = set()
for path in files:
    if not path.exists(): continue
    rel = path.relative_to(root).as_posix();owned.add(rel)
    dest = target/rel;dest.parent.mkdir(parents=True,exist_ok=True);shutil.copy2(path,dest)
manifest = target/'.airbit-files.json'
if manifest.exists():
    for rel in set(json.loads(manifest.read_text()))-owned:
        old = (target/rel).resolve()
        if target.resolve() not in old.parents:
            raise SystemExit('Ruta invalida en el manifiesto anterior.')
        if old.is_file():old.unlink()
manifest.write_text(json.dumps(sorted(owned),indent=2)+'\n',encoding='utf-8')
print(json.dumps({'checkout':str(checkout),'firmware':str(target),'archivos':len(owned),'publicado':False}))
