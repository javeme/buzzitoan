#pragma once
#include "blib/ConfigOption.h"
#include "brpc/Server.h"

#include "src/controller/Messages.h"
#include "src/nameserver/RpcAPI.h"


namespace buzzitoan{
namespace controller{

class Manager : public brpc::RpcService
{
public:
	Manager(const String& name, const Config& config);
	virtual ~Manager();

public:
	int runJob(JobRequest* job);

private:
	buzzitoan::nameserver::RpcApi m_nameserver;
};

}}
