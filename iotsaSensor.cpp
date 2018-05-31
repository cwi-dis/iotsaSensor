#include "iotsaSensor.h"
#include "iotsaConfigFile.h"

void
IotsaSensorMod::handler() {
  bool anyChanged = false;
  for (uint8_t i=0; i<server->args(); i++){
    if( server->argName(i) == "interval") {
    	if (needsAuthentication()) return;
    	String sInterval = server->arg(i);
    	interval = sInterval.toInt();
    	anyChanged = true;
    }
  }
  if (anyChanged) configSave();

  String message = "<html><head><title>Timed Sensor Module</title></head><body><h1>Timed Sensor Module</h1>";
  message += "<form method='get'>Interval (ms): <input name='interval' value='";
  message += String(interval);
  message += "'><br><input type='submit'></form>";
  server->send(200, "text/html", message);
}

void
IotsaSensorMod::apiHandler() {
  String message;
  buffer.toJSON(message);
  server->send(200, "application/json", message);
}

void IotsaSensorMod::setup() {
  configLoad();
}

void IotsaSensorMod::serverSetup() {
  server->on("/sensor", std::bind(&IotsaSensorMod::handler, this));
  server->on("/api", std::bind(&IotsaSensorMod::apiHandler, this));
}

void IotsaSensorMod::configLoad() {
  IotsaConfigFileLoad cf("/config/sensor.cfg");
  cf.get("interval", interval, 1000);
 
}

void IotsaSensorMod::configSave() {
  IotsaConfigFileSave cf("/config/sensor.cfg");
  cf.put("interval", interval);
}

void IotsaSensorMod::loop() {
  if (millis() >= lastReading + interval) {
    lastReading = millis();
    int value = analogRead(A0);
    buffer.add(value);
  }
}

String IotsaSensorMod::info() {
  String message = "<p>Timed sensor readings. See <a href=\"/sensor\">/sensor</a> for configuration, <a href=\"/api\">/api</a> for readings.</p>";
  return message;
}

void SensorBuffer::add(SensorBufferItemValueType value)
{
  if (nItem >= SENSORBUFFERSIZE) compact();
  items[nItem].value = value;
  items[nItem].timestamp = millis();
  nItem++;
}

void SensorBuffer::compact()
{
  int toRemove = nItem - SENSORBUFFERMINSIZE;
  if (toRemove <= 0) return;
  memmove(items, items+toRemove, SENSORBUFFERMINSIZE*sizeof(SensorBufferItem));
  nItem -= toRemove;
}

void SensorBuffer::toJSON(String &json)
{
  if (nItem == 0) {
    json += "{}";
    return;
  }
  uint32_t curTime = items[0].timestamp;
  json += "{\"timestamp\":" + String(curTime);
  for (int i=0; i<nItem; i++) {
    json += ",{\"dt\":" + String(items[i].timestamp-curTime) + ",\"v\":" + String(items[i].value) + "}";
    curTime = items[i].timestamp;
  }
  json += "}";
}

