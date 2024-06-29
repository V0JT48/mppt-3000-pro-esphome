import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor
from esphome import pins
from esphome.const import (
    CONF_ID,
    CONF_INPUT,
    CONF_OUTPUT,
    CONF_SDA,
    CONF_SCL,
    CONF_ADDRESS,
    UNIT_EMPTY,
    UNIT_VOLT,
    UNIT_AMPERE,
    UNIT_WATT,
    UNIT_KILOWATT_HOURS,
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

pin_with_input_and_output_support = pins.internal_gpio_pin_number(
    {CONF_OUTPUT: True, CONF_INPUT: True}
)

CONFIG_SCHEMA = cv.Schema(
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
            unit_of_measurement=UNIT_KILOWATT_HOURS, icon="mdi:lightning-bolt", accuracy_decimals=1
        ),
        cv.Optional(CONF_SENSOR_T): sensor.sensor_schema(
            unit_of_measurement=UNIT_KILOWATT_HOURS, icon="mdi:lightning-bolt", accuracy_decimals=0
        ),
        cv.Optional(CONF_SENSOR_O): sensor.sensor_schema(
            unit_of_measurement=UNIT_EMPTY, icon="mdi:power-socket-eu", accuracy_decimals=0
        ),
        cv.Optional(CONF_SCL, default="SCL"): pin_with_input_and_output_support,
        cv.Optional(CONF_SDA, default="SDA"): pin_with_input_and_output_support,
        cv.Optional(CONF_ADDRESS, default=0x3F): cv.one_of(0x3F, 0x27, int=True),
    }
).extend(cv.polling_component_schema("30s"))


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

    cg.add(var.set_sda_pin(config[CONF_SDA]))
    cg.add(var.set_scl_pin(config[CONF_SCL]))
    cg.add(var.set_address(config[CONF_ADDRESS]))
