#pragma once
#include "blib/Object.h"
#include "blib/Converter.h"


namespace buzzitoan{
namespace controller{

using namespace blib;

/////////////////////////////////////////////////////////////////////
// class ControllerRequest
class ControllerRequest : public Object
{
public:
	DECLARE_DCLASS(ControllerRequest)

	FIELD(String, reqeustId);
public:
	ControllerRequest(const String& reqeustId="") : reqeustId(reqeustId) {}
	virtual ~ControllerRequest() {}
};

/////////////////////////////////////////////////////////////////////
// class ControllerResponse
class ControllerResponse : public Object
{
public:
	DECLARE_DCLASS(ControllerResponse)

	FIELD(String, reqeustId);
public:
	ControllerResponse(const String& reqeustId="") : reqeustId(reqeustId) {}
	virtual ~ControllerResponse() {}
};


/////////////////////////////////////////////////////////////////////
// class JobRequest
class JobRequest : public ControllerRequest
{
public:
	DECLARE_DCLASS(JobRequest)

	FIELD(String, id);
	FIELD(String, type);
	FIELD(String, name);
	FIELD(String, description);
	FIELD(std::vector<String>, args);

	FIELD(String, prepare);
	FIELD(String, map);
	FIELD(String, reduce);

	FIELD(ControllerResponse*, testObj);
public:
	JobRequest() : testObj(new ControllerResponse()) {}
	virtual ~JobRequest() { delete testObj; }
private:
	JobRequest(const JobRequest&) { throw Exception("Not implemented"); }
	JobRequest& operator=(const JobRequest&) { throw Exception("Not implemented"); }
};

}}
