#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>
#include <Keypad.h>

// --- PIN CONFIGURATION ---
#define RST_PIN   9
#define SS_PIN    10
#define SERVO_PIN 8

// --- HARDWARE INITIALIZATION ---
MFRC522 rfid(SS_PIN, RST_PIN);
Servo lockServo;

// --- KEYPAD SETUP ---
const byte ROWS = 4; 
const byte COLS = 4; 
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {7, 6, 5, 4}; 
byte colPins[COLS] = {3, 2, A0, A1}; 
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// --- SYSTEM CREDENTIALS ---
String validUID = "A1B2C3D4"; // Exemplu de UID Card
String validPIN = "1234";
String inputPIN = "";
bool isUnlocked = false;

void setup() {
    Serial.begin(9600);
    SPI.begin();
    rfid.PCD_Init();
    
    lockServo.attach(SERVO_PIN);
    lockServo.write(0); // System locked by default
    
    Serial.println("System Initialized. Awaiting Card or PIN...");
}

void loop() {
    // 1. Check for RFID Card
    if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) {
        String cardUID = getUID();
        if (cardUID == validUID) {
            Serial.println("Access Granted: Valid Card");
            unlockSafe();
        } else {
            Serial.println("Access Denied: Invalid Card");
        }
        rfid.PICC_HaltA();
    }

    // 2. Check for Keypad Input
    char key = keypad.getKey();
    if (key) {
        if (key == '#') { // Enter button
            if (inputPIN == validPIN) {
                Serial.println("Access Granted: Valid PIN");
                unlockSafe();
            } else {
                Serial.println("Access Denied: Invalid PIN");
            }
            inputPIN = ""; // Reset PIN buffer
        } else if (key == '*') { // Lock button
            lockSafe();
        } else {
            inputPIN += key;
        }
    }
}

// --- HELPER FUNCTIONS ---
String getUID() {
    String uid = "";
    for (byte i = 0; i < rfid.uid.size; i++) {
        uid += String(rfid.uid.uidByte[i] < 0x10 ? "0" : "");
        uid += String(rfid.uid.uidByte[i], HEX);
    }
    uid.toUpperCase();
    return uid;
}

void unlockSafe() {
    if (!isUnlocked) {
        lockServo.write(90); // Rotate servo to unlock
        isUnlocked = true;
        delay(3000); // Keep open for 3 seconds
    }
}

void lockSafe() {
    if (isUnlocked) {
        lockServo.write(0);
        isUnlocked = false;
        Serial.println("System Locked.");
    }
}
