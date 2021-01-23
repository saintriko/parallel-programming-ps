#pragma once
#include <vector>
#include <memory>
#include "../Task/ITask.h"

class CBasicTaskExecutor
{
public:
	CBasicTaskExecutor(std::vector<std::shared_ptr<ITask>> tasks);

	~CBasicTaskExecutor();

	void Execute();
private:
	struct SPoolImpl;

	SPoolImpl* m_impl;
};