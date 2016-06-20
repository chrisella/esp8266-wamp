#include "WampClient.h"
#include <ArduinoJson.h>

#include <stdlib.h>

WampClient::WampClient()
{
  _state = WampState::NOT_CONNECTED;
}

WampClient::~WampClient()
{

}

// TODO: Deal with messages based on the current state etc
void WampClient::onMessage (const char * str)
{
    char * cpStr = strdup(str); // Copy str into an mutatable char *
    DynamicJsonBuffer jsonBuffer;
    JsonArray& root = jsonBuffer.parseArray(str);
    if (!root.success())
    {
        // Ignore this message ?
        return;
    }
    MessageCodes command = static_cast<MessageCodes>((int)(root[0]));
    switch (command)
    {
        case MessageCodes::WELCOME:
            OnWelcome((int)root[1], root[2]);
            break;
        case MessageCodes::ABORT:
            OnAbort();
            break;
        case MessageCodes::GOODBYE:
            OnGoodbye();
            break;
        case MessageCodes::PUBLISHED:
            OnPublished((int)root[1], (int)root[2]);
            break;
    }
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
    root.add(GenerateRolesObject(jsonBuffer));
    SendJson(root);
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

void WampClient::Publish(JsonObject& options, const char * topic, JsonArray * args)
{
    StaticJsonBuffer<200> jsonBuffer;
    JsonArray& root = jsonBuffer.createArray();
    root.add((int)MessageCodes::PUBLISH);
    root.add(GenerateRequestId());
    root.add(options);
    root.add(topic);
    if (args != NULL)
    {
        root.add(*args);
    }
    SendJson(root);
    _state = WampState::AWAITING_PUBLISHED;
}

void WampClient::Subscribe(JsonObject& options, const char * topic)
{
    StaticJsonBuffer<200> b;
    JsonArray& r = b.createArray();
    r.add((int)MessageCodes::SUBSCRIBE);
    r.add(GenerateRequestId());
    r.add(options);
    r.add(topic);
    SendJson(r);
    _state = WampState::AWAITING_SUBSCRIBED;
}

void WampClient::Unsubscribe(int subId)
{
    StaticJsonBuffer<200> b;
    JsonArray& r = b.createArray();
    r.add((int)MessageCodes::UNSUBSCRIBE);
    r.add(GenerateRequestId());
    r.add(subId);
    SendJson(r);
    _state = WampState::AWAITING_UNSUBSCRIBED;
}

void WampClient::Call(JsonObject& options, const char * procedure, JsonArray * args)
{
    StaticJsonBuffer<200> b;
    JsonArray& r = b.createArray();
    r.add((int)MessageCodes::CALL);
    r.add(GenerateRequestId());
    r.add(options);
    r.add(procedure);
    if (args != NULL)
    {
        r.add(*args);
    }
    SendJson(r);
    _state = WampState::AWAITING_CALL_RESULT;
}

void WampClient::Register(JsonObject& options, const char * procedure)
{
    StaticJsonBuffer<200> b;
    JsonArray& r = b.createArray();
    r.add((int)MessageCodes::REGISTER);
    r.add(GenerateRequestId());
    r.add(options);
    r.add(procedure);
    SendJson(r);
    _state = WampState::AWAITING_REGISTERED;
}

void WampClient::Unregister(int regId)
{
    StaticJsonBuffer<200> b;
    JsonArray& r = b.createArray();
    r.add((int)MessageCodes::UNREGISTER);
    r.add(GenerateRequestId());
    r.add(regId);
    SendJson(r);
    _state = WampState::AWAITING_UNREGISTERED;
}

void WampClient::OnError(MessageCodes code)
{
    // TODO: Handle different errors based on the _state and the incoming error
}

void WampClient::OnWelcome(int sessionId, JsonObject& detailsDict)
{
    if (_state != WampState::AWAITING_WELCOME)
    {
        // Invalid, A welcome MUST be the response to a HELLO
        // TODO: Disconnect and fail?
        return;
    }
    // TODO: Process sessionId and details dictionary
}

void WampClient::OnAbort()
{

}

void WampClient::OnGoodbye()
{

}

void WampClient::OnPublished(int publishRequestId, int publicationId)
{
    if (_state != WampState::AWAITING_PUBLISHED)
    {
        // TODO: Check spec, how to handle unexpected message ordering
        return;
    }
    // TODO: Compare the PublishRequestId to the original request somehow. Requires saving it somewhere temporarily
}

void WampClient::OnSubscribed(int subRequestId, int subId)
{
    if (_state != WampState::AWAITING_SUBSCRIBED)
    {
        // TODO: Check spec, how to handle unexpected message ordering
        return;
    }
    // TODO: Save the subId somewhere so we know we're subscribed and it's available for Unsubscribe if required (Save as topic -> subId dictionary/map ?)
}

void WampClient::OnUnsubscribed(int unsubRequestId)
{
    if (_state != WampState::AWAITING_UNSUBSCRIBED)
    {
        // TODO: Check spec, how to handle unexpected message ordering
        return;
    }
    // TODO: Remove subscription from local dictionary/map
}

void WampClient::OnResult()
{

}

void WampClient::OnRegistered(int registeredId)
{
    if (_state != WampState::AWAITING_REGISTERED)
    {
        // TODO: Check spec, how to handle unexpected message ordering
        return;
    }
    // TODO: Save the regId to a dictionary/map for later unregistering
}

void WampClient::OnUnregistered()
{
    if (_state != WampState::AWAITING_UNREGISTERED)
    {
        // TODO: Check spec, how to handle unexpected message ordering
        return;
    }
// TODO: Remove the registered procedure id from the local dicationary/map
}

int WampClient::GenerateRequestId()
{
    return rand();
}

void WampClient::SendJson(JsonObject& obj)
{
    char b[512]; // TODO: This may need to be increased ?
    obj.printTo(b, sizeof(b));
    _sendMessage(b);
}

void WampClient::SendJson(JsonArray& arr)
{
    char b[512]; // TODO: This may need to be increased ?
    arr.printTo(b, sizeof(b));
    _sendMessage(b);
}

void WampClient::SendString(const char * str)
{
    _sendMessage(str);
}

JsonObject& WampClient::GenerateRolesObject(JsonBuffer& buffer)
{
    JsonObject& o = buffer.createObject();
    // TODO: Add to roles object
    return o;
}
