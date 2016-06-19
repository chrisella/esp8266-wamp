/**
 * Blink
 * Turns on an LED on for one second,
 * then off for one second, repeatedly.
 */
#include "Arduino.h"
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#include <WebSocketsClient.h>

#include <Hash.h>

#include <WampClient.h>

//#include <ArduinoJson.h>
//#include "MessageCodes.h"

ESP8266WiFiMulti WiFiMulti;
WebSocketsClient webSocket;

WampClient wampClient;

//#define USE_SERIAL Serial1
#define USE_SERIAL Serial

// HACKING AROUND BITS, DELETE WHEN DONE////
////////////////////////////////////////////

void onSendMessage(const char * message)
{
    webSocket.sendTXT(message);
}

void webSocketEvent(WStype_t type, uint8_t * payload, size_t length)
{
    switch (type) {
        case WStype_DISCONNECTED:
            USE_SERIAL.printf("[WSc] Disconnected!\n");
            break;
        case WStype_CONNECTED:
            {
                USE_SERIAL.printf("[WSc] Connected to url: %s\n",  payload);

                // send message to server when Connected
                webSocket.sendTXT("[1,\"realm1\",{\"roles\":{\"publisher\":{},\"subscriber\":{}}}]");
            }
            break;
        case WStype_TEXT:
            USE_SERIAL.printf("[WSc] get text: %s\n", payload);

			// send message to server
			// webSocket.sendTXT("message here");
            break;
        case WStype_BIN:
            USE_SERIAL.printf("[WSc] get binary length: %u\n", length);
            hexdump(payload, length);

            // send data to server
            // webSocket.sendBIN(payload, length);
            break;
    }
}

void setup() {
    // USE_SERIAL.begin(921600);
    USE_SERIAL.begin(115200);

    //Serial.setDebugOutput(true);
    USE_SERIAL.setDebugOutput(true);

    USE_SERIAL.println();
    USE_SERIAL.println();
    USE_SERIAL.println();

      for(uint8_t t = 4; t > 0; t--) {
          USE_SERIAL.printf("[SETUP] BOOT WAIT %d...\n", t);
          USE_SERIAL.flush();
          delay(1000);
      }

    WiFiMulti.addAP("MilleniumFalcon", "goldie123");

    //WiFi.disconnect();
    while(WiFiMulti.run() != WL_CONNECTED) {
        delay(100);
    }

    webSocket.begin("chrisella.com", 9889, "/ws", "wamp.2.json");
    // //webSocket.setAuthorization("user", "Password"); // HTTP Basic Authorization
    webSocket.onEvent(webSocketEvent);

    wampClient.onSendMessage(onSendMessage);
    wampClient.Hello("realm1");

    // StaticJsonBuffer<200> jsonBuffer;
    // JsonArray& root = jsonBuffer.createArray();
    // root.add((int)MessageCodes::HELLO);
    // root.add("somerealm");
    // root.add(jsonBuffer.createObject());
    // root.prettyPrintTo(USE_SERIAL);
}

void loop() {
    webSocket.loop();
}
