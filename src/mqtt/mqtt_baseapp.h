#pragma once

#include <iostream>
#include <map>
#include <thread>

#include <Poco/Util/ServerApplication.h>
#include <Poco/Util/HelpFormatter.h>
#include <Poco/Util/Option.h>
#include <Poco/Util/OptionSet.h>
#include <Poco/Util/AbstractConfiguration.h>
#include <mqtt/mqtt.h>

#include "common/log.h"
#include "common/json.h"

using namespace Poco::Util;

namespace onevu {

class MqttBaseApplication : public ServerApplication {

public:

    using mqtt_callback_t = std::function<void(json&&)>;

    MqttBaseApplication(const std::string& name)
        : _name(name) {}
    MqttBaseApplication(const std::string& name, const std::string & config) 
        : _name(name), _configFile(config) {}

    virtual ~MqttBaseApplication() {}

    void mqtt_subscribe(const std::string &topic, mqtt_callback_t func);
    void mqtt_send_message(const std::string& topic, const std::string& payload);

protected:
    void initialize(Application &self) {
        logger().information("Version: XXX");

        Poco::Path cfg_path(_configFile);
        if(this->findFile(cfg_path)){
            logger().information("Load config from file %s", _configFile);
            loadConfiguration(_configFile);
        }
        else {
            logger().information("Load config from default, if present");
            loadConfiguration();
        }

        ServerApplication::initialize(self);
        
        this->start_mqtt_client();
    }

    void uninitialize() {
        this->stop_mqtt_client();

        ServerApplication::uninitialize();
    }
    void reinitialize(Application &self) {
        ServerApplication::reinitialize(self);
        //  add your own reinitialize code here
    }
    
    void defineOptions(OptionSet &options) {
        ServerApplication::defineOptions(options);

        options.addOption(
            Option("help", "h",
                    "display help  information on command lne arguments")
                .required(false)
                .repeatable(false)
                .callback(OptionCallback<MqttBaseApplication>(
                    this, &MqttBaseApplication::handleHelp))
        );
        options.addOption(
            Option("config", "f",
                    "load configuration data from a file, default is lpr_stream.properties")
                .required(false)
                .repeatable(false)
                .argument("file")
                .callback(OptionCallback<MqttBaseApplication>(
                    this, &MqttBaseApplication::handleConfig))
        );
        options.addOption(
            Option("version", "v",  "display version of application and exit")
                .required(false)
                .repeatable(false)
                .callback(OptionCallback<MqttBaseApplication>(
                    this, &MqttBaseApplication::handleVersion))
        );
    }

    void handleHelp(const std::string  &name, const std::string& value) {
        _helpRequested = true;
        displayHelp();
        stopOptionsProcessing();
        exit(Application::EXIT_OK);
    }
    void handleVersion(const std::string &name, const  std::string& value) {
        logger().information("Version:  XXXX");
        exit(Application::EXIT_OK);
    }
    
    void handleDefine(const std::string &name, const  std::string& value) {
        defineProperty(value);
    }

    void handleConfig(const std::string &name, const  std::string& value) {
        loadConfiguration(value);
    }
    void displayHelp() {
        HelpFormatter helpFormatter(options());
        helpFormatter.setCommand(commandName());
        helpFormatter.setUsage("OPTIONS");
        helpFormatter.setHeader("Onevu lpr help:");
        helpFormatter.format(std::cout);
    }
    void  defineProperty(const std::string  &def){
        std::string  name;
        std::string  value;
        std::string::size_type pos = def.find('=');
        if(pos != std::string::npos){
            name.assign(def, 0, pos);
            value.assign(def, pos + 1, def.length() - pos);
        }
        else{
            name = def;
        }
        config().setString(name, value);
    }

    MQTTClient _mqtt;

    bool _helpRequested = false;
    std::string _configFile = "lpr_stream.properties";
    std::string _name;

private:
    void start_mqtt_client();
    void stop_mqtt_client();
    static int mqttmsg_callback_handler(void *context, char *topic, int topiclen, MQTTClient_message *msg);

    using mqtt_callback_iter = std::map<std::string, mqtt_callback_t>::iterator;
    std::map<std::string, mqtt_callback_t> _mqtt_callbacks;

    std::thread _mqtt_thread;
    bool _mqtt_thread_stop_flag = false;
};

}
