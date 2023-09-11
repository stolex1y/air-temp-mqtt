# MQTT client for sending station temperatures
Temperature readings are taken from the 
[open API](https://api.data.gov.sg/v1/environment/air-temperature) 
via the HTTPS protocol, 
after which they are sent to the MQTT broker over a secure channel using SSL. 
[Mosquitto](https://test.mosquitto.org/) is used as an MQTT broker.

The client implementation uses the following libraries: 
* [Boost.Beast](https://github.com/boostorg/beast) for HTTP
* [libmosquittopp](https://mosquitto.org/) for MQTT

Compiled program: air_temp_mqtt.
