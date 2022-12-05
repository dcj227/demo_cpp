#pragma once

#include <stdlib.h>
#include <stdint.h>
#include <string>
#include <map>
#include <memory>
#include <rdkafkacpp.h>
#include "log.h"


namespace kafka_client_producer {

class MyDeliveryReportCb : public RdKafka::DeliveryReportCb {
public:
    virtual void dr_cb (RdKafka::Message &message);
};
class MyOAuthBearerTokenRefreshCb : public RdKafka::OAuthBearerTokenRefreshCb {
public:
    virtual void oauthbearer_token_refresh_cb (const std::string &oauthbearer_config);
};
class MyEventCb : public RdKafka::EventCb {
public:
    virtual void event_cb (RdKafka::Event &event);
};
class MyPartitionerCb : public RdKafka::PartitionerCb {
public:
    virtual int32_t partitioner_cb (const RdKafka::Topic *topic,
                                const std::string *key,
                                int32_t partition_cnt,
                                void *msg_opaque);
};
class MyPartitionerKeyPointerCb : public RdKafka::PartitionerKeyPointerCb {
public:
    virtual int32_t partitioner_cb (const RdKafka::Topic *topic,
                                const void *key,
                                size_t key_len,
                                int32_t partition_cnt,
                                void *msg_opaque);
};
class MySocketCb : public RdKafka::SocketCb {
public:
    virtual int socket_cb (int domain, int type, int protocol);
};
class MyOpenCb : public RdKafka::OpenCb {
public:
    virtual int open_cb (const std::string &path, int flags, int mode);
};

class KafkaClientProducer {
public:
    KafkaClientProducer();
    ~KafkaClientProducer();

    int Initialize(const std::string& brokers, const std::map<std::string, std::string>& kv);
    int SetTopicConf(const std::string& topic_name, const std::map<std::string, std::string>& kv);
    int Produce(const std::string& topic_name, const std::string& key, const std::string& message, void* msg_opaque);

private:
    std::string brokers_;
    std::shared_ptr<RdKafka::Conf> conf_;
    std::shared_ptr<RdKafka::Producer> producer_;
    struct TconfTopic {
        std::shared_ptr<RdKafka::Conf> c;
        std::shared_ptr<RdKafka::Topic> t;
    };
    std::map<std::string, TconfTopic> tconf_topics_;

    std::unique_ptr<RdKafka::DeliveryReportCb> dr_cb_;
    std::unique_ptr<RdKafka::OAuthBearerTokenRefreshCb> oauthbearer_token_refresh_cb_;
    std::unique_ptr<RdKafka::EventCb> event_cb_;
    std::unique_ptr<RdKafka::PartitionerCb> partitioner_cb_;
    std::unique_ptr<RdKafka::PartitionerKeyPointerCb> partitioner_key_cb_;
    std::unique_ptr<RdKafka::SocketCb> socket_cb_;
    std::unique_ptr<RdKafka::OpenCb> open_cb_;
};

}   // namespace kafka_client_producer