#pragma once
#include <string>
#include <fstream>

class LogFileWriter
{
public:
	LogFileWriter(std::string path)
	{
		m_outStream = std::ofstream(path);
	}

	~LogFileWriter()
	{
		m_outStream.flush();
	}

	void Write(const std::string& str)
	{
		m_outStream << str;
	}

private:
	std::ofstream m_outStream;
};