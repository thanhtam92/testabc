
#include "mqtt/mqtt_baseapp.h"
#include "mqtt/mqtt.h"

namespace onevu {

void MqttBaseApplication::start_mqtt_client() {
    LOG_INFO("Starting mqtt client");

    int rc;
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    MQTTClient_create(&_mqtt, "tcp://localhost:1883", this->_name.c_str(),
                      MQTTCLIENT_PERSISTENCE_NONE, NULL);
    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;

    MQTTClient_setCallbacks(_mqtt, this, NULL, &MqttBaseApplication::mqttmsg_callback_handler, NULL);
    if ((rc = MQTTClient_connect(_mqtt, &conn_opts)) != MQTTCLIENT_SUCCESS)
    {
        LOG_PERROR("'%s' to connect to mqtt broker\n", MQTTClient_strerror(rc));
        return;
    }
    
    _mqtt_thread = std::thread([&](){
        _mqtt_thread_stop_flag = false;
        while(1){
            onevu::mqtt_flush_queue(&_mqtt);
            // LOG_DEBUG("mqtt_flush_queue");
            
            if(_mqtt_thread_stop_flag){
                break;
            }
        }
    });

    LOG_INFO("Started mqtt client");
}

void MqttBaseApplication::stop_mqtt_client() {
    LOG_INFO("Stopping mqtt client");

    _mqtt_thread_stop_flag = true;
    _mqtt_thread.join();

    std::for_each(_mqtt_callbacks.begin(), _mqtt_callbacks.end(),
                  [&](std::pair<std::string, mqtt_callback_t> pair) {
                      onevu::mqtt_unsubscribe_topic(&_mqtt, pair.first);
                  });

    _mqtt_callbacks.clear();

    MQTTClient_disconnect(_mqtt, 10000);

    MQTTClient_destroy(&_mqtt);
    LOG_INFO("Stopped mqtt client");
}

int MqttBaseApplication::mqttmsg_callback_handler(void *context, char *topic, int topiclen, MQTTClient_message *msg)
{
    MqttBaseApplication *application = static_cast<onevu::MqttBaseApplication*>(context);

    std::string _topic(topic);
    std::string _msg((char *)msg->payload, msg->payloadlen);

#ifndef ONEVU_PRODUCTION
    LOG_DEBUG("On message/topic: '%s', msg: %s\n", topic, _msg.c_str());
#endif

    MqttBaseApplication::mqtt_callback_iter it = application->_mqtt_callbacks.find(_topic);
    if (it != application->_mqtt_callbacks.end())
    {
        // try-catch
        try
        {
            json msg_json = json::parse(std::move(_msg));
            it->second(std::move(msg_json));
        }
        catch (std::exception &e)
        {
            LOG_ERROR("On topic '%s': %s, input_msg: %s\n", _topic.c_str(), e.what(), _msg.c_str());
        }
    }
    else
    {
        LOG_WARNING("Don't have callback to handle message of topic '%s'\n", _topic.c_str());
    }

release_msg:
    MQTTClient_freeMessage(&msg);
    MQTTClient_free(topic);

    return 1;
}

void MqttBaseApplication::mqtt_subscribe(const std::string &topic, mqtt_callback_t func) {
    this->_mqtt_callbacks[topic] = func;
    mqtt_subscribe_topic(&_mqtt, topic.c_str());
}

void MqttBaseApplication::mqtt_send_message(const std::string& topic, const std::string& payload) {
    onevu::mqtt_enqueue_msg(topic, payload);
}

}