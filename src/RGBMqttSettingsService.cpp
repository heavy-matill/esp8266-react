#include <RGBMqttSettingsService.h>

RGBMqttSettingsService::RGBMqttSettingsService(AsyncWebServer* server, FS* fs, SecurityManager* securityManager) :
    _httpEndpoint(RGBMqttSettings::read,
                  RGBMqttSettings::update,
                  this,
                  server,
                  RGB_BROKER_SETTINGS_PATH,
                  securityManager,
                  AuthenticationPredicates::IS_AUTHENTICATED),
    _fsPersistence(RGBMqttSettings::read, RGBMqttSettings::update, this, fs, RGB_BROKER_SETTINGS_FILE) {
}

void RGBMqttSettingsService::begin() {
  _fsPersistence.readFromFS();
}
