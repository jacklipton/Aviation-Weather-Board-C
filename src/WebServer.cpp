#include "WebServer.h"

WebServer::WebServer() : server(80) {}

void WebServer::setup() {
    server.on("/", HTTP_GET, std::bind(&WebServer::handleRoot, this));
    // Add more routes as needed
    server.begin();
    Serial.println("Web Server started on port 80");
}

void WebServer::handleClient() {
    server.handleClient();
}

void WebServer::handleRoot() {
    server.send(200, "text/plain", "ESP32 Weather Server is Running");
}