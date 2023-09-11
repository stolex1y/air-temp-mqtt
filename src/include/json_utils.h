#ifndef AIR_TEMP_MQTT_JSON_UTILS_H
#define AIR_TEMP_MQTT_JSON_UTILS_H

#include <initializer_list>
#include <string>
#include <boost/json.hpp>
#include <stdexcept>

namespace air_temp::json_utils {
    namespace json = boost::json;

    const json::object &
    verify_value_as_object(
            const json::value &json_value,
            std::initializer_list<const std::string> fields = {}
    );

    double get_number_as_double(const json::object &json_object, const std::string &key);
}


#endif //AIR_TEMP_MQTT_JSON_UTILS_H
