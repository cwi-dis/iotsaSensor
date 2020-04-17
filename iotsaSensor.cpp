#include "iotsaSensor.h"
#include "iotsaConfigFile.h"

#ifdef IOTSA_WITH_WEB
void
IotsaSensorMod::handler() {
  bool anyChanged = false;
  if( server->hasArg("interval")) {
    if (needsAuthentication()) return;
    String sInterval = server->arg("interval");
    interval = sInterval.toInt();
    anyChanged = true;
  }
  if (anyChanged) configSave();

  String message = "<html><head><title>Timed Sensor Module</title></head><body><h1>Timed Sensor Module</h1>";
  message += "<form method='get'>Interval (ms): <input name='interval' value='";
  message += String(interval);
  message += "'><br><input type='submit'></form>";
  server->send(200, "text/html", message);
}

String IotsaSensorMod::info() {
  String message = "<p>Timed sensor readings. See <a href=\"/sensor\">/sensor</a> for configuration, <a href=\"/api\">/api</a> for readings.</p>";
  return message;
}
#endif // IOTSA_WITH_WEB

bool IotsaSensorMod::getHandler(const char *path, JsonObject& reply) {
  buffer.toJSON(reply);
  reply["interval"] = interval;
  return true;
}

bool IotsaSensorMod::putHandler(const char *path, const JsonVariant& request, JsonObject& reply) {
  if (!request.is<JsonObject>()) return false;
  JsonObject reqObj = request.as<JsonObject>();
  if (!reqObj.containsKey("interval")) return false;
  interval = reqObj["interval"];
  configSave();
  return true;
}

void IotsaSensorMod::setup() {
  configLoad();
}

void IotsaSensorMod::serverSetup() {
#ifdef IOTSA_WITH_WEB
  server->on("/sensor", std::bind(&IotsaSensorMod::handler, this));
#endif
  api.setup("/api/sensor", true, true);
  name = "sensor";
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

void SensorBuffer::toJSON(JsonObject &replyObj)
{
  if (nItem == 0) return;
  uint32_t curTime = items[0].timestamp;
  replyObj["timestamp"] = curTime;
  JsonArray values = replyObj.createNestedArray("data");

  for (int i=0; i<nItem; i++) {
    uint32_t delta = items[i].timestamp-curTime;
    JsonObject curValue = values.createNestedObject();
    curValue["dt"] = delta;
    curValue["v"] = items[i].value;
    curTime = items[i].timestamp;
  }
}

