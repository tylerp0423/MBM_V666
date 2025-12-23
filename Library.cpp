#include "MBMV666Lib.h"

void MBMV666Lib::setupESPNow() {
  WiFi.mode(WIFI_STA);
  esp_now_init();
  esp_now_register_send_cb(onDataSent);
  esp_now_register_recv_cb(onDataRecv);
  addPeer(slaveMac1);
  addPeer(slaveMac2);
}

void MBMV666Lib::onDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("Data sent, status: ");
  Serial.println(status);
}

void MBMV666Lib::onDataRecv(const uint8_t *mac, const uint8_t *data, int len) {
  memcpy(myData, data, len);
  runPayloadFromCommand();
}

void MBMV666Lib::setupAI() {
  tf.begin(model_tflite);
}

void MBMV666Lib::runAI() {
  tf.predict(test_input, test_output);
  Serial.print("AI prediction: ");
  Serial.println(test_output);
}

// Implement the rest of the functions similarly
