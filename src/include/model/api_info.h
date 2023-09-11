#ifndef AIR_TEMP_MQTT_API_INFO_H
#define AIR_TEMP_MQTT_API_INFO_H

#include <string>
#include <boost/json.hpp>

namespace air_temp {

    struct api_info {
        std::string status;

        static api_info from_json(const boost::json::value &json);
    };

}

#endif //AIR_TEMP_MQTT_API_INFO_H
