"""Captura COM9 sin ordenar reset, con hora local y salida a un archivo de log."""
from pathlib import Path
import argparse
import datetime as dt
import sys
import time
import serial

def main():
    sys.stdout.reconfigure(encoding="utf-8",errors="replace")
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--port",default="COM9")
    parser.add_argument("--baud",type=int,default=115200)
    parser.add_argument("--seconds",type=float,default=0,help="0: hasta Ctrl+C")
    parser.add_argument("--output",type=Path)
    parser.add_argument("--command",choices=["d","v","h","a","w","l","n"],help="Enviar al abrir; sin reiniciar")
    args=parser.parse_args()
    if args.seconds<0: parser.error("--seconds debe ser 0 o positivo")
    root=Path(__file__).resolve().parent.parent
    output=args.output or root/"logs"/("airbit-"+dt.datetime.now().strftime("%Y%m%d-%H%M%S")+".log")
    port=serial.Serial(port=None,baudrate=args.baud,timeout=.1)
    port.dtr=False;port.rts=False;port.port=args.port
    try: port.open()
    except serial.SerialException as exc:
        print(f"No se pudo abrir {args.port}. Cierra el otro monitor antes de capturar. {exc}",file=sys.stderr)
        return 1
    output.parent.mkdir(parents=True,exist_ok=True)
    pending=bytearray()
    started=time.monotonic()
    try:
        with output.open("a",encoding="utf-8",buffering=1) as log:
            print(f"Monitor {args.port} a {args.baud}; log: {output}",flush=True)
            if args.command: port.write((args.command+"\n").encode("ascii"))
            while not args.seconds or time.monotonic()-started<args.seconds:
                pending.extend(port.read(4096))
                while b"\n" in pending:
                    line,_,pending=pending.partition(b"\n")
                    text=line.decode("utf-8","replace").rstrip("\r")
                    stamp=dt.datetime.now().astimezone().isoformat(timespec="milliseconds")
                    record=f"{stamp} {text}"
                    log.write(record+"\n")
                    print(record,flush=True)
                if len(pending)>8192: # Limitar un flujo sin saltos de linea.
                    log.write(dt.datetime.now().astimezone().isoformat()+" "+pending.decode("utf-8","replace")+"\n")
                    pending.clear()
    except KeyboardInterrupt: pass
    finally:
        port.close()
        if pending:
            with output.open("a",encoding="utf-8") as log:
                log.write(dt.datetime.now().astimezone().isoformat()+" "+pending.decode("utf-8","replace")+"\n")
    return 0
if __name__=="__main__": raise SystemExit(main())
