#include "client.h"
#include <libserialport.h>
#include <stdio.h>
#include <string.h>
#include <pb_encode.h>
#include "messages.pb.h"

sp_return check(enum sp_return result);

extern "C" {

int createSerialSession(const char* devicePath, unsigned int baudrate, SessionContainer** _container) {
    SessionContainer* container = new SessionContainer();


    sp_return serial_result = check(sp_get_port_by_name(devicePath, &container->port));
    if(serial_result != sp_return::SP_OK) {
        return -1;
    }

    serial_result = check(sp_open(container->port, SP_MODE_WRITE));
    if(serial_result != sp_return::SP_OK) {
        return -1;
    }

    check(sp_set_baudrate(container->port, baudrate));
    check(sp_set_bits(container->port, 8));
    check(sp_set_parity(container->port, SP_PARITY_NONE));
    check(sp_set_stopbits(container->port, 1));
    check(sp_set_flowcontrol(container->port, SP_FLOWCONTROL_NONE));

    *_container = container;

    return 0;
}

void destroySession(SessionContainer* container) {
    check(sp_close(container->port));
    sp_free_port(container->port);
    delete container;
}

int sendPulse(SessionContainer* container) {
    Pulse pulse = Pulse_init_default;
    uint8_t buffer[500];
    pb_ostream_t stream = pb_ostream_from_buffer(&buffer[0], sizeof(buffer));
    if(!pb_encode(&stream, Pulse_fields, &pulse)) {
        return -1;
    }

    if(sp_blocking_write(container->port, buffer, stream.bytes_written, 1000) != (int)stream.bytes_written) {
        return -1;
    }
}

}

sp_return check(enum sp_return result)
{
        /* For this example we'll just exit on any error by calling abort(). */
        char *error_message;

        switch (result) {
        case SP_ERR_ARG:
                printf("Error: Invalid argument.\n");
        case SP_ERR_FAIL:
                error_message = sp_last_error_message();
                printf("Error: Failed: %s\n", error_message);
                sp_free_error_message(error_message);
        case SP_ERR_SUPP:
                printf("Error: Not supported.\n");
        case SP_ERR_MEM:
                printf("Error: Couldn't allocate memory.\n");
        case SP_OK:
        default:
                return result;
        }
}
