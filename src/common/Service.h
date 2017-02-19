#pragma once
#include "blib/Object.h"


namespace buzzitoan{

class Service : public blib::Object
{
public:
	virtual void start() = 0;
	virtual void wait() = 0;
	virtual void stop() = 0;
};

}
