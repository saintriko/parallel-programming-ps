#pragma once
#include "LogFileWritter.h"
#include "CLinkedList.h"
#include <windows.h>

class LogBuffer
{
public:
	LogBuffer(LogFileWriter* writter)
		: m_writer(writter)
		, m_criticaSection(CRITICAL_SECTION())
	{
		if (!InitializeCriticalSectionAndSpinCount(&m_criticaSection, 0x00000400))
		{
			throw std::exception("Faield to create critical section");
		}

		m_writeEvent = CreateEvent(nullptr, TRUE, FALSE, TEXT("WriteEvent"));

		InitLogSizeMonitoringThread();
	}

	~LogBuffer()
	{
		CloseHandle(m_logSizeMonitoringThread);
		CloseHandle(m_writeEvent);
		DeleteCriticalSection(&m_criticaSection);
	}

	void Log(std::string str)
	{
		EnterCriticalSection(&m_criticaSection);

		if (m_list.GetSize() >= 500)
		{
			ReleaseBuffer();
		}

		m_list.Append(str);

		LeaveCriticalSection(&m_criticaSection);
	}

	void ReleaseBuffer()
	{
		if (!SetEvent(m_writeEvent))
		{
			throw std::runtime_error("Failed to set event");
		}

		if (WaitForSingleObject(m_logSizeMonitoringThread, INFINITE) == WAIT_OBJECT_0) {
			ResetEvent(m_writeEvent);
			InitLogSizeMonitoringThread();
		}
		else 
		{
			throw std::runtime_error("Failed to set event");
		}
	}

	void InitLogSizeMonitoringThread()
	{
		m_logSizeMonitoringThread = CreateThread(nullptr, 0, &LogSizeMonitoringThread, static_cast<void*>(this), 0, nullptr);
	}

private:

	static DWORD WINAPI LogSizeMonitoringThread(CONST LPVOID logBufferLpVoid)
	{
		LogBuffer* logBuffer = static_cast<LogBuffer*>(logBufferLpVoid);

		if (WaitForSingleObject(logBuffer->m_writeEvent, INFINITE) == WAIT_OBJECT_0)
		{
			for (const auto& el : logBuffer->m_list)
			{
				logBuffer->m_writer->Write(el);
			}

			logBuffer->m_list.Clear();
		}

		ExitThread(EXIT_SUCCESS);
	}

	CLinkedList<std::string> m_list;

	CRITICAL_SECTION m_criticaSection;

	HANDLE m_logSizeMonitoringThread;

	HANDLE m_writeEvent;

	LogFileWriter* m_writer;
};

