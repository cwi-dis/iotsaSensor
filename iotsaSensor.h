#ifndef _IOTSASENSOR_H_
#define _IOTSASENSOR_H_
#include "iotsa.h"

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
  int interval;
};

#endif
