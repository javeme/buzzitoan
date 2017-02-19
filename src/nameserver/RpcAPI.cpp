#pragma once
#include "blib/ErrorHandler.h"
#include "blib/Log.h"

#include "src/common/Utils.h"
#include "src/nameserver/RpcAPI.h"
#include "src/nameserver/Service.h"


namespace buzzitoan{
namespace nameserver{

RpcApi::CallbackRpcService::CallbackRpcService(cstring name )
	: brpc::RpcService(name)
{
	as(CallbackRpcService::onNameChanged);
	as(CallbackRpcService::onNodeChanged);
}

void RpcApi::CallbackRpcService::onNameChanged(cstring event,
	cstring name, cstring action)
{
	Log* log = Log::getLogger();
	log->info(String::format("onNameChanged(): %s %s", name, action));
	;
}

void RpcApi::CallbackRpcService::onNodeChanged(cstring event,
	cstring node, cstring action)
{
	Log* log = Log::getLogger();
	log->info(String::format("onNodeChanged(): %s %s", node, action));
	;
}


RpcApi::RpcApi(const String& url, const String& user,
	const String& password, int timeout)
	: checker("", ""),
	  dispatcher("callback"),
	  client(url, &dispatcher, &checker, "text/json", timeout),
	  brpc::RpcApi(SERVICE_NAME)
{
	this->init(&client, buzzitoan::Utils::getLoginArgs(url, user, password));

	this->subscribe("nameserver.nameChanged", "callback.onNameChanged");
	this->subscribe("nameserver.nodeChanged", "callback.onNodeChanged");
}

RpcApi::~RpcApi()
{
	try {
		logout();
	} catch(Exception& e) {
		blib::ErrorHandler::handle(e);
	}
}

ArrayList<String> RpcApi::select(const String& name, int count)
{
	brpc::ObjectList args;
	args.addValue(name);
	args.addValue(count);
	return call<ArrayList<String>>("select", args);
}

String RpcApi::select(const String& name)
{
	ArrayList<String> list = this->select(name, 1);
	if (list.size() == 0)
		throw NotFoundException("There is no any node from " + name);
	return list[0];
}

}}
