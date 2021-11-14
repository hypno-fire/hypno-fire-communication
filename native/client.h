#ifndef CLIENT_H
#define CLIENT_H
#include <libserialport.h>

struct SessionContainer {
    int fd;
    struct sp_port* port;
};

extern "C" {
    int createSerialSession(const char* devicePath, unsigned int baudRate, SessionContainer** container);
    void destroySession(SessionContainer* container);
    int sendPrime(SessionContainer* container);
    int sendPulse(SessionContainer* container, int universe, int layer, int key, int color, int fadeInTicks, int durationTicks, int fadeOutTicks);
    int sendReset(SessionContainer* container);
    int sendLayerConfiguration(SessionContainer* container, int layer, int layerBrush);
}

#endif // CLIENT_H
