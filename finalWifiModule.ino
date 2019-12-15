#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>

#define FIREBASE_HOST "cheogger.firebaseio.com" 
#define FIREBASE_AUTH "9tGxaWQgfggfx7u5jl0ntAlmVO82gcSaGqTayx1L" 
#define WIFI_SSID "Torviik" 
#define WIFI_PASSWORD "nCrp5kP2" 

String passedData = "";

void setup()
{
  Serial.begin(9600);
  
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD); 
  Serial.print("connecting"); 
  while (WiFi.status() != WL_CONNECTED) { 
    Serial.print("."); 
    delay(500); 
  } 
  Serial.println(); 
  Serial.print("connected: "); 
  Serial.println(WiFi.localIP()); 
   
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH); 
}

void loop() {
  passedData = Serial.readString();
  
  if (passedData.length() > 2) {
      Serial.println(passedData);
      String gameName = Firebase.pushString("games", passedData); 
      // HANDLING ERROR
      if (Firebase.failed()) { 
        Serial.print("pushing game failed: "); 
        Serial.println(Firebase.error());   
        return; 
      } 

      Serial.print("Game named ");
      Serial.println(gameName + " successfully created!");

      delay(3000);
  }
  
}
