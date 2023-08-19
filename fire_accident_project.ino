#include <SoftwareSerial.h>
const int flameSensorPin = A0;
const int irSensorPin = 2;
const int relayPin = 3;         
const int warningLedPin = 13;
const int buzzerPin = 9; 
const int gsmTxPin = 7;  
const int gsmRxPin = 8;   

SoftwareSerial gsmSerial(gsmTxPin, gsmRxPin);      

// Threshold values
const int flameThreshold = 500;  
const int irThreshold = 800;     

// Variable to track system state
bool isFlameDetected = false;

void setup() {
  pinMode(flameSensorPin, INPUT);
  pinMode(irSensorPin, INPUT);
  pinMode(relayPin, OUTPUT);
  pinMode(warningLedPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  gsmSerial.begin(9600);
  Serial.begin(9600);
}

void loop() {
  int flameValue = analogRead(flameSensorPin);
  int irValue = digitalRead(irSensorPin);

  Serial.print("Flame Value: ");
  Serial.println(flameValue);
  
  Serial.print("IR Value: ");
  Serial.println(irValue);

  if (flameValue > flameThreshold && irValue < irThreshold) {
    if (!isFlameDetected) {
      isFlameDetected = true;
      digitalWrite(warningLedPin, HIGH);  // Turn on warning LED
      digitalWrite(relayPin, HIGH);      // Turn on motor (assuming relay is active LOW)
      digitalWrite(buzzerPin, HIGH);     // Turn on buzzer
      delay(2000);  // Spray water for 2 seconds (adjust as needed)
      digitalWrite(relayPin, LOW);       // Turn off motor
      digitalWrite(buzzerPin, LOW);      // Turn off buzzer
      sendAlertMessage();                // Function to send alert message
      delay(5000);  // Delay to avoid continuous detection during the flame event
    }
  } else {
    isFlameDetected = false;
    digitalWrite(warningLedPin, LOW);   
  }

  delay(1000);  
}

void sendAlertMessage() {
  gsmSerial.println("AT"); 
  delay(100);
  if (gsmSerial.find("OK")) {
    gsmSerial.println("Accident occured");  
    delay(100);
    gsmSerial.print("AT+CMGS=\"+1234567890\"");  // Replace with recipient's phone number
    delay(100);
    gsmSerial.write((byte)26);  // Ctrl+Z to end the SMS
    delay(100);
}
}
