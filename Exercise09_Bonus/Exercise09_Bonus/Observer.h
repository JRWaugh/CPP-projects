#pragma once
class Observer
{
public:
	virtual void HandleLimitReached() = 0;
};