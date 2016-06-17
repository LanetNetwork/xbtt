#pragma once

#include "client.h"

class Cconnection : public Cclient, boost::noncopyable
{
public:
	int run();
	void read(const std::string&);
	int recv();
	int send();
	virtual void process_events(int);
	Cconnection(const Csocket&, const sockaddr_in&);
private:
	sockaddr_in m_a;
	time_t m_ctime;
	int m_state;
	std::array<char, 4 << 10> m_read_b;
	shared_data m_write_b;
	str_ref m_r;
	mutable_str_ref m_w;
};
