import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import select
from esphome.const import (
    ENTITY_CATEGORY_CONFIG,
    ICON_DATABASE,
    ICON_LIGHTBULB,
    CONF_ID
)

from .. import levoit_ns, CONF_LEVOIT_ID, Levoit

DEPENDENCIES = ["levoit"]
CODEOWNERS = ["@acvigue"]

CONF_PURIFIER_FAN_MODE = "fan_mode"
CONF_PURIFIER_AUTO_MODE = "auto_mode"
CONF_PURIFIER_NIGHTLIGHT_MODE = "nightlight_mode"


LevoitSelect = levoit_ns.class_("LevoitSelect", cg.Component, select.Select)
LevoitSelectPurpose = levoit_ns.enum("LevoitSelectPurpose", True)

CONFIG_SCHEMA = (
    cv.Schema({
        cv.GenerateID(CONF_LEVOIT_ID): cv.use_id(Levoit),
        cv.Optional(CONF_PURIFIER_FAN_MODE): select.select_schema(LevoitSelect, entity_category=ENTITY_CATEGORY_CONFIG, icon=ICON_DATABASE),
        cv.Optional(CONF_PURIFIER_AUTO_MODE): select.select_schema(LevoitSelect, entity_category=ENTITY_CATEGORY_CONFIG, icon=ICON_DATABASE),
        cv.Optional(CONF_PURIFIER_NIGHTLIGHT_MODE): select.select_schema(LevoitSelect, entity_category=ENTITY_CATEGORY_CONFIG, icon=ICON_LIGHTBULB),
    })
)


async def to_code(config):
    parent = await cg.get_variable(config[CONF_LEVOIT_ID])

    nightlight = None
    if CONF_PURIFIER_NIGHTLIGHT_MODE in config:
        nightlight = CONF_PURIFIER_NIGHTLIGHT_MODE
    

    if config_fan_mode := config.get(CONF_PURIFIER_FAN_MODE):
        var = cg.new_Pvariable(config_fan_mode[CONF_ID], parent, LevoitSelectPurpose.PURIFIER_FAN_MODE)
        # TODO: Better way to detect core200s model
        # Core300s and Core400s dont have nightlight mode but auto option
        if nightlight != CONF_PURIFIER_NIGHTLIGHT_MODE:
            await select.register_select(var, config_fan_mode, options=["Manual", "Sleep", "Auto"])
        else:
            await select.register_select(var, config_fan_mode, options=["Manual", "Sleep"])

        await cg.register_component(var, config_fan_mode)

    if config_auto_mode := config.get(CONF_PURIFIER_AUTO_MODE):
        var = cg.new_Pvariable(config_auto_mode[CONF_ID], parent, LevoitSelectPurpose.PURIFIER_AUTO_MODE)
        await select.register_select(var, config_auto_mode, options=["Default", "Quiet", "Efficient"])
        await cg.register_component(var, config_auto_mode)

    if config_nightlight_mode := config.get(CONF_PURIFIER_NIGHTLIGHT_MODE):
        var = cg.new_Pvariable(config_nightlight_mode[CONF_ID], parent, LevoitSelectPurpose.PURIFIER_NIGHTLIGHT_MODE)
        await select.register_select(var, config_nightlight_mode, options=["Off", "Low", "High"])
        await cg.register_component(var, config_nightlight_mode)
