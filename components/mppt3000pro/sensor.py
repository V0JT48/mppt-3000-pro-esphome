import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor, binary_sensor
from esphome import pins
from esphome.const import (
    CONF_ID,
    CONF_SDA,
    CONF_SCL,
    CONF_ADDRESS,
    UNIT_EMPTY,
    UNIT_VOLT,
    UNIT_AMPERE,
    UNIT_WATT,
    UNIT_KILOWATT_HOURS,
    PLATFORM_ESP32,
    )

mppt3000pro_ns = cg.esphome_ns.namespace("mppt3000pro")
MPPT3000PRO = mppt3000pro_ns.class_(
    "MPPT3000PRO", cg.PollingComponent
)

CONF_SENSOR_V = "voltage"
CONF_SENSOR_A = "current"
CONF_SENSOR_W = "power"
CONF_SENSOR_D = "energy_day"
CONF_SENSOR_T = "energy_total"
CONF_SENSOR_O = "output"
CONF_SENSOR_N = "no_data_count"
CONF_SENSOR_ON = "on_state"
CONF_ON_PIN = "on_pin"


CONFIG_SCHEMA = cv.All(
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(MPPT3000PRO),
            cv.Optional(CONF_SENSOR_V): sensor.sensor_schema(
                unit_of_measurement=UNIT_VOLT, icon="mdi:lightning-bolt", accuracy_decimals=0
            ),
            cv.Optional(CONF_SENSOR_A): sensor.sensor_schema(
                unit_of_measurement=UNIT_AMPERE, icon="mdi:lightning-bolt", accuracy_decimals=1
            ),
            cv.Optional(CONF_SENSOR_W): sensor.sensor_schema(
                unit_of_measurement=UNIT_WATT, icon="mdi:lightning-bolt", accuracy_decimals=0
            ),
            cv.Optional(CONF_SENSOR_D): sensor.sensor_schema(
                unit_of_measurement=UNIT_KILOWATT_HOURS, icon="mdi:lightning-bolt", accuracy_decimals=1, device_class="energy", state_class="total_increasing"
            ),
            cv.Optional(CONF_SENSOR_T): sensor.sensor_schema(
                unit_of_measurement=UNIT_KILOWATT_HOURS, icon="mdi:lightning-bolt", accuracy_decimals=0, device_class="energy", state_class="total_increasing"
            ),
            cv.Optional(CONF_SENSOR_O): sensor.sensor_schema(
                unit_of_measurement=UNIT_EMPTY, icon="mdi:power-socket-eu", accuracy_decimals=0
            ),
            cv.Optional(CONF_SENSOR_N): sensor.sensor_schema(
                unit_of_measurement=UNIT_EMPTY, accuracy_decimals=0
            ),
            cv.Optional(CONF_SENSOR_ON): binary_sensor.binary_sensor_schema(),
            cv.Optional(CONF_SCL, default="SCL"): pins.internal_gpio_pin_number,
            cv.Optional(CONF_SDA, default="SDA"): pins.internal_gpio_pin_number,
            cv.Optional(CONF_ADDRESS, default=0x3F): cv.one_of(0x3F, 0x27, int=True),
            cv.Optional(CONF_ON_PIN): pins.gpio_input_pullup_pin_schema,
        }
    ).extend(cv.polling_component_schema("60s")),
    cv.only_on([PLATFORM_ESP32]),
)



async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)

    if sensor_V_config := config.get(CONF_SENSOR_V):
        sens = await sensor.new_sensor(sensor_V_config)
        cg.add(var.set_sensor_V(sens))

    if sensor_A_config := config.get(CONF_SENSOR_A):
        sens = await sensor.new_sensor(sensor_A_config)
        cg.add(var.set_sensor_A(sens))

    if sensor_W_config := config.get(CONF_SENSOR_W):
        sens = await sensor.new_sensor(sensor_W_config)
        cg.add(var.set_sensor_W(sens))

    if sensor_D_config := config.get(CONF_SENSOR_D):
        sens = await sensor.new_sensor(sensor_D_config)
        cg.add(var.set_sensor_D(sens))

    if sensor_T_config := config.get(CONF_SENSOR_T):
        sens = await sensor.new_sensor(sensor_T_config)
        cg.add(var.set_sensor_T(sens))

    if sensor_O_config := config.get(CONF_SENSOR_O):
        sens = await sensor.new_sensor(sensor_O_config)
        cg.add(var.set_sensor_O(sens))

    if sensor_N_config := config.get(CONF_SENSOR_N):
        sens = await sensor.new_sensor(sensor_N_config)
        cg.add(var.set_sensor_N(sens))

    if sensor_ON_config := config.get(CONF_SENSOR_ON):
        sens = await binary_sensor.new_binary_sensor(sensor_ON_config)
        cg.add(var.set_sensor_ON(sens))

    cg.add(var.set_sda_pin(config[CONF_SDA]))
    cg.add(var.set_scl_pin(config[CONF_SCL]))
    cg.add(var.set_address(config[CONF_ADDRESS]))
    if CONF_ON_PIN in config:
        pin_on = await cg.gpio_pin_expression(config[CONF_ON_PIN])
        cg.add(var.set_on_pin(pin_on))
