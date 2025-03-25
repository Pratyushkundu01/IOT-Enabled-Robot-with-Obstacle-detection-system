# IOT-Enabled-Robot-with-Obstacle-detection-system
I developed an IoT-enabled robotic car system integrating ESP32 NodeMCU, Arduino Uno, ultrasonic sensors, a servo motor, and an L298N motor driver for motion control. The robot is remotely operated via Blynk IoT, while real-time ultrasonic sensor data is transmitted over TCP/IP for visualization in Processing 4 software.

The ESP32 manages motor control, WiFi communication, and data transmission, while the Arduino Uno handles sensor input and servo motor control. The ultrasonic sensor scans for obstacles, with data transmitted to ESP32 via UART. The ESP32 then sends live readings to Processing 4 for sonar-based obstacle mapping and visualization. The L298N motor driver enables movement, stopping the robot when obstacles are detected within 10 cm.

This system demonstrates real-time wireless data streaming, IoT-based control, and sonar-based obstacle detection, forming the foundation for autonomous navigation. Future developments will focus on enhancing autonomy and efficiency.
