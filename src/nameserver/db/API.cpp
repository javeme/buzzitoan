#include "src/nameserver/db/API.h"


namespace buzzitoan{
namespace nameserver{
namespace db{

API::API(cstring url) : m_db(url), m_session(&m_db)
{
}

API::NodeList API::findNodesByTypeName(const String& type, int max)
{
	// TODO: implement db join
	QueryResult rs = m_session.query<db::NameModel>()
		.filter(db::NameModel().type.query() == type)
		.all();
	if (rs->size() == 0)
		throw NotFoundException(type);
	db::NameModel* name = dynamic_cast<db::NameModel*>(rs->getFirst());
	return this->findNodesByTypeId(name->_id, max);
}

API::NodeList API::findNodesByTypeId(int type, int max)
{
	QueryResult rs = m_session.query<db::NodeModel>()
		.filter(db::NodeModel().type.query() == type)
		.limit(max)
		.all();

	NodeList nodes;
	for (unsigned int i = 0; i < rs->size(); i++)
	{
		db::Model* item = rs->get(i);
		nodes.add(dynamic_cast<db::NodeModel*>(item));
	}

	// NOTE: clear QueryResult so that these nodes don't be auto deleted.
	rs->clear();

	return nodes;
}

template <typename Table>
void createAndCheck(brpc::Session& session)
{
	bool success = session.createTable<Table>();
	if (!success) {
		blib::String err = "Failed to create table: ";
		err += Table::tableName();
		throw brpc::SQLException(err);
	}
}

template <typename Table>
void dropAndCheck(brpc::Session& session)
{
	bool success = session.dropTable<Table>();
	if (!success) {
		blib::String err = "Failed to drop table: ";
		err += Table::tableName();
		throw brpc::SQLException(err);
	}
}

void API::initDB(int version)
{
	createAndCheck<db::NameModel>(m_session);
	createAndCheck<db::NodeModel>(m_session);
}

void API::destroyDB(int version)
{
	dropAndCheck<db::NameModel>(m_session);
	dropAndCheck<db::NodeModel>(m_session);
}

}}}
