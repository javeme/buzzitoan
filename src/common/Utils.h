#pragma once
#include "brpc/Client.h"


namespace buzzitoan{

class Utils : public blib::Object
{
public:
	static brpc::ObjectList getLoginArgs(const blib::String& url,
		const blib::String& user="", const blib::String& password="")
	{
		brpc::ObjectList loginArgs;

		brpc::Url urlObj(url);
		if(user.empty() && !urlObj.getUsername().empty())
			loginArgs.addValue(urlObj.getUsername());
		else
			loginArgs.addValue(user);

		if(user.empty() && !urlObj.getPassword().empty())
			loginArgs.addValue(urlObj.getPassword());
		else
			loginArgs.addValue(password);

		return std::move(loginArgs);
	}
};

}
