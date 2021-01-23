#include "CFunctionalTask.h"

CFunctionalTask::CFunctionalTask(CallableTask func)
	: m_taskFunction(std::move(func))
{
}

void CFunctionalTask::Execute()
{
	m_taskFunction();
}