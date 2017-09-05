#ifndef _IOTSASENSOR_H_
#define _IOTSASENSOR_H_
#include "iotsa.h"

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
  void toJSON(String &json);
  int nItem;
  SensorBufferItem items[SENSORBUFFERSIZE];
};

class IotsaSensorMod : public IotsaMod {
public:
  using IotsaMod::IotsaMod;
  void setup();
  void serverSetup();
  void loop();
  String info();
protected:
  void configLoad();
  void configSave();
  void handler();
  void apiHandler();
  int interval;
  uint32_t lastReading;
  SensorBuffer buffer;
};

#endif
