#ifndef AIR_TEMP_MQTT_MQTT_ERROR_H
#define AIR_TEMP_MQTT_MQTT_ERROR_H

#include <stdexcept>

namespace air_temp {
    class mqtt_error : public std::runtime_error {
    public:
        explicit mqtt_error(int error);

    private:
        static std::string error_to_string(int error);
    };
}

#endif //AIR_TEMP_MQTT_MQTT_ERROR_H
