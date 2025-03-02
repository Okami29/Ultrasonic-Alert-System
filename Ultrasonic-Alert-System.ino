#include <WiFi.h>
#include <WebServer.h>

// WiFi Access Point Credentials
const char* ssid = "ESP32_AP";
const char* password = "12345678";

// Web server on port 80
WebServer server(80);

// LED and Ultrasonic Sensor Pins
const int ledPin = 35;
const int trigPin = 5;
const int echoPin = 18;

bool ledState = false; // LED state
int distanceLimit = 100; // Default distance limit in cm

// Function to measure distance
long getDistance() {
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    long duration = pulseIn(echoPin, HIGH);
    long distance = duration * 0.034 / 2; // Convert to cm
    return distance;
}

void setup() {
    pinMode(ledPin, OUTPUT);
    digitalWrite(ledPin, LOW);

    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);

    Serial.begin(115200);

    WiFi.softAP(ssid, password);
    Serial.println("Access Point started");
    Serial.print("AP IP address: ");
    Serial.println(WiFi.softAPIP());

    server.on("/", handleRoot);
    server.on("/setLimit", handleSetLimit);
    server.on("/getDistance", handleGetDistance);

    server.begin();
    Serial.println("Web server started");
}

void loop() {
    server.handleClient();
}

// Serve the HTML page
void handleRoot() {
    String html = "<!DOCTYPE html>"
                  "<html>"
                  "<head>"
                  "<title>ESP32 Ultrasonic Sensor</title>"
                  "<style>"
                  "body { text-align: center; font-family: Arial, sans-serif; transition: background-color 0.5s; }"
                  ".red { background-color: red; }"
                  "</style>"
                  "<script>"
                  "function updateDistance() {"
                  " fetch('/getDistance')"
                  " .then(response => response.json())"
                  " .then(data => {"
                  " document.getElementById('distance').innerText = data.distance + ' cm';"
                  " document.body.className = data.alert ? 'red' : '';"
                  " });"
                  "}"
                  "function setLimit() {"
                  " var limit = document.getElementById('limitInput').value;"
                  " fetch('/setLimit?value=' + limit);"
                  "}"
                  "setInterval(updateDistance, 1000);"
                  "</script>"
                  "</head>"
                  "<body>"
                  "<h1>ESP32 Ultrasonic Sensor</h1>"
                  "<p>Distance: <span id='distance'>-- cm</span></p>"
                  "<input type='number' id='limitInput' value='100' placeholder='Set Distance Limit' />"
                  "<button onclick='setLimit()'>Set Limit</button>"
                  "</body>"
                  "</html>";
    server.send(200, "text/html", html);
}

// Handle setting distance limit
void handleSetLimit() {
    if (server.hasArg("value")) {
        distanceLimit = server.arg("value").toInt();
        Serial.print("New Distance Limit Set: ");
        Serial.println(distanceLimit);
    }
    server.send(200, "text/plain", "Limit updated");
}

// Handle getting distance and checking alert condition
void handleGetDistance() {
    long distance = getDistance();
    bool alert = distance < distanceLimit;

    // Update LED based on alert state
    digitalWrite(ledPin, alert ? HIGH : LOW);

    // Send JSON response
    String jsonResponse = "{ \"distance\": " + String(distance) + ", \"alert\": " + String(alert ? "true" : "false") + " }";
    server.send(200, "application/json", jsonResponse);
}
