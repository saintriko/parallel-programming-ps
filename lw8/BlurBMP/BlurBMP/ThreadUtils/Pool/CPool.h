#pragma once
#include <vector>
#include <memory>
#include "../Task/Itask.h"

class CPool
{
public:
	CPool(std::vector<std::shared_ptr<ITask>> tasks, unsigned threadsCount);

	~CPool();

	void Execute();
private:
	struct SPoolImpl;

	SPoolImpl* m_impl;
};
