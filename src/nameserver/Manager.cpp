#pragma once
#include "src/nameserver/Manager.h"
#include "src/nameserver/Service.h"


namespace buzzitoan{
namespace nameserver{

Manager::Manager(const String& name, const Config& config)
	: RpcService(name),
	  m_dbAPI(config[SERVICE_NAME]["db"])
{
	regObject(name, this);
	as(Manager::select);
}

Manager::~Manager()
{
}

void Manager::notifyNameChanged(const String& name, const String& action)
{
	brpc::ObjectList args;
	args.addValue(name);
	args.addValue(action);
	this->publishEvent("nameChanged", args);
}

void Manager::notifyNodeChanged(const String& node, const String& action)
{
	brpc::ObjectList args;
	args.addValue(node);
	args.addValue(action);
	this->publishEvent("nodeChanged", args);
}

ArrayList<String> Manager::select(const String& name, int count)
{
	printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>select node from name: %s\n", name.c_str());
	// select `count` node from name `name`
	ArrayList<String> list;
	auto nodes = m_dbAPI.findNodesByTypeName(name, count);
	for (unsigned int i = 0; i < nodes.size(); i++)
	{
		list.add(nodes[i]->host.value());
	}
	return list;
}

}}
