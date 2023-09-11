find_package(Boost 1.83.0 REQUIRED COMPONENTS json)
find_package(OpenSSL REQUIRED)

add_executable(air_temp_mqtt
        main.cpp
        rest_temperature_repository.cpp
        json_utils.cpp
        model/api_info.cpp
        model/temperature.cpp
        mqtt_error.cpp
        mqtt_temperature_repository.cpp
)

target_link_libraries(air_temp_mqtt PRIVATE
        "${OPENSSL_LIBRARIES}"
        "${Boost_LIBRARIES}"
        mosquittopp
)
target_include_directories(air_temp_mqtt PRIVATE ${Boost_INCLUDE_DIRS} include)

configure_file(../mosquitto.org.crt mosquitto.org.crt COPYONLY)
