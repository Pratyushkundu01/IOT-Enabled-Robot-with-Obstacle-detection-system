import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.net.Socket;

Socket socket;
BufferedReader reader;

String esp32_ip = "192.168.184.231";  // Change to your ESP32's actual IP
int esp32_port = 80;  // Matches ESP32's TCP server

int angle = 0;
float distance = 0;
boolean increasing = true;
PFont font;
int lastDataTime = 0;
int lastAngleUpdateTime = 0;  // Track last angle update

void setup() {
  size(600, 600);
  font = createFont("Consolas", 16, true);
  textFont(font);
  connectToESP32();  // Connect to ESP32 on startup
}

void draw() {
  background(10, 10, 20); // Sci-Fi Dark Background
  translate(width / 2, height - 50); // Move radar to bottom-center

  drawRadarGrid();
  drawRadarLine();
  drawObjectDetection();
  displayData();
  
  // Receive data every 5ms
  if (millis() - lastDataTime > 5) { 
    receiveData();
    lastDataTime = millis();
  }
  
  // Update angle every 50ms
  if (millis() - lastAngleUpdateTime > 50) {
    updateAngle();
    lastAngleUpdateTime = millis();
  }
}

void drawRadarGrid() {
  stroke(0, 255, 0, 150);
  noFill();
  for (int r = 100; r <= 300; r += 100) {
    strokeWeight(2);
    arc(0, 0, r * 2, r * 2, PI, TWO_PI);
  }
  for (int a = 0; a <= 180; a += 30) {
    float x = 300 * cos(radians(a));
    float y = -300 * sin(radians(a));
    stroke(0, 200, 255, 100);
    strokeWeight(1);
    line(0, 0, x, y);
  }
}

void drawRadarLine() {
  stroke(255, 0, 0);
  strokeWeight(2);
  float xLine = 300 * cos(radians(angle));
  float yLine = -300 * sin(radians(angle));
  line(0, 0, xLine, yLine);
}

void drawObjectDetection() {
  if (distance > 2 && distance < 300) {
    float mappedDist = map(distance, 0, 300, 0, 300);
    float xObj = mappedDist * cos(radians(angle));
    float yObj = -mappedDist * sin(radians(angle));
    fill(255, 0, 0, 200);
    stroke(255, 50, 50, 150);
    ellipse(xObj, yObj, 12, 12);
  }
}

void displayData() {
  fill(0, 255, 0);
  text("Angle: " + angle + "°", -280, -50);
  text("Distance: " + distance + " cm", -280, -30);
}

void connectToESP32() {
  try {
    println("Connecting to ESP32...");
    socket = new Socket(esp32_ip, esp32_port);
    reader = new BufferedReader(new InputStreamReader(socket.getInputStream()));
    println("Connected to ESP32 at " + esp32_ip);
  } catch (Exception e) {
    println("Connection Failed: " + e.getMessage());
    socket = null;
  }
}

void receiveData() {
  try {
    if (socket != null && reader.ready()) {
      String received = reader.readLine().trim();
      if (received.length() > 0) {
        float newDistance = float(received);
        if (newDistance > 0 && newDistance < 400) { // Filter out noise
          distance = newDistance;
        }
      }
    }
  } catch (Exception e) {
    println("Error Receiving Data: " + e.getMessage());
    reconnectESP32();
  }
}

void reconnectESP32() {
  try {
    if (socket != null) socket.close();
    delay(50);
    connectToESP32();
  } catch (Exception e) {
    println("Reconnection Failed: " + e.getMessage());
  }
}

void updateAngle() {
  if (increasing) {
    angle += 2;
    if (angle >= 180) increasing = false;
  } else {
    angle -= 2;
    if (angle <= 0) increasing = true;
  }
}
