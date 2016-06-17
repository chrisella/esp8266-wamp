# ESP8266-WAMP
A library to implement the WAMP communication specification on the ESP8266 chip.

This is a bare bones PlatformIO project at the moment, it uses the ArduinoWebSockets library by Markus (Links2004) which is currently included in the lib folder until my pull request is accepted with a fix that is required.

The idea behind this library will be to allow an ESP8266 (or other compatible) chip to be part of a WAMP network by implementing the basic client requirements of the latest spec available here: 
https://cdn.rawgit.com/wamp-proto/wamp-proto/master/rfc/draft-oberstet-hybi-tavendo-wamp.html#rfc.section.6
