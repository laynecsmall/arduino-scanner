import serial, pdb, time, re, atexit, sys, datetime
from urllib import request, parse

store_url = "http://localhost/new/result"
device_name = "scanner-ad"

crossing_data = []
string = ""

print("ready to open")

ser = serial.Serial('COM5', baudrate=115200, timeout=.2)

if len(sys.argv) > 1:
    time.sleep(3)
    tag = ' '.join(sys.argv[1:])
    print("Setting tag: '{}'".format(tag))

    ser.write(bytes(tag, 'ascii'))
    time.sleep(1)
    print(ser.read_all().decode('ascii'))


def send_results(url, device_name, tag, crossing_data):
    request_dict = {"device_name": device_name,
                    "time": datetime.datetime.now(), 
                    "tag": tag,
                    "raw_results": crossing_data}
    request_data = parse.urlencode(request_dict).encode()
    req = request.Request(store_url, data=request_data)

    resp = request.urlopen(req)

while True:
    if ser.in_waiting > 0:
        string += ser.read_all().decode("ascii") 
        match = re.search("Total crossings: (\d+)\r?\n\r?\n?Tag: ([\w:, ]+)\r?\n\r?\n?([\d \n\r]+)+\r?\n?=+", string, re.MULTILINE)
        if match:
            crossing_data = match.groups()[2].strip()
            if match.groups()[1] == None:
                tag = ""
            else:
                tag = match.groups()[1].strip()
            print("===============")
            print("Crossings: {}".format(match.groups()[0]))
            print("Tag: {}".format(tag))
            print("----------------")
            print(crossing_data)
            string = ""
            send_results(store_url, device_name, tag, crossing_data)
    sys.stdout.flush()

@atexit.register
def exit_proceedure():
    print("exiting")
    ser.close()

