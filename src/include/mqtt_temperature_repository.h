#ifndef AIR_TEMP_MQTT_MQTT_TEMPERATURE_REPOSITORY_H
#define AIR_TEMP_MQTT_MQTT_TEMPERATURE_REPOSITORY_H

#include <string>

#include <mosquittopp.h>

#include "model/api_info.h"
#include "model/temperature.h"

namespace air_temp {
    class mqtt_temperature_repository : private mosqpp::mosquittopp {

    public:
        explicit mqtt_temperature_repository();

        void send_air_temp(const temperature &temperature);

        void send_api_info(const api_info &api_info);

        void connect();

        void disconnect();

    private:
        static std::string double_to_string(double d, int precision = 1);

        void publish_with_reconnect(
                const std::string &topic,
                const std::string &payload,
                int qos = 2,
                bool retain = true
        );
    };
}

#endif //AIR_TEMP_MQTT_MQTT_TEMPERATURE_REPOSITORY_H
