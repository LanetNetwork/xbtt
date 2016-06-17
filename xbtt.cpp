#include "stdafx.h"
#include "config.h"
#include "server.h"

std::string g_conf_file = "xbt_tracker.conf";
const char* g_service_name = "XBT Tracker";

static int main1()
{
	srand(static_cast<int>(time(NULL)));
	Cconfig config;
	if (config.load(g_conf_file))
		std::cerr << "Unable to read " << g_conf_file << std::endl;
	try
	{
		if (config.m_mysql_host != "-")
			srv_database().open(config.m_mysql_host, config.m_mysql_user, config.m_mysql_password, config.m_mysql_database, true);
	}
	catch (bad_query& e)
	{
		std::cerr << e.what() << std::endl;
		return 1;
	}
	if (!config.m_query_log.empty())
	{
		static std::ofstream os(config.m_query_log.c_str());
		srv_database().set_query_log(&os);
	}
	return srv_run(config.m_mysql_table_prefix, config.m_mysql_host != "-", g_conf_file);
}


int main(int argc, char* argv[])
{
	if (argc >= 2)
	{
		if (!strcmp(argv[1], "--conf_file") && argc >= 3)
			g_conf_file = argv[2];
		else
		{
			std::cerr << "  --conf_file arg (=xbt_tracker.conf)" << std::endl;
			return 1;
		}
	}
	return main1();
}
