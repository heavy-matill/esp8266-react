#ifndef RGBStateService_h
#define RGBStateService_h

#include <RGBMqttSettingsService.h>

#include <HttpEndpoint.h>
#include <MqttPubSub.h>
#include <WebSocketTxRx.h>

#include <FSPersistence.h>

#define PRINT_DELAY 5000

#define DEFAULT_CMD 0
#define DEFAULT_BRIGHTNESS_STATE 255
#define DEFAULT_OFF_STATE 0
#define DEFAULT_MODE 0
#define MIN_STATE 0
#define MAX_STATE 255

#define RGB_SETTINGS_ENDPOINT_PATH "/rest/rgbState"
#define RGB_SETTINGS_SOCKET_PATH "/ws/rgbState"
#define RGB_SETTINGS_FILE "/config/rgbSettings.json"
typedef void (*voidCmdProcessor)(String &cmd);// Create a type to point to a funciton.
void setColor(uint8_t r, uint8_t g, uint8_t b);
class RGBState {
 public:
  uint8_t cmd, mode;  // off //light //color //animate //paused
  uint8_t brightness;
  uint8_t r1, g1, b1;
  uint8_t r2, g2, b2;
  uint16_t angle;
  uint8_t thres;
  String cmdString;

  static void read(RGBState& settings, JsonObject& root) {
    root["cmd"] = settings.cmd;
    root["mode"] = settings.mode;
    root["brightness"] = settings.brightness;
    root["c1"]["r"] = settings.r1;
    root["c1"]["g"] = settings.g1;
    root["c1"]["b"] = settings.b1;
    root["c2"]["r"] = settings.r2;
    root["c2"]["g"] = settings.g2;
    root["c2"]["b"] = settings.b2;
    root["angle"]= settings.angle;
    root["thres"]= settings.thres;
    Serial.println("read root:");
    serializeJsonPretty(root, Serial);
    Serial.println();
  }

  static StateUpdateResult update(JsonObject& root, RGBState& settings) {
    Serial.println("updated root:");
    serializeJsonPretty(root, Serial);
    Serial.println();
    settings.cmd = root["cmd"] | DEFAULT_CMD;
    switch (settings.cmd) {
      case DEFAULT_CMD:
        // no command
        break;
      case 250:
        // store
        Serial.println("Will store data");
        return StateUpdateResult::CHANGED;
        break;
      case 251:
        // load
        Serial.println("Will load data");
        return StateUpdateResult::CHANGED;
        break;
      default:
        // unknown command
        Serial.print("Unknown command: ");
        Serial.println(settings.cmd);
    }

    settings.mode = root["mode"] | DEFAULT_MODE;
    settings.brightness = root["brightness"] | DEFAULT_BRIGHTNESS_STATE;
    settings.r1 = root["c1"]["r"] | DEFAULT_OFF_STATE;
    settings.g1 = root["c1"]["g"] | DEFAULT_OFF_STATE;
    settings.b1 = root["c1"]["b"] | DEFAULT_OFF_STATE;
    settings.r2 = root["c2"]["r"] | DEFAULT_OFF_STATE;
    settings.g2 = root["c2"]["g"] | DEFAULT_OFF_STATE;
    settings.b2 = root["c2"]["b"] | DEFAULT_OFF_STATE;
    settings.angle = root["angle"] | DEFAULT_OFF_STATE;
    settings.thres = root["thres"] | DEFAULT_OFF_STATE;
    return StateUpdateResult::CHANGED;
  }

  static void haRead(RGBState& settings, JsonObject& root) {
    Serial.println("haRead root:");
    serializeJsonPretty(root, Serial);
    Serial.println();
    char strByt[4];
    strByt[3] = {0};
    sprintf(strByt, "%u", settings.brightness);  // convert to an unsigned string
    root["state"] = strByt;
  }

  static StateUpdateResult haUpdate(JsonObject& root, RGBState& settings) {
    Serial.println("haUpdated root:");
    serializeJsonPretty(root, Serial);
    Serial.println();
    String state = root["state"];
    // parse new brightness state
    int32_t newStateLong = strtol(state.c_str(), 0, 10);
    if (MIN_STATE <= newStateLong && newStateLong <= MAX_STATE) {
      // change the new state, if required
      uint8_t newState = (uint8_t)newStateLong;
      if (settings.brightness != newState) {
        settings.brightness = newState;
        return StateUpdateResult::CHANGED;
      }
      return StateUpdateResult::UNCHANGED;
    }
    return StateUpdateResult::ERROR;  // out of range
  }
};

class RGBStateService : public StatefulService<RGBState> {
 public:
  RGBStateService(AsyncWebServer* server,
                  FS* fs,
                  SecurityManager* securityManager,
                  AsyncMqttClient* mqttClient,
                  RGBMqttSettingsService* rgbMqttSettingsService);
  void begin();
 private:
  HttpEndpoint<RGBState> _httpEndpoint;
  MqttPubSub<RGBState> _mqttPubSub;
  WebSocketTxRx<RGBState> _webSocket;
  AsyncMqttClient* _mqttClient;
  RGBMqttSettingsService* _rgbMqttSettingsService;
  FSPersistence<RGBState> _fsPersistence;  

  void registerConfig();
  void onConfigUpdated();
};

#endif
