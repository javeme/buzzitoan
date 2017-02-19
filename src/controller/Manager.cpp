#pragma once
#include "src/controller/Manager.h"
#include "src/controller/Service.h"


namespace buzzitoan{
namespace controller{

Manager::Manager(const String& name, const Config& config)
	: RpcService(name),
	  m_nameserver(config[buzzitoan::controller::SERVICE_NAME]["nameserver"])
{
	regObject(name, this);
	as(Manager::runJob);
}

Manager::~Manager()
{
}

int Manager::runJob(JobRequest* job)
{
	checkNullPtr(job);
	/* schedule the job */

	// get a best compute node
	String computer = m_nameserver.select("computer");

	// send job to the compute node
	//m_computers.getComputer(computer)->runJob(job);

	printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>> scheduled job %s to server %s\n",
		((String)job->name).c_str(), computer.c_str());
	return 0;
}

}}
