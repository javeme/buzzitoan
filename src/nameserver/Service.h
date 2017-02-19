#pragma once
#include "blib/ConfigOption.h"

#include "src/common/Service.h"
#include "src/nameserver/Manager.h"


namespace buzzitoan{
namespace nameserver{

const static cstring SERVICE_NAME = "nameserver";

class Service : public buzzitoan::Service
{
protected:
	buzzitoan::nameserver::Manager m_dispatcher;
	brpc::DefaultAuthChecker m_checker;
	String m_url;
	brpc::RpcServer m_server;
public:
	Service(const String& name, const Config& config)
		: m_dispatcher(name, config),
		  m_checker(config[SERVICE_NAME]["user"],
		            config[SERVICE_NAME]["password"]),
		  m_url(config[SERVICE_NAME]["url"]),
		  m_server(m_url, &m_checker, config[SERVICE_NAME]["contentType"])
	{
	}
public:
	virtual void start() {
		m_server.start(&m_dispatcher);
	}

	virtual void wait() {
		m_server.wait();
	}

	virtual void stop() {
		m_server.stop();
	}
};

}}
