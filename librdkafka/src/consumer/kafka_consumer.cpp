#include "kafka_consumer.h"
#include <chrono>

namespace kafka_client_consumer {

// void MyDeliveryReportCb::dr_cb (RdKafka::Message &message) {
//     // TODO
// }
void MyOAuthBearerTokenRefreshCb::oauthbearer_token_refresh_cb (const std::string &oauthbearer_config) {
    // TODO
}
void MyEventCb::event_cb (RdKafka::Event &event) {
    // TODO
}
// int32_t MyPartitionerCb::partitioner_cb (const RdKafka::Topic *topic,
//                                     const std::string *key,
//                                     int32_t partition_cnt,
//                                     void *msg_opaque) {
//     // TODO
//     return 0;
// }
// int32_t MyPartitionerKeyPointerCb::partitioner_cb (const RdKafka::Topic *topic,
//                                     const void *key,
//                                     size_t key_len,
//                                     int32_t partition_cnt,
//                                     void *msg_opaque) {
//     // TODO
//     return 0;
// }
int MySocketCb::socket_cb (int domain, int type, int protocol) {
    // TODO
    return 0;
}
int MyOpenCb::open_cb (const std::string &path, int flags, int mode) {
    // TODO
    return 0;
};
void MyRebalanceCb::rebalance_cb (RdKafka::KafkaConsumer *consumer,
                            RdKafka::ErrorCode err,
                            std::vector<RdKafka::TopicPartition*>&partitions) {
    // TODO
}
void MyOffsetCommitCb::offset_commit_cb(RdKafka::ErrorCode err,
                                std::vector<RdKafka::TopicPartition*>&offsets) {
    // TODO
}


KafkaClientConsumer::KafkaClientConsumer() {
    // TODO
    //oauthbearer_token_refresh_cb_ = std::unique_ptr<RdKafka::OAuthBearerTokenRefreshCb>(new MyOAuthBearerTokenRefreshCb());
    event_cb_ = std::unique_ptr<RdKafka::EventCb>(new MyEventCb());
    //socket_cb_ = std::unique_ptr<RdKafka::SocketCb>(new MySocketCb());
    //open_cb_ = std::unique_ptr<RdKafka::OpenCb>(new MyOpenCb());
    //rebalance_cb_ = std::unique_ptr<RdKafka::RebalanceCb>(new MyRebalanceCb());
    //offset_commit_cb_ = std::unique_ptr<RdKafka::OffsetCommitCb>(new MyOffsetCommitCb());
}

KafkaClientConsumer::~KafkaClientConsumer() {
    if (t_.joinable()) {
        t_.join();
    }
}

int KafkaClientConsumer::Initialize(const std::string& brokers, const std::string& group,
                    const std::map<std::string, std::string>& kv, MsgHandler msg_handler) {
    brokers_ = brokers;
    group_name_ = group;
    msg_handler_ = msg_handler;

    conf_ = std::shared_ptr<RdKafka::Conf>(RdKafka::Conf::create(RdKafka::Conf::ConfType::CONF_GLOBAL));

    std::string errstr;
    for (auto& it : kv) {
        RdKafka::Conf::ConfResult ret = conf_->set(it.first, it.second, errstr);
        if (ret != RdKafka::Conf::ConfResult::CONF_OK) {
            LOG(ERROR) << "set fail, " << it.first << ":" << it.second << " errstr:" << errstr;
            return -1;
        }
        LOG(INFO) << it.first << ":" << it.second;
    }
    RdKafka::Conf::ConfResult ret = conf_->set("bootstrap.servers", brokers, errstr);
    if (ret != RdKafka::Conf::ConfResult::CONF_OK) {
        LOG(ERROR) << "set fail, " << "bootstrap.servers" << ":" << brokers << " errstr:" << errstr;
        return -1;
    }
    LOG(INFO) << "bootstrap.servers:" << brokers;
    ret = conf_->set("group.id", group_name_, errstr);
    if (ret != RdKafka::Conf::ConfResult::CONF_OK) {
        LOG(ERROR) << "set fail, " << "group.id" << ":" << brokers << " errstr:" << errstr;
        return -1;
    }
    LOG(INFO) << "group.id:" << group;

    // 设置回调函数配置
    if (oauthbearer_token_refresh_cb_) {
        ret = conf_->set("oauthbearer_token_refresh_cb", oauthbearer_token_refresh_cb_.get(), errstr);
        if (ret != RdKafka::Conf::ConfResult::CONF_OK) {
            LOG(ERROR) << "set oauthbearer_token_refresh_cb fail, errstr:" << errstr;
            return -1;
        }
        LOG(INFO) << "set oauthbearer_token_refresh_cb succ.";
    }
    if (event_cb_) {
        ret = conf_->set("event_cb", event_cb_.get(), errstr);
        if (ret != RdKafka::Conf::ConfResult::CONF_OK) {
            LOG(ERROR) << "set event_cb fail, errstr:" << errstr;
            return -1;
        }
        LOG(INFO) << "set event_cb succ.";
    }
    if (socket_cb_) {
        ret = conf_->set("socket_cb", socket_cb_.get(), errstr);
        if (ret != RdKafka::Conf::ConfResult::CONF_OK) {
            LOG(ERROR) << "set socket_cb fail, errstr:" << errstr;
            return -1;
        }
        LOG(INFO) << "set socket_cb succ.";
    }
    if (open_cb_) {
        ret = conf_->set("open_cb", open_cb_.get(), errstr);
        if (ret != RdKafka::Conf::ConfResult::CONF_OK) {
            LOG(ERROR) << "set open_cb fail, errstr:" << errstr;
            return -1;
        }
        LOG(INFO) << "set open_cb succ.";
    }
    if (rebalance_cb_) {
        ret = conf_->set("rebalance_cb", rebalance_cb_.get(), errstr);
        if (ret != RdKafka::Conf::ConfResult::CONF_OK) {
            LOG(ERROR) << "set rebalance_cb fail, errstr:" << errstr;
            return -1;
        }
        LOG(INFO) << "set rebalance_cb succ.";
    }
    if (offset_commit_cb_) {
        ret = conf_->set("offset_commit_cb", offset_commit_cb_.get(), errstr);
        if (ret != RdKafka::Conf::ConfResult::CONF_OK) {
            LOG(ERROR) << "set offset_commit_cb fail, errstr:" << errstr;
            return -1;
        }
        LOG(INFO) << "set offset_commit_cb succ.";
    }

    consumer_ = std::shared_ptr<RdKafka::KafkaConsumer>(RdKafka::KafkaConsumer::create(conf_.get(), errstr));
    if (!consumer_) {
        LOG(ERROR) << "KafkaConsumer create fail, errstr:" << errstr;
        return -2;
    }
    return 0;
}

int KafkaClientConsumer::SetTopicForSubscribe(const std::string& topic_name, const std::map<std::string, std::string>& kv) {
    auto conf = std::shared_ptr<RdKafka::Conf>(RdKafka::Conf::create(RdKafka::Conf::ConfType::CONF_TOPIC));
    std::string errstr;
    for (auto& it : kv) {
        RdKafka::Conf::ConfResult ret = conf->set(it.first, it.second, errstr);
        if (ret != RdKafka::Conf::ConfResult::CONF_OK) {
            LOG(ERROR) << "set fail, " << it.first << ":" << it.second << " errstr:" << errstr;
            return -1;
        }
        LOG(INFO) << it.first << ":" << it.second;
    }

    auto topic = std::shared_ptr<RdKafka::Topic>(RdKafka::Topic::create(consumer_.get(), topic_name, conf.get(), errstr));
    if (!topic) {
        LOG(ERROR) << "Topic create fail, errstr:" << errstr;
        return -2;
    }
    TconfTopic& tt = tconf_topics_[topic_name];
    tt.c = conf;
    tt.t = topic;

    return 0;
}

int KafkaClientConsumer::Start() {
    std::vector<std::string> topics;
    for (auto it : tconf_topics_) {
        topics.push_back(it.first);
    }
    RdKafka::ErrorCode err = consumer_->subscribe(topics);
    if (err) {
        LOG(ERROR) << "subscribe fail, str:" << RdKafka::err2str(err);
        return -1;
    }

    stop_ = false;
    std::thread t(&KafkaClientConsumer::func, this);
    std::swap(t_,t);

    return 0;
}

int KafkaClientConsumer::Stop() {
    stop_ = true;
    if (t_.joinable()) {
        t_.join();
    }
    return 0;
}

int KafkaClientConsumer::Consume() {
    auto message = std::shared_ptr<RdKafka::Message>(consumer_->consume(consume_timeout_ms_));
    if (message->err() != RdKafka::ErrorCode::ERR_NO_ERROR) {
        // LOG
        return message->err();
    }
    if (msg_handler_) {
        std::string topic = message->topic_name();
        std::string key;
        key.assign(static_cast<const char*>(message->key_pointer()), message->key_len());
        std::string payload;
        payload.assign(static_cast<const char*>(message->payload()), message->len());
        int32_t partition = message->partition();
        int64_t offset = message->offset();
        msg_handler_(topic, partition, offset, key, payload);
    }

    return RdKafka::ErrorCode::ERR_NO_ERROR;
}

void KafkaClientConsumer::func() {
    while (!stop_) {
        if (Consume() != RdKafka::ErrorCode::ERR_NO_ERROR) {
            std::this_thread::sleep_for(std::chrono::microseconds(10));
        }
    }
}

}   // namespace kafka_client_consumer {
