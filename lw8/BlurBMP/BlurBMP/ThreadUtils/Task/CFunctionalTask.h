#pragma once
#include "ITask.h"
#include <functional>

class CFunctionalTask : public ITask
{
public:
	using CallableTask = std::function<void()>;

	CFunctionalTask(CallableTask func);

	void Execute() override;
	
private:
	CallableTask m_taskFunction;
};
