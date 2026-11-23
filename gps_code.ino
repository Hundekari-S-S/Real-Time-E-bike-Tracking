#include <WiFi.h>
#include <HTTPClient.h>
#include <TinyGPSPlus.h>
#include <HardwareSerial.h>

const char* ssid = "ssid name";     
const char* password = "password of wifi";  
String botToken = "botToken from telegram";  
String chatID   = "chat ID";    

TinyGPSPlus gps;
HardwareSerial SerialGPS(1);
const int WIFI_LED = 2;      
const int ALERT_LED = 4;     
double fenceLat, fenceLon;     
double fenceHalfWidth = 30.0;  
double fenceHalfHeight = 30.0; 
bool fenceSet = false;
bool wasInsideGeofence = false;
String routePoints = "";   
int pointCounter = 0;      
const int MAX_POINTS = 10; 
long lastUpdateId = 0;

void sendToTelegram(String message) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String url = "https://api.telegram.org/bot" + botToken +
                 "/sendMessage?chat_id=" + chatID +
                 "&text=" + message;
    http.begin(url);
    http.GET();
    http.end();
  }
}

void latLonToMeters(float lat1, float lon1, float lat2, float lon2, double &dx, double &dy) {
  double R = 6371000.0; 
  double dLat = (lat2 - lat1) * PI / 180.0;
  double dLon = (lon2 - lon1) * PI / 180.0;
  double latRad = lat1 * PI / 180.0;
  dx = dLon * R * cos(latRad);
  dy = dLat * R;
}

void setGeofence(float lat, float lon) {
  fenceLat = lat;
  fenceLon = lon;
  fenceSet = true;
  String msg = "📍 Geofence set (±30 m)\nCenter: " 
               + String(lat, 6) + "," + String(lon, 6);
  sendToTelegram(msg);
}
void checkGeofence(float lat, float lon) {
  if (!fenceSet) return;
  double dx, dy;
  latLonToMeters(fenceLat, fenceLon, lat, lon, dx, dy);
  bool isInside = (abs(dx) <= fenceHalfWidth && abs(dy) <= fenceHalfHeight);
  if (isInside && !wasInsideGeofence) {
    sendToTelegram("✅ Entered safe geofence (30 m area).");
    digitalWrite(ALERT_LED, LOW);
    wasInsideGeofence = true;
  } 
  else if (!isInside && wasInsideGeofence) {
    String alert = "🚨 Exited geofence!\nLocation: https://maps.google.com/?q=" 
                   + String(lat, 6) + "," + String(lon, 6);
    sendToTelegram(alert);
    digitalWrite(ALERT_LED, HIGH);
    wasInsideGeofence = false;
  }
}

void checkTelegramCommands(float lat, float lon) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String url = "https://api.telegram.org/bot" + botToken +
                "/getUpdates?offset=" + String(lastUpdateId + 1);
    http.begin(url);
    int httpCode = http.GET();
    if (httpCode == 200) {
      String payload = http.getString();
      int idx = payload.lastIndexOf("\"update_id\":");
      if (idx != -1) {
        lastUpdateId = payload.substring(idx + 13, payload.indexOf(",", idx)).toInt();
      }

      if (payload.indexOf("/location") != -1) {
        String msg = "📍 Current Location: https://maps.google.com/?q=" 
                     + String(lat, 6) + "," + String(lon, 6);
        sendToTelegram(msg);
      }
      if (payload.indexOf("/status") != -1) {
        if (wasInsideGeofence) {
          sendToTelegram("✅ Bike is INSIDE the 30 m safe zone.");
        } else {
          sendToTelegram("🚨 Bike is OUTSIDE the 30 m safe zone!");
        }
      }
      if (payload.indexOf("/route") != -1) {
        if (routePoints.length() > 0) {
          String cleanRoute = routePoints;
          if (cleanRoute.endsWith("/")) cleanRoute.remove(cleanRoute.length() - 1);
          String routeLink = "🛣 Route: https://www.google.com/maps/dir/" + cleanRoute;
          sendToTelegram(routeLink);
        } else {
          sendToTelegram("⚠ No route recorded yet.");
        }
      }
    }
    http.end();
  }
}

void setup() {
  Serial.begin(115200);
  SerialGPS.begin(9600, SERIAL_8N1, 16, 17);  // RX=16, TX=17

  pinMode(WIFI_LED, OUTPUT);    
  pinMode(ALERT_LED, OUTPUT);    
  digitalWrite(WIFI_LED, LOW);
  digitalWrite(ALERT_LED, LOW);

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\n✅ WiFi Connected!");
  digitalWrite(WIFI_LED, HIGH);
  sendToTelegram("🚴 Bike Tracker Started! Waiting for GPS fix...");
}

void loop() {
  while (SerialGPS.available() > 0) {
    gps.encode(SerialGPS.read());

    if (gps.location.isUpdated()) {
      float lat = gps.location.lat();
      float lon = gps.location.lng();
      Serial.printf("LAT: %.6f, LNG: %.6f\n", lat, lon);
      if (!fenceSet) {
        setGeofence(lat, lon);
      }
      checkGeofence(lat, lon);
      routePoints += String(lat, 6) + "," + String(lon, 6) + "/";
      pointCounter++;
      if (pointCounter >= MAX_POINTS) {
        String cleanRoute = routePoints;
        if (cleanRoute.endsWith("/")) cleanRoute.remove(cleanRoute.length() - 1);
        String routeLink = "🛣 Route Update: https://www.google.com/maps/dir/" + cleanRoute;
        sendToTelegram(routeLink);
        pointCounter = 0;
      }
      checkTelegramCommands(lat, lon);
      delay(5000); 
    }
  }
}