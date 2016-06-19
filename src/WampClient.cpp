#include "WampClient.h"

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

void WampClient::Hello()
{
    _sendMessage("[1,\"realm1\",{\"roles\":{\"publisher\":{},\"subscriber\":{}}}]");
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
