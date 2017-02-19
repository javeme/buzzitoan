#pragma once
#include "src/nameserver/db/Models.h"


namespace buzzitoan{
namespace nameserver{
namespace db{

class API : public blib::Object
{
public:
	API(cstring url);
public:
	typedef ArrayList<SmartPtr<db::NodeModel>> NodeList;

	virtual NodeList findNodesByTypeName(const String& type, int max);
	virtual NodeList findNodesByTypeId(int type, int max);

public:
	virtual void initDB(int version);
	virtual void destroyDB(int version);
	//virtual void upgrade(int version);

private:
	brpc::DatabaseConnection m_db;
	brpc::Session m_session;
};

}}}
