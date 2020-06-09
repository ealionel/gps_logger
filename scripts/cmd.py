import serial
import sys
import time

dev = serial.Serial('/dev/ttyUSB0', 115200)

# COMMANDS DEFINITION

LIST_CMD = "ls"
DL_CMD = "dl"
INT_CMD_DEFAULT = -2

LS_TOK = "LS"
LS_TOK_END = "LS_"

DL_TOK = "DL"
DL_TOK_END = "DL_"

ERROR_TOK = "ER"

def next_line(dev):
    return dev.readline().decode('ascii', 'ignore').strip('\r\n')


def get_cmd(cmd, arg):
    int_cmd = INT_CMD_DEFAULT
    if (cmd == LIST_CMD):
        int_cmd = -1
    elif (cmd == DL_CMD):
        if (not arg):
            print('dl: No argument ID specified')
        int_cmd = int(arg)
    else:
        print(f'Unknown command : {cmd}')
    
    int_cmd = int_cmd.to_bytes(length=2, byteorder='little', signed=True)

    return int_cmd

def read_output():
    line = next_line(dev)
    parsed = []

    if (line == ERROR_TOK):
        print("Error with command or arguments")
        return
    elif (line == LS_TOK):
        print('Listing log entries :')
        while (True):
            line = next_line(dev)
            if (line == LS_TOK_END):
                break
            parsed.append(parse_index_entry(line).copy())
        format_index_entries(parsed)

    elif (line == DL_TOK):
        log_id = next_line(dev)
        date = next_line(dev)
        time = next_line(dev)
        print(f'Downloading RUN #{log_id}')
        
        while (True):
            line = next_line(dev)
            if (line == DL_TOK_END):
                break

            parsed += line + '\n'

        save_log_file(log_id, date, time, parsed)
    else:
        print('Unknown token received')

    return parsed

def convert_date(date, separator):
    tmp = date.split(separator)
    for i in range(len(tmp)):
        tmp[i] = tmp[i].rjust(2, '0')

    return ''.join(tmp)

def save_log_file(log_id, date, time, content):
    date_fmt = convert_date(date, '/')
    time_fmt = convert_date(time, ':')

    filename = log_id + "_" + date_fmt + time_fmt + ".csv"

    f = open(filename, 'w')
    f.write(''.join(content))
    f.close()

    print('Saved ' + filename)

def format_index_entries(entries):
    for e in entries:
        # print(e)
        print(f'-> #{e["id"]} (Created on {e["date"]} at {e["time"]})')

def parse_index_entry(line):
    parsed = line.split(',')
    
    entry = {
        'id': parsed[0],
        'date': parsed[1],
        'time': parsed[2]
    }

    return entry

if __name__ == "__main__":
    if (len(sys.argv) < 2):
        print('You must specify a command')
        exit

    cmd = sys.argv[1]
    arg = -2

    if (len(sys.argv) > 2):
        arg = sys.argv[2]

    # Token sent by arduino when initialized
    while (True):
        if (next_line(dev) == 'i'):
            break

    coded_cmd = get_cmd(cmd, arg)
    dev.write(coded_cmd)
    read_output()