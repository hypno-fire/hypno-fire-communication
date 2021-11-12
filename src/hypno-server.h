#ifndef SERVER_H
#define SERVER_H

#include <Stream.h>
#include "messages.pb.h"
#include "PacketSerial.h"

typedef void (*HypnoServerCallbackFunction)(const HypnoMessage* message);

class _DerivedPacketSerial : public PacketSerial
{
public:
    HypnoServerCallbackFunction callbackFunction = nullptr;
};

class HypnoServer
{
public:
    HypnoServer(Stream& stream);
    void update();
    void setCallback(HypnoServerCallbackFunction callbackFunction);
private:
    void onPacketReceived(const uint8_t* buffer, size_t size);
    Stream& _stream;
    _DerivedPacketSerial _packetSerial;
};

#endif // SERVER_H
