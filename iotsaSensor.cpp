#include "iotsaSensor.h"
#include "iotsaConfigFile.h"

void
IotsaSensorMod::handler() {
  bool anyChanged = false;
  for (uint8_t i=0; i<server.args(); i++){
    if( server.argName(i) == "interval") {
    	if (needsAuthentication()) return;
    	String sInterval = server.arg(i);
    	interval = sInterval.toInt();
    	anyChanged = true;
    }
  }
  if (anyChanged) configSave();

  String message = "<html><head><title>Timed Sensor Module</title></head><body><h1>Timed Sensor Module</h1>";
  message += "<form method='get'>Interval (ms): <input name='interval' value='";
  message += String(interval);
  message += "'><br><input type='submit'></form>";
  server.send(200, "text/html", message);
}

void IotsaSensorMod::setup() {
  configLoad();
}

void IotsaSensorMod::serverSetup() {
  server.on("/sensor", std::bind(&IotsaSensorMod::handler, this));
}

void IotsaSensorMod::configLoad() {
  IotsaConfigFileLoad cf("/config/sensor.cfg");
  cf.get("interval", interval, 0);
 
}

void IotsaSensorMod::configSave() {
  IotsaConfigFileSave cf("/config/sensor.cfg");
  cf.put("interval", interval);
}

void IotsaSensorMod::loop() {
}

String IotsaSensorMod::info() {
  String message = "<p>Timed sensor readings. See <a href=\"/sensor\">/sensor</a> for configuration, <a href=\"/api\">/api</a> for readings.</p>";
  return message;
}
