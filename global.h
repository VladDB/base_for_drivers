#pragma once

#include <string>

#if __GNUC__ >= 8
#include <filesystem>
namespace fs = std::filesystem;
#else
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;
#endif

using namespace std;

extern const bool notWriteToFile;
extern FILE* LOG_FILE;
extern const int LOG_SIZE;

extern void initLogger(string logPath, string objectName);
extern void doLog(const char* log, bool writeToFile = true);
extern void deinitLog();
extern void getNowDt(string &str);
extern string ReplaceAll(string str, const string& from, const string& to);
uint16_t Calculate_CRC_CCITT(const uint8_t *buffer, int len);
