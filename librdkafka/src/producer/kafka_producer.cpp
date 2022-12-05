#include "kafka_producer.h"

namespace kafka_client_producer {

void MyDeliveryReportCb::dr_cb (RdKafka::Message &message) {
    // TODO
}
void MyOAuthBearerTokenRefreshCb::oauthbearer_token_refresh_cb (const std::string &oauthbearer_config) {
    // TODO
}
void MyEventCb::event_cb (RdKafka::Event &event) {
    // TODO
}
int32_t MyPartitionerCb::partitioner_cb (const RdKafka::Topic *topic,
                                    const std::string *key,
                                    int32_t partition_cnt,
                                    void *msg_opaque) {
    // TODO
    return 0;
}
int32_t MyPartitionerKeyPointerCb::partitioner_cb (const RdKafka::Topic *topic,
                                    const void *key,
                                    size_t key_len,
                                    int32_t partition_cnt,
                                    void *msg_opaque) {
    // TODO
    return 0;
}
int MySocketCb::socket_cb (int domain, int type, int protocol) {
    // TODO
    return 0;
}
int MyOpenCb::open_cb (const std::string &path, int flags, int mode) {
    // TODO
    return 0;
};

KafkaClientProducer::KafkaClientProducer() {
    // TODO
    dr_cb_ = std::unique_ptr<RdKafka::DeliveryReportCb>(new MyDeliveryReportCb());
    //oauthbearer_token_refresh_cb_ = std::unique_ptr<RdKafka::OAuthBearerTokenRefreshCb>(new MyOAuthBearerTokenRefreshCb());
    event_cb_ = std::unique_ptr<RdKafka::EventCb>(new MyEventCb());
    //partitioner_cb_ = std::unique_ptr<RdKafka::PartitionerCb>(new MyPartitionerCb());
    //partitioner_key_cb_ = std::unique_ptr<RdKafka::PartitionerKeyPointerCb>(new MyPartitionerKeyPointerCb());
    //socket_cb_ = std::unique_ptr<RdKafka::SocketCb>(new MySocketCb());
    //open_cb_ = std::unique_ptr<RdKafka::OpenCb>(new MyOpenCb());
}

KafkaClientProducer::~KafkaClientProducer() {
    if (producer_) {
        producer_->flush(3000);
    }
}

int KafkaClientProducer::Initialize(const std::string& brokers, const std::map<std::string, std::string>& kv) {
    brokers_ = brokers;

    conf_ = std::shared_ptr<RdKafka::Conf>(RdKafka::Conf::create(RdKafka::Conf::CONF_GLOBAL));

    std::string errstr;
    RdKafka::Conf::ConfResult ret = RdKafka::Conf::ConfResult::CONF_OK;
    for (auto& it : kv) {
        ret = conf_->set(it.first, it.second, errstr);
        if (ret != RdKafka::Conf::ConfResult::CONF_OK) {
            LOG(ERROR) << "set fail, " << it.first << ":" << it.second << " errstr:" << errstr;
            return -1;
        }
        LOG(INFO) << it.first << ":" << it.second;
    }
    ret = conf_->set("bootstrap.servers", brokers, errstr);
    if (ret != RdKafka::Conf::ConfResult::CONF_OK) {
        LOG(ERROR) << "set fail, " << "bootstrap.servers" << ":" << brokers << " errstr:" << errstr;
        return -1;
    }
    LOG(INFO) << "bootstrap.servers" << ":" << brokers;

    // 设置回调函数配置
    if (!dr_cb_) {
        ret = conf_->set("dr_cb", dr_cb_.get(), errstr);
        if (ret != RdKafka::Conf::ConfResult::CONF_OK) {
            LOG(ERROR) << "set cb fail, errstr:" << errstr;
            return -1;
        }
        LOG(INFO) << "set cb succ.";
    }
    if (!oauthbearer_token_refresh_cb_) {
        ret = conf_->set("oauthbearer_token_refresh_cb", oauthbearer_token_refresh_cb_.get(), errstr);
        if (ret != RdKafka::Conf::ConfResult::CONF_OK) {
            LOG(ERROR) << "set oauthbearer_token_refresh_cb fail, errstr:" << errstr;
            return -1;
        }
        LOG(INFO) << "set oauthbearer_token_refresh_cb succ.";
    }
    if (!event_cb_) {
        ret = conf_->set("event_cb", event_cb_.get(), errstr);
        if (ret != RdKafka::Conf::ConfResult::CONF_OK) {
            LOG(ERROR) << "set event_cb fail, errstr:" << errstr;
            return -1;
        }
        LOG(INFO) << "set event_cb succ.";
    }
    if (!socket_cb_) {
        ret = conf_->set("socket_cb", socket_cb_.get(), errstr);
        if (ret != RdKafka::Conf::ConfResult::CONF_OK) {
            LOG(ERROR) << "set socket_cb fail, errstr:" << errstr;
            return -1;
        }
        LOG(INFO) << "set socket_cb succ.";
    }
    if (!open_cb_) {
        ret = conf_->set("open_cb", open_cb_.get(), errstr);
        if (ret != RdKafka::Conf::ConfResult::CONF_OK) {
            LOG(ERROR) << "set open_cb fail, errstr:" << errstr;
            return -1;
        }
        LOG(INFO) << "set open_cb succ.";
    }


    producer_ = std::shared_ptr<RdKafka::Producer>(RdKafka::Producer::create(conf_.get(), errstr));
    if (!producer_) {
        LOG(ERROR) << "Producer create fail, errstr:" << errstr;
        return -2;
    }

    return 0;
}

int KafkaClientProducer::SetTopicConf(const std::string& topic_name, const std::map<std::string, std::string>& kv) {
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
    if (!partitioner_cb_) {
        RdKafka::Conf::ConfResult ret = conf->set("partitioner_cb", partitioner_cb_.get(), errstr);
        if (ret != RdKafka::Conf::ConfResult::CONF_OK) {
            LOG(ERROR) << "set partitioner_cb fail, errstr:" << errstr;
            return -1;
        }
        LOG(INFO) << "set partitioner_cb succ.";
    }
    if (!partitioner_key_cb_) {
        RdKafka::Conf::ConfResult ret = conf->set("partitioner_key_cb", partitioner_key_cb_.get(), errstr);
        if (ret != RdKafka::Conf::ConfResult::CONF_OK) {
            LOG(ERROR) << "set partitioner_key_cb fail, errstr:" << errstr;
            return -1;
        }
        LOG(INFO) << "set partitioner_key_cb succ.";
    }

    auto topic = std::shared_ptr<RdKafka::Topic>(RdKafka::Topic::create(producer_.get(), topic_name, conf.get(), errstr));
    if (!topic) {
        LOG(ERROR) << "Topic create fail, errstr:" << errstr;
        return -2;
    }
    TconfTopic& tt = tconf_topics_[topic_name];
    tt.c = conf;
    tt.t = topic;

    return 0;
}

int KafkaClientProducer::Produce(const std::string& topic_name, const std::string& key, const std::string& message, void* msg_opaque) {
    auto it = tconf_topics_.find(topic_name);
    RdKafka::ErrorCode err = RdKafka::ERR_NO_ERROR;
    while (true) {
        if (it != tconf_topics_.end()) {
            err = producer_->produce(it->second.t.get(),
                            RdKafka::Topic::PARTITION_UA,
                            RdKafka::Producer::RK_MSG_COPY,
                            const_cast<char*>(message.c_str()), message.size(), &key, msg_opaque);
        } else {
            err = producer_->produce(topic_name,
                            RdKafka::Topic::PARTITION_UA,
                            RdKafka::Producer::RK_MSG_COPY,
                            const_cast<char*>(message.c_str()), message.size(),
                            key.c_str(), key.size(), 0, msg_opaque);
        }
        if (err == RdKafka::ERR_NO_ERROR) {
            break;
        }
        if (err != RdKafka::ERR__QUEUE_FULL) {
            LOG(ERROR) << "produce fail, err:" << err << " str:" << RdKafka::err2str(err);
            break;
        }
        // LOG(WARNING) << "produce retry , err:" << err << " msg:" << RdKafka::err2str(err);
        producer_->poll(1000/*block for max 1000ms*/);
    }
    producer_->poll(0);
    return err;
}

}   // namespace kafka_client_producer