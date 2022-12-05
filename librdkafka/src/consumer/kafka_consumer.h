#pragma once

#include <stdlib.h>
#include <stdint.h>
#include <string>
#include <map>
#include <memory>
#include <thread>
#include <rdkafkacpp.h>
#include "log.h"

namespace kafka_client_consumer {

using MsgHandler = void (*)(std::string& topic, int32_t& partition, int64_t& offset, std::string& key, std::string& payload);

class MyOAuthBearerTokenRefreshCb : public RdKafka::OAuthBearerTokenRefreshCb {
public:
    virtual void oauthbearer_token_refresh_cb (const std::string &oauthbearer_config);
};
class MyEventCb : public RdKafka::EventCb {
public:
    virtual void event_cb (RdKafka::Event &event);
};
// class MyPartitionerCb : public RdKafka::PartitionerCb {
// public:
//     virtual int32_t partitioner_cb (const RdKafka::Topic *topic,
//                                 const std::string *key,
//                                 int32_t partition_cnt,
//                                 void *msg_opaque);
// };
// class MyPartitionerKeyPointerCb : public RdKafka::PartitionerKeyPointerCb {
// public:
//     virtual int32_t partitioner_cb (const RdKafka::Topic *topic,
//                                 const void *key,
//                                 size_t key_len,
//                                 int32_t partition_cnt,
//                                 void *msg_opaque);
// };
class MySocketCb : public RdKafka::SocketCb {
public:
    virtual int socket_cb (int domain, int type, int protocol);
};
class MyOpenCb : public RdKafka::OpenCb {
public:
    virtual int open_cb (const std::string &path, int flags, int mode);
};
class MyRebalanceCb : public RdKafka::RebalanceCb {
public:
    virtual void rebalance_cb (RdKafka::KafkaConsumer *consumer,
                            RdKafka::ErrorCode err,
                            std::vector<RdKafka::TopicPartition*>&partitions);
};
class MyOffsetCommitCb : public RdKafka::OffsetCommitCb {
public:
    virtual void offset_commit_cb(RdKafka::ErrorCode err,
                                std::vector<RdKafka::TopicPartition*>&offsets);
};

class KafkaClientConsumer {
public:
    KafkaClientConsumer();
    ~KafkaClientConsumer();

    int Initialize(const std::string& brokers, const std::string& group,
                   const std::map<std::string, std::string>& kv, MsgHandler msg_handler);
    int SetTopicForSubscribe(const std::string& topic_name, const std::map<std::string, std::string>& kv);

    int Start();    // 创建线程消费kafka消息
    int Stop();
private:
    int Consume();
    void func();
private:
    const int consume_timeout_ms_ = 1000;    // 1000ms 
    std::string brokers_;
    std::string group_name_;
    std::shared_ptr<RdKafka::Conf> conf_;
    std::shared_ptr<RdKafka::KafkaConsumer> consumer_;
    struct TconfTopic {
        std::shared_ptr<RdKafka::Conf> c;
        std::shared_ptr<RdKafka::Topic> t;
    };
    std::map<std::string, TconfTopic> tconf_topics_;

    MsgHandler msg_handler_ = nullptr;
    bool stop_ = false;
    std::thread t_;

    std::unique_ptr<RdKafka::OAuthBearerTokenRefreshCb> oauthbearer_token_refresh_cb_;
    std::unique_ptr<RdKafka::EventCb> event_cb_;
    std::unique_ptr<RdKafka::PartitionerCb> partitioner_cb_;
    std::unique_ptr<RdKafka::PartitionerKeyPointerCb> partitioner_key_cb_;
    std::unique_ptr<RdKafka::SocketCb> socket_cb_;
    std::unique_ptr<RdKafka::OpenCb> open_cb_;
    std::unique_ptr<RdKafka::RebalanceCb> rebalance_cb_;
    std::unique_ptr<RdKafka::OffsetCommitCb> offset_commit_cb_;
};

}   // namespace kafka_client_consumer