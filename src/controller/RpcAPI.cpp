#pragma once
#include "blib/ErrorHandler.h"

#include "src/controller/RpcAPI.h"
#include "src/controller/Service.h"


namespace buzzitoan{
namespace controller{

RpcApi::CallbackRpcService::CallbackRpcService(cstring name)
	: brpc::RpcService(name)
{
	as(CallbackRpcService::onNotify);
}

void RpcApi::CallbackRpcService::onNotify(cstring event, int status)
{
	;
}


RpcApi::RpcApi(const String& url, const String& user,
	const String& password, int timeout)
	: checker("", ""),
	  dispatcher("callback"),
	  client(url, &dispatcher, &checker, "text/json", timeout),
	  brpc::RpcApi(SERVICE_NAME)
{
	brpc::ObjectList loginArgs;
	loginArgs.addValue(user);
	loginArgs.addValue(password);
	this->init(&client, loginArgs);

	this->subscribe("controller.notify", "callback.onNotify");
}

RpcApi::~RpcApi()
{
	try {
		logout();
	} catch(Exception& e) {
		blib::ErrorHandler::handle(e);
	}
}

int RpcApi::runJob(JobRequest* job)
{
	brpc::ObjectList args;
	args.add(brpc::objectToMap(job));
	return call<int>("runJob", args);
}

}}
