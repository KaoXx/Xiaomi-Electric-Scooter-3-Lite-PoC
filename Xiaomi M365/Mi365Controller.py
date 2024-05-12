from bluepy import btle
from bluepy.btle import Scanner, DefaultDelegate
from bluepy.btle import BTLEDisconnectError, BTLEGattError
import codecs
import signal
import sys
import os

# Características
CARACTERISTICA_UUID = "6e400002-b5a3-f393-e0a9-e50e24dcca9e"
# Comandos
BLOQUEAR = "55aa032003700168ff"
DESBLOQUEAR = "55aa032003710167ff"
# Constantes
DURACION_ESPERA = 3
NOMBRE_ARCHIVO = "scooterAddr.txt"
COMANDO = ""

if len(sys.argv) != 3:
    raise Exception("Uso: python script.py [escanear|guardado] [bloquear|desbloquear]")

if sys.argv[2] == "bloquear":
    COMANDO = BLOQUEAR
elif sys.argv[2] == "desbloquear":
    COMANDO = DESBLOQUEAR
else:
    raise Exception("Comando no válido")

def manejador(signum, frame):
    print('\nSaliendo...\n')
    os.system('setterm -cursor on')
    sys.exit(0)

signal.signal(signal.SIGINT, manejador)

class DelegadoEscaneo(DefaultDelegate):
    def __init__(self):
        DefaultDelegate.__init__(self)

escaner = Scanner().withDelegate(DelegadoEscaneo())
dispositivos = escaner.scan(2)

def agregar_direccion_conocida(direccion):
    archivo_existe = os.path.exists('./' + NOMBRE_ARCHIVO)

    if archivo_existe:
        with open(NOMBRE_ARCHIVO) as archivo:
            direcciones = [linea.strip() for linea in archivo]
        for addr in direcciones:
            if addr == direccion:
                return None
    f = open(NOMBRE_ARCHIVO, "a")
    print(direccion, file=f)
    f.close()

def escribir_comando(dispositivo, comando):
    signal.alarm(DURACION_ESPERA)
    peri = btle.Peripheral(dispositivo)
    caracteristicas = peri.getCharacteristics(uuid=CARACTERISTICA_UUID)[0]
    caracteristicas.write(codecs.decode(comando, 'hex'))
    peri.disconnect()
    print("¡Éxito!")
    agregar_direccion_conocida(dispositivo.addr)

def escribir_dispositivos(dispositivos, comando):
    for dispositivo in dispositivos:
        try:
            print("Intentando enviar comando al dispositivo", dispositivo.addr, dispositivo.getScanData())
            escribir_comando(dispositivo, comando)
        except (BTLEDisconnectError, BTLEGattError, TimeoutError):
            print("No se pudo conectar")

def obtener_direcciones_conocidas(dispositivos):
    archivo_existe = os.path.exists('./' + NOMBRE_ARCHIVO)
    dispositivos_conocidos = []
    if archivo_existe:
        print('El archivo existe')
        with open(NOMBRE_ARCHIVO) as archivo:
            direcciones_conocidas = [linea.strip() for linea in archivo]
        print('[%s]' % ','.join(map(str, direcciones_conocidas)))

        for dispositivo in dispositivos:
            for addr in direcciones_conocidas:
                if dispositivo.addr == addr:
                    dispositivos_conocidos.append(dispositivo)
    else:
        raise Exception("No hay direcciones guardadas en el archivo" + NOMBRE_ARCHIVO)

    return dispositivos_conocidos

if sys.argv[1] == "escanear":
    print("Escaneando", len(dispositivos), "dispositivos en área de bluetooth")
    escribir_dispositivos(dispositivos, COMANDO)
elif sys.argv[1] == "guardado":
    dispositivos_conocidos = obtener_direcciones_conocidas(dispositivos)
    if dispositivos_conocidos:
        print("Escaneando", len(dispositivos_conocidos), "dispositivos en área de bluetooth")
        escribir_dispositivos(dispositivos_conocidos, COMANDO)
    else:
        raise Exception("No se pudo encontrar ningún dispositivo conocido en el área")
else:
    raise Exception("Argumentos incorrectos")
