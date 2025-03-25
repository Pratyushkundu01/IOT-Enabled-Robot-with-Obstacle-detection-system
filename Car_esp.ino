// ✅ Blynk Setup (Fixes compilation error)
#define BLYNK_TEMPLATE_ID "TMPL3TLQEgx5G"
#define BLYNK_TEMPLATE_NAME "Control DC Motor"
#define BLYNK_AUTH_TOKEN "Agy_WK2oHVfV5iDah3KaDKlhmv9pWxVD"

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

// 🔗 WiFi Credentials
char ssid[] = "Redmi 13 5G";  
char pass[] = "a1b2c3d4";  

// ⚙ Motor Driver Pins
#define IN1 25
#define IN2 26
#define IN3 27
#define IN4 14
#define ENA 33  
#define ENB 32  

int speedValue = 200;  
int obstacleDistance = 100;  

// 🎮 Blynk Virtual Pins
#define V_FORWARD V1
#define V_BACKWARD V2
#define V_LEFT V3
#define V_RIGHT V4
#define V_STOP V5
#define V_SPEED V6

WiFiServer server(80);  // WebSocket server for Processing 4
WiFiClient client;       // Persistent client for TCP transmission

unsigned long lastTransmission = 0;  // Timer for sending data

void setup() {
    Serial.begin(115200);
    Serial2.begin(9600, SERIAL_8N1, 16, 17);  // ESP32 RX=16, TX=17 (Connected to Arduino TX=3, RX=2)

    // 🔗 Connect to WiFi
    WiFi.begin(ssid, pass);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\n✅ WiFi Connected: " + WiFi.localIP().toString());

    // 🎮 Start Blynk
    Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

    // 📡 Start TCP Server
    server.begin();

    // ⚙ Setup Motor Pins
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT);
    pinMode(ENA, OUTPUT);
    pinMode(ENB, OUTPUT);

    stopCar();
}

// 🚗 Move Forward
BLYNK_WRITE(V_FORWARD) {
    if (param.asInt() && obstacleDistance > 10) {
        moveForward();
    }
}

// 🔙 Move Backward
BLYNK_WRITE(V_BACKWARD) {
    if (param.asInt()) {
        moveBackward();
    }
}

// ↩ Turn Left
BLYNK_WRITE(V_LEFT) {
    if (param.asInt()) {
        turnLeft();
    }
}

// ↪ Turn Right
BLYNK_WRITE(V_RIGHT) {
    if (param.asInt()) {
        turnRight();
    }
}

// ⏹ Stop Car
BLYNK_WRITE(V_STOP) {
    if (param.asInt()) {
        stopCar();
    }
}

// ⚡ Adjust Speed
BLYNK_WRITE(V_SPEED) {
    speedValue = param.asInt();
    analogWrite(ENA, speedValue);
    analogWrite(ENB, speedValue);
}

// 🚀 Function to Move Forward
void moveForward() {
    Serial.println("Moving Forward");
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    analogWrite(ENA, speedValue);
    analogWrite(ENB, speedValue);
}

// 🔄 Move Backward
void moveBackward() {
    Serial.println("Moving Backward");
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
    analogWrite(ENA, speedValue);
    analogWrite(ENB, speedValue);
}

// ⬅ Turn Left
void turnLeft() {
    Serial.println("Turning Left");
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    analogWrite(ENA, speedValue);
    analogWrite(ENB, speedValue);
}

// ➡ Turn Right
void turnRight() {
    Serial.println("Turning Right");
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
    analogWrite(ENA, speedValue);
    analogWrite(ENB, speedValue);
}

// 🛑 Stop Car
void stopCar() {
    Serial.println("Stopping Car");
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
    analogWrite(ENA, 0);
    analogWrite(ENB, 0);
}

// 📡 Read Ultrasonic Data from Arduino
void readUltrasonicData() {
    if (Serial2.available()) {
        obstacleDistance = Serial2.parseInt();
        Serial.print("📡 Distance Received (in cm): ");
        Serial.println(obstacleDistance);

        if (obstacleDistance > 0) {
            sendToProcessing(obstacleDistance);
        }

        if (obstacleDistance <= 10 && obstacleDistance > 0) {
            Serial.println("⚠️ Obstacle detected! Stopping car.");
            stopCar();
        }
    }
}

// 🌍 Send Data to Processing 4 via TCP (With 100ms Delay)
void sendToProcessing(int distance) {
    unsigned long currentMillis = millis();
    
    // Send data every 100ms
    if (currentMillis - lastTransmission >= 5) {
        lastTransmission = currentMillis;
        
        if (!client.connected()) {
            client = server.available();  // Accept new client if available
        }

        if (client) {
            client.print(String(distance) + "\n");
            Serial.println("📡 Sent to Processing: " + String(distance));
        }
    }
}

void loop() {
    Blynk.run();
    readUltrasonicData();
}
