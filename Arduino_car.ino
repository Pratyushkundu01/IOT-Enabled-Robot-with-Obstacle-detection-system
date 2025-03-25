#include <Servo.h>

#define TRIG_PIN 7
#define ECHO_PIN 6
#define SERVO_PIN 9
#define OBSTACLE_DISTANCE 10

Servo myServo;
int angle = 0;
bool forward = true;

void setup() {
    Serial.begin(9600);
    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);
    
    myServo.attach(SERVO_PIN);
    myServo.write(0);
}

long getDistance() {
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);

    long duration = pulseIn(ECHO_PIN, HIGH);
    int distance = duration * 0.034 / 2;  // Convert to cm

    if (distance == 0 || distance > 400) {
        return -1;  // Invalid reading
    }
    return distance;
}

void loop() {
    myServo.write(angle);
    delay(50);

    int distance = getDistance();

    if (distance != -1) {
        Serial.print("Angle: ");
        Serial.print(angle);
        Serial.print(" Distance: ");
        Serial.println(distance);
    } else {
        Serial.println("No valid reading");
    }

    if (distance > 0 && distance < OBSTACLE_DISTANCE) {
        Serial.println("STOP");
    }

    if (forward) {
        angle += 5;
        if (angle >= 180) forward = false;
    } else {
        angle -= 5;
        if (angle <= 0) forward = true;
    }

    delay(100);
}
