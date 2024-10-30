
#include <mutex>
#include <queue>
#include <tuple>
#include <Poco/Event.h>

#include "mqtt.h"

#define ONEVU_MODULE "mqtt"
#include "common/log.h"

#define QOS 1
#define TIMEOUT 1000L

namespace onevu
{

static std::queue<std::tuple<std::string, std::string>> msg_queue;
static const uint32_t MAX_SENDING_MSG_NUM = 5;
static Poco::Event dataEvent(true);

static int mqtt_send_msg_async_LOCAL(MQTTClient *client, const std::string &topic, const std::string &payload, MQTTClient_deliveryToken *token)
{
// #ifndef DYNIM_PRODUCTION
//     LOG_DEBUG("Send mqtt message with topic ' %s'\n", topic.c_str());
// #endif

    return MQTTClient_publish(*client, topic.c_str(), payload.size(), payload.c_str(), QOS, 0, token);
}

void mqtt_send_msg_sync(MQTTClient *client, const std::string &topic, const std::string &payload)
{
    MQTTClient_deliveryToken token;

    mqtt_send_msg_async_LOCAL(client, topic, payload, &token);
    int rc = MQTTClient_waitForCompletion(*client, token, TIMEOUT);

    if(rc != MQTTCLIENT_SUCCESS){
        LOG_PERROR("'%s' to send mqtt message with topic '%s', errorcode '%d'\n", MQTTClient_strerror(rc), topic.c_str(), rc);
        // throw DynimError(ERROR_MQTT_SEND_MSG_FAILURE);
    }
}

void mqtt_send_msg_async(MQTTClient *client, const std::string &topic, const std::string &payload)
{
    MQTTClient_deliveryToken token;

    int rc = mqtt_send_msg_async_LOCAL(client, topic, payload, &token);
    if(rc != MQTTCLIENT_SUCCESS){
        LOG_PERROR("'%s' to send mqtt message with topic '%s', errorcode '%d'\n", MQTTClient_strerror(rc), topic.c_str(), rc);
        // throw DynimError(ERROR_MQTT_SEND_MSG_FAILURE);
    }
}

void mqtt_enqueue_msg(const std::string& topic, const std::string& payload)
{
    msg_queue.emplace(topic, payload);
    dataEvent.set();
}

void mqtt_flush_queue(MQTTClient *client)
{
    dataEvent.tryWait(10);
    for(uint32_t i = 0; i < MAX_SENDING_MSG_NUM; i++)
    {
        if(msg_queue.empty())
            break;
        
        auto&& [topic, payload] = msg_queue.front();
        
        mqtt_send_msg_async(client, topic, payload);

        msg_queue.pop();

    }
}

void mqtt_subscribe_topic(MQTTClient *client, const std::string& topic){
    MQTTClient_subscribe(*client, topic.c_str(), QOS);
}

void mqtt_unsubscribe_topic(MQTTClient *client, const std::string& topic){
    MQTTClient_unsubscribe(*client, topic.c_str());
}

} // namespace onevu
