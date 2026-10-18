"""Prueba HTTP local de Airbit. El guardado real solo se prueba con la opcion explicita."""
import argparse,ipaddress,json,time
from pathlib import Path
from urllib.request import Request,build_opener,ProxyHandler
from urllib.error import HTTPError
from urllib.parse import urlencode

parser=argparse.ArgumentParser(description=__doc__)
parser.add_argument('--acceso',type=Path,default=Path('.pio/validacion/acceso_portal.json'))
parser.add_argument('--guardar-red-actual',action='store_true')
args=parser.parse_args()
access=json.loads(args.acceso.read_text(encoding='utf-8'))
ip=ipaddress.IPv4Address(access['station_ip'])
if not ip.is_private: raise SystemExit('La prueba solo admite una IP local privada.')
base='http://'+str(ip)
key=access['ap_password']
opener=build_opener(ProxyHandler({}))
results={}
def call(path,method='GET',data=None,token=None):
    headers={}
    if token is not None: headers['X-Airbit-Token']=token
    body=None
    if data is not None:
        body=urlencode(data).encode();headers['Content-Type']='application/x-www-form-urlencoded'
    request=Request(base+path,data=body,headers=headers,method=method)
    try:
        with opener.open(request,timeout=7) as response:
            return response.status,response.read().decode('utf-8'),dict(response.headers)
    except HTTPError as error:
        return error.code,error.read().decode('utf-8',errors='replace'),dict(error.headers)
code,html,headers=call('/')
assert code==200 and 'id="wifi-form"' in html and 'section-calidad' in html
assert not any(h.lower()=='www-authenticate' for h in headers)
assert key not in html
assert headers.get('Cache-Control')=='no-store'
results['pagina_sin_autenticacion']=code
code,body,headers=call('/api/estado')
assert code==200
assert not any(h.lower()=='www-authenticate' for h in headers)
state=json.loads(body)
assert state['ap']['activa'] and state['ap']['ssid']==access['ap_name']
assert 'claveWiFi' not in body and 'claveAP' not in body and key not in body
assert state['calibracion']['valida'] and 270<=state['calibracion']['muestras']<=300
reference=state['calibracion']['referencia']
ssid=state['wifi']['ssid']
token=state['csrf']
results['estado_sin_autenticacion']=code
results['referencia_adc']=reference
results['muestras']=state['calibracion']['muestras']
code,_,_=call('/api/wifi','POST',{'ssid':ssid,'password':'','redAbierta':'0'})
assert code==403,code
results['sin_token']=code
code,_,_=call('/api/wifi','POST',{'ssid':'','password':'','redAbierta':'0'},token=token)
assert code==400,code
results['ssid_vacio']=code
code,_,_=call('/api/wifi','POST',{'ssid':ssid,'password':'abc','redAbierta':'0'},token=token)
assert code==400,code
results['clave_corta']=code
if args.guardar_red_actual:
    assert state['wifi']['conectado'] and not state['calibracion']['activa']
    code,body,_=call('/api/wifi','POST',{'ssid':ssid,'password':'','redAbierta':'1' if state['wifi']['redAbierta'] else '0'},token=token)
    assert code==200 and json.loads(body)['guardado'] is True,(code,body)
    results['guardado_misma_red']=code
    connected=False
    for _ in range(20):
        time.sleep(1)
        try:
            code,body,_=call('/api/estado')
            after=json.loads(body)
            if code==200 and after['wifi']['conectado']:
                assert after['wifi']['ssid']==ssid
                assert after['calibracion']['referencia']==reference
                assert after['ap']['activa']
                connected=True;break
        except (OSError,ValueError): pass
    assert connected,'No se confirmo la reconexion en el plazo de prueba'
    results['reconexion_confirmada']=True
    results['calibracion_conservada']=True
output=Path('.pio/validacion/portal_http.json')
output.write_text(json.dumps(results,indent=2),encoding='utf-8')
print(json.dumps(results,ensure_ascii=True))
