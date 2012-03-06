#ifndef _LOG_H
#define _LOG_H

#include <string>

#define TEST
#define LOG_FILE_PATH "C:\\DebugInfo.log"

#ifdef TEST
	#define LogData Logger::Instance().Log
	#define SetLogFile Logger::SetLogFilePath
#else
	#define LogData
	#define SetLogFile
#endif

typedef enum tagLogLevel
{
	LOG_TRACE,
	LOG_INFO,
	LOG_WARNING,
	LOG_ERROR,
	LOG_FATAL,
	LOG_NONE = 10,
}LogLevel;

class Logger
{
public:
	static Logger& Instance();
	
	static void SetLogFilePath(const std::string& strFilePath, bool bAppend = false);
	static void SetLogLevel(const LogLevel Level);
	static void Initialise(bool bAppend = false);
	static void Dispose();
	
	void Log(const LogLevel Level, const char *Format, ...);
	
private:
	Logger();
	Logger(Logger const&);
	Logger& operator=(Logger const&);
	~Logger();

	static FILE*        m_hLogFile;
	static std::string	m_strFilePath;
	static LogLevel	    m_LogLevel;
};

#endif//_LOG_H