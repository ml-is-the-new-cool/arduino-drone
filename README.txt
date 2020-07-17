## Thrust measures

![thrust.png](:/e0a6faedd1a84abd938456e009542d5b)

So I should at the very least develop 20% more thrust to handle the 1230g of my drone...

## Some things to remember

### Battery

- Never go under 2.8-3V or above 4.2V per cell.

### Code & implementation

- Don't use Serial.print() or affiliates on the Arduino controlling the drone because it corrupts APC.

## Some things to try

- https://fritzing.org/
- measure tension at each motor with every configuration (1, 2, 3, 4 motors running)
- is there a pb with 1000-2000 range for PWN ?
- measure tension/thrust for each motor ESC plugged onto the PBD & without
- change propellers / motor calibration to have negative weight on a weight == to the one expected with positive values (600g/motor) 


================================================================================
LIENS
================================================================================

https://www.youtube.com/watch?v=yFBvC_zRiek
https://www.youtube.com/watch?v=pjBnFZGfsjI

=== RC / Arduino to Arduino ===
https://www.firediy.fr/article/utiliser-sa-radiocommande-avec-un-arduino-drone-ch-6
https://www.instructables.com/id/Long-Range-18km-Arduino-to-Arduino-Wireless-Commun/

=== RC / LORA ===
https://github.com/spleenware/LoraDrone
https://github.com/winiak/LoRaRC
https://github.com/PepeTheFroggie/EspCopter32
https://github.com/sandeepmistry/arduino-LoRa

=====
liens
=====
# Useful links

- https://www.dummies.com/programming/electronics/electronics-for-dummies-cheat-sheet/

- https://duckduckgo.com/?q=Lipo+Battery+Voltage+Tester%2F+Low+Voltage+Alarm&t=canonical&ia=shopping

- http://www.brokking.net/ymfc-32_auto_main.html

- https://www.mydronelab.com/blog/arduino-quadcopter.html

- https://www.firediy.fr/article/realisation-du-cadre-drone-ch-4

- https://github.com/lobodol/drone-flight-controller/blob/master/drone-flight-controller.ino

- https://forum.arduino.cc/index.php?topic=418938.0


http://www.readytosky.com/e_products/index_fl.asp?big_id=1&small_id=104&page=2
http://www.readytosky.com/e_productshow/?787-RS2212-920KV-CW&CCW-Brushless-Motor-787.html
https://www.ecalc.ch/xcoptercalc.php?lang=fr
https://www.google.com/search?q=readytosky+technical+data&sxsrf=ALeKk01cF8nqjzf0YG7TOMgtN_MD7u4gRw:1591623659676&tbm=isch&source=iu&ictx=1&fir=behL04G_CG6u6M%253A%252CUJVfDL16hYM2YM%252C_&vet=1&usg=AI4_-kQzq-OqCJpG6uIC4-gtVshPMkh-CQ&sa=X&ved=2ahUKEwiZkueWrPLpAhXlsnEKHcpqAmcQ9QEwBXoECAkQCw&biw=1848&bih=942#imgrc=behL04G_CG6u6M:
https://www.drone-doctors.fr/moteurs/372-510-moteur-2212-920kv-readytosky.html
https://www.drone-doctors.fr/chassis-loisir-et-video/366-chassis-s500-480-mm-avec-train-carbone.html
https://artofcircuits.com/?__cf_chl_jschl_tk__=549cebe6797a581c388e3d429ab98ae81ed5f33e-1591695335-0-ATr3qZgjTMzaEzkVNpxvUR0WdnLe4W1AZL6znmxJ9mBHssTBNqew2gwtwevJAq37LzdmLoPM35VNCccStzoD9GKAb92AYcKswVhYQkYw34_dZXL5EoAgyWsug_gw8i7NkrZPYA657KhGqsNCzhN4gIMSlZIvoalIefTVY9JxmDdc2uWfi4bf854-9fv-lqEAQxwO6hgJlVU9YxbQZt6vsV8j4k_t6VF217gl1yffw_3iymD59GXMxv7lCK5HBMbLX0ZPsArAJF83KF-V0thsdHM
https://forum.arduino.cc/index.php?topic=396450
https://forum.arduino.cc/index.php?topic=225329.msg1810764#msg1810764
https://forum.arduino.cc/index.php?topic=373340.0

========
montants
========
94.89€ -> première livraison
31.92€ -> LiPo
06.30€ -> gaines thermo mr bricolage
12.70€ -> gaines & connecteurs balles
04.00€ -> connecteurs XT60
15.50€ -> connecteurs & breadboard mirage des ondes
17.00€ -> achats divers à l'atelier
29.90€ -> fer à souder & fil de fer
15.00€ -> petit matos

212.21€ -> total

=================
achat petit matos
=================
1 plaque électronique vierge, 9
3 rubans de picots, 3
1 paire de connecteurs XT60, 3.50
1 sac de protection pour accumulateur LiPo, 9
1 connecteur XT60 vers Dean, 4


================================================================================
MOTION MECHANICS
================================================================================
red propelers, 1 & 3, clockwise
black propelers, 2 & 4, counter-clockwise


up, yaw (lacet) axis, power up all motors
down, yaw (lacet) axis, power down all motors

forward, pitch (tangage) axis, power down the front pair & power up the back pair
backward, pitch (tangage) axis, power up the front pair & power down the back pair

left side shift, roll (roulis) axis, power down the left pair & power up the right pair
right side shift, roll (roulis) aixs, power up the left pair & power down the right pair

clockwise rotation, yaw (lacet) axis, power up the 1 & 3 pair & power down the 2 & 4 pair
counter-clockwise rotation, yaw (lacet) axis, power down the 1 & 3 pair & power up the 2 & 4 pair

================================================================================
ESC POWER
================================================================================
1. send <+>
2. power up LiPo battery
3. send <->
4. send power like <motor_1, motor_2, motor_3, motor_4> where 1000 < motor_[x] < 2000

all down, <1000,1000,1000,1000>
all up, <1100,1100,1100,1100>

all clockwise, <1100,1100,1000,1000>
all counter-clockwise, <1000,1000,1100,1100>

===

motor 2 stops immediately when receiving 0
motor 4 doesn't stop immediately when receiving 0

================================================================================
THRUST
================================================================================
pwn, thurst
150, 37 
300, 90
500, 200
600, 300
700, 490
800, 580
900, 700
1000, 850

================================================================================
CODE DOCUMENTATION
================================================================================
/**
 * (1, jaune, 5)
 * (3, vert,  6)
 * 
 * (2, bleu,  7)
 * (4, rouge, 8)
 *
 * hélices : (2,4,blanc) (1,3,rien)
 */

/**
 * Usage, according to documentation(https://www.firediy.fr/files/drone/HW-01-V4.pdf) :
 * 
 * 1. Plug your Arduino to your computer with USB cable, open terminal,
 *    then type 1 to send max throttle to every ESC to enter programming mode.
 * 2. Power up your ESCs.
 *    You must hear "beep1 beep2 beep3" tones meaning the power supply is OK.
 * 3. After 2sec, "beep beep" tone emits, meaning the throttle highest point has
 *    been correctly confirmed.
 * 4. Type 0 to send min throttle.
 * 5. Several "beep" tones emits, which means the quantity of the lithium battery cells
 *    (3 beeps for a 3 cells LiPo).
 * 6. A long beep tone emits meaning the throttle lowest point has been correctly confirmed.
 * 7. Type 2 to launch test function. This will send min to max throttle to ESCs to test them.
 *
 * @author mlisthenewcool <contact@hippolyte-debernardi.com>
 */

/**
 * <Servo.h> is used to control the speed of the motors since they work as servomotors.
 */
