#include "model/api_info.h"

#include <string>

#include <boost/json/value.hpp>

#include "json_utils.h"

namespace air_temp {

    static const std::string STATUS_KEY = "status";

    api_info api_info::from_json(const boost::json::value &json) {
        auto &object = json_utils::verify_value_as_object(json, {STATUS_KEY});
        return {
                object.at(STATUS_KEY).as_string().c_str(),
        };
    }
}
