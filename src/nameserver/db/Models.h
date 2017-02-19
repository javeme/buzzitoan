#pragma once
#include "brpc/ORM.h"


namespace buzzitoan{
namespace nameserver{
namespace db{

using namespace blib;
using namespace brpc;

class NameModel : public brpc::Model
{
public:
	DECLARE_DCLASS(NameModel);
public:
	TABLE("names");
	ID(int, "_id");
	COLUME8(uuid, varchar<32>, "uuid", true, true, true, "0", true);
	COLUME_N(type, varchar<64>, "type");
	COLUME_N(description, varchar<256>, "description");

	//FUNCTION(hosts, ArrayList<varchar<256>>, "select host from nodes where nodes.master=_id");
};

class NodeModel : public brpc::Model
{
public:
	DECLARE_DCLASS(NodeModel);
public:
	TABLE("nodes");
	ID(int, "_id");
	COLUME8(uuid, varchar<32>, "uuid", true, true, true, "0", true);
	COLUME_N(name, varchar<64>, "name");
	COLUME_N(host, varchar<256>, "host");
	//COLUME_N(type, int, "type");
	COLUME_F(type, int, "type", "names(_id)");
};

}}}
