#pragma once
#include "blib/Object.h"
#include "blib/Converter.h"


namespace buzzitoan{
namespace nameserver{

using namespace blib;

#define DISABLE_CLONE(Cls) \
    Cls(const Cls&) { throw Exception("Not implemented"); }\
    Cls& operator=(const Cls&) { throw Exception("Not implemented"); }


/////////////////////////////////////////////////////////////////////
// class NameserverRequest
class NameserverRequest : public Object
{
public:
	DECLARE_DCLASS(NameserverRequest)

	FIELD(String, reqeustId);
public:
	NameserverRequest(const String& reqeustId="") : reqeustId(reqeustId) {}
	virtual ~NameserverRequest() {}
};

/////////////////////////////////////////////////////////////////////
// class NameserverResponse
class NameserverResponse : public Object
{
public:
	DECLARE_DCLASS(NameserverResponse)

	FIELD(String, reqeustId);
public:
	NameserverResponse(const String& reqeustId="") : reqeustId(reqeustId) {}
	virtual ~NameserverResponse() {}
};


/////////////////////////////////////////////////////////////////////
// class NameRequest
class NameRequest : public NameserverRequest
{
public:
	DECLARE_DCLASS(NameRequest)

	FIELD(String, id);
	FIELD(String, type);
	FIELD(ArrayList<String>, hosts);
	FIELD(String, description);
public:
	NameRequest() {}
	virtual ~NameRequest() {}
private:
	DISABLE_CLONE(NameRequest);
};


/////////////////////////////////////////////////////////////////////
// class NameResponse
class NameResponse : public NameserverResponse
{
public:
	DECLARE_DCLASS(NameResponse)

	FIELD(String, id);
	FIELD(String, type);
	FIELD(ArrayList<String>, hosts);
	FIELD(String, description);
public:
	NameResponse() {}
	virtual ~NameResponse() {}
private:
	DISABLE_CLONE(NameResponse);
};


/////////////////////////////////////////////////////////////////////
// class NodeRequest
class NodeRequest : public NameserverRequest
{
public:
	DECLARE_DCLASS(NodeRequest)

	FIELD(String, id);
	FIELD(String, type);
	FIELD(String, host);
	FIELD(String, description);
	FIELD(String, status);
public:
	NodeRequest() {}
	virtual ~NodeRequest() {}
private:
	DISABLE_CLONE(NodeRequest);
};


/////////////////////////////////////////////////////////////////////
// class NodeResponse
class NodeResponse : public NameserverResponse
{
public:
	DECLARE_DCLASS(NodeResponse)

	FIELD(String, id);
	FIELD(String, type);
	FIELD(String, host);
	FIELD(String, description);
	FIELD(String, status);
public:
	NodeResponse() {}
	virtual ~NodeResponse() {}
private:
	DISABLE_CLONE(NodeResponse);
};

}}
