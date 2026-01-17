# MBMV666 - Complete Hardware Documentation

---

## 📋 Parts List (Bill of Materials)

### Core Boards ($53-70)
- **ESP32-S3-N16R8 Dev Module** — $8-12 (AliExpress, Amazon)
- **ESP32 CYD 2432S028** — $15-20 (Cheap Yellow Display)
- **M5StickC Plus 1.1** — $25-30 (Official M5Stack)
- **ESP32-WROOM-32 DevKitV1** — $5-8 (Generic)

### Add-On Modules ($40-80)
- **PN532 RFID/NFC Module** — $5-8 (I2C version)
- **CC1101 RF Transceiver** — $2-4 (433MHz recommended)
- **NEO-6M GPS Module** — $8-12 (with ceramic antenna)
- **ELM327 OBD2 Bluetooth** — $10-20 (v1.5 or v2.1)
- **Dual Relay Module (HL-52S)** — $2-4 (10A rating)
- **CH341A USB Programmer** — $3-5 (black edition)
- **0.96" OLED Display (I2C)** — $3-5 (SSD1306)
- **Wii Nunchuck Controller** — $5-10 (official or clone)

### Accessories ($15-30)
- **2x SOIC8 Test Clips** — $2-4 each
- **8dBi WiFi Antenna (RP-SMA)** — $8-12 (dual-band 2.4/5.8GHz)
- **2x U.FL to RP-SMA Pigtail** — $2-3 (15cm length)
- **2x MicroSD Cards** — $8-20 (64GB or 128GB)
- **4.7kΩ Resistors (2x)** — $0.50 (I2C pull-ups)
- **Jumper Wire Kit** — $3-5 (male-male, male-female)

### Power System ($15-25)
- **4-Port USB Hub** — $10-15 (powered, 5V/3A minimum)
- **4x USB Cables** — $5-10 (mixed Micro-USB, USB-C)

### Optional Upgrades ($50-200)
- **10,000mAh Power Bank** — $20-30 (portable operation)
- **3D Printed Enclosure** — $15-50 (custom design)
- **FLIR Lepton Thermal Camera** — $150 (component diagnostics)
- **Logic Analyzer (8-channel)** — $80 (protocol analysis)

**Total System Cost:**
- **Minimum (4 boards only):** $70
- **Recommended (all add-ons):** $180
- **Complete (with upgrades):** $250-400

---

## 🔌 Complete Wiring Guide

### I2C Bus (All 4 Boards)

```
ESP32-S3 Master:
  GPIO 8  (SDA) ────────┬──── CYD GPIO 21 (SDA)
                        ├──── M5StickC GPIO 32 (SDA)
                        └──── ESP32 GPIO 21 (SDA)

  GPIO 9  (SCL) ────────┬──── CYD GPIO 22 (SCL)
                        ├──── M5StickC GPIO 33 (SCL)
                        └──── ESP32 GPIO 22 (SCL)

  GND ──────────────────┬──── CYD GND
                        ├──── M5StickC GND
                        └──── ESP32 GND

  3.3V ─────┬──── 4.7kΩ ──── SDA (pull-up)
            └──── 4.7kΩ ──── SCL (pull-up)
```

**Total Wires Needed:** 8 wires (2 SDA, 2 SCL, 4 GND)

---

### Board 1: ESP32-S3-N16R8 Pinout

```
PN532 RFID (I2C):
  VCC ──── 3.3V
  GND ──── GND
  SDA ──── GPIO 21
  SCL ──── GPIO 22

CC1101 RF (SPI):
  VCC ──── 3.3V
  GND ──── GND
  MOSI ─── GPIO 11
  MISO ─── GPIO 12
  SCK ──── GPIO 13
  CS ───── GPIO 10
  GDO0 ─── GPIO 14

ELM327 OBD2 (UART):
  TX ───── GPIO 44 (S3 RX)
  RX ───── GPIO 43 (S3 TX)
  GND ──── GND

8dBi Antenna:
  U.FL Connector on S3 board
  (Requires resistor modification — see below)
```

---

### Board 2: ESP32 CYD Pinout

```
Built-in Components:
  TFT Display: Built-in (SPI)
  Touchscreen: Built-in (SPI, CS=33, IRQ=36)
  SD Card: Built-in (CS=5)

0.96" OLED (I2C):
  VCC ──── 3.3V
  GND ──── GND
  SDA ──── GPIO 21 (shared with I2C bus)
  SCL ──── GPIO 22 (shared with I2C bus)

Wii Nunchuck (I2C):
  VCC ──── 3.3V
  GND ──── GND
  SDA ──── GPIO 21 (shared with I2C bus)
  SCL ──── GPIO 22 (shared with I2C bus)
```

**Note:** OLED (0x3C) and Nunchuck (0x52) have different I2C addresses, no conflict.

---

### Board 3: M5StickC Plus Pinout

```
Built-in Components:
  Display: Built-in (SPI)
  IMU (MPU6886): Built-in (I2C internal)
  Battery: 200mAh (built-in)
  Buttons: GPIO 37 (A), GPIO 39 (B)

NEO-6M GPS (UART):
  VCC ──── 5V (from USB or external)
  GND ──── GND
  TX ───── GPIO 36 (M5 RX)
  RX ───── GPIO 0 (M5 TX)
```

---

### Board 4: ESP32 DevKitV1 Pinout

```
Dual SD Cards (SPI):
  SD1:
    CS ───── GPIO 5
    MOSI ─── GPIO 23 (shared)
    MISO ─── GPIO 19 (shared)
    SCK ──── GPIO 18 (shared)
    VCC ──── 3.3V
    GND ──── GND

  SD2:
    CS ───── GPIO 15
    MOSI ─── GPIO 23 (shared)
    MISO ─── GPIO 19 (shared)
    SCK ──── GPIO 18 (shared)
    VCC ──── 3.3V
    GND ──── GND

Dual Relay Module:
  VCC ──── 5V (from USB)
  GND ──── GND
  IN1 ──── GPIO 25
  IN2 ──── GPIO 32

CH341A Programmer (Direct SPI):
  VCC ──── 3.3V
  GND ──── GND
  MOSI ─── GPIO 13
  MISO ─── GPIO 12
  SCK ──── GPIO 14
  CS ───── GPIO 4
  EN ───── GPIO 27 (power control)
```

---

## 🔧 ESP32-S3 Antenna Modification Guide

### Problem
ESP32-S3 boards ship with internal PCB antenna active by default. External U.FL connector is **not connected** until you move a 0Ω resistor.

### Solution (Requires Soldering)

**Tools Needed:**
- Soldering iron (fine tip)
- Tweezers
- Magnifying glass (or phone camera zoom)
- Flux (optional but helpful)

**Steps:**

1. **Locate the antenna switch components:**
   - Flip S3 board over (component side up)
   - Look near the U.FL connector
   - Find two 0Ω resistors labeled `R3` and `R4`
   - Or solder pads labeled `PCB ANT` and `EXT ANT`

2. **Current state (factory default):**
   - Resistor installed at `R3` (PCB antenna position)
   - `R4` position is empty (external antenna disabled)

3. **Modification:**
   - Heat `R3` and gently remove the resistor with tweezers
   - Move resistor to `R4` position
   - Solder in place

4. **Alternative (if no resistors visible):**
   - Some boards use solder jumpers instead
   - Bridge `EXT ANT` pads with solder blob
   - Cut/scrape away `PCB ANT` connection

5. **Test:**
   - Connect 8dBi antenna via U.FL pigtail
   - Flash WiFi scan sketch
   - Compare signal strength (should see +10-15 dBm improvement)

**Visual Guide:** [See hardware/antenna_mod/photos/](hardware/antenna_mod/)

---

## 📐 Physical Dimensions

### Individual Boards
- **ESP32-S3:** 25mm × 50mm
- **CYD:** 60mm × 85mm (with case)
- **M5StickC:** 18mm × 48mm × 95mm (with battery)
- **ESP32 DevKit:** 28mm × 52mm

### Recommended Enclosure Size
- **Compact Stack:** 100mm × 150mm × 50mm
- **Spread Layout:** 200mm × 150mm × 30mm
- **Portable (M5 only):** 20mm × 50mm × 100mm

---

## 🔋 Power Requirements

### Individual Board Consumption
- **S3 Master:** 80-240mA (idle-active, WiFi on)
- **CYD Display:** 150-300mA (backlight dependent)
- **M5StickC:** 50-150mA (has battery, 2-3 hours)
- **ESP32 GPIO:** 80-200mA (relay dependent)

**Total System Peak:** ~1.2A @ 5V (6W)

### Power Supply Options
1. **USB Hub (Recommended):** 4-port powered hub, 5V/3A minimum
2. **Individual USB Chargers:** 4x phone chargers (messy cables)
3. **Bench Power Supply:** 5V/3A with barrel jack breakouts
4. **Battery Pack:** 10,000mAh power bank (5-8 hours runtime)

---

## 🧪 Testing & Validation

### Initial Power-On Test (Per Board)
1. Connect via USB (no wiring yet)
2. Flash test sketch (blink LED or serial "Hello")
3. Verify serial output @ 115200 baud
4. Check power LED (should be solid)

### I2C Communication Test
1. Wire all 4 boards together (I2C + GND only)
2. Flash Master with I2C scanner sketch
3. Should detect: 0x10 (CYD), 0x11 (M5), 0x12 (GPIO)
4. If not detected: check SDA/SCL wires, pull-up resistors

### Add-On Module Test
1. Connect one module at a time
2. Flash corresponding test sketch
3. Verify functionality before adding next module
4. Document any pin conflicts

**Full Test Checklist:** [tools/test_scripts/hardware_validation.md](tools/test_scripts/hardware_validation.md)

---

## 🛡️ Safety Warnings

### Electrical Safety
- **Never connect 5V to 3.3V pins** — Instant chip death
- **Check polarity on all connections** — Red=VCC, Black=GND
- **Relays handle 250VAC** — Danger of shock, use caution
- **CH341A flash voltage** — Some chips are 1.8V, not 3.3V

### Thermal Safety
- **S3 can get hot** — 70-80°C under load (WiFi + PSRAM)
- **Add heatsink** — Small aluminum heatsink recommended
- **Ventilation** — Don't seal enclosure completely

### Legal Safety
- **FCC Compliance** — External antenna may exceed regulations
- **Warranty Void** — Resistor modification voids S3 warranty
- **Professional Use** — Some services require licenses/certification

---

## 📦 Recommended Suppliers

### North America
- **Amazon** — Fast shipping, easy returns
- **DigiKey/Mouser** — Genuine components, higher prices
- **Adafruit** — Quality modules, tutorials included

### International
- **AliExpress** — Cheapest, 2-4 week shipping
- **Banggood** — Good selection, faster than AliExpress
- **LCSC** — JLCPCB partner, bulk discounts

### Specialty
- **M5Stack Official Store** — M5StickC Plus guaranteed genuine
- **Espressif Direct** — ESP32 dev boards from manufacturer
- **eBay** — Used/refurb parts, hit-or-miss quality

---

## 🔄 Upgrade Path

### Phase 1 (Current — $180)
4 boards + basic add-ons

### Phase 2 (+$50)
Add thermal camera, logic analyzer

### Phase 3 (+$100)
Add 4G module, LoRa, battery pack

### Phase 4 (+$200)
Custom PCB integration, professional enclosure

---

**Next:** [Firmware Installation Guide](firmware/README.md)
