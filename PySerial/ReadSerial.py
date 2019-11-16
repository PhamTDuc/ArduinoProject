import serial
import random
import time
import eel

data = {'temp': 0, 'humid': 0, 'intensity': 0, 'pressure': 0}

ser = serial.Serial()


@eel.expose
def callPort(pt, baud, timeout=1):
    try:
        if(ser.is_open):
            ser.close()
        ser.port = pt
        ser.baudrate = baud
        ser.timeout = timeout
        ser.open()
        eel.sleep(1.0)
        while ser.is_open:
            try:
                dataPort = ser.readline()
                data['temp'] = float(dataPort)
                # data['temp'] = random.randint(0, 1000)

                dataPort = ser.readline()
                data['humid'] = float(dataPort)
                # data['humid'] = random.randint(1, 500)
                dataPort = ser.readline()
                data['pressure'] = float(dataPort)
                dataPort = ser.readline()
                data['intensity'] = float(dataPort)
                eel.valid()
            except ValueError:
                eel.invalid("<b>Data Invalid</b>: Wrong Port or Poor Signal")
            eel.sleep(0.1)
    except (serial.SerialException, ValueError):
        eel.invalid("Port is Invalid or already Opened <b>Port: </b>" + pt + "<b>   Baudrate: </b>" + str(baud))


@eel.expose
def python_method():
    return data


my_options = {
    'mode': "chrome",  # or "chrome-app",
    'host': 'localhost',
    'port': 8020,
    'chromeFlags': ["--browser-startup-dialog"]
}
eel.init("web")
try:
    eel.start("main.html", block=True, options=my_options)
except (SystemExit, MemoryError, KeyboardInterrupt):
    pass
