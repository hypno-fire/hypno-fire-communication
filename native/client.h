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
    int sendPulse(SessionContainer* container);
}

#endif // CLIENT_H
