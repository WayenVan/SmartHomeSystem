# SIMPLE SMART HOME SYSTEM

## INTRODUCTION

*SMART HOME SYSTEM* is mainly designed for improving life qualities of  mankind when staying home, particularly in functions of **monitoring** and **controlling**.

## SETUP

* Raspberry PI
* Various sensors (e.g., temperature sensor, humidity sensor, gesture sensor)
* Mobile phone using Android system
* motor
* WebCame

## Device Specific

- [Servo Motor SG90](https://components101.com/servo-motor-basics-pinout-datasheet)
- [CSI camera modul](https://www.raspberrypi.org/documentation/linux/software/libcamera/csi-2-usage.md)
- [Infrared termperature sensor](https://uk.rs-online.com/web/p/infrared-temperature-sensors/8226421/?cm_mmc=UK-PLA-DS3A-_-google-_-CSS_UK_EN_Automation_%26_Control_Gear_Whoop-_-Infrared+Temperature+Sensors_Whoop-_-8226421&matchtype=&aud-772940708119:pla-343565880236&gclid=Cj0KCQiAx9mABhD0ARIsAEfpavSBlWvqE5DV7wFdamN3IiRncqKzo75-zerfkuAlQXv0zkEklwCS5RMaApJXEALw_wcB&gclsrc=aw.ds)
- [HC-SR501 PIR sensor](https://components101.com/hc-sr501-pir-sensor)

## MAIN FUNCTIONS

It is a big work to complete a completely *SMART HOME SYSTEM*, therefore, we choose some cool and interesting thoughts to roughly display the possibilities of the concept, **SMART HOME**.

### Facial recoginition with temperature check

Using opencv to recognize host face meanwhile check temperature (because of COVID-19)  for unlocking the door.

### Smart Door Lock Controller 

Using Android APP to input the command of "OPEN DOOR", then through HTTP protocol to send the message to the Raspberry PI, and then PI processes the message and output the signal of  action "OPEN". Finally, the signal is sent to motor, and motor does the action of "OPEN", using string tied to the door lock. Also, this door lock controller can be activated by facial recognition

### Real-time Reminder of Indoor Temperature and Humidity

Temperature sensor and humidity sensor send the signal to the Raspberry PI, and PI do filtering. After filtering, PI display the data real-time on the console, meanwhile, send the data to the Android APP, through HTTP protocol.

### Infrared Anti-theft Reminder

Using infrared sensor to send the signal to the Raspberry PI, and PI do filtering. After filtering, PI judge whether there is a person existed in home or not, of course, real-time. And this function can link with the first function door controller, to judge whether the person is a theft or not. If there is a theft, PI will send the alert data to the Android APP.

