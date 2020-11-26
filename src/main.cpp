#include <ESP8266React.h>
#include <LightMqttSettingsService.h>
#include <LightStateService.h>
#include <RGBMqttSettingsService.h>
#include <RGBStateService.h>



#define SERIAL_BAUD_RATE 115200

AsyncWebServer server(80);
ESP8266React esp8266React(&server);
void process_byte_buf(String& cmd) {
  Serial.print("received command: ");
  for (size_t i = 0; i < 5; i++) {
    Serial.print(cmd[i]);
  }
  Serial.println();
}
LightMqttSettingsService lightMqttSettingsService =
    LightMqttSettingsService(&server, esp8266React.getFS(), esp8266React.getSecurityManager());
LightStateService lightStateService = LightStateService(&server,
                                                        esp8266React.getSecurityManager(),
                                                        esp8266React.getMqttClient(),
                                                        &lightMqttSettingsService);
RGBMqttSettingsService rgbMqttSettingsService =
    RGBMqttSettingsService(&server, esp8266React.getFS(), esp8266React.getSecurityManager());
RGBStateService rgbStateService = RGBStateService(&server,
                                                  esp8266React.getFS(),
                                                  esp8266React.getSecurityManager(),
                                                  esp8266React.getMqttClient(),
                                                  &rgbMqttSettingsService);



void setup() {
  // start serial and filesystem
  Serial.begin(SERIAL_BAUD_RATE);

  // start the framework and demo project
  esp8266React.begin();

  // load the initial light settings
  lightStateService.begin();
  //rgbStateService._process_command_ptr = process_byte_buf;
  rgbStateService.begin();

  // start the light service
  lightMqttSettingsService.begin();
  rgbMqttSettingsService.begin();

  // start the server
  server.begin();
}

void loop() {
  // run the framework's loop function
  esp8266React.loop();
}
