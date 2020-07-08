red propelers, 1 & 3, clockwise
black propelers, 2 & 4, counter-clockwise

================================================================================
MOTION MECHANICS
================================================================================

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
