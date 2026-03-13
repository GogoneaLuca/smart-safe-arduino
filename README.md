# Smart Safe Security System 🛡️⚙️

An embedded C++ project built on the Arduino platform, demonstrating hardware-software integration for a dual-factor physical security system. 

## 📌 Project Overview
This project simulates a secure vault mechanism. It requires either an authorized RFID token or a correct PIN code to actuate a servo-motor lock. It demonstrates low-level component initialization, continuous event loop polling, and memory-safe string handling in C++.

## 🛠️ Tech Stack & Hardware
* **Language:** C++ (Arduino Framework)
* **Hardware Components:**
  * Arduino Uno R3
  * MFRC522 RFID Reader (SPI Communication)
  * 4x4 Matrix Membrane Keypad
  * SG90 Micro Servo Motor
* **Libraries:** `SPI.h`, `MFRC522.h`, `Keypad.h`, `Servo.h`

## 🧠 System Architecture
1. **Event Loop:** The `loop()` function continuously polls for two asynchronous events: an SPI interrupt from the RFID reader or a matrix scan from the keypad.
2. **Authentication:** * **RFID:** Reads the UID bytes, converts them to a hexadecimal string, and compares them against the authorized matrix.
   * **PIN:** Buffers character inputs until the `#` (Enter) key is pressed, preventing buffer overflows before validation.
3. **Actuation:** Valid states trigger PWM (Pulse Width Modulation) signals to the Servo, rotating the locking mechanism exactly 90 degrees.
