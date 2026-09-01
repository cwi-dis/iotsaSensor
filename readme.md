# iotsaSensor - web server to record time series from a sensor

> **Archived 2026-09-01.** This was an unfinished 2018 experiment (last real code
> work then; "work in progress" was never resolved). Its idea — sample an analog
> pin at an interval, serve the readings over the network — is fully covered, and
> done properly, by **[iotsaDataLogger](https://github.com/cwi-dis/iotsaDataLogger)**:
> LittleFS-backed storage that survives reboot, NTP wall-clock timestamps,
> deep-sleep for battery use, CSV readout, a configurable pin — and it is actually
> deployed. iotsaSensor by contrast kept its whole buffer in RAM (lost on reboot,
> ~17 minutes at the default interval), timestamped only in `millis()` since boot,
> and re-serialised the entire buffer on every read. It was never migrated to the
> iotsa v3 API. Use iotsaDataLogger instead.

IotsaSensor is work in progress.
It reads an analog sensor repeatedly at a settable interval and records these readings in a buffer.
The buffer can then be read over the web as JSON data.

Home page is <https://github.com/cwi-dis/iotsaSensor>.

## Software requirements

* Arduino IDE, v1.6 or later.
* The iotsa framework, download from <https://github.com/cwi-dis/iotsa>.
* The new LiquidCrystal library, download from <https://bitbucket.org/fmalpartida/new-liquidcrystal>.

## Hardware requirements

* an esp8266 board, such as an ESP-12, ESP-201 or iotsa board.
* An analog sensor.



## Building the software

To be provided.

## Operation

To be provided.