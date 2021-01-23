#pragma once

class ITask
{
public:
	virtual ~ITask() = default;

	virtual void Execute() = 0;
};