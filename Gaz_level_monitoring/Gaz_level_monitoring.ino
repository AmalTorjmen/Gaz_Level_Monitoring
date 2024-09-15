#include <WiFi.h>
#include <WiFiClient.h> // Used to send or receive data over the network once the ESP32 is connected to Wi-Fi.
#include <BlynkSimpleEsp32.h> // Integration of Blynk with ESP32.

#define BLYNK_TEMPLATE_ID "TMPL2KX2MY0zG"  // Uniquely identifies the template model on Blynk.
#define BLYNK_TEMPLATE_NAME "Gas Level Monitoring"  // Project name.
#define BLYNK_AUTH_TOKEN "Your_Blynk_Auth_Token"  // Unique key that allows your device to connect to Blynk.

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "Your_WiFi_SSID";
char pass[] = "Your_WiFi_Password";

BlynkTimer timer;  // 'timer' is the name of the object created from the BlynkTimer class.

#define Buzzer 25    // GPIO25
#define LedG 26     
#define LedR 27      
#define GasSensor 34    

int Gas_Val = 0;

void setup() {
  
  Serial.begin(115200);
  pinMode(LedG, OUTPUT);
  pinMode(LedR, OUTPUT);
  pinMode(Buzzer, OUTPUT);
  pinMode(GasSensor, INPUT);
  Blynk.begin(auth, ssid, pass); // Initialize ESP32's connection to the Blynk platform.
  delay(2000); // Ensure the connection is established before moving to the next step.
  timer.setInterval(500L, SensorData); // Indicates that the SensorData function should be called every 500 milliseconds.
 
}

void loop() 
{
  Blynk.run(); // Handles communication with the Blynk platform.
  timer.run(); // Manages periodic tasks defined with the timer, such as sensor readings.
}

void SensorData() {
  Gas_Val = analogRead(GasSensor);
  Gas_Val = map(Gas_Val, 0, 4095, 0, 100); // Converts values from one range to another.
  Serial.println(Gas_Val);
  Blynk.virtualWrite(V0, Gas_Val);  // Used to send data from ESP32 to the Blynk platform.
  
  if (Gas_Val < 50) {
    digitalWrite(LedG, HIGH);
    digitalWrite(LedR, LOW);
    digitalWrite(Buzzer, LOW);
  }
  else {
    Blynk.logEvent("gas_detected", "Gas detected");
    digitalWrite(LedG, LOW);
    digitalWrite(LedR, HIGH);
    digitalWrite(Buzzer, HIGH);
  }

}
