#pragma once

#include <string>
#include <MQTTClient.h>

namespace onevu
{

// Should not use "mqtt_send_msg_sync" and "mqtt_send_msg_async" function in mqtt callback
// Use "mqtt_enqueue_msg" as a solution

void mqtt_send_msg_sync(MQTTClient *client, const std::string &topic, const std::string &payload);

void mqtt_send_msg_async(MQTTClient *client, const std::string &topic, const std::string &payload);

void mqtt_enqueue_msg(const std::string& topic, const std::string& payload);

void mqtt_send_msg_queue(MQTTClient *client);

void mqtt_subscribe_topic(MQTTClient *client, const std::string &topic);

void mqtt_unsubscribe_topic(MQTTClient *client, const std::string &topic);

}; // namespace onevu
