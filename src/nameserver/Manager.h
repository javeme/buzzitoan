#pragma once
#include "blib/ConfigOption.h"
#include "brpc/Server.h"

#include "src/nameserver/Messages.h"
#include "src/nameserver/db/API.h"


namespace buzzitoan{
namespace nameserver{

class Manager : public brpc::RpcService
{
public:
	Manager(const String& name, const Config& config);
	virtual ~Manager();

public:
	void notifyNameChanged(const String& name, const String& action);
	void notifyNodeChanged(const String& node, const String& action);
public:
	ArrayList<String> select(const String& name, int count);

private:
	db::API m_dbAPI;
};

}}
