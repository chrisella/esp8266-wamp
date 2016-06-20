#ifndef __WAMP_CLIENT_H__
#define __WAMP_CLIENT_H__

#include <Arduino.h>
#include <functional>
#include <map>

#include <ArduinoJson.h>

#include "WampState.h"
#include "MessageCodes.h"

class WampClient {
public:
    WampClient();
    ~WampClient();


    void onMessage (const char * str);
    void onMessage (String str);

    void onSendMessage (std::function<void(const char *)> sendCallback);

    void Hello(const char * realm); // Awaits Welcome
    void Abort();
    void Goodbye();
    void Publish(JsonObject& options, const char * topic, JsonArray * args = NULL); // Awaits Published
    void Subscribe(JsonObject& options, const char * topic);  // Awaits Subscribed
    void Unsubscribe(int subId); // Awaits Unsubscribed
    void Call(JsonObject& options, const char * procedure, JsonArray * args = NULL); // Awaits Result
    void Register(JsonObject& options, const char * procedure); // Awaits Registered
    void Unregister(int regId); // Awaits Unregistered

private:

    void OnError(MessageCodes code);
    void OnWelcome(int sessionId, JsonObject& detailsDict);
    void OnAbort();
    void OnGoodbye();
    void OnPublished(int publishRequestId, int publicationId);
    void OnSubscribed(const char * topic, int subRequestId, int subId);
    void OnUnsubscribed(const char * topic, int unsubRequestId);
    void OnResult();
    void OnRegistered(const char * procedure, int regRequestId, int procId);
    void OnUnregistered(const char * procedure, int unregRequestId);

    JsonObject& GenerateRolesObject(JsonBuffer& buffer);

private:
    WampState _state;

    int GenerateRequestId();

    void SendJson(JsonObject& obj);
    void SendJson(JsonArray& arr);
    void SendString(const char * str);
    std::function<void(const char *)> _sendMessage;

private:

    std::map<const char *, int> _procedures;
    std::map<const char *, int> _subscriptions;

};

#endif
