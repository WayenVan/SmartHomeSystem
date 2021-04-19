import adafruit_amg88xx
import board
import busio
import time

i2c = board.I2C()
amg = adafruit_amg88xx.AMG88XX(i2c)

while True:
    print(amg.pixels[4][4])
    time.sleep(1)