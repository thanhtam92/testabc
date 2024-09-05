#pragma once
#include <Poco/Util/Application.h>
#include <mqtt/mqtt.h>

using namespace Poco::Util;

namespace onevu {

class MqttBaseApplication : public Application {

public:
    MqttBaseApplication(const std::string& name);

    virtual ~MqttBaseApplication() {}

    static MQTTClient mqtt;

protected:
    std::string _name;

};

}
