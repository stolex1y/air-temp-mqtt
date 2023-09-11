#ifndef AIR_TEMP_MQTT_REST_TEMPERATURE_REPOSITORY_H
#define AIR_TEMP_MQTT_REST_TEMPERATURE_REPOSITORY_H

#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ssl/context.hpp>
#include <boost/beast/http.hpp>
#include <boost/json.hpp>
#include <boost/asio/ssl/stream.hpp>
#include <string>

#include "model/api_info.h"
#include "model/temperature.h"

namespace air_temp {

    class rest_temperature_repository {
    public:
        struct response {
            air_temp::api_info api_info;
            std::vector<temperature> temperatures;
        };

        explicit rest_temperature_repository(boost::asio::io_context &ioc);

        void connect();

        void close();

        response get_air_temperature();

    private:
        boost::asio::io_context &ioc;
        boost::asio::ssl::context ssl_ctx;
        boost::asio::ssl::stream<boost::asio::ip::tcp::socket> stream;

        boost::beast::http::response<
                boost::beast::http::dynamic_body
        >
        make_request(const std::string &target);

        static bool on_peer_verify(
                bool preverified,
                boost::asio::ssl::verify_context &context
        );

        static boost::json::value parse_json_from_body(
                const boost::beast::http::response<
                        boost::beast::http::dynamic_body
                > &response
        );
    };
}

#endif //AIR_TEMP_MQTT_REST_TEMPERATURE_REPOSITORY_H
