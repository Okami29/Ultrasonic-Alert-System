# Ultrasonic-Alert-System
This ESP32 project hosts a web server to monitor an HC-SR04 ultrasonic sensor. Users set a distance limit, and if the measured distance falls below it, the webpage turns red and an LED lights up. When the distance returns to a safe range, the webpage resets, and the LED turns off, providing real-time alerts.

# Components Required:
- ESP32
- HC-SR04 Ultrasonic Sensor
- 220Ω resistor (for LED)
- LED
- Breadboard & Jumper wires

# Connections

<b>HC-SR04 - ESP32</b>
- VCC (Power) - 5V
- GND (Ground) - GND
- Trig (Trigger) - GPIO 5
- Echo (Echo) - GPIO 18

<b>LED - ESP32</b>
- Anode (Long leg) - GPIO 35 (Through a 220Ω resistor)
- Cathode (Short leg) - GND
