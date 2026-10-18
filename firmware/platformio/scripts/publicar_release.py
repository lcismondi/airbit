"""Publica una release de Airbit ya revisada y sus assets; autentica sin imprimir tokens."""
from pathlib import Path
import argparse, hashlib, json, os, re, subprocess
from urllib.request import Request, urlopen
from urllib.error import HTTPError
from urllib.parse import urlencode, urlparse

parser = argparse.ArgumentParser(description=__doc__)
parser.add_argument('--repo', required=True, help='propietario/repositorio')
parser.add_argument('--commit', required=True)
parser.add_argument('--notas', type=Path, required=True)
parser.add_argument('--assets', type=Path, required=True)
args = parser.parse_args()
if not re.fullmatch(r'[A-Za-z0-9_.-]+/[A-Za-z0-9_.-]+', args.repo):parser.error('Repositorio invalido')
if not re.fullmatch(r'[0-9a-f]{40}', args.commit):parser.error('Commit invalido')
folder = args.assets.resolve()
manifests = list(folder.glob('airbit-*-esp8266.json'))
if len(manifests)!=1:raise SystemExit('Se requiere un manifiesto de release unico')
manifest = json.loads(manifests[0].read_text())
if manifest['commit']!=args.commit or manifest['configuracion_privada_incluida'] is not False or manifest['entorno']!='airbit_release':
    raise SystemExit('El manifiesto no corresponde al commit o al entorno publico')
version = manifest.get('version', '')
if not re.fullmatch(r'[0-9]+\.[0-9]+\.[0-9]+', version):raise SystemExit('Version de release invalida')
prefix = 'airbit-v'+version+'-esp8266'
expected = {prefix+suffix for suffix in ['.bin', '.elf', '.json']}
if manifests[0].name != prefix+'.json':raise SystemExit('Nombre del manifiesto inesperado')
checks = {}
for line in (folder/'SHA256SUMS.txt').read_text().splitlines():
    digest,name = line.split('  ',1)
    if name not in expected or name in checks or not re.fullmatch(r'[0-9a-f]{64}', digest):raise SystemExit('Nombre o checksum de asset invalido')
    if hashlib.sha256((folder/name).read_bytes()).hexdigest()!=digest:raise SystemExit('Checksum incorrecto: '+name)
    checks[name]=digest
names = sorted(checks)+['SHA256SUMS.txt']
if set(checks)!=expected:raise SystemExit('Se esperan BIN, ELF, manifiesto y SHA256SUMS')
if manifest.get('binario')!=prefix+'.bin' or manifest.get('sha256')!=checks[prefix+'.bin'] or manifest.get('bytes')!=(folder/(prefix+'.bin')).stat().st_size:
    raise SystemExit('El binario no coincide con el manifiesto')
token = os.environ.get('GH_TOKEN') or os.environ.get('GITHUB_TOKEN')
if not token:
    env=dict(os.environ,GIT_TERMINAL_PROMPT='0',GCM_INTERACTIVE='never')
    credentials=subprocess.run(['git','credential','fill'],input='protocol=https\nhost=github.com\n\n',text=True,capture_output=True,env=env,timeout=20)
    fields=dict(line.split('=',1) for line in credentials.stdout.splitlines() if '=' in line)
    token=fields.get('password')
if not token:raise SystemExit('No hay autenticacion GitHub disponible')
def api(url, method='GET', payload=None, binary=None):
    if urlparse(url).hostname not in ['api.github.com','uploads.github.com']:raise SystemExit('Host de GitHub no permitido')
    data=binary if binary is not None else (json.dumps(payload,ensure_ascii=False).encode() if payload is not None else None)
    headers={'Authorization':'Bearer '+token,'Accept':'application/vnd.github+json','X-GitHub-Api-Version':'2026-03-10','User-Agent':'Airbit-release'}
    if data is not None:headers['Content-Type']='application/octet-stream' if binary is not None else 'application/json'
    request=Request(url,data=data,headers=headers,method=method)
    with urlopen(request,timeout=60) as response:return json.load(response)
base='https://api.github.com/repos/'+args.repo
repo=api(base)
if not repo.get('permissions',{}).get('push'):raise SystemExit('No hay permiso de escritura en el repositorio')
tag='v'+manifest['version']
# La etiqueta ya subida debe identificar exactamente el commit del manifiesto.
ref=api(base+'/git/ref/tags/'+tag)
obj=ref['object']
for _ in range(5):
    if obj['type']=='commit':break
    if obj['type']!='tag':raise SystemExit('Tipo de etiqueta inesperado')
    obj=api(base+'/git/tags/'+obj['sha'])['object']
if obj['type']!='commit' or obj['sha']!=args.commit:raise SystemExit('La etiqueta remota no corresponde al commit del manifiesto')
# El commit debe existir en GitHub antes de crear la release.
commit=api(base+'/commits/'+args.commit)
if commit['sha']!=args.commit:raise SystemExit('Commit remoto inesperado')
try:
    release=api(base+'/releases/tags/'+tag)
    if not release['draft']:raise SystemExit('La release ya esta publicada; no se modifica')
except HTTPError as error:
    if error.code!=404:raise
    release=api(base+'/releases','POST',{'tag_name':tag,'target_commitish':args.commit,'name':'Airbit '+tag,'body':args.notas.read_text(encoding='utf-8'),'draft':True,'prerelease':False})
existing={asset['name']:asset for asset in release.get('assets',[])}
for name in names:
    path=folder/name;digest=hashlib.sha256(path.read_bytes()).hexdigest()
    if name in existing:
        asset=existing[name]
        if asset.get('digest')!='sha256:'+digest or asset['size']!=path.stat().st_size:raise SystemExit('Asset previo diferente: '+name)
    else:
        url=release['upload_url'].split('{',1)[0]+'?'+urlencode({'name':name})
        asset=api(url,'POST',binary=path.read_bytes())
        if asset['state']!='uploaded' or asset['size']!=path.stat().st_size:raise SystemExit('No se confirmo el asset: '+name)
        if asset.get('digest') and asset['digest']!='sha256:'+digest:raise SystemExit('Digest remoto incorrecto: '+name)
    print(json.dumps({'asset':name,'bytes':asset['size'],'sha256':digest}),flush=True)
# Publicar es el ultimo paso, una vez comprobados todos los adjuntos.
release=api(base+'/releases/'+str(release['id']),'PATCH',{'draft':False,'make_latest':'true'})
print(json.dumps({'publicada':not release['draft'],'url':release['html_url'],'tag':release['tag_name']}))
