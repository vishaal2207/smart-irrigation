#include <WiFi.h>
#include <WebServer.h>
#include "frontend.h"

// Wi-Fi Access Point credentials
const char* ssid = "Smart-Irrigation";
const char* password = "Irrigation123";

// Web Server on port 80
WebServer server(80);

// Mock State Variables
int zone1Moisture = 32;
int zone2Moisture = 58;
String waterLevel = "OK";
float flow = 0.0;
bool pump = false;
bool valve1 = false;
bool valve2 = false;
String mode = "AUTO";
bool lora = false;

// Function to send CORS headers (good practice for APIs)
void sendCORSHeaders() {
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.sendHeader("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
  server.sendHeader("Access-Control-Allow-Headers", "Content-Type");
}

// ---- Static File Handlers ----

void handleRoot() {
  server.send(200, "text/html", index_html);
}

void handleCss() {
  server.send(200, "text/css", style_css);
}

void handleJs() {
  server.send(200, "application/javascript", script_js);
}

// ---- API Endpoints ----

void handleStatus() {
  sendCORSHeaders();
  
  // Construct JSON manually to avoid external library dependencies for beginners
  String json = "{";
  json += "\"zone1Moisture\":" + String(zone1Moisture) + ",";
  json += "\"zone2Moisture\":" + String(zone2Moisture) + ",";
  json += "\"waterLevel\":\"" + waterLevel + "\",";
  json += "\"flow\":" + String(flow) + ",";
  json += "\"pump\":" + String(pump ? "true" : "false") + ",";
  json += "\"valve1\":" + String(valve1 ? "true" : "false") + ",";
  json += "\"valve2\":" + String(valve2 ? "true" : "false") + ",";
  json += "\"mode\":\"" + mode + "\",";
  json += "\"lora\":" + String(lora ? "true" : "false");
  json += "}";
  
  server.send(200, "application/json", json);
}

void handleZone1() {
  sendCORSHeaders();
  if (!server.hasArg("plain")) {
    server.send(400, "text/plain", "Body not received");
    return;
  }
  
  String body = server.arg("plain");
  if (body.indexOf("\"ON\"") > 0) {
    valve1 = true;
    Serial.println("ZONE1 ON");
  } else if (body.indexOf("\"OFF\"") > 0) {
    valve1 = false;
    Serial.println("ZONE1 OFF");
  }
  
  server.send(200, "application/json", "{\"success\":true}");
}

void handleZone2() {
  sendCORSHeaders();
  if (!server.hasArg("plain")) {
    server.send(400, "text/plain", "Body not received");
    return;
  }
  
  String body = server.arg("plain");
  if (body.indexOf("\"ON\"") > 0) {
    valve2 = true;
    Serial.println("ZONE2 ON");
  } else if (body.indexOf("\"OFF\"") > 0) {
    valve2 = false;
    Serial.println("ZONE2 OFF");
  }
  
  server.send(200, "application/json", "{\"success\":true}");
}

void handleMode() {
  sendCORSHeaders();
  if (!server.hasArg("plain")) {
    server.send(400, "text/plain", "Body not received");
    return;
  }
  
  String body = server.arg("plain");
  if (body.indexOf("\"AUTO\"") > 0) {
    mode = "AUTO";
    Serial.println("MODE AUTO");
  } else if (body.indexOf("\"MANUAL\"") > 0) {
    mode = "MANUAL";
    Serial.println("MODE MANUAL");
  }
  
  server.send(200, "application/json", "{\"success\":true}");
}

void handleAllOff() {
  sendCORSHeaders();
  valve1 = false;
  valve2 = false;
  pump = false;
  Serial.println("ALL OFF");
  
  server.send(200, "application/json", "{\"success\":true}");
}

// Handle preflight requests for POST
void handleOptions() {
  sendCORSHeaders();
  server.send(204);
}

void setup() {
  Serial.begin(115200);
  delay(1000); // Give Serial monitor time to connect

  Serial.println("\nSmart Irrigation ESP32 Server");

  // Setup ESP32 as a Wi-Fi Access Point
  WiFi.softAP(ssid, password);
  IPAddress IP = WiFi.softAPIP();
  
  Serial.println("WiFi AP started");
  Serial.print("SSID: ");
  Serial.println(ssid);
  Serial.print("IP: ");
  Serial.println(IP); // Will print 192.168.4.1

  // Map Routes to Functions
  
  // Static Files
  server.on("/", HTTP_GET, handleRoot);
  server.on("/style.css", HTTP_GET, handleCss);
  server.on("/script.js", HTTP_GET, handleJs);

  // API Routes
  server.on("/api/status", HTTP_GET, handleStatus);
  server.on("/api/zone1", HTTP_POST, handleZone1);
  server.on("/api/zone2", HTTP_POST, handleZone2);
  server.on("/api/mode", HTTP_POST, handleMode);
  server.on("/api/alloff", HTTP_POST, handleAllOff);

  // Handle CORS Preflight
  server.on("/api/zone1", HTTP_OPTIONS, handleOptions);
  server.on("/api/zone2", HTTP_OPTIONS, handleOptions);
  server.on("/api/mode", HTTP_OPTIONS, handleOptions);
  server.on("/api/alloff", HTTP_OPTIONS, handleOptions);

  // Start the server
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  // Listen for incoming clients
  server.handleClient();
}
