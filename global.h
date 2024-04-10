
#pragma once

#include <vector>
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

extern void initLogger(string taskPath, string objectName);
extern void doLog(const char* log, bool writeToFile = true);
extern void deinitLog();
extern void getNowDt(string &str);
extern int parseStringDt(string str, tm &dt);
extern string ReplaceAll(string str, const string& from, const string& to);
