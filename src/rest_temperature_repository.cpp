#include "rest_temperature_repository.h"

#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ssl/context.hpp>
#include <boost/beast/http.hpp>
#include <boost/json.hpp>
#include <boost/beast/core/flat_buffer.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/connect.hpp>
#include <boost/beast/core/buffers_to_string.hpp>
#include <boost/asio/ssl/stream.hpp>

#include <string>

#include "model/api_info.h"
#include "model/temperature.h"

namespace air_temp {

    static const std::string HOST = "api.data.gov.sg";
    static const std::string AIR_TEMP_TARGET = "/v1/environment/air-temperature";

    namespace beast = boost::beast;
    namespace http = beast::http;
    namespace asio = boost::asio;
    namespace ssl = asio::ssl;
    namespace json = boost::json;
    using tcp = asio::ip::tcp;

    rest_temperature_repository::rest_temperature_repository(asio::io_context &ioc) :
            ioc(ioc),
            ssl_ctx{ssl::context::sslv23_client},
            stream{ioc, ssl_ctx} {
        ssl_ctx.set_default_verify_paths();
        ssl_ctx.set_verify_mode(ssl::verify_peer);
        ssl_ctx.set_verify_callback(rest_temperature_repository::on_peer_verify);
        stream.lowest_layer().close();
    };

    void rest_temperature_repository::connect() {
        tcp::resolver resolver(ioc);
        const auto results = resolver.resolve(HOST, "https");
        asio::connect(stream.lowest_layer(), results);
        stream.handshake(ssl::stream_base::client);
    }

    void rest_temperature_repository::close() {
        stream.lowest_layer().shutdown(tcp::socket::shutdown_both);
        stream.lowest_layer().close();
    }

    rest_temperature_repository::response rest_temperature_repository::get_air_temperature() {
        response response;
        json::value resp_json = parse_json_from_body(make_request(AIR_TEMP_TARGET));
        auto &temps_json = resp_json.at("items").as_array()[0].at("readings").as_array();
        std::transform(
                temps_json.begin(),
                temps_json.end(),
                std::inserter(response.temperatures, response.temperatures.begin()),
                temperature::from_json
        );
        auto &api_info_json = resp_json.at("api_info").as_object();
        response.api_info = api_info::from_json(api_info_json);
        return response;
    }

    http::response<http::dynamic_body> rest_temperature_repository::make_request(const std::string &target) {
        http::request<http::string_body> req{http::verb::get, target, 11};
        req.set(http::field::host, HOST);
        req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
        http::write(stream, req);

        beast::flat_buffer buffer;
        http::response<http::dynamic_body> res;
        http::read(stream, buffer, res);
        return res;
    }

    bool rest_temperature_repository::on_peer_verify(
            bool preverified,
            ssl::verify_context &context
    ) {
        return true;
    }

    json::value rest_temperature_repository::parse_json_from_body(const http::response<http::dynamic_body> &response) {
        return json::parse(buffers_to_string(response.body().data()));
    }
}
