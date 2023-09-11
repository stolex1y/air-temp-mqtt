#include <iostream>
#include <string>
#include <thread>
#include <ostream>
#include <unordered_set>

#include "mqtt_error.h"
#include "rest_temperature_repository.h"
#include "mqtt_temperature_repository.h"

using namespace air_temp;

int main() {
    boost::asio::io_context ioc;
    rest_temperature_repository rest_temp_repo{ioc};
    mqtt_temperature_repository mqtt_temp_repo{};

    const std::unordered_set<std::string> stations{"S50", "S107", "S60"};

    try {
        rest_temp_repo.connect();
        mqtt_temp_repo.connect();
        while (true) {
            const auto response = rest_temp_repo.get_air_temperature();
            const auto &temperatures = response.temperatures;
            const auto &api_info = response.api_info;
            for (const auto &temp: temperatures) {
                if (stations.find(temp.station_id) != stations.end()) {
                    mqtt_temp_repo.send_air_temp(temp);
                }
            }
            mqtt_temp_repo.send_api_info(api_info);
            std::this_thread::sleep_for(std::chrono::minutes (5));
        }
    } catch (const boost::system::system_error &e) {
        std::cerr << "Rest repository failure: " << e.what()
                  << " (" << e.code() << ")." << std::endl;
        return e.code().value();
    } catch (const mqtt_error &e) {
        std::cerr << "Error with mqtt repository: " << e.what() << std::endl;
    }
}
