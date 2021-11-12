#include "hypno-server.h"
#include "pb_decode.h"

void _callback(const void* sender, const uint8_t* buffer, size_t size)
{
    _DerivedPacketSerial* packetSerial = (_DerivedPacketSerial*)sender;
    HypnoMessage message;
    pb_istream_t memoryStream = pb_istream_from_buffer(buffer, size);
    if(pb_decode(&memoryStream, HypnoMessage_fields, &message)) {
        if(packetSerial->callbackFunction) {
            packetSerial->callbackFunction(&message);
        }
    }
}

HypnoServer::HypnoServer(Stream& stream) : _stream(stream)
{
    _packetSerial.setStream(&_stream);
    _packetSerial.setPacketHandler(_callback);
}

void HypnoServer::update()
{
    _packetSerial.update();
}

void HypnoServer::setCallback(HypnoServerCallbackFunction callbackFunction)
{
    _packetSerial.callbackFunction = callbackFunction;
}