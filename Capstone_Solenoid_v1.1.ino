#include <SPI.h>
#include <MFRC522.h>
#include <WiFi.h>
#include <HTTPClient.h>

#define SS_PIN 5
#define RST_PIN 22
#define RELAY_PIN 4

#define LED_LOCK_RED_PIN 16
#define LED_UNLOCK_GREEN_PIN 17
#define BUZZER_PIN 21  // Change buzzer pin to 21

MFRC522 mfrc522(SS_PIN, RST_PIN);

//const char* ssid = "Ely iPhone";
//const char* password = "romeogenelaso30200000";
//const char* serverName = "http://192.168.0.10/log_rental.php"; // Replace with your XAMPP server IP

void setup() {
  Serial.begin(115200);
  SPI.begin();
  mfrc522.PCD_Init();

  pinMode(RELAY_PIN, OUTPUT);
  pinMode(LED_LOCK_RED_PIN, OUTPUT);
  pinMode(LED_UNLOCK_GREEN_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT); // Initialize the buzzer pin as output
  
  // Ensure solenoid is locked, LED is red, and buzzer is off on startup
  digitalWrite(RELAY_PIN, LOW);
  digitalWrite(LED_LOCK_RED_PIN, HIGH);
  digitalWrite(LED_UNLOCK_GREEN_PIN, LOW);
  digitalWrite(BUZZER_PIN, LOW);

 /* WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");*/
}

void loop() {
  String rfidTag = readRFIDTag(); // Read the RFID tag

  if (rfidTag != "") { // Check if a tag was read
    Serial.println("RFID Tag: " + rfidTag);
    if (rfidTag == "BCB0D9D9") {
      toggleSolenoid();
      //logToServer(rfidTag);
    }
  }
}

String readRFIDTag() {
  if (!mfrc522.PICC_IsNewCardPresent()) {
    return ""; // No new card present
  }
  if (!mfrc522.PICC_ReadCardSerial()) {
    return ""; // Could not read the card
  }

  String rfidTag = "";
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    rfidTag += String(mfrc522.uid.uidByte[i], HEX);
  }
  rfidTag.toUpperCase(); // Convert to uppercase
  mfrc522.PICC_HaltA();  // Halt PICC

  return rfidTag; // Return the tag
}

void toggleSolenoid() {
  static bool isLocked = true;
  if (isLocked) {
    digitalWrite(RELAY_PIN, HIGH); // Unlock solenoid
    digitalWrite(LED_LOCK_RED_PIN, LOW); // Turn off red LED
    digitalWrite(LED_UNLOCK_GREEN_PIN, HIGH); // Turn on green LED
    playUnlockedSound(); // Play the unlocked sound pattern
    isLocked = false;
    Serial.println("Bike unlocked");
  } else {
    digitalWrite(RELAY_PIN, LOW); // Lock solenoid
    digitalWrite(LED_UNLOCK_GREEN_PIN, LOW); // Turn off green LED
    digitalWrite(LED_LOCK_RED_PIN, HIGH); // Turn on red LED
    playLockedSound(); // Play the locked sound pattern
    isLocked = true;
    Serial.println("Bike locked");
  }
}

void playUnlockedSound() {
  // Single long beep for unlocking
  digitalWrite(BUZZER_PIN, HIGH);
  delay(500); // Long beep duration
  digitalWrite(BUZZER_PIN, LOW);
}

void playLockedSound() {
  // Two short beeps for locking
  for (int i = 0; i < 2; i++) {
    digitalWrite(BUZZER_PIN, HIGH);
    delay(100); // Short beep duration
    digitalWrite(BUZZER_PIN, LOW);
    delay(100); // Short pause between beeps
  }
}

/*void logToServer(String rfidTag) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String serverPath = "http://127.0.0.1/log_rental.php?tag=" + rfidTag;

    http.begin(serverPath.c_str());
    int httpResponseCode = http.GET();

    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println(response);
    } else {
      Serial.println("Error on HTTP request");
    }
    http.end();
  }
}*/
