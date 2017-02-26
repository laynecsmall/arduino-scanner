import serial, pdb, time, re, sys, datetime
from urllib import request, parse

def send_results(base_url, device_name, tag, crossing_data):
    url = base_url + "/new/result"
    request_dict = {"device_name": device_name,
                    "time": datetime.datetime.now(), 
                    "tag": tag,
                    "raw_results": crossing_data}
    request_data = parse.urlencode(request_dict).encode()
    req = request.Request(url, data=request_data)

    resp = request.urlopen(req)

def create_device(base_url, device_name, device_type, sensor_x, sensor_y):
    url = base_url + "/new/device"
    request_dict = {"device_name": device_name,
            "device_type": device_type,
            "sensor_x": sensor_x,
            "sensor_y": sensor_y}

    request_data = parse.urlencode(request_dict).encode()
    req = request.Request(url, data=request_data)

    resp = request.urlopen(req)

store_url = "http://localhost"
device_name = "results"

crossing_data = []
string = ""

print("ready to open")

ser = serial.Serial('COM5', baudrate=115200, timeout=.2)

if sys.argv[1] == 'new_device':
    print("creating device")
    print("Name: {}".format(sys.argv[2]))
    print("Type: {}".format(sys.argv[3]))
    print("Sensor x: {}".format(sys.argv[4]))
    print("Sensor y: {}".format(sys.argv[5]))
    create_device(store_url, sys.argv[2], sys.argv[3], sys.argv[4], sys.argv[5])

    print ("Creation done")
    sys.exit(0)
if "help" in sys.argv[1]:
    print("""
    usage: 
        forward-crossing-to-store.py [help|new_device|tag] <param 1> ...

        if help given, show this message
        if new_device given, then create a new device in the db then exit.
        new_device paramaters:
            name - device name
            type - device type
            sensor_x - x width of device
            sensor_y - y width of device. 

            Example usage:

            forward-crossing-to-store.py new_device test-device arduino-scanner 10 1

        if you give a tag instead of one of the other commands, then that tag will be passed to the scanner, and will be added to any crossing data forwarded to the store.
        allowed characters are anything alphanumeric, comma or colon.
        
        Example usage:

        forward-crossing-to-store.py a:1,b:3,test:true,nodebug
        """)
    sys.exit(0)

elif len(sys.argv) > 1:
    time.sleep(3)
    tag = ' '.join(sys.argv[1:])
    print("Setting tag: '{}'".format(tag))

    ser.write(bytes(tag, 'ascii'))
    time.sleep(1)
    print(ser.read_all().decode('ascii'))


try:
    string = ""
    while True:
        if ser.in_waiting > 0:
            time.sleep(0.04)
            string += ser.read_all().decode("ascii") 

        match = re.search("Total crossings: (\d+)\r?\n\r?\n?\n\r?\n?([\d \n\r]+)+\r?\n?=+", string, re.MULTILINE)

        if match:
            crossing_data = match.groups()[1].strip()
            print("===============")
            print("Crossings: {}".format(match.groups()[0]))
            print("Tag: {}".format(tag))
            print("----------------")
            print(crossing_data)
            string = ""
            send_results(store_url, device_name, tag, crossing_data)
            time.sleep(0.2)
            ser.flushInput()
            ser.flushOutput()

        sys.stdout.flush()
        time.sleep(0.5)

except KeyboardInterrupt:
    #pdb.set_trace() //enable to allow entering debugging with ctrl + c
    pass

