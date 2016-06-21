#ifndef __WAMP_CLIENT_H__
#define __WAMP_CLIENT_H__

#include <Arduino.h>
#include <functional>
#include <map>
#include <string>

#include <ArduinoJson.h>
#include <WebSocketsClient.h>

#include "WampState.h"
#include "MessageCodes.h"

typedef std::function<void()> TSubscribeCallback;


class WampClient {
public:
    WampClient(WebSocketsClient& transport);
    ~WampClient();

    // New Methods
    void connect(const char * realm);
    void connect(String realm);

    void disconnect(const char * realm);
    void disconnect(String realm);

    void subscribe  (const char * topic, TSubscribeCallback cb);
    void subscribe  (std::string& topic, TSubscribeCallback cb);
    void unsubscribe(const char * topic, TSubscribeCallback cb);
    void unsubscribe(std::string& topic, TSubscribeCallback cb);

    void publish    (const char * topic, JsonArray& args);
    void publish    (std::string& topic, JsonArray& args);

private:

    void send(JsonArray& object);
    void send(JsonObject& object);
    void send(const char * str);
    void send(String str);

    WebSocketsClient * _transport;


public:
    // Old Methods
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
    void OnSubscribed(int subRequestId, int subId);
    void OnUnsubscribed(const char * topic, int unsubRequestId);
    void OnResult();
    void OnRegistered(int regRequestId, int procId);
    void OnUnregistered(const char * procedure, int unregRequestId);

    JsonObject& GenerateRolesObject(JsonBuffer& buffer);

private:
    WampState _state;

    int GenerateRequestId();

    const char * UriFromRequestId(int reqId);
    bool RemoveRequestId(int reqId);

    void SendJson(JsonObject& obj);
    void SendJson(JsonArray& arr);
    void SendString(const char * str);
    std::function<void(const char *)> _sendMessage;

private:

    std::map<const char *, int> _procedures;
    std::map<const char *, int> _subscriptions;

    std::map<int, const char *> _requestIdToUri;

};

#endif
