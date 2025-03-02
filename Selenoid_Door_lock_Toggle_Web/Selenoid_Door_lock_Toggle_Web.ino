#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "Aku";  // Ganti dengan SSID WiFi Anda
const char* password = "aa145399";  // Ganti dengan Password WiFi Anda

WebServer server(80);
int relay = 12;  // Pin relay
bool relayState = false; // Status relay

void handleRoot() {
    String html = R"rawliteral(
    <!DOCTYPE html>
    <html>
    <head>
        <title>ESP32 Door Lock</title>
        <meta name="viewport" content="width=device-width, initial-scale=1">
        <style>
            body { font-family: Arial, sans-serif; text-align: center; margin: 50px; }
            h2 { color: #333; }
            .toggle-container { display: flex; align-items: center; justify-content: center; gap: 10px; margin-top: 20px; }
            .toggle-switch { position: relative; display: inline-block; width: 50px; height: 28px; }
            .toggle-switch input { opacity: 0; width: 0; height: 0; }
            .slider { position: absolute; cursor: pointer; top: 0; left: 0; right: 0; bottom: 0;
                      background-color: #ccc; transition: .4s; border-radius: 34px; }
            .slider:before { position: absolute; content: ""; height: 20px; width: 20px; left: 4px;
                             bottom: 4px; background-color: white; transition: .4s; border-radius: 50%; }
            input:checked + .slider { background-color: #007bff; }
            input:checked + .slider:before { transform: translateX(22px); }
            .status { font-size: 18px; font-weight: bold; color: #555; }
        </style>
    </head>
    <body>
        <h2>ESP32 Door Lock Control</h2>
        <div class="toggle-container">
            <label class="toggle-switch">
                <input type="checkbox" id="toggle" onclick="toggleRelay()" %CHECKED%>
                <span class="slider"></span>
            </label>
            <span class="status">%STATUS%</span>
        </div>

        <script>
            function toggleRelay() {
                var xhr = new XMLHttpRequest();
                xhr.open("GET", "/toggle", true);
                xhr.send();
                setTimeout(() => { location.reload(); }, 500);
            }
        </script>
    </body>
    </html>
    )rawliteral";

    // Ganti %CHECKED% dan %STATUS% berdasarkan status relay
    html.replace("%CHECKED%", relayState ? "checked" : "");
    html.replace("%STATUS%", relayState ? "On" : "Off");

    server.send(200, "text/html", html);
}

void handleToggle() {
    relayState = !relayState;
    digitalWrite(relay, relayState ? LOW : HIGH);
    server.sendHeader("Location", "/");
    server.send(303);
}

void setup() {
    Serial.begin(115200);
    Serial.println("\nMemulai koneksi WiFi...");
    
    WiFi.begin(ssid, password);
    int counter = 0;
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
        counter++;
        if (counter > 20) { // Jika lebih dari 10 detik tidak konek, restart ESP32
            Serial.println("\nGagal terhubung ke WiFi, restart ESP32...");
            ESP.restart();
        }
    }
    
    Serial.println("\nWiFi Connected!");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());

    pinMode(relay, OUTPUT);
    digitalWrite(relay, HIGH); // Matikan relay saat awal

    // Daftarkan handler agar server bisa diakses
    server.on("/", handleRoot);
    server.on("/toggle", handleToggle);
    server.begin(); // Mulai server
    Serial.println("Server dimulai di port 80!");
}

void loop() {
    server.handleClient();
}
