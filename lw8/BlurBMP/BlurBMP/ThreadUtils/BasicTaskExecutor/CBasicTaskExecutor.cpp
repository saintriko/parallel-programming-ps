#include "CBasicTaskExecutor.h"
#include <thread>
#include <Windows.h>

struct CBasicTaskExecutor::SPoolImpl {
	std::vector<std::shared_ptr<ITask>> tasks;
};

static DWORD WINAPI ThreadProc(CONST LPVOID lpParam)
{
	ITask* task = (ITask*)lpParam;
	task->Execute();
	ExitThread(0);
}


CBasicTaskExecutor::CBasicTaskExecutor(std::vector<std::shared_ptr<ITask>> tasks)
{
	m_impl = new SPoolImpl();
	m_impl->tasks = std::move(tasks);
}

CBasicTaskExecutor::~CBasicTaskExecutor()
{
	delete m_impl;
}

void CBasicTaskExecutor::Execute()
{
	//std::vector<std::thread> threads;

	//for (size_t i = 0; i < m_impl->tasks.size(); i++)
	//{
	//	threads.emplace_back(std::thread([&](std::shared_ptr<ITask> task) {
	//		task->Execute();
	//		}, m_impl->tasks[i]));
	//}

	//for (auto& thread : threads)
	//{
	//	thread.join();
	//}

	std::vector<HANDLE> handles(m_impl->tasks.size());

	for (size_t i = 0; i < m_impl->tasks.size(); i++)
	{
		handles[i] = CreateThread(NULL, 0, &ThreadProc, m_impl->tasks[i].get(), CREATE_SUSPENDED, NULL);
	}

	for (const auto& handle : handles)
	{
		ResumeThread(handle);
	}

	WaitForMultipleObjects(static_cast<DWORD>(handles.size()), handles.data(), true, INFINITE);
}
