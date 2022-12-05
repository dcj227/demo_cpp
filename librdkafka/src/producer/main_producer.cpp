#include <stdlib.h>
#include <stdint.h>
#include <iostream>
#include "kafka_producer.h"

using namespace kafka_client_producer;

int main() {
    KafkaClientProducer producer;
    std::map<std::string, std::string> conf;
    producer.Initialize("192.168.50.2:9092", conf);
    producer.Produce("topic_test", "key", "message", nullptr);
    return 0;
}
