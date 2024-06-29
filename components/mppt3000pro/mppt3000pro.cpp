#include "esphome/core/log.h"
#include "mppt3000pro.h"

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

uint16_t tmp_V = 0;
uint16_t tmp_A = 0;
uint16_t tmp_W = 0;
uint16_t tmp_D = 0;
uint16_t tmp_T = 0;
uint8_t  tmp_O = DEFAULT_OUTPUT;

void send_data(void){
  V = tmp_V;
  A = tmp_A;
  W = tmp_W;
  D = tmp_D;
  T = tmp_T;
  O = tmp_O;
  new_data = true;
  ESP_LOGI("MPPT3000", "Received values: %dV %.1fA %dW D %.1fkWh T %dkWh Output %d", V, A/10.0, W, D/10.0, T, O);
}

void decode_character(void){
  if (character > '.' && character <= 'z') { // printable char filter and skip decimal point for A and D
    if((character >= '0') & (character <= '9')) {
        number = number*10 + (character - '0');
        prev_char_num = true;
    }
    else if(prev_char_num){
      switch (character){
        case 'V':
          tmp_V=number; 
          break;
        case 'A':
          tmp_A=number;
          break;
        case 'W':
          tmp_W=number;
          break;
        case 'k':
          switch(power) {
            case 'D':
              tmp_D = number;
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
        case 'O':
          tmp_O=number;
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
      case 0x09:
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
  ESP_LOGI("MPPT3000", "Setup done");
}

void MPPT3000PRO::loop() {
  
}

void MPPT3000PRO::update() {
  if(new_data){
    if (this->sensor_V_ != nullptr)
      this->sensor_V_->publish_state(V);
    if (this->sensor_A_ != nullptr)
      this->sensor_A_->publish_state(A/10.0);
    if (this->sensor_W_ != nullptr)
      this->sensor_W_->publish_state(W);
    if (this->sensor_D_ != nullptr)
      this->sensor_D_->publish_state(D/10.0);
    if (this->sensor_T_ != nullptr)
      this->sensor_T_->publish_state(T);
    if (this->sensor_O_ != nullptr)
      this->sensor_O_->publish_state(O);
    no_data_count = 0;
    new_data = false; //Invalidate data
  }
  else {
    no_data_count++;
    ESP_LOGI("MPPT3000", "No new data %d time(s).", no_data_count);
  }

}

void MPPT3000PRO::dump_config() {
    ESP_LOGCONFIG(TAG, "MPPT-3000PRO");
    ESP_LOGCONFIG(TAG, "  SDA Pin: GPIO%u", this->sda_pin_);
    ESP_LOGCONFIG(TAG, "  SCL Pin: GPIO%u", this->scl_pin_);
}

} //namespace mppt3000pro
} //namespace esphome
