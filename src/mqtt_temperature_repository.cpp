#include "mqtt_temperature_repository.h"

#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

#include <string>
#include <sstream>

#include <mosquittopp.h>
#include <iostream>

#include "model/api_info.h"
#include "model/temperature.h"
#include "mqtt_error.h"

namespace air_temp {
    static const std::string HOST = "test.mosquitto.org";
    static const int PORT = 8885;
    static const std::string TRUST_STORE = "mosquitto.org.crt";
    static const std::string ROOT_TOPIC = "api";
    static const std::string TEMP_TOPIC = ROOT_TOPIC + "/temperature";
    static const std::string STATUS_TOPIC = ROOT_TOPIC + "/status";
    static const std::string USERNAME = "rw";
    static const std::string PASSWORD = "readwrite";

    namespace uuids = boost::uuids;
    namespace mqtt = mosqpp;

    mqtt_temperature_repository::mqtt_temperature_repository() :
            mqtt::mosquittopp(
                    to_string(uuids::random_generator()()).c_str(),
                    false
            ) {
    }

    void mqtt_temperature_repository::send_air_temp(const temperature &temperature) {
        const std::string payload = double_to_string(temperature.value, 1);
        publish_with_reconnect(
                TEMP_TOPIC + "/" + temperature.station_id,
                payload
        );
    }

    void mqtt_temperature_repository::send_api_info(const api_info &api_info) {
        const std::string &payload = api_info.status;
        publish_with_reconnect(STATUS_TOPIC, payload);
    }

    void mqtt_temperature_repository::connect() {
        loop_start();
        tls_set(TRUST_STORE.c_str());
        username_pw_set(USERNAME.c_str(), PASSWORD.c_str());
        auto result = mqtt::mosquittopp::connect(HOST.c_str(), PORT, 600);
        if (result != MOSQ_ERR_SUCCESS)
            throw mqtt_error(result);
    }

    void mqtt_temperature_repository::disconnect() {
        auto result = mqtt::mosquittopp::disconnect();
        if (result != MOSQ_ERR_SUCCESS) {
            loop_stop(true);
            throw mqtt_error(result);
        }
        loop_stop(true);
    }

    std::string mqtt_temperature_repository::double_to_string(const double d, int precision) {
        std::stringstream stream;
        stream.precision(precision);
        stream << std::fixed;
        stream << d;
        return stream.str();
    }

    void mqtt_temperature_repository::publish_with_reconnect(
            const std::string &topic,
            const std::string &payload,
            int qos,
            bool retain
    ) {
        int result;
        do {
            result = publish(
                    nullptr,
                    topic.c_str(),
                    int(payload.length()),
                    payload.c_str(),
                    qos,
                    retain
            );
            if (result == MOSQ_ERR_CONN_LOST) {
                if (reconnect() != MOSQ_ERR_SUCCESS)
                    break;
            }
        } while (result == MOSQ_ERR_CONN_LOST);
        if (result != MOSQ_ERR_SUCCESS)
            throw mqtt_error(result);
    }
}
