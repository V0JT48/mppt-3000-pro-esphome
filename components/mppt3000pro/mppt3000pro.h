#pragma once

#include "esphome/core/component.h"
#include "esphome/core/hal.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/binary_sensor/binary_sensor.h"
#include "Wire.h"


namespace esphome {
namespace mppt3000pro {

class MPPT3000PRO : public sensor::Sensor, public binary_sensor::BinarySensor, public PollingComponent {
  public:  
    void set_sensor_V(sensor::Sensor *sensor_V) { sensor_V_ = sensor_V; }
    void set_sensor_A(sensor::Sensor *sensor_A) { sensor_A_ = sensor_A; }
    void set_sensor_W(sensor::Sensor *sensor_W) { sensor_W_ = sensor_W; }
    void set_sensor_D(sensor::Sensor *sensor_D) { sensor_D_ = sensor_D; }
    void set_sensor_T(sensor::Sensor *sensor_T) { sensor_T_ = sensor_T; }
    void set_sensor_O(sensor::Sensor *sensor_O) { sensor_O_ = sensor_O; }
    void set_sensor_N(sensor::Sensor *sensor_N) { sensor_N_ = sensor_N; }
    void set_sensor_ON(binary_sensor::BinarySensor *sensor_ON) { sensor_ON_ = sensor_ON; }
    void set_sda_pin(uint8_t sda_pin) { sda_pin_ = sda_pin; }
    void set_scl_pin(uint8_t scl_pin) { scl_pin_ = scl_pin; }
    void set_address(uint16_t address) { address_ = address; }
    void set_on_pin(GPIOPin *on_pin) { this->on_pin_ = on_pin; }

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
    sensor::Sensor *sensor_N_;
    binary_sensor::BinarySensor *sensor_ON_;
    uint8_t sda_pin_;
    uint8_t scl_pin_;
    uint16_t address_;
    GPIOPin *on_pin_{nullptr};
};

} //namespace mppt3000pro
} //namespace esphome
