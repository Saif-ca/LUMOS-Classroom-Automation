#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
#include <DHT.h>

// Wi-Fi Credentials
#define WIFI_SSID "hostspotname"
#define WIFI_PASSWORD "hotspotPassword"

// Firebase Credentials
#define API_KEY "add your api code"
#define DATABASE_URL "firebase link"

// Firebase Authentication
#define USER_EMAIL "firebase mail id"
#define USER_PASSWORD "password"

// Pin Definitions
#define DHT_PIN 4       //D2 (GPIO4)
#define DHT_TYPE DHT11
#define LDR_PIN A0      //A0 (Analog Input)
#define PIR_PIN 5       //D1 (GPIO5)
#define LED_PIN 16      //D0 (GPIO16)
#define LIGHT_RELAY 12  //D6 (GPIO12)
#define FAN_RELAY 14    //D5 (GPIO14)
#define ACS712_PIN A0 

// Firebase Objects
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;
DHT dht(DHT_PIN, DHT_TYPE);

void connectWiFi() {
    Serial.print("Connecting to Wi-Fi");
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    int retryCount = 0;
    while (WiFi.status() != WL_CONNECTED && retryCount < 20) {
        Serial.print(".");
        delay(500);
        retryCount++;
    }
    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("\nWi-Fi Connected!");
    } else {
        Serial.println("\nFailed to connect to Wi-Fi!");
    }
}

void setup() {
    Serial.begin(115200);
    pinMode(LDR_PIN, INPUT);
    pinMode(PIR_PIN, INPUT);
    pinMode(LED_PIN, OUTPUT);
    pinMode(LIGHT_RELAY, OUTPUT);
    pinMode(FAN_RELAY, OUTPUT);

    dht.begin();
    connectWiFi();

    // Firebase Configuration
    config.api_key = API_KEY;
    config.database_url = DATABASE_URL;
    auth.user.email = USER_EMAIL;
    auth.user.password = USER_PASSWORD;

    Firebase.begin(&config, &auth);
    Firebase.reconnectWiFi(true);

    Serial.println("Firebase Initialized!");
}

void updateFirebaseData(const char* path, float value) {
    if (!isnan(value)) {  
        if (Firebase.setFloat(fbdo, path, value)) {
            Serial.print(path);
            Serial.println(" updated successfully!");
        } else {
            Serial.print("Failed to update ");
            Serial.print(path);
            Serial.println(": " + fbdo.errorReason());
        }
    } else {
        Serial.print("Skipping NaN value for ");
        Serial.println(path);
    }
}

float readCurrent() {
    int sensorValue = analogRead(A0);
    float voltage = (sensorValue / 1023.0) * 3.3;  // Convert ADC value to voltage (assuming 3.3V)
    float current = (voltage - 2.5) / 0.185;       // ACS712-5A formula (185mV per A)
    return current;
}

int readLDR() {
    return analogRead(A0);
}

void loop() {
    if (WiFi.status() != WL_CONNECTED) {
        connectWiFi();
    }

    if (Firebase.ready()) {
        int ldrValue = analogRead(LDR_PIN);
        int pirState = digitalRead(PIR_PIN);
        float temperature = dht.readTemperature();
        float humidity = dht.readHumidity();

        Serial.println("\n--- Sensor Readings ---");
        Serial.print("Temp (C): ");
        Serial.println(temperature);
        Serial.print("Humidity (%): ");
        Serial.println(humidity);
        Serial.print("LDR Value: ");
        Serial.println(ldrValue);
        Serial.print("PIR State: ");
        
        if (pirState == HIGH) {
            Serial.println("Motion Detected!");
            digitalWrite(LED_PIN, HIGH);
        } else {
            Serial.println("No Motion");
            digitalWrite(LED_PIN, LOW);
        }

        // Upload sensor data
        updateFirebaseData("/classrooms/2021/IT/classroom101/devices/sensors/temperature", temperature);
        updateFirebaseData("/classrooms/2021/IT/classroom101/devices/sensors/moisture", humidity);
        updateFirebaseData("/classrooms/2021/IT/classroom101/devices/sensors/light", ldrValue);
        delay(100);
        float current = readCurrent();
        Serial.print("Current Consumption (A): ");
        Serial.println(current);
        updateFirebaseData("/classrooms/2021/IT/classroom101/devices/sensors/energyflow", current);
        
        if (Firebase.setBool(fbdo, "/classrooms/2021/IT/classroom101/devices/sensors/motion", pirState)) {
            Serial.println("Motion data sent!");
        } else {
            Serial.println("Failed to send motion data: " + fbdo.errorReason());
        }

        // Get control values from Firebase
        bool autoMode = false, lightState = false, fanState = false;

        if (Firebase.getBool(fbdo, "/classrooms/2021/IT/classroom101/devices/autoMode")) {
            autoMode = fbdo.boolData();
            Serial.print("Auto mode: ");
            Serial.println(autoMode);
        } else {
            Serial.println("Failed to get autoMode: " + fbdo.errorReason());
        }

        if (Firebase.getBool(fbdo, "/classrooms/2021/IT/classroom101/devices/light")) {
            lightState = fbdo.boolData();
            Serial.print("lightVal: ");
            Serial.println(lightState);
        } else {
            Serial.println("Failed to get light state: " + fbdo.errorReason());
        }

        if (Firebase.getBool(fbdo, "/classrooms/2021/IT/classroom101/devices/fan")) {
            fanState = fbdo.boolData();
            Serial.print("fanVal: ");
            Serial.println(fanState);
        } else {
            Serial.println("Failed to get fan state: " + fbdo.errorReason());
        }

        // Control devices
      if (!autoMode) {
          if (lightState) {
          analogWrite(LIGHT_RELAY, 1023);  // Ensure MAX brightness
          Serial.println("✅ LED Turned ON");
        } else {
          analogWrite(LIGHT_RELAY, 0);     // Turn OFF LED
          Serial.println("❌ LED Turned OFF");    }
      }else {
          if (pirState == HIGH) {
            //Serial.print("LDR Value: ");
            //Serial.println(ldrValue);
            int brightness = map(ldrValue, 0, 1023, 0, 1023); 
            int fanSpeed = (temperature >= 30.0) ? 1023 : (temperature >= 25.0) ? 512 : (temperature >= 20.0) ? 256 : 0;
            //Serial.print("Calculated Brightness: ");
            //Serial.println(brightness);
            
            analogWrite(LIGHT_RELAY, brightness);
            analogWrite(FAN_RELAY, fanSpeed);
          } else {
            analogWrite(LIGHT_RELAY, 0);
            analogWrite(FAN_RELAY, 0);
          }
    }
      if (!autoMode) {
        if (fanState) {
          analogWrite(FAN_RELAY, 1023);  // Ensure MAX speed
          Serial.println("✅ FAN Turned ON");
        } else {
        analogWrite(FAN_RELAY, 0);     // Turn OFF FAN
        Serial.println("❌ FAN Turned OFF");    }
    }

     } else {
        Serial.println("Firebase not ready...");
        }

    delay(3000);
}
