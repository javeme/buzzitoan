#include "stdafx.h"
#include <process.h>
#include "CheckMemLeaks.h"
CHECK_MEMORY_LEAKS

#include "blib/SimpleCfgFile.h"
#include "blib/Log.h"
#include "blib/ErrorHandler.h"
#include "blib/SocketTools.h"

#include "src/controller/Service.h"
#include "src/controller/RpcAPI.h"

#include "src/nameserver/Service.h"
#include "src/nameserver/RpcAPI.h"

using namespace blib;
using namespace brpc;

class LogErrorHandler : public blib::IErrorHandler
{
public:
	virtual bool handle(Exception& e)
	{
		Log* log = Log::getLogger();
		log->warn(e.toString());
		return true;
	}
};

// global conf
blib::SimpleCfgFile CONF;

void initOptions()
{
	// brpc
	brpc::ConfigGroup brpc("brpc");
	brpc.registerOption(BooleanOption("debug", true,
		"whether output debug messages of brpc"));
	CONF.registerGroup(brpc);

	// log group
	brpc::ConfigGroup log("log");
	log.registerOption(StringOption("path", "/var/log/buzzitoan", "log path"));
	log.registerOption(StringOption("level", "info", "log level"));
	log.registerOption(StringOption("format",
		"$time [Thread-$thread] [$level] $message",
		"log format"));
	CONF.registerGroup(log);

	// servername group
	brpc::ConfigGroup nameserver(buzzitoan::nameserver::SERVICE_NAME);
	nameserver.registerOption(StringOption("user", "test",
		"default user name"));
	nameserver.registerOption(StringOption("password", "123456",
		"password for the default user"));

	nameserver.registerOption(StringOption("url", "http://0.0.0.0:8089",
		"service address to bind"));
	nameserver.registerOption(StringOption("contentType", "text/json",
		"default content type for serialization"));

	nameserver.registerOption(StringOption("db",
		"mysql://127.0.0.1:3306/test" \
			"?username=root&password=0315&charset=utf8",
		"database connection info"));

	CONF.registerGroup(nameserver);

	// controller group
	brpc::ConfigGroup controller(buzzitoan::controller::SERVICE_NAME);
	controller.registerOption(StringOption("user", "test",
		"default user name"));
	controller.registerOption(StringOption("password", "123456",
		"password for the default user"));

	controller.registerOption(StringOption("url", "http://0.0.0.0:8088",
		"service address to bind"));
	controller.registerOption(StringOption("contentType", "text/json",
		"default content type for serialization"));

	controller.registerOption(StringOption("nameserver",
		"http://test:123456@127.0.0.1:8089",
		"nameserver service address and user/password"));

	CONF.registerGroup(controller);
}

void initConf(const String& confFile)
{
	CONF.parseFromFile(confFile);
}

void initLog(const String& name)
{
	// brpc debug
	bool brpcDebug = CONF["brpc"].option<bool>("debug");
	brpc::BRpcUtil::setBrpcDebug(brpcDebug);

	// init log file
	String logPath = CONF["log"]["path"];
	String logLevel = CONF["log"]["level"];
	String logFormat = CONF["log"]["format"];

	String file = String::format("%s/%s.log", logPath.c_str(), name.c_str());
	LogManager::instance().initLogger(name, file, Log::string2Level(logLevel));

	Log* log = Log::getLogger(name);
	log->updateFormatter(logFormat);
}

void run(int argc, char* argv[])
{
	// parse args
	String serviceName = argv[1];

	// make sure all of Serializers to be linked by static lib
	initSerializers();

	// nameserver
	if(serviceName == "nameserver") {
		if(argc > 2 && String(argv[2]) == "initdb") {
			String url = CONF[buzzitoan::nameserver::SERVICE_NAME]["db"];
			buzzitoan::nameserver::db::API db(url);
			db.initDB(1); // delete please use db.destroyDB()
			printf("nameserver db has been inited!\n");
			return;
		}
		buzzitoan::nameserver::Service nameserver(serviceName, CONF);
		nameserver.start();
		nameserver.wait();
	}
	// controller
	else if(serviceName == "controller") {
		buzzitoan::controller::Service controller(serviceName, CONF);
		controller.start();
		controller.wait();
	}
	else if(serviceName == "test") {
		// sum of [10, 20, 30, 40, 50, 60, 70, 80, 90]
		int numbers[] = {10, 20, 30, 40, 50, 60, 70, 80, 90};
		//int result = restAPI.sum(numbers);
		//int result = rpcAPI.sum(numbers);

		cstring url = "http://127.0.0.1:8088";
		cstring name = "test", password = "123456";
		buzzitoan::controller::RpcApi controller(url, name, password);
		buzzitoan::controller::JobRequest job;
		job.name = "test-job";
		job.id = "test-id";
		job.type = "test-type";
		job.description = "test-description";
		job.testObj->reqeustId = "test-reqeustId";
		int status = controller.runJob(&job);
		printf("job scheduled: %d\n", status);
	}
}

int main(int argc, char* argv[])
{
	_CrtSetBreakAlloc(252);

	// parse args
	if(argc < 2) {
		printf("Usage: buzzitoan <service>\n");
		return -1;
	}
	String serviceName = argv[1];

	printf("start buzzitoan %s...\n", serviceName.c_str());

	// init
	LogErrorHandler logHandler;
	ErrorHandler::setHandler(&logHandler);

	System::instance().init();
	SocketTools::initSocketContext();

	try {
		initOptions();
		initConf("/etc/buzzitoan/buzzitoan.ini");
		initLog(serviceName);
	} catch(Exception& e) {
		e.printException();
	}

	// run
	Log* log = Log::getLogger(serviceName);
	log->info("======== buzzitoan start ========");

	try {
		run(argc, argv);
	} catch(Exception& e) {
		e.printStackTrace();
		log->error("exit with exception: " + e.toString());
	}

	// destroy
	log->info("======== buzzitoan end ========");

	SocketTools::cleanUpSocketContext();
	System::instance().destroy();

	system("pause");
	return 0;
}
