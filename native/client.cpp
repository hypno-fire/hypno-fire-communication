#include "client.h"
#include <libserialport.h>
#include <stdio.h>
#include <string.h>
#include <pb_encode.h>
#include "messages.pb.h"
#include "Encoding/COBS.h"

sp_return check(enum sp_return result);

int sendMessage(SessionContainer* container, HypnoMessage* message);

extern "C" {

int createSerialSession(const char* devicePath, unsigned int baudrate, SessionContainer** _container)
{
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

void destroySession(SessionContainer* container)
{
    check(sp_close(container->port));
    sp_free_port(container->port);
    delete container;
}

int sendPrime(SessionContainer* container)
{
    HypnoMessage message = HypnoMessage_init_default;
    message.which_message = HypnoMessage_prime_tag;
    message.message.prime = Prime_init_default;

    return sendMessage(container, &message);
}

int sendPulse(SessionContainer* container, int universe, int layer, int key, int color, int fadeInTicks, int durationTicks, int fadeOutTicks)
{
    HypnoMessage message = HypnoMessage_init_default;
    Pulse pulse = Pulse_init_default;

    pulse.universe = universe;
    pulse.layer = layer;
    pulse.key = key;
    pulse.color = color;
    pulse.fade_in_ticks = fadeInTicks;
    pulse.duration_ticks = durationTicks;
    pulse.fade_out_ticks = fadeOutTicks;

    message.message.pulse = pulse;
    message.which_message = HypnoMessage_pulse_tag;

    return sendMessage(container, &message);
}

int sendReset(SessionContainer* container)
{
    HypnoMessage message = HypnoMessage_init_default;
    message.message.reset = Reset_init_default;
    message.which_message = HypnoMessage_reset_tag;

    return sendMessage(container, &message);
}

int sendLayerConfiguration(SessionContainer* container, int layer, int layerBrush)
{
    HypnoMessage message = HypnoMessage_init_default;
    message.which_message = HypnoMessage_layer_configuration_tag;

    LayerConfiguration layerConfiguration = LayerConfiguration_init_default;
    layerConfiguration.has_layer_brush = true;
    layerConfiguration.layer_brush = (LayerBrush)layerBrush;
    layerConfiguration.layer_number = layer;
    message.message.layer_configuration = layerConfiguration;

    return sendMessage(container, &message);
}

}

sp_return check(enum sp_return result)
{
    char *error_message;
    switch (result) {
    case SP_ERR_ARG:
        printf("Error: Invalid argument.\n");
        break;
    case SP_ERR_FAIL:
        error_message = sp_last_error_message();
        printf("Error: Failed: %s\n", error_message);
        sp_free_error_message(error_message);
        break;
    case SP_ERR_SUPP:
        printf("Error: Not supported.\n");
        break;
    case SP_ERR_MEM:
        printf("Error: Couldn't allocate memory.\n");
        break;
    case SP_OK:
    default:
        return result;
    }
    return result;
}

int sendMessage(SessionContainer* container, HypnoMessage* message)
{
    uint8_t buffer[500];
    pb_ostream_t stream = pb_ostream_from_buffer(&buffer[0], sizeof(buffer));

    if(!pb_encode(&stream, HypnoMessage_fields, message)) {
        return -1;
    }

    uint8_t encodedBuffer[COBS::getEncodedBufferSize(stream.bytes_written)];
    size_t numEncoded = COBS::encode(buffer, stream.bytes_written, encodedBuffer);

    if(check(sp_blocking_write(container->port, encodedBuffer, numEncoded, 1000)) < 0) {
        return -1;
    }

    encodedBuffer[0] = 0;
    if(check(sp_blocking_write(container->port, encodedBuffer, 1, 1000)) <= SP_OK) {
        return -1;
    }

    return 0;
}
