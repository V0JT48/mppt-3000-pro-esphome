#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "Wire.h"


namespace esphome {
namespace mppt3000pro {

class MPPT3000PRO : public sensor::Sensor, public PollingComponent {
  public:  
    void set_sensor_V(sensor::Sensor *sensor_V) { sensor_V_ = sensor_V; }
    void set_sensor_A(sensor::Sensor *sensor_A) { sensor_A_ = sensor_A; }
    void set_sensor_W(sensor::Sensor *sensor_W) { sensor_W_ = sensor_W; }
    void set_sensor_D(sensor::Sensor *sensor_D) { sensor_D_ = sensor_D; }
    void set_sensor_T(sensor::Sensor *sensor_T) { sensor_T_ = sensor_T; }
    void set_sensor_O(sensor::Sensor *sensor_O) { sensor_O_ = sensor_O; }
    void set_sda_pin(uint8_t sda_pin) { sda_pin_ = sda_pin; }
    void set_scl_pin(uint8_t scl_pin) { scl_pin_ = scl_pin; }
    void set_address(uint16_t address) { address_ = address; }

    void setup() override;
    void loop() override;
    void update() override;
    void dump_config() override;
    float get_setup_priority() const override { return esphome::setup_priority::AFTER_WIFI; }

  protected:
    sensor::Sensor *sensor_V_;
    sensor::Sensor *sensor_A_;
    sensor::Sensor *sensor_W_;
    sensor::Sensor *sensor_D_;
    sensor::Sensor *sensor_T_;
    sensor::Sensor *sensor_O_;
    uint8_t sda_pin_;
    uint8_t scl_pin_;
    uint16_t address_;
};

} //namespace mppt3000pro
} //namespace esphome
