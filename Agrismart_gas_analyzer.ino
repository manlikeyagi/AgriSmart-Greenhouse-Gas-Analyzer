#define BLYNK_TEMPLATE_ID "TMPL2mekn8ZbZ"
#define BLYNK_TEMPLATE_NAME "AgriSmart Gas Analyser"
#define BLYNK_AUTH_TOKEN "gL9jN3hre_hHPCrXGu9a2KM4J_ZzxfID"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

#define BLYNK_PRINT Serial // Comment this out to disable prints and save space

char auth[] = "gL9jN3hre_hHPCrXGu9a2KM4J_ZzxfID";

/* WiFi credentials */
char ssid[] = "Tope";
char pass[] = "buydata000";

BlynkTimer timer;
BlynkTimer blinkTimer;

int mq135 = A0; // smoke sensor is connected with the analog pin A0 
int data = 0;
int blinkTimerId = -1; // Store the ID of the blinking timer

void setup() 
{
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);
  timer.setInterval(1000L, getSendData); // new data will be updated every 1 sec 
}

void loop() 
{
  timer.run(); // Initiates BlynkTimer
  Blynk.run();
  blinkTimer.run(); // Initiates blink timer
}

/***************************************************
 * Toggle LED state
 **************************************************/
void toggleLED()
{
  static bool ledState = false;
  ledState = !ledState;
  Blynk.virtualWrite(V2, ledState ? 255 : 0); // Toggle LED state
}

/***************************************************
 * Send Sensor data to Blynk and control LED
 **************************************************/
void getSendData()
{
  data = analogRead(mq135); 
  Blynk.virtualWrite(V1, data);

  if (data > 500)
  {
    Blynk.logEvent("smoke_detected", "Smoke Detected"); 
    if (blinkTimerId == -1) {
      blinkTimerId = blinkTimer.setInterval(500L, toggleLED); // Blink LED every 500 ms
    }
  }
  else
  {
    if (blinkTimerId != -1) {
      blinkTimer.deleteTimer(blinkTimerId); // Stop blinking
      blinkTimerId = -1;
      Blynk.virtualWrite(V2, 0); // Turn off LED
    }
  }
}
