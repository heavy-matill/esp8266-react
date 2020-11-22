#include <RGBStateService.h>

RGBStateService::RGBStateService(AsyncWebServer* server,
                                 FS* fs,
                                 SecurityManager* securityManager,
                                 AsyncMqttClient* mqttClient,
                                 RGBMqttSettingsService* rgbMqttSettingsService) :
    _httpEndpoint(RGBState::read,
                  RGBState::update,
                  this,
                  server,
                  RGB_SETTINGS_ENDPOINT_PATH,
                  securityManager,
                  AuthenticationPredicates::IS_AUTHENTICATED),
    _fsPersistence(RGBState::read, RGBState::update, this, fs, RGB_SETTINGS_FILE),
    _mqttPubSub(RGBState::haRead, RGBState::haUpdate, this, mqttClient),
    _webSocket(RGBState::read,
               RGBState::update,
               this,
               server,
               RGB_SETTINGS_SOCKET_PATH,
               securityManager,
               AuthenticationPredicates::IS_AUTHENTICATED),
    _mqttClient(mqttClient),
    _rgbMqttSettingsService(rgbMqttSettingsService) {
  // configure led to be output
  // pinMode(LED_PIN, OUTPUT); ??
  _fsPersistence.disableUpdateHandler();

  // configure MQTT callback
  _mqttClient->onConnect(std::bind(&RGBStateService::registerConfig, this));

  // configure update handler for when the light settings change
  _rgbMqttSettingsService->addUpdateHandler([&](const String& originId) { registerConfig(); }, false);

  // configure settings service update handler to update LED state
  addUpdateHandler([&](const String& originId) { onConfigUpdated(); }, false);
}

void RGBStateService::begin() {
  _fsPersistence.readFromFS();

  onConfigUpdated();
}
#define RedLED 15
#define GreenLED 12
#define BlueLED 13
void setColor(uint8_t r, uint8_t g, uint8_t b) {
  analogWrite(RedLED, r);
  analogWrite(GreenLED, g);
  analogWrite(BlueLED, b);
}

void RGBStateService::onConfigUpdated() {
  // digitalWrite(LED_PIN, _state.ledOn ? LED_ON : LED_OFF);
  // set Brightness ??
  uint8_t temp_cmd = _state.cmd;
  _state.cmd = DEFAULT_CMD;
  Serial.print("onConfigUpdated command: ");
  Serial.println(temp_cmd);
  //
  switch (temp_cmd) {
    case DEFAULT_CMD:
      break;
    case 250:
      // save
      _fsPersistence.writeToFS();
      Serial.println("stored data");
      return;
      break;
    case 251:
      // load
      _fsPersistence.readFromFS();
      Serial.println("loaded data");
      break;
  }
  
  Serial.print("onConfigUpdated mode: ");
  Serial.println(_state.mode);
  //
  switch (_state.mode) {
    case 0:
      // light
      setColor(_state.brightness, _state.brightness, _state.brightness);
      break;
    case 1:
      // C1
      setColor(_state.r1, _state.g1, _state.b1);
      break;
    case 2:
      // C2
      setColor(_state.r2, _state.g2, _state.b2);
      break;
  }
}

void RGBStateService::registerConfig() {
  if (!_mqttClient->connected()) {
    return;
  }
  String configTopic;
  String subTopic;
  String pubTopic;

  DynamicJsonDocument doc(256);
  _rgbMqttSettingsService->read([&](RGBMqttSettings& settings) {
    configTopic = settings.mqttPath + "/config";
    subTopic = settings.mqttPath + "/set";
    pubTopic = settings.mqttPath + "/state";
    doc["~"] = settings.mqttPath;
    doc["name"] = settings.name;
    doc["unique_id"] = settings.uniqueId;
  });
  doc["cmd_t"] = "~/set";
  doc["stat_t"] = "~/state";
  doc["schema"] = "json";
  doc["brightness"] = false;

  String payload;
  serializeJson(doc, payload);
  _mqttClient->publish(configTopic.c_str(), 0, false, payload.c_str());

  _mqttPubSub.configureTopics(pubTopic, subTopic);
}
