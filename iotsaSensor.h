#ifndef _IOTSASENSOR_H_
#define _IOTSASENSOR_H_
#include "iotsa.h"
#include "iotsaApi.h"

typedef uint16_t SensorBufferItemValueType;
typedef struct {
  SensorBufferItemValueType value;
  uint32_t timestamp;
} SensorBufferItem;

#define SENSORBUFFERSIZE 1024
#define SENSORBUFFERMINSIZE 512
class SensorBuffer
{
public:
  SensorBuffer()
  : nItem(0)
  {}
  void add(SensorBufferItemValueType value);
  void compact();
  void toJSON(JsonObject& reply);
  int nItem;
  SensorBufferItem items[SENSORBUFFERSIZE];
};

class IotsaSensorMod : public IotsaApiMod {
public:
  IotsaSensorMod(IotsaApplication &_app) : IotsaApiMod(_app) {}
  void setup() override;
  void serverSetup() override;
  void loop() override;
  String info() override;
  using IotsaBaseMod::needsAuthentication;
protected:
  void configLoad() override;
  void configSave() override;
  void handler();
  bool getHandler(const char *path, JsonObject& reply) override;
  bool putHandler(const char *path, const JsonVariant& request, JsonObject& reply) override;
  int interval;
  uint32_t lastReading;
  SensorBuffer buffer;
};

#endif
