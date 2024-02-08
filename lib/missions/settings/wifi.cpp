#include "system.h"
#include "settings.h"

class WifiInterfaceClass {
public:
    void readInput() {
        if (readButton(LEFT, 3)) {
            finishSettingMission = true;
        }
        if (readButton(ENTER, 5)) {
            // Connect Procedure
            if (WiFi.status() != WL_CONNECTED) {
                setupAP();
                connect();
            } else {
                WiFi.disconnect();
            }
        }
    }
    
    void setupAP() {
        // Connect to wifi
        // Step 1: Show Screen
        u8g2.clearBuffer();
        u8g2.setCursor(52,10);
        u8g2.print("Wifi");
        u8g2.setCursor(3,20);
        u8g2.print("Status: Connecting");
        u8g2.setCursor(3,30);
        u8g2.print("Use your phone to");
        u8g2.setCursor(3,40);
        u8g2.print("connect this device");
        u8g2.setCursor(3,50);
        u8g2.print("and visit 10.0.1.1");
        u8g2.setCursor(3,60);
        u8g2.print("to configure");
        u8g2.sendBuffer();

        // Step 2: Switch Wifi Mode to AP-STA, Setup AP Environment
        Serial.println("Configuring access point...");
        WiFi.mode(WIFI_AP);
        networks = scanNetworks();
        IPAddress AP_local_IP(10, 0, 1, 1);
        IPAddress AP_gateway(10, 0, 1, 1);
        IPAddress AP_subnet(255, 255, 255, 0);
        dnsServer.start(53, "*", AP_local_IP);
        WiFi.softAPConfig(AP_local_IP, AP_gateway, AP_subnet);
        WiFi.softAP("ESP32-AP[Wifi Configuration]");
        server->begin();
        Serial.println("Server started");
        // Server started.
    }

    void connect() {

        while (!readButton(LEFT, 3)) {
            dnsServer.processNextRequest();
            WiFiClient client = server->available();
            if (client) {
                String currentLine = "";
                while (client.connected()) {
                    if (client.available()) {
                        char c = client.read();
                        if (c == '\n') {
                            if (currentLine.length() == 0) {
                                // Write the HTTP response
                                writeAPResponse(client);
                            }
                            else {
                                if (currentLine.startsWith("GET") && currentLine[5] == 'C') {
                                    // If clicked connect btn on phone,
                                    // it will send a GET request like:
                                    // GET /C?ssid=SSID&p=PASSWORD HTTP/1.1
                                    // we'll process it here and try to connect.
                                    if (connectionRequest(currentLine)) {                    
                                        u8g2.clearBuffer();
                                        u8g2.setCursor(52,10);
                                        u8g2.print("Wifi");
                                        u8g2.setCursor(3,20);
                                        u8g2.print("Connect Success");
                                        u8g2.sendBuffer();
                                        delay(1000);
                                    } else {
                                        u8g2.clearBuffer();
                                        u8g2.setCursor(52,10);
                                        u8g2.print("Wifi");
                                        u8g2.setCursor(3,20);
                                        u8g2.print("Connect Failed");
                                        u8g2.sendBuffer();
                                        delay(1000);
                                    }
                                    break;
                                }
                                currentLine = "";
                            }
                        } else if (c != '\r') {
                            currentLine += c;
                        }
                    }
                }
                client.stop();
            }
        }
    }

    bool connectionRequest(String params) {
        // Format: C?ssid=SSID&p=PASSWORD
        // Copied from another's, not that elegant.
        int i = params.indexOf('=', 0);
        int i2 = params.indexOf('&', 0);
        String ssid = params.substring(i + 1, i2);
        ssid.replace('+', ' '); // remove html encoding

        i = params.indexOf('=', i2);
        String pass = params.substring(i + 1, params.length() - 9);
        Serial.println(ssid);
        Serial.println(pass);
        
        return connectWifi(ssid, pass); // Globals. Me hates it
    }

    bool connectWifi(String ssid, String pass) {
        ssid.toCharArray(ssidA, 99);
        pass.toCharArray(passA, 99);
        WiFi.mode(WIFI_STA);
        WiFi.disconnect();
        Serial.println("Trying to connect to " + ssid + " with passwd:" + pass);
        WiFi.begin(ssidA, passA);

        delay(2000);
        if (WiFi.status() == WL_CONNECTED) {
            return true;
        } else {
            return false;
        }
    }

    void writeAPResponse(WiFiClient client) {
        // Write OK Header
        Serial.println("Writing AP response to client");
        client.println("HTTP/1.1 200 OK");
        client.println("Content-type:text/html");
        client.println();

        String htmlParsed = "<!DOCTYPE html><html><head>	<title>ESP32 connect to Wifi</title>	<link rel=\"stylesheet\" href=\"https://cdnjs.cloudflare.com/ajax/libs/mini.css/3.0.1/mini-default.min.css\"></head><body>	<h1 align=\"center\">ESP32 connect to Wifi</h1>	<br /><br />	<form action=\"/C\" method=\"GET\">		<fieldset>			<legend>Connect to wifi</legend>			<div class=\"col-sm-12 col-md-6\">				<select name=\"ssid\">					%SSIDLIST%				</select>			</div>			<div class=\"row\">				<div class=\"col-sm-8 col-md-8\">					<label for=\"password\">Password</label>					<input name=\"p\" type=\"password\" id=\"password\" placeholder=\"Password\" />				</div>			</div>			<button class=\"submit\">Connect</button>			</div>		</fieldset>	</form></body></html>";
        htmlParsed.replace("%SSIDLIST%", networks);
        client.print(htmlParsed);
        client.println();
        Serial.println("Wrote AP response to client");
    }

    String scanNetworks()
    {
        Serial.println("scan start");

        // WiFi.scanNetworks will return the number of networks found
        int n = WiFi.scanNetworks();
        Serial.println("scan done");
        if (n == 0)
        {
            Serial.println("no networks found");
        }
        else
        {
            Serial.print(n);
            Serial.println(" networks found");
            String htmloptions = "";
            for (int i = 0; i < n; ++i)
            {
                // Print SSID and RSSI for each network found
                Serial.print(i + 1);
                Serial.print(": ");
                Serial.print(WiFi.SSID(i));
                Serial.print(" (");
                Serial.print(WiFi.RSSI(i));
                Serial.print(")");
                Serial.println((WiFi.encryptionType(i) == WIFI_AUTH_OPEN) ? " " : "*");
                htmloptions += "<option>" + String(WiFi.SSID(i)) + "</option>";
                delay(10);
                }
            Serial.println(htmloptions);
            return htmloptions;
        }
        Serial.println("");
        return "";
    }

    char ssidA[100], passA[100];
    WiFiServer *server = new WiFiServer(80);
    String networks = "";
    DNSServer dnsServer;

};

void wifi() {
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_t0_11_tr);
    WifiInterfaceClass* WifiInterface = new WifiInterfaceClass();
    while (!finishSettingMission) {
        u8g2.clearBuffer();
        if (WiFi.status() != WL_CONNECTED) {
            u8g2.setCursor(52, 10);
            u8g2.print("Wifi");
            u8g2.setCursor(3, 20);
            u8g2.print("Status: Disconnected");
            u8g2.setCursor(3, 30);
            u8g2.print("Press Enter to");
            u8g2.setCursor(3, 40);
            u8g2.print("connect");
        } else {
            u8g2.setCursor(52, 10);
            u8g2.print("Wifi");
            u8g2.setCursor(3, 20);
            u8g2.print("Status: Connected");
            u8g2.setCursor(3, 30);
            u8g2.print("Press Enter to");
            u8g2.setCursor(3, 40);
            u8g2.print("disconnect");
            u8g2.setCursor(3, 50);
            u8g2.print("SSID:" + String(WiFi.SSID()));
            u8g2.setCursor(3, 60);
            u8g2.print("IP:" + WiFi.localIP().toString());
        }
        WifiInterface->readInput();
        u8g2.sendBuffer();
    }
    delete WifiInterface;
}