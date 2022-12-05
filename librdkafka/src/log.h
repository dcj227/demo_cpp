#pragma once

#include <time.h>
#include <string.h>
#include <cstdio>
#include <sstream>
#include <string>

// windows:
// #define FILENAME(x) strrchr(x,'\\')?strrchr(x,'\\')+1:x
// linux :
#define FILENAME(x) strrchr(x,'/')?strrchr(x,'/')+1:x

enum __LOG_LEVEL__ {
    DEBUG = 0,
    INFO = 1,
    WARN = 2,
    ERROR = 3
};
const static char* __LOG_LEVEL_STR__[] = {"DEBUG","INFO","WARN","ERROR"};

#define LOG(level) \
    logging::LogMessage(__FILE__, __LINE__, __FUNCTION__, level).stream()

namespace logging {

class LogMessage{
public:
    LogMessage(const char* file, int line, const char* function, __LOG_LEVEL__ level) {
        time_t now = time(0);
        struct tm *info = localtime(&now);
        char buffer[32] = {0};
        strftime(buffer, 80, "%Y-%m-%d %H:%M:%S", info);
        const char* filename = FILENAME(file);
        stream_ << buffer << " [" << __LOG_LEVEL_STR__[level] << "] " << filename << ":" << line << ":" << function << " ";
    }
    ~LogMessage() {
        stream_ << std::endl;
        fprintf(stderr, "%s", stream_.str().c_str());
        fflush(stderr);
    }

    std::ostream& stream() { return stream_;}

private:
    std::ostringstream stream_;
};

}