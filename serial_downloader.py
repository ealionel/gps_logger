import serial
import sys

dev = serial.Serial('/dev/ttyUSB0', 115200)

FILE_START = "CMD_FS"
FILE_END = "CMD_FS_"

log_filename = ''
log_content = ''

def nextLine(dev):
    return dev.readline().decode('ascii', 'ignore').strip('\r\n')

while True:
    line = nextLine(dev)

    if (line == FILE_START):
        log_filename = nextLine(dev) + ".csv"
        nextLine(dev)
        nextLine(dev)
        while True:
            line = nextLine(dev)
            if (line == FILE_END):
                break
            log_content += line + "\n"
        
        f = open(log_filename, "w")
        f.write(log_content)
        f.close()
        print("Logged " + log_filename)

    print(line)