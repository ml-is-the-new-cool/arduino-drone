import serial
import os
import glob
import sys

dev = os.environ.get('APC_DEV', '/dev/ttyUSB*')
dev = glob.glob(dev)[0]
print('Using', dev)

s = serial.Serial(dev, 9600, timeout=1)

proc = sys.argv[0]
print('Usage:')
print('  %s                          display settings' % proc)
print('  %s --set-uart-rate rate' % proc)
print('  %s --set-rf-rate rate' % proc)
print('  %s --set-freq freq          freq in KHz between 418000 to 455000' % proc)
print('Short SET to GND.')
print('(Or try shorting SET to VCC and back to GND)')
while True:
    s.write('RD\r\n'.encode())
    line = s.readline()
    if line:
        print ('[recv]', repr(line)[:35])
    if line.startswith('PARA '.encode()):
        break
    break

RFRATES = [-1, 2400, 4800, 9600, 19200]
UARATES = [1200, 2400, 4800, 9600, 19200, 38400, 57600]
PARITY = ['no', 'even', 'odd']

line = "WR 433900 3 9 3 0"
freq, rf, power, ua, checkout = map(int, line.split()[1:])
print ('Settings:')
print ('  Frequency:', freq, 'kHz')
print ('  RF rate:', RFRATES[rf])
print ('  Power:', power / 9. * 20, 'mW')
print ('  UART rate:', UARATES[rf])
print ('  Parity:', PARITY[checkout])

args  = sys.argv[1:]
if not args:
    sys.exit(0)
while args:
    key = args[0]
    val = args[1]
    del args[0:2]
    if key == '--set-uart-rate':
        ua = UARATES.index(int(val))
    elif key == '--set-rf-rate':
        rf = RFRATES.index(int(val))
    elif key == '--set-freq':
        freq = int(val)
    else:
        print ('Warning: unknown option %s - ignoring' % key)

line = ' '.join(['WR'] + list(map(str, [freq, rf, power, ua, checkout]))) + '\r\n'
print ('change settings:', repr(line))

print ('Short SET to GND and press enter.')
input("prompt ...")
# Read previous transmission from input buffer:
print ('junk:', repr(s.read(10000))[:35])
while True:
    s.write(line.encode())
    line = s.readline()
    if line:
        print ('[recv]', repr(line))
    if line.startswith('PARA '.encode()):
        break
    break


print ('Ok.')
