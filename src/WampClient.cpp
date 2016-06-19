#include "WampClient.h"
#include <ArduinoJson.h>

#include "MessageCodes.h"

WampClient::WampClient()
{
  _state = WampState::NOT_CONNECTED;
}

WampClient::~WampClient()
{

}

void WampClient::onMessage (const char * str)
{
    // TODO: Deal with messages based on the current state etc
    
}

void WampClient::onMessage (String str)
{
    onMessage (str.c_str());
}

void WampClient::onSendMessage (std::function<void(const char *)> sendCallback)
{
    _sendMessage = sendCallback;
}

void WampClient::Hello(const char * realm)
{
    StaticJsonBuffer<200> jsonBuffer;
    JsonArray& root = jsonBuffer.createArray();
    root.add((int)MessageCodes::HELLO);
    root.add(realm);
    JsonObject& rolesObj = jsonBuffer.createObject();
    rolesObj["publisher"] = jsonBuffer.createObject();
    rolesObj["subscriber"] = jsonBuffer.createObject();
    root.add(rolesObj);
    char helloRequest[200];
    root.printTo(helloRequest, sizeof(helloRequest));
    // TODO: Skip the copy to buffer and use with .print() straight to a stream
    _sendMessage(helloRequest);
    // TODO: Detect if the send failed in some way to prevent going into
    // awaiting state if so.
    _state = WampState::AWAITING_WELCOME;
}

void WampClient::Abort()
{

}

void WampClient::Goodbye()
{

}

void WampClient::Error()
{

}

void WampClient::Publish()
{

}

void WampClient::Subscribe()
{

}

void WampClient::Unsubscribe()
{

}

void WampClient::Call()
{

}

void WampClient::Register()
{

}

void WampClient::Unregister()
{

}
