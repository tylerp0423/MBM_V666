# MBMV666 - Security & Stealth Operations Guide

**Ninja Mode | Anti-Forensics | OPSEC Hardening**

---

## ⚠️ Legal Disclaimer

These features are designed for:
- **Authorized penetration testing** (with written contracts)
- **Privacy protection** (journalists, activists in hostile regions)
- **Secure operations** (preventing data leaks, tampering)

**NEVER use for illegal activities.** Unauthorized access, hacking, or surveillance is a crime.

---

## 🥷 Stealth Mode Features

### 1. Hardware Invisibility

**WiFi/BLE Kill Switch**
```cpp
// Firmware implementation
#define WIFI_KILL_PIN 27  // Physical switch or button

void setup() {
  pinMode(WIFI_KILL_PIN, INPUT_PULLUP);
  
  if (digitalRead(WIFI_KILL_PIN) == LOW) {
    WiFi.mode(WIFI_OFF);
    btStop();  // Disable Bluetooth
  }
}
```

**Benefits:**
- Impossible to detect via WiFi/BLE scans
- RF emissions reduced to zero
- Cannot be traced via MAC address

---

**LED Blackout Mode**
```cpp
// Disable all status LEDs
#define LED_PIN 48
pinMode(LED_PIN, OUTPUT);
digitalWrite(LED_PIN, LOW);  // Permanently off

// OLED display can also be disabled
display.ssd1306_command(SSD1306_DISPLAYOFF);
```

**Benefits:**
- No visual indicators of activity
- Blend into dark environments
- Battery savings (minor)

---

**MAC Address Randomization**
```cpp
#include <esp_wifi.h>

void randomizeMAC() {
  uint8_t mac[6];
  esp_read_mac(mac, ESP_MAC_WIFI_STA);
  
  // Randomize last 3 bytes (keep OUI for plausibility)
  mac[3] = random(0, 255);
  mac[4] = random(0, 255);
  mac[5] = random(0, 255);
  
  esp_wifi_set_mac(WIFI_IF_STA, mac);
}

void setup() {
  randomizeMAC();  // New identity every boot
}
```

**Benefits:**
- Prevents MAC tracking across sessions
- Bypass MAC whitelist/blacklist (sometimes)
- Appear as different device

---

### 2. Data Protection

**SD Card Encryption (Linux LUKS Compatible)**

Encrypt SD card on Linux PC before use:
```bash
# One-time setup
sudo cryptsetup luksFormat /dev/sdX
sudo cryptsetup open /dev/sdX mbmv666_encrypted
sudo mkfs.ext4 /dev/mapper/mbmv666_encrypted

# Daily use
sudo cryptsetup open /dev/sdX mbmv666_encrypted
sudo mount /dev/mapper/mbmv666_encrypted /mnt/mbmv666
# ... copy files ...
sudo umount /mnt/mbmv666
sudo cryptsetup close mbmv666_encrypted
```

**ESP32 cannot decrypt LUKS natively**, but you can:
- Store encrypted data blobs on SD
- Decrypt via Python script on Android/PC
- Use AES-encrypted files (ESP32 can handle this)

---

**AES-256 File Encryption (ESP32 Native)**
```cpp
#include <mbedtls/aes.h>

void encryptFile(const char* input, const char* output, const uint8_t* key) {
  mbedtls_aes_context aes;
  mbedtls_aes_init(&aes);
  mbedtls_aes_setkey_enc(&aes, key, 256);
  
  // Read, encrypt, write in 16-byte blocks
  uint8_t buffer[16];
  uint8_t encrypted[16];
  
  File inFile = SD.open(input, FILE_READ);
  File outFile = SD.open(output, FILE_WRITE);
  
  while (inFile.available()) {
    inFile.read(buffer, 16);
    mbedtls_aes_crypt_ecb(&aes, MBEDTLS_AES_ENCRYPT, buffer, encrypted);
    outFile.write(encrypted, 16);
  }
  
  inFile.close();
  outFile.close();
  mbedtls_aes_free(&aes);
}
```

---

**Secure Erase on Tamper Detection**
```cpp
#include <M5StickCPlus.h>

void setup() {
  M5.IMU.Init();
  
  // Monitor accelerometer for sudden movement
  float accX, accY, accZ;
  M5.IMU.getAccelData(&accX, &accY, &accZ);
  
  // If device is thrown/dropped (> 3G force)
  if (abs(accX) > 3.0 || abs(accY) > 3.0 || abs(accZ) > 3.0) {
    emergencyWipe();
  }
}

void emergencyWipe() {
  SD.remove("/sensitive_data.bin");
  SD.remove("/logs/operations.txt");
  // Overwrite free space with random data
  File wipe = SD.open("/wipe.tmp", FILE_WRITE);
  for (int i = 0; i < 1000000; i++) {
    wipe.write(random(0, 255));
  }
  wipe.close();
  ESP.restart();
}
```

---

### 3. Communication Security

**I2C Bus Encryption (AES-128)**
```cpp
// Master sends encrypted commands
void sendEncryptedCommand(uint8_t addr, uint8_t cmd, uint8_t* data, size_t len) {
  uint8_t encrypted[16];
  mbedtls_aes_crypt_ecb(&aes_ctx, MBEDTLS_AES_ENCRYPT, data, encrypted);
  
  Wire.beginTransmission(addr);
  Wire.write(cmd);
  Wire.write(encrypted, 16);
  Wire.endTransmission();
}

// Slave decrypts received data
void i2cReceiveHandler(int bytes) {
  uint8_t cmd = Wire.read();
  uint8_t encrypted[16];
  Wire.readBytes(encrypted, 16);
  
  uint8_t decrypted[16];
  mbedtls_aes_crypt_ecb(&aes_ctx, MBEDTLS_AES_DECRYPT, encrypted, decrypted);
  
  processCommand(cmd, decrypted);
}
```

**Benefits:**
- Prevents I2C bus sniffing
- Logic analyzer cannot read commands
- Secure multi-board communication

---

**ESP-NOW Encrypted Mesh**

ESP-NOW has built-in encryption (WPA2-level):
```cpp
#include <esp_now.h>

void setup() {
  esp_now_init();
  
  // Set encryption key (16 bytes)
  uint8_t key[16] = {0x01, 0x02, 0x03, ... };
  esp_now_set_pmk(key);
  
  // Pair with encrypted peer
  esp_now_peer_info_t peer;
  memcpy(peer.peer_addr, peerMAC, 6);
  peer.encrypt = true;  // Enable encryption
  memcpy(peer.lmk, key, 16);
  esp_now_add_peer(&peer);
}
```

---

### 4. Anti-Forensics

**Zero Logging Mode**
```cpp
#define ZERO_LOG_MODE true  // Set in config

void logEvent(const char* message) {
  if (ZERO_LOG_MODE) {
    return;  // Discard all log writes
  } else {
    // Normal logging to SD card
    File log = SD.open("/logs/events.txt", FILE_APPEND);
    log.println(message);
    log.close();
  }
}
```

**Benefits:**
- No evidence of operations
- RAM-only operation (volatile, erased on power loss)
- Cannot reconstruct activity timeline

---

**Metadata Scrubbing**
```cpp
void removeEXIF(const char* imagePath) {
  // Strip EXIF metadata from photos
  // Prevents GPS location, camera model, timestamp leaks
  
  File img = SD.open(imagePath, FILE_READ);
  // ... read JPEG, skip EXIF segments, write clean file ...
  img.close();
}
```

---

**Secure File Deletion (DoD 5220.22-M)**
```cpp
void secureDelete(const char* path) {
  File file = SD.open(path, FILE_WRITE);
  size_t fileSize = file.size();
  
  // Pass 1: Overwrite with 0x00
  for (size_t i = 0; i < fileSize; i++) file.write(0x00);
  file.seek(0);
  
  // Pass 2: Overwrite with 0xFF
  for (size_t i = 0; i < fileSize; i++) file.write(0xFF);
  file.seek(0);
  
  // Pass 3: Overwrite with random data
  for (size_t i = 0; i < fileSize; i++) file.write(random(0, 255));
  
  file.close();
  SD.remove(path);  // Finally delete file entry
}
```

---

### 5. Operational Security (OPSEC)

**USB Serial Lockout**
```cpp
void setup() {
  #ifndef DEBUG_MODE
    // Disable USB serial after first boot
    Serial.end();
    USB.end();
  #endif
}
```

**Benefits:**
- No debug access via USB
- Prevents forensic extraction
- Only accessible via I2C or WiFi (if enabled)

---

**Firmware Upload Protection**
```cpp
#define UPLOAD_BUTTON_PIN 0  // Boot button

void setup() {
  pinMode(UPLOAD_BUTTON_PIN, INPUT_PULLUP);
  
  // Only allow firmware upload if button held during boot
  if (digitalRead(UPLOAD_BUTTON_PIN) == HIGH) {
    // Normal boot
    startFirmware();
  } else {
    // Upload mode (user intentionally held button)
    // Allow Arduino IDE to flash
  }
}
```

---

**Flash Write Protection**
```cpp
#include <esp_flash_encrypt.h>

void setup() {
  // Enable flash encryption (CANNOT BE REVERSED)
  esp_flash_encryption_enable();
  
  // Set write-protect on critical partitions
  esp_flash_write_protect_set(true);
}
```

⚠️ **WARNING:** Once enabled, flash encryption is **permanent**. Bricked board if you lose encryption key.

---

### 6. Emergency Protocols

**Dead Man's Switch**
```cpp
#define CHECKIN_INTERVAL_MS (24 * 60 * 60 * 1000)  // 24 hours
unsigned long lastCheckin = 0;

void loop() {
  if (millis() - lastCheckin > CHECKIN_INTERVAL_MS) {
    emergencyWipe();  // No checkin = assume compromised
  }
  
  // Checkin via button press or I2C command
  if (buttonPressed() || i2cCheckinReceived()) {
    lastCheckin = millis();
  }
}
```

---

**Remote Wipe Command**
```cpp
void i2cReceiveHandler(int bytes) {
  uint8_t cmd = Wire.read();
  
  if (cmd == 0xFF) {  // Emergency wipe code
    emergencyWipe();
    ESP.restart();
  }
}
```

**Usage:**
```bash
# From master board terminal
i2c_send 0x10 0xFF  # Wipe display board
i2c_send 0x11 0xFF  # Wipe mobile board
i2c_send 0x12 0xFF  # Wipe GPIO board
```

---

## 🛡️ Recommended Security Configuration

### Tier 1: Basic Privacy
- MAC randomization enabled
- WiFi/BLE off by default
- LED blackout mode
- Delete logs after 7 days

### Tier 2: Standard OPSEC
- Tier 1 features +
- AES file encryption for sensitive data
- USB serial disabled after setup
- Firmware upload protection (button required)

### Tier 3: High Security
- Tier 2 features +
- I2C bus encryption
- Zero logging mode (RAM only)
- Secure file deletion (7-pass wipe)
- Tamper detection (accelerometer)

### Tier 4: Maximum Paranoia
- Tier 3 features +
- Flash encryption (irreversible)
- Dead man's switch (24hr checkin)
- Remote wipe capability
- JTAG debug disabled (fuse burning)

---

## 🔍 Threat Modeling

### What We're Protecting Against:

**Low-Skill Attackers:**
- Casual snooping (WiFi scan reveals device)
- Physical theft (data readable on SD card)
- Mitigation: MAC randomization, file encryption

**Medium-Skill Attackers:**
- I2C bus sniffing (logic analyzer)
- Firmware extraction (flash dump via JTAG)
- Mitigation: I2C encryption, flash write-protect

**High-Skill Attackers:**
- Chip-level forensics (decapping, electron microscope)
- Side-channel attacks (power analysis, EM radiation)
- Mitigation: **Hardware limitations** — cannot defend against this with ESP32 alone

---

## 📋 Security Checklist

Before deploying for sensitive operations:

- [ ] WiFi disabled (or kill switch installed)
- [ ] Bluetooth disabled
- [ ] MAC address randomizes on boot
- [ ] LED blackout mode active
- [ ] Sensitive files encrypted (AES-256)
- [ ] Log retention policy set (auto-delete old logs)
- [ ] USB serial disabled
- [ ] Firmware upload requires button hold
- [ ] Emergency wipe command tested
- [ ] Dead man's switch configured (if needed)
- [ ] Physical tamper detection enabled (if needed)
- [ ] I2C encryption enabled (if multi-board)
- [ ] Flash write protection enabled (optional)

---

## ⚖️ Legal & Ethical Use

**ALWAYS:**
- Obtain written authorization before pentesting
- Inform clients about data collection/storage
- Comply with local laws (CFAA, DMCA, GDPR, etc.)
- Use security features to **protect**, not **harm**

**NEVER:**
- Access systems without permission
- Deploy on networks you don't own
- Use for surveillance/stalking
- Sell services for illegal purposes

**Gray Areas:**
- Personal privacy tools (legal in most places)
- Security research (cite responsible disclosure)
- Journalist/activist protection (consult legal counsel)

---

**Next:** [AI Agent Configuration](ai_agents/README.md)
