#ifndef MBMV666LIB_H
#define MBMV666LIB_H

#include <WiFi.h>
#include <esp_now.h>
#include <Wire.h>
#include <SPI.h>
#include <IRremote.h>
#include <SD.h>
#include <FS.h>
#include <SPIFFS.h>
#include <XP2046_Touchscreen.h>
#include <TFT_eSPI.h>
#include <ArduinoNunchuk.h>
#include <EloquentTinyML.h>
#include <eloquent_tinyml/tensorflow.h>

struct struct_message {
  int id;
  char payload[256];
};

const int NUMBER_OF_INPUTS = 4;
const int NUMBER_OF_OUTPUTS = 1;
const int TENSOR_ARENA_SIZE = 8*1024;

void setupESPNow();
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status);
void OnDataRecv(const uint8_t *mac_addr, const uint8_t *data, int data_len);
void setupAI();
void runAI();
void busI2CScan();
void busSPIDump();
void busUARTRead();
void busCANScan();
void busIRBlast();
void busSubGHzReplay();
void runBusPayload();
void m5sharkBadUSB();
void m5sharkRFIDClone();
void runPayload();
void runPayloadFromCommand();
void draw666Menu();
void nunchukTask();
void logToDrive();

extern TFT_eSPI tft;
extern XP2046_Touchscreen ts;
extern IRsend irsend;
extern ArduinoNunchuk nunchuk;
extern char offloadRoot[64];
extern bool isCYD, isM5, isS3, isWROOM;
extern int selected;
extern String items[26];

#endif
