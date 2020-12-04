#include <RGBStateService.h>
#define FASTLED_RGBW
#define FASTLED_ESP8266_DMA
#include <FastLED.h>
#define LED_PIN 3
#define COLOR_ORDER RGB
#define CHIPSET NEOPIXEL
#define BRIGHTNESS 255

#define max(a, b) ((a) > (b) ? (a) : (b))
#define min(a, b) ((a) < (b) ? (a) : (b))

#define SERPENTINE true

#define MATRIX_15_5_
#define LINE_LETTER_K

#ifdef MATRIX_15_5
  #define NUM_X 15
  #define NUM_Y 5
  #define NUM_LEDS NUM_X* NUM_Y
  const uint8_t* line_xy = nullptr;
  const uint8_t* line_order = nullptr;
#endif

#ifdef LINE_LETTER_K
  #define NUM_X 4
  #define NUM_Y 5
  #define NUM_LEDS 10
  const uint8_t line_xy[20] = {0,0,0,1,0,2,0,3,0,4,1,1,2,2,3,3,3,1,4,0};
  const uint8_t line_order[12] = {4,3,2,1,0,1,5,6,7,6,8,9};
#endif

#define NUM_MAX max(NUM_X, NUM_Y)
#define NUM_MIN min(NUM_X, NUM_Y)

CRGB leds[NUM_LEDS];

uint16_t XY(uint8_t x, uint8_t y) {
  uint16_t i;

  if (SERPENTINE == false) {
    i = (y * NUM_X) + x;
  }

  if (SERPENTINE == true) {
    if (y & 0x01) {
      // Odd rows run backwards
      uint8_t reverseX = (NUM_X - 1) - x;
      i = (y * NUM_X) + reverseX;
    } else {
      // Even rows run forwards
      i = (y * NUM_X) + x;
    }
  }

  return i;
}

uint16_t XYsafe(uint8_t x, uint8_t y) {
  if (x >= NUM_X)
    return -1;
  if (y >= NUM_Y)
    return -1;
  return XY(x, y);
}

CRGB interpCol(CRGB& c1, CRGB& c2, float v, float thres) {
  if (v < -thres)
    return c1;
  if (v > thres || thres == 0)
    return c2;
  else {
    fract8 amountOfC2 = (fract8)((v / thres + (float)1) * (float)127.5);
    return blend(c1, c2, amountOfC2);
  }
}

void fillSplit(CRGB& c1, CRGB& c2, int16_t angle, uint8_t percBlur, uint8_t percCenter, const uint8_t* line_xy) {
  float x_mid = (float)NUM_X * (float)percCenter / 100;
  float y_mid = (float)NUM_Y * (float)percCenter / 100;
  float thres = (float)percBlur / 100;
  if (angle > 135) {
    angle -= 180;
    if (angle <= 135) {
      CRGB cTemp = CRGB();
      cTemp = c1;
      c1 = c2;
      c2 = cTemp;
    } else {
      angle -= 180;
    }
  }

  if ((-45 < angle) && (angle <= 45)) {
    double angleD = angle / 180.0 * PI;
    double tanPart = tan(-angleD);
    thres *= (float)(NUM_Y + NUM_X * fabs(tanPart)) / 2;  /// cos(angleD);

    if (line_xy) {
      for (uint8_t i = 0; i < NUM_LEDS; i++) {
        float y_lim = y_mid + (line_xy[i * 2] - x_mid) * tanPart;
        leds[i] = interpCol(c1, c2, line_xy[i * 2 + 1] - y_lim, thres);
      }
    } else {
      for (uint8_t x = 0; x < NUM_X; x++) {
        float y_lim = y_mid + (x - x_mid) * tanPart;
        for (uint8_t y = 0; y < NUM_Y; y++) {
          leds[XY(x, y)] = interpCol(c1, c2, y - y_lim, thres);
        }
      }
    }
  } else {
    if ((45 < angle) && (angle <= 135)) {
      angle -= 90;
      double angleD = angle / 180.0 * PI;  // thres /= cos(angleD);
      double tanPart = tan(angleD);
      thres *= (float)(NUM_X + NUM_Y * fabs(tanPart)) / 2;  /// cos(angleD);

      if (line_xy) {
        for (uint8_t i = 0; i < NUM_LEDS; i++) {
          float x_lim = x_mid + (line_xy[i * 2 + 1] - y_mid) * tanPart;
          leds[i] = interpCol(c1, c2, line_xy[i * 2] - x_lim, thres);
        }
      } else {
        for (uint8_t y = 0; y < NUM_Y; y++) {
          float x_lim = x_mid + (y - y_mid) * tanPart;
          for (uint8_t x = 0; x < NUM_X; x++) {
            leds[XY(x, y)] = interpCol(c1, c2, x - x_lim, thres);
          }
        }
      }
    }
  }
}

void fillRainbow(int16_t angle, uint8_t percBlur, uint8_t percCenter, const uint8_t* points) {
  Serial.println("fillRainbow");
  Serial.println(angle);
  Serial.println(percBlur);
  Serial.println(percCenter);
  // percBlur 100% -> detlahue = 256/2/LEN  50% full rainbow
  // percCenter offset in starting hue
  // angle offsets the starting hue according to perc*256
  float x_mid = (float)NUM_X * (float)percCenter / 100;
  float y_mid = (float)NUM_Y * (float)percCenter / 100;
  float fac_offs = (float)percBlur / 50;
  double angleD = angle / 180.0 * PI;  // thres /= cos(angleD);
  uint8_t h_offs_x = cos(angleD) * fac_offs * 256 / NUM_MIN;
  uint8_t h_offs_y = sin(angleD) * fac_offs * 256 / NUM_MIN;
  uint8_t h_offs_center = percCenter * 256 / 100;
  if (points) {
    for (uint8_t i = 0; i < NUM_LEDS; i++) {
      leds[i] = CHSV(points[i * 2] * h_offs_x + points[i * 2 + 1] * h_offs_y + h_offs_center, 255, 255);
    }
  } else {
    for (uint8_t x = 0; x < NUM_X; x++) {
      for (uint8_t y = 0; y < NUM_Y; y++) {
        leds[XY(x, y)] = CHSV(x * h_offs_x + y * h_offs_y + h_offs_center, 255, 255);
      }
    }
  }
}

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

  FastLED.addLeds<CHIPSET, LED_PIN>(leds, NUM_LEDS);  //.setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(BRIGHTNESS);
}
#define RedLED 15
#define GreenLED 12
#define BlueLED 13
void setColor(uint8_t r, uint8_t g, uint8_t b) {
  analogWrite(RedLED, r);
  analogWrite(GreenLED, g);
  analogWrite(BlueLED, b);
  fill_solid(leds, NUM_LEDS, CRGB(r, g, b));
  FastLED.show();
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
    case 248:
      _state.cmdString = "248";
      break;
    case 249:
      _state.cmdString = "249";
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
  CRGB c1 = CRGB(_state.r1, _state.g1, _state.b1);
  CRGB c2 = CRGB(_state.r2, _state.g2, _state.b2);
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
    case 3:
      // C2
      break;
      /// CRGB c1 = CRGB(_state.r1, _state.g1, _state.b1);
      // CRGB c2 = CRGB(_state.r2, _state.g2, _state.b2);
      // fillSplit(c1, c2, _state.angle, _state.blur, _state.center, nullptr);
      // FastLED.show();
    case 4:
      fillSplit(c1, c2, _state.angle, _state.blur, _state.center, line_xy);
      FastLED.show();
      break;
    case 5:
      fillRainbow(_state.angle, _state.blur, _state.center, line_xy);
      FastLED.show();
      break;
  }
  //_process_command_ptr(_state.cmdString);
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
