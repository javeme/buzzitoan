#pragma once
#include "brpc/Client.h"

#include "src/nameserver/Messages.h"


namespace buzzitoan{
namespace nameserver{

class RpcApi : public brpc::RpcApi
{
protected:
	class CallbackRpcService : public brpc::RpcService
	{
	public:
		CallbackRpcService(cstring name);
		void onNameChanged(cstring event, cstring name, cstring action);
		void onNodeChanged(cstring event, cstring node, cstring action);
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
	// admin api for name
	ArrayList<NameResponse*> listNames();
	NameResponse* createName(NameRequest* node);
	void deleteName(const String& name);
	NameResponse* showName(const String& name);

	// admin api for node
	// TODO: support filter and pagenation
	ArrayList<NodeResponse*> listNodes();
	NodeResponse* addNode(NodeRequest* node);
	void removeNode(const String& node);
	NodeResponse* showNode(const String& node);

	// api for selecting nodes from a name
	ArrayList<String> select(const String& name, int count);
	String select(const String& name);
};

}}
