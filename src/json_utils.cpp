#include "json_utils.h"

#include <boost/json.hpp>

#include <initializer_list>
#include <string>
#include <stdexcept>

namespace air_temp::json_utils {
    namespace json = boost::json;

    const json::object &
    verify_value_as_object(
            const json::value &json_value,
            const std::initializer_list<const std::string> fields
    ) {
        if (!json_value.is_object())
            throw std::runtime_error("Invalid json_utils value: must be an object.");
        const auto &json_object = json_value.as_object();
        for (const auto &field: fields) {
            if (!json_object.contains(field))
                throw std::runtime_error("Invalid json_utils value: must contain field with key '" + field + "'.");
        }
        return json_object;
    }

    double get_number_as_double(const json::object &json_object, const std::string &key) {
        auto &value = json_object.at(key);

        if (!value.is_number())
            throw std::runtime_error("Invalid json_utils value with key '" + key + "': must be a number.");

        if (value.is_double())
            return value.as_double();
        else
            return double(value.as_int64());
    }
}
