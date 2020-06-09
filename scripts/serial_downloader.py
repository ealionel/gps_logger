import serial
import sys

dev = serial.Serial('/dev/ttyUSB0', 115200)

FILE_START = "DL"
FILE_END = "DL_"

log_filename = ''
log_content = ''

def nextLine(dev):
    return dev.readline().decode('ascii', 'ignore').strip('\r\n')

print('Waiting for data to be read...')

while True:
    line = nextLine(dev)

    if (line == FILE_START):
        log_id = nextLine(dev)
        date = nextLine(dev)
        time = nextLine(dev)
        while True:
            line = nextLine(dev)
            if (line == FILE_END):
                break
            log_content += line + "\n"
        
        filename = log_id + '_' + date.replace('/', '', 3) + time.replace(':', '', 3) + ".csv"
        f = open(filename, "w")
        f.write(log_content)
        f.close()
        print('Log received :')
        print(f"id :\t{log_id}")
        print(f"Date :\t{date}")
        print(f"Time :\t{time}")
        print(f"Saved as {filename}")
        

    # print(line)