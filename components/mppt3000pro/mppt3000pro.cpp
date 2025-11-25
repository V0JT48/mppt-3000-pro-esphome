#include "esphome/core/log.h"
#include "mppt3000pro.h"
#include "MedianFilterLib.h"

namespace esphome {
namespace mppt3000pro {

const uint8_t DEFAULT_OUTPUT = 1;

bool new_data = false;

bool first_4bit = true;
char character = ' ';
bool prev_char_num = false;

uint16_t number = 0;
char power = 'Y';

uint16_t V;
uint16_t A;
uint16_t W;
uint16_t D;
uint16_t T;
uint8_t  O;
uint16_t no_data_count = 0;
bool on_state = false;

uint16_t tmp_V = 0;
uint16_t tmp_A = 0;
uint16_t tmp_W = 0;
uint16_t tmp_D = 0;
uint8_t zero_D_count = 0;
uint8_t boot_D_count = 0;
uint16_t tmp_T = 0;
uint8_t  tmp_O = DEFAULT_OUTPUT;

MedianFilter<uint16_t> med_V(3);
MedianFilter<uint16_t> med_A(3);
MedianFilter<uint16_t> med_W(3);
MedianFilter<uint16_t> med_D(3);
MedianFilter<uint16_t> med_T(3);

void send_data(void){
  V = med_V.AddValue(tmp_V);
  A = med_A.AddValue(tmp_A);
  W = med_W.AddValue(tmp_W);  
  if (tmp_D >= D ){ // additional missed character value filter
    D = med_D.AddValue(tmp_D);
    zero_D_count = 0;
  } else if (tmp_D == 0){
    zero_D_count++;
    if(zero_D_count > 5){
      D = med_D.AddValue(tmp_D);
    }
    ESP_LOGI("MPPT3000", "D zero counter: %d", zero_D_count);
  }
  if (tmp_T >= T ){ // additional missed character value filter
    T = med_T.AddValue(tmp_T);
  }
  O = tmp_O;
  ESP_LOGI("MPPT3000", "Received values: %dV %.1fA %dW D %.1fkWh T %dkWh Output %d", V, A/10.0, W, D/10.0, T, O);
}

void decode_character(void){
  if (character > '.' && character <= 'z' && character != 'O') { // printable char filter and skip decimal point for A and D, skip O from ON OFF
    if((character >= '0') & (character <= '9')) {
        number = number*10 + (character - '0');
        prev_char_num = true;
    }
    else if(prev_char_num){
      switch (character){
        case 'V':
          if (number > 400 || number < 40) break; // upper and lower limit
          tmp_V=number; 
          break;
        case 'A':
          if (number > 200) break; // upper limit
          tmp_A=number;
          break;
        case 'W':
          if (number > 4000) break; // upper limit
          tmp_W=number;
          break;
        case 'k':
          switch(power) {
            case 'D':
              if (number > 600) break; // upper limit
              tmp_D = number;
              if (boot_D_count > 1) {
                new_data = true; // mark data valid after D value is received to avoid mid day reset in case of node reboot
              } else {
                boot_D_count++;
              }
              
              break;
            case 'T':
              tmp_T = number;
              send_data();
              break;
            /*case 'Y': // unused value for yesterday energy
              tmp_Y = number;
              break; */
          }
          power = 'Y';
          break;
        case 'N':
          if (number == 1 || number == 2) tmp_O=number; // ON, skipped OFF, -
          break;
      }
      number = 0;
      prev_char_num = false;
    }
    else {
      switch (character){
        case 'D':
          power = 'D';
          break;
        case 'Y':
          power = 'Y';
          break;
        case 'T':
          power = 'T';
          break;
        case 'M':
          ESP_LOGI("MPPT3000", "Inverter Reboot");
          break;
      }
      number = 0;
      prev_char_num = false;
    }
  }
}

void I2C_RxHandler(int numBytes)
{
  uint8_t RxByte;
  while(Wire.available()) {  // Read Any Received Data
    RxByte = Wire.read();
    switch(RxByte & 0x0F){
      case 0x09: // Data write, low E pin
        if(first_4bit){ // first 4bit, high bits
          character = RxByte & 0xF0;
          first_4bit = false;
        } else {
          // second 4bit, low bits
          character = (RxByte >> 4) | character;
          decode_character();
          first_4bit = true;
        }
        break;
      case 0x0D: // Data write, high E pin, do nothing
        break;
      default: // Other commands, reset first_4bit flag
        first_4bit = true;
        break;
    }
    
    
  }
}

static const char *TAG = "mppt3000pro.sensor";

void MPPT3000PRO::setup() {
  if (sda_pin_ != scl_pin_) {
    Wire.setPins(sda_pin_, scl_pin_); //Default SDA (GPIO21) and SCL (GPIO22)
  }
  Wire.begin(address_);
  Wire.onReceive(I2C_RxHandler);
  if (this->on_pin_ != nullptr) {
    this->on_pin_->setup();
  }
  // Fill medians with initial zeroes
  med_V.AddValue(0);
  med_V.AddValue(0);
  med_A.AddValue(0);
  med_A.AddValue(0);
  med_W.AddValue(0);
  med_W.AddValue(0);
  med_D.AddValue(0);
  med_D.AddValue(0);
  med_T.AddValue(0);
  med_T.AddValue(0);
  ESP_LOGI("MPPT3000", "Setup done");
}

void MPPT3000PRO::loop() {
  
}

void MPPT3000PRO::update() {
  if (this->sensor_ON_ != nullptr && this->on_pin_ != nullptr) {
    on_state = this->on_pin_->digital_read();
    this->sensor_ON_->publish_state(on_state);
  }
  if(new_data){
    if (this->sensor_V_ != nullptr && V < 400)
      this->sensor_V_->publish_state(V);
    if (this->sensor_A_ != nullptr && A < 200)
      this->sensor_A_->publish_state(A/10.0);
    if (this->sensor_W_ != nullptr  && W < 4000)
      this->sensor_W_->publish_state(W);
    if (this->sensor_D_ != nullptr && D < 600)
      this->sensor_D_->publish_state(D/10.0);
    if (this->sensor_T_ != nullptr && T > 0)
      this->sensor_T_->publish_state(T);
    if (this->sensor_O_ != nullptr)
      this->sensor_O_->publish_state(O);
    no_data_count = 0;
    new_data = false; //Invalidate data
  }
  else {
    no_data_count++;
    ESP_LOGI("MPPT3000", "No new data %d time(s).", no_data_count);
    if (no_data_count > 10 && on_state) {
      Wire.end();
      delay(30);
      Wire.begin(address_);
      Wire.onReceive(I2C_RxHandler);
      no_data_count = 1;
    }
  }
  if (this->sensor_N_ != nullptr)
    this->sensor_N_->publish_state(zero_D_count);
}

void MPPT3000PRO::dump_config() {
    ESP_LOGCONFIG(TAG, "MPPT-3000PRO");
    ESP_LOGCONFIG(TAG, "  SDA Pin: GPIO%u", this->sda_pin_);
    ESP_LOGCONFIG(TAG, "  SCL Pin: GPIO%u", this->scl_pin_);
    LOG_PIN("  ON Pin: ", this->on_pin_);
}

} //namespace mppt3000pro
} //namespace esphome