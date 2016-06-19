#ifndef __WAMP_CLIENT_H__
#define __WAMP_CLIENT_H__

#include <Arduino.h>
#include <functional>

#include "WampState.h"

class WampClient {
public:
  WampClient();
  ~WampClient();


  void onMessage (const char * str);
  void onMessage (String str);

  void onSendMessage (std::function<void(const char *)> sendCallback);

private:

    void Hello(); // Awaits Welcome
    void Abort();
    void Goodbye();
    void Error();
    void Publish(); // Awaits Published
    void Subscribe();  // Awaits Subscribed
    void Unsubscribe(); // Awaits Unsubscribed
    void Call(); // Awaits Result
    void Register(); // Awaits Registered
    void Unregister(); // Awaits Unregistered

    WampState _state;

    std::function<void(const char *)> _sendMessage;
};

#endif
