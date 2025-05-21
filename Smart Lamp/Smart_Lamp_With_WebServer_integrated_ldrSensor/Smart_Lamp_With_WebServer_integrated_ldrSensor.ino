#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "ASHABUL KAHFI";  // Ganti dengan SSID WiFi Anda
const char* password = "nyaoteunyaho";  // Ganti dengan Password WiFi Anda

#define relayPin 4       // Pin relay/lampu
#define ldrSensor 35     // Pin sensor LDR
const int batasCahaya = 1000; // Batas cahaya (sesuaikan dengan keadaan ruangan)

WebServer server(80);
bool relayState = false; // Status relay
bool autoMode = true;   // Mode otomatis (true = LDR kontrol, false = manual)

void handleRoot() {
    int nilaiCahaya = analogRead(ldrSensor); // Baca nilai LDR
    String html = R"rawliteral(
    <!DOCTYPE html>
    <html>
    <head>
        <title>Smart Lamp</title>
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
        <h2>ESP32 Smart Lamp Control</h2>
        <p>Nilai Cahaya: %LDR_VALUE%</p>
        <p>Mode: %MODE%</p>
        <div class="toggle-container">
            <label class="toggle-switch">
                <input type="checkbox" id="toggle" onclick="toggleRelay()" %CHECKED%>
                <span class="slider"></span>
            </label>
            <span class="status">%STATUS%</span>
        </div>
        <div style="margin-top: 20px;">
            <button onclick="setMode('auto')">Mode Otomatis</button>
            <button onclick="setMode('manual')">Mode Manual</button>
        </div>

        <script>
            function toggleRelay() {
                var xhr = new XMLHttpRequest();
                xhr.open("GET", "/toggle", true);
                xhr.send();
                setTimeout(() => { location.reload(); }, 500);
            }
            function setMode(mode) {
                var xhr = new XMLHttpRequest();
                xhr.open("GET", "/mode?value=" + mode, true);
                xhr.send();
                setTimeout(() => { location.reload(); }, 500);
            }
        </script>
    </body>
    </html>
    )rawliteral";

    // Ganti placeholder dengan nilai aktual
    html.replace("%CHECKED%", relayState ? "checked" : "");
    html.replace("%STATUS%", relayState ? "On" : "Off");
    html.replace("%LDR_VALUE%", String(nilaiCahaya));
    html.replace("%MODE%", autoMode ? "Auto" : "Manual");

    server.send(200, "text/html", html);
}

void handleToggle() {
    if (!autoMode) { // Hanya izinkan toggle manual jika tidak dalam mode otomatis
        relayState = !relayState;
        digitalWrite(relayPin, relayState ? LOW : HIGH); // Logika aktif rendah
    }
    server.sendHeader("Location", "/");
    server.send(303);
}

void handleMode() {
    String mode = server.arg("value");
    if (mode == "auto") {
        autoMode = true;
    } else if (mode == "manual") {
        autoMode = false;
    }
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
        if (counter > 20) { // Restart jika gagal konek setelah 10 detik
            Serial.println("\nGagal terhubung ke WiFi, restart ESP32...");
            ESP.restart();
        }
    }

    Serial.println("\nWiFi Connected!");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());

    pinMode(relayPin, OUTPUT);
    pinMode(ldrSensor, INPUT);
    digitalWrite(relayPin, HIGH); // Matikan relay saat awal

    // Daftarkan handler
    server.on("/", handleRoot);
    server.on("/toggle", handleToggle);
    server.on("/mode", handleMode);
    server.begin();
    Serial.println("Server dimulai di port 80!");
}

void loop() {
    server.handleClient();

    // Kontrol otomatis berdasarkan LDR jika dalam mode auto
    if (autoMode) {
        int nilaiCahaya = analogRead(ldrSensor);
        Serial.print("Nilai Cahaya: ");
        Serial.println(nilaiCahaya);

        if (nilaiCahaya < batasCahaya) {
            relayState = true;
            digitalWrite(relayPin, HIGH); 
        } else {
            relayState = false;
            digitalWrite(relayPin, LOW); 
        }
    }

    delay(200);
}