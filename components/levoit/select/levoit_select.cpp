#include "esphome/core/log.h"
#include "levoit_select.h"

namespace esphome {
namespace levoit {

static const char *const TAG = "levoit.select";

void LevoitSelect::setup() {
  this->parent_->register_listener(LevoitPayloadType::STATUS_RESPONSE,
                                   [this](const uint8_t *payloadBuf, size_t payloadLen) {
                                     if (this->purpose_ == LevoitSelectPurpose::PURIFIER_FAN_MODE) {
                                       uint8_t purifierFanMode = payloadBuf[5];
                                       if (purifierFanMode == 0x00) {
                                         this->publish_state("Manual");
                                       } else if (purifierFanMode == 0x01) {
                                         this->publish_state("Sleep");
                                       } else if (purifierFanMode == 0x02) {
                                         this->publish_state("Auto");
                                       }
                                     } else if (this->purpose_ == LevoitSelectPurpose::PURIFIER_AUTO_MODE) {
                                       uint8_t purifierAutoMode = payloadBuf[15];
                                       if (purifierAutoMode == 0x00) {
                                         this->publish_state("Default");
                                       } else if (purifierAutoMode == 0x01) {
                                         this->publish_state("Quiet");
                                       } else if (purifierAutoMode == 0x02) {
                                         this->publish_state("Efficient");
                                       }
                                     } else if (this->purpose_ == LevoitSelectPurpose::PURIFIER_NIGHTLIGHT_MODE) {
                                       uint8_t purifierNightlightMode = payloadBuf[12];
                                       if (purifierNightlightMode == 0x00) {
                                         this->publish_state("Off");
                                       } else if (purifierNightlightMode == 0x32) {
                                         this->publish_state("Low");
                                       } else if (purifierNightlightMode == 0x64) {
                                         this->publish_state("High");
                                       }
                                     }
                                   });
}

void LevoitSelect::control(const std::string &value) {
  if (this->purpose_ == LevoitSelectPurpose::PURIFIER_FAN_MODE) {
    if (value == "Manual") {
      switch (this->parent_->device_model_) {
        case LevoitDeviceModel::CORE_400S:
          // enter manual mode by setting speed
          // TODO: would be nice to set to last known speed setting; but
          // can't access that from here afaik
          this->parent_->send_command(LevoitCommand{.payloadType = LevoitPayloadType::SET_FAN_MANUAL,
                                                    .packetType = LevoitPacketType::SEND_MESSAGE,
                                                    .payload = {0x00, 0x00, 0x01, 0x01}});
          break;

        default:
          // set fan mode = 0
          this->parent_->send_command(LevoitCommand{.payloadType = LevoitPayloadType::SET_FAN_MODE,
                                                    .packetType = LevoitPacketType::SEND_MESSAGE,
                                                    .payload = {0x00, 0x00}});
      }

    } else if (value == "Sleep") {
      this->parent_->send_command(LevoitCommand{.payloadType = LevoitPayloadType::SET_FAN_MODE,
                                                .packetType = LevoitPacketType::SEND_MESSAGE,
                                                .payload = {0x00, 0x01}});
    } else if (value == "Auto") {
      this->parent_->send_command(LevoitCommand{.payloadType = LevoitPayloadType::SET_FAN_MODE,
                                                .packetType = LevoitPacketType::SEND_MESSAGE,
                                                .payload = {0x00, 0x02}});
    }
  } else if (this->purpose_ == LevoitSelectPurpose::PURIFIER_AUTO_MODE) {
    if (value == "Default") {
      this->parent_->send_command(LevoitCommand{.payloadType = LevoitPayloadType::SET_FAN_AUTO_MODE,
                                                .packetType = LevoitPacketType::SEND_MESSAGE,
                                                .payload = {0x00, 0x00, 0x00, 0x00}});
    } else if (value == "Quiet") {
      this->parent_->send_command(LevoitCommand{.payloadType = LevoitPayloadType::SET_FAN_AUTO_MODE,
                                                .packetType = LevoitPacketType::SEND_MESSAGE,
                                                .payload = {0x00, 0x01, 0x00, 0x00}});
    } else if (value == "Efficient") {
      this->parent_->send_command(LevoitCommand{.payloadType = LevoitPayloadType::SET_FAN_AUTO_MODE,
                                                .packetType = LevoitPacketType::SEND_MESSAGE,
                                                .payload = {0x00, 0x02, 0xEC, 0x04}});
    }
  } else if (this->purpose_ == LevoitSelectPurpose::PURIFIER_NIGHTLIGHT_MODE){
    if (value == "Off") {
      this->parent_->send_command(LevoitCommand{.payloadType = LevoitPayloadType::SET_NIGHTLIGHT,
                                                .packetType = LevoitPacketType::SEND_MESSAGE,
                                                .payload = {0x00, 0x00,0x00}});
    } else if (value == "Low") {
      this->parent_->send_command(LevoitCommand{.payloadType = LevoitPayloadType::SET_NIGHTLIGHT,
                                                .packetType = LevoitPacketType::SEND_MESSAGE,
                                                .payload = {0x00, 0x00,0x32}});
    } else if (value == "High") {
      this->parent_->send_command(LevoitCommand{.payloadType = LevoitPayloadType::SET_NIGHTLIGHT,
                                                .packetType = LevoitPacketType::SEND_MESSAGE,
                                                .payload = {0x00, 0x00,0x64}});
    }
  }
}

void LevoitSelect::dump_config() { LOG_SELECT("", "Levoit Select", this); }

}  // namespace levoit
}  // namespace esphome
