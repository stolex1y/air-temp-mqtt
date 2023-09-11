#include "mqtt_error.h"

#include <stdexcept>
#include <mosquittopp.h>
#include <cerrno>

namespace air_temp {
    namespace mqtt = mosqpp;

    mqtt_error::mqtt_error(int error) : std::runtime_error(error_to_string(error)) {}

    std::string mqtt_error::error_to_string(int error) {
        switch (error) {
            case MOSQ_ERR_NO_CONN:
                return "Client isn't connected.";
            case MOSQ_ERR_PROTOCOL:
                return "There is a protocol error communicating with the broker.";
            case MOSQ_ERR_PAYLOAD_SIZE:
                return "Payload length is too large.";
            case MOSQ_ERR_QOS_NOT_SUPPORTED:
                return "The QoS is greater than that supported by the broker.";
            case MOSQ_ERR_OVERSIZE_PACKET:
                return "The resulting packet would be larger than supported by the broker.";
            case MOSQ_ERR_ERRNO:
                return "Errno: " + std::to_string(errno) + ".";
            default:
                return "Unknown error (" + std::to_string(error) + ").";
        }
    }
}
