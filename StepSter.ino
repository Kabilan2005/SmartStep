#define BLYNK_TEMPLATE_ID "TMPL37C5CTY43"
#define BLYNK_TEMPLATE_NAME "Stepster"
#define BLYNK_AUTH_TOKEN "W2WXG6aatl3xMjQb3eBLpSB5qxp6p7T9"

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <TinyGPS++.h>
#include <HardwareSerial.h>



char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "Phone";
char pass[] = "";

// Sample Latitude and Logitude
double refLat = 13.0827;
double refLng = 80.2707;

TinyGPSPlus gps;
HardwareSerial gpsSerial(2);  // Serial2 for GPS
String mapsLink;

void setup() {
  Serial.begin(9600);
  gpsSerial.begin(9600, SERIAL_8N1, 16, 17);
  // RX pin 16, TX pin 17
  Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
}

void loop() {
  Blynk.run();
  Blynk.virtualWrite(V0, LOW);

  while (gpsSerial.available() > 0) {
    gps.encode(gpsSerial.read());
  }

  if (gps.location.isValid()) {
    Serial.println("GPS Data => Lat: " + String(gps.location.lat(), 6) + ", Lng: " + String(gps.location.lng(), 6) + ", Alt: " + String(gps.altitude.meters()));

    Serial.println("Date: " + String(gps.date.day()) + "/" + String(gps.date.month()) + "/" + String(gps.date.year()) + " Time: " + String(gps.time.hour()) + ":" + String(gps.time.minute()) + ":" + String(gps.time.second()));

    // Generate Google Maps link
    mapsLink = "https://www.google.com/maps/@";
    mapsLink += String(gps.location.lat(), 6);
    mapsLink += ",";
    mapsLink += String(gps.location.lng(), 6);
    mapsLink += ",16z";

    Serial.print("Google Maps Link: ");
    Serial.println(mapsLink);

    double distanceMeters = TinyGPSPlus::distanceBetween(refLat, refLng, gps.location.lat(), gps.location.lng());
    Blynk.virtualWrite(V4, distanceMeters);
    if (distanceMeters > 50) Blynk.virtualWrite(V0, HIGH);
  }
  Blynk.virtualWrite(V1, gps.location.lng());
  Blynk.virtualWrite(V2, gps.location.lat());
  Blynk.virtualWrite(V3, mapsLink);


  delay(100000);  // Delay for 10 seconds
}
