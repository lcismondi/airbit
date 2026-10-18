"""Comprueba DNS y sondas HTTP reales sin cambiar la configuracion de Airbit."""
import argparse, ipaddress, json, secrets, socket, struct
from pathlib import Path
from urllib.request import Request, build_opener, ProxyHandler, HTTPRedirectHandler
from urllib.error import HTTPError

class SinRedireccion(HTTPRedirectHandler):
    def redirect_request(self, req, fp, code, msg, headers, newurl):
        return None

parser = argparse.ArgumentParser(description=__doc__)
parser.add_argument('--acceso', type=Path, default=Path('.pio/validacion/acceso_portal.json'))
parser.add_argument('--ip', help='IP de la interfaz a probar; usar 192.168.4.1 estando conectado al AP')
args = parser.parse_args()
access = json.loads(args.acceso.read_text(encoding='utf-8'))
ip = str(ipaddress.IPv4Address(args.ip or access['station_ip']))
assert ipaddress.ip_address(ip).is_private
opener = build_opener(ProxyHandler({}), SinRedireccion())
base = 'http://' + ip
report = {'interfaz_probada': ip, 'dns': {}, 'http': {}}
for host in ['connectivitycheck.gstatic.com', 'captive.apple.com', 'www.msftconnecttest.com', 'prueba-airbit.invalid']:
    transaction = secrets.randbits(16)
    question = b''.join(bytes([len(part)]) + part.encode('ascii') for part in host.split('.')) + b'\x00' + struct.pack('!HH', 1, 1)
    query = struct.pack('!6H', transaction, 0x0100, 1, 0, 0, 0) + question
    with socket.socket(socket.AF_INET, socket.SOCK_DGRAM) as sock:
        sock.settimeout(6)
        sock.sendto(query, (ip, 53))
        reply, origin = sock.recvfrom(512)
    ident, flags, questions, answers, _, _ = struct.unpack('!6H', reply[:12])
    assert ident == transaction and flags & 0x8000 and flags & 0x000f == 0 and questions == 1 and answers == 1
    assert origin == (ip, 53)
    # DNSServer contesta con la pregunta y un RR A que apunta al AP.
    offset = 12 + len(question)
    assert reply[12:offset] == question and reply[offset:offset+2] == b'\xc0\x0c'
    kind, cls, ttl, size = struct.unpack('!HHIH', reply[offset+2:offset+12])
    answer_ip = socket.inet_ntoa(reply[offset+12:offset+16])
    assert kind == 1 and cls == 1 and ttl == 0 and size == 4 and answer_ip == access['ap_ip']
    report['dns'][host] = answer_ip
for path, host in [('/generate_204', 'connectivitycheck.gstatic.com'), ('/gen_204', 'connectivitycheck.gstatic.com'),
                   ('/hotspot-detect.html', 'captive.apple.com'), ('/library/test/success.html', 'captive.apple.com'),
                   ('/connecttest.txt', 'www.msftconnecttest.com'), ('/ncsi.txt', 'www.msftncsi.com'),
                   ('/redirect', 'www.msftconnecttest.com'), ('/fwlink', 'go.microsoft.com'),
                   ('/success.txt', 'detectportal.firefox.com'), ('/canonical.html', 'captive.apple.com')]:
    try:
        with opener.open(Request(base+path, headers={'Host': host}), timeout=7) as response:
            code = response.status; headers = response.headers
    except HTTPError as error:
        code = error.code; headers = error.headers
    assert code == 302 and headers.get('Location') == base+'/' and headers.get('Cache-Control') == 'no-store', (path, code, dict(headers))
    assert not headers.get('WWW-Authenticate')
    report['http'][path] = code
with opener.open(base+'/', timeout=7) as response:
    assert response.status == 200 and b'id="wifi-form"' in response.read()
with opener.open(base+'/api/estado', timeout=7) as response:
    state = json.loads(response.read())
assert state['ap']['activa'] and state['ap']['cautivo']
report['wifi_conectado'] = state['wifi']['conectado']
report['calibracion_referencia'] = state['calibracion']['referencia']
report['calibracion_muestras'] = state['calibracion']['muestras']
if ip == access['ap_ip']:
    for path in ['/', '/ruta-desconocida']:
        try:
            opener.open(Request(base+path, headers={'Host': 'prueba-airbit.invalid'}), timeout=7)
            raise AssertionError('Falta redireccion del host o ruta cautiva')
        except HTTPError as error:
            assert error.code == 302 and error.headers.get('Location') == base+'/'
    report['host_y_ruta_ap'] = True
Path('.pio/validacion/portal_cautivo.json').write_text(json.dumps(report, indent=2)+'\n', encoding='utf-8')
print(json.dumps(report, ensure_ascii=True))
