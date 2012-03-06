#include "Log.h"
#include <stdarg.h>
#include <windows.h>

FILE* Logger::m_hLogFile = NULL;
LogLevel Logger::m_LogLevel = LOG_TRACE;
std::string Logger::m_strFilePath = LOG_FILE_PATH;

const char* LogLevelStr[] = {
    "TRACE",
    "INFO",
    "WARNING",
    "ERROR",
    "CRITICAL",
    "FATAL",
};

Logger& Logger::Instance() 
{
  static Logger LoggerInstance;
  return LoggerInstance;
}

void Logger::SetLogFilePath(const std::string& strFilePath, bool bAppend)
{
	m_strFilePath = strFilePath;
	Dispose();
	Initialise(bAppend);
}

void Logger::SetLogLevel(const LogLevel Level)
{
	m_LogLevel = Level;
}


Logger::Logger()
{
	Initialise(false);
}

Logger::~Logger()
{
	Dispose();
}

void Logger::Initialise(bool bAppend)
{
	if (m_strFilePath.length() > 0) {
		m_hLogFile = bAppend ? fopen(m_strFilePath.c_str(), "a+") 
			                 : fopen(m_strFilePath.c_str(), "w+");	
	}
}

void Logger::Dispose()
{
	if (NULL != m_hLogFile) {
		fflush(m_hLogFile);
		fclose(m_hLogFile);
		m_hLogFile = NULL;
	}
}

void Logger::Log(const LogLevel Level, const char *Format, ...)
{
	if (m_LogLevel > Level) return;

	if (NULL == m_hLogFile) return;	

	char szBuffer[1024];

	va_list args;
    va_start(args, Format);
	vsprintf(szBuffer, Format, args);
	va_end(args);

	SYSTEMTIME st;		
	GetLocalTime(&st);
	if (0 > fprintf(m_hLogFile, "[%02u:%02u:%02u:%03u]\t[%s]\t%s\n", 
		st.wHour, st.wMinute, st.wSecond, st.wMilliseconds, 
		LogLevelStr[Level], szBuffer)) {
		Dispose();
	}
	else {
		fflush(m_hLogFile);
	}
}