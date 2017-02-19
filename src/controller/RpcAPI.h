#pragma once
#include "brpc/Client.h"

#include "src/controller/Messages.h"


namespace buzzitoan{
namespace controller{

class RpcApi : public brpc::RpcApi
{
protected:
	class CallbackRpcService : public brpc::RpcService
	{
	public:
		CallbackRpcService(cstring name);
		void onNotify(cstring event, int status);
	};
protected:
	CallbackRpcService dispatcher;
	brpc::DefaultAuthChecker checker;
	brpc::RpcClient client;
public:
	RpcApi(const String& url, const String& user="",
		const String& password="", int timeout=1000*10);
	virtual ~RpcApi();
public:
	int runJob(JobRequest* job);
};

}}
