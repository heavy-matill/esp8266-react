#ifndef RGBMqttSettingsService_h
#define RGBMqttSettingsService_h

#include <HttpEndpoint.h>
#include <FSPersistence.h>
#include <ESPUtils.h>

#define RGB_BROKER_SETTINGS_FILE "/config/brokerSettings.json"
#define RGB_BROKER_SETTINGS_PATH "/rest/brokerSettings"

class RGBMqttSettings {
 public:
  String mqttPath;
  String name;
  String uniqueId;

  static void read(RGBMqttSettings& settings, JsonObject& root) {
    root["mqtt_path"] = settings.mqttPath;
    root["name"] = settings.name;
    root["unique_id"] = settings.uniqueId;
  }

  static StateUpdateResult update(JsonObject& root, RGBMqttSettings& settings) {
    settings.mqttPath = root["mqtt_path"] | ESPUtils::defaultDeviceValue("homeassistant/rgb/");
    settings.name = root["name"] | ESPUtils::defaultDeviceValue("rgb-");
    settings.uniqueId = root["unique_id"] | ESPUtils::defaultDeviceValue("rgb-");
    return StateUpdateResult::CHANGED;
  }
};

class RGBMqttSettingsService : public StatefulService<RGBMqttSettings> {
 public:
  RGBMqttSettingsService(AsyncWebServer* server, FS* fs, SecurityManager* securityManager);
  void begin();

 private:
  HttpEndpoint<RGBMqttSettings> _httpEndpoint;
  FSPersistence<RGBMqttSettings> _fsPersistence;
};

#endif  // end RGBMqttSettingsService_h
