#include "model/temperature.h"

#include <boost/json.hpp>

#include <string>

#include "json_utils.h"

namespace air_temp {
    static const std::string STATION_ID_KEY = "station_id";
    static const std::string VALUE_KEY = "value";

    temperature temperature::from_json(const boost::json::value &json) {
        auto &object = json_utils::verify_value_as_object(json, {STATION_ID_KEY, VALUE_KEY});
        return {
                object.at(STATION_ID_KEY).as_string().c_str(),
                json_utils::get_number_as_double(object, VALUE_KEY)
        };
    }
}

