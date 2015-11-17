#
# Connect to FIO6 and GND, output to counter.log and screen, kill with Ctrl+C
#
import u3
from datetime import datetime
from time import sleep
f = open('counter.log', 'a')
def counter():
    d = u3.U3()
    d.configU3()
    d.getCalibrationData()
    d.configIO(TimerCounterPinOffset = 6, EnableCounter1 = True, NumberOfTimersEnabled = 1, FIOAnalog = 15)
    u3.Counter1(Reset=True)
    while(True):
        print datetime.now(), d.getFeedback(u3.Counter1(Reset = False) )[0]
        f.write(str(datetime.now() ) + " " +  str(d.getFeedback(u3.Counter1(Reset = False) )[0]) + "\n")
        sleep(0.5)
def main():
    try:
        counter()
    except (KeyboardInterrupt, EOFError):
        f.write(str(datetime.now() ) + " counter stopped.\n")
        pass

if __name__ == '__main__':
    main()
