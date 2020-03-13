#include "Logger.h"

Logger* Logger::_instance = nullptr;
wofstream Logger::_logFile;

void Logger::log(const wstring& s)
{
	if (!_logFile.is_open())
		_logFile.open(LOG_FILE_NAME, ios::out);
		//_logFile.open(LOG_FILE_NAME, ios::out | ios::app);
	_logFile << s << WENDL;
	wcout << s << WENDL;
	_logFile.flush();
};

Logger& Logger::i()
{
	if (!_instance)
		_instance = new Logger();
	return *_instance;
};

Logger::Logger()
{};

Logger::~Logger()
{
	_logFile.close();
};