#ifndef AIR_TEMP_MQTT_TEMPERATURE_H
#define AIR_TEMP_MQTT_TEMPERATURE_H

#include <boost/json.hpp>
#include <string>

namespace air_temp {
    struct temperature {
        std::string station_id;
        double value;

        static temperature from_json(const boost::json::value &json);
    };
}

#endif //AIR_TEMP_MQTT_TEMPERATURE_H
