#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <iostream>
#include "log.h"
#include "kafka_consumer.h"

#include<sys/time.h>

using namespace kafka_client_consumer;

uint64_t GetNowUs() {
    struct timeval tv;
    gettimeofday(&tv,NULL);
    return tv.tv_sec * 1000000 + tv.tv_usec;
}

void msg_handler(std::string& topic, int32_t& partition, int64_t& offset, std::string& key, std::string& payload)
{
    LOG(INFO) << "topic:" << topic << " key:" << key << " payload:" << payload << " partition:" << partition << " offset:" << offset << std::endl;
}

int main() {
    KafkaClientConsumer consumer;
    std::map<std::string, std::string> conf;
    if (consumer.Initialize("192.168.50.2:9092", "test", conf, msg_handler) != 0) {
        LOG(INFO) << "consumer.Initialize fail" << std::endl;
        return -1;
    }
    std::map<std::string, std::string> tconf;
    if (consumer.SetTopicForSubscribe("topic_test", tconf) != 0) {
        LOG(ERROR) << "AddTopic fail, topic:" << "topic_test";
        return -2;
    }
    consumer.Start();
    // consumer.Stop();
    return 0;
}
