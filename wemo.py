from ctypes import *

wemoDll = cdll.LoadLibrary("WeMo.dll")

def turn_on():
    wemoDll.turnOn()

def turn_off():
    wemoDll.turnOff()

def get_state():
    return wemoDll.getState() > 0

if __name__ == '__main__':
    import time
    turn_on()
    print get_state()
    time.sleep(3.0)
    turn_off()
    print get_state()

