#ifndef LOGGING_H
#define LOGGING_H

#include "Singleton.h"
#include <ostream>
#include <fstream>

class Logger : public Singleton<Logger>
{
public:
	Logger()  : filename_("logfile.seg"), is_file_open(false) 
	{

	}
	
	~Logger() 
	{

	}

	enum ELoggingLevel {SEG_DEBUG=1, SEG_WARNING, SEG_ERROR, SEG_UNDEFINED};
	bool isFileOpen() const { return is_file_open; }
	
	void addLogMessage(const char * message, ELoggingLevel logging_level)
	{
		
	}

private:
	const char * filename_;
	bool is_file_open;
	std::ofstream file_;

private:
	bool openLogFile()
	{
		file_.open(filename_);
		if (!file_.is_open())
			return false;
		else
		{
			is_file_open = true;
			file_ << "***File created and open for business.***\n\n";
			return true;
		}
	}

	bool closeLogFile()
	{
		file_.close();
		is_file_open = false;
	}
};

#endif
