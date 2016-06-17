#pragma once

#include <boost/noncopyable.hpp>

#include <sys/epoll.h>

class Cepoll: boost::noncopyable
{
	public:
		int create();
		int ctl(int op, int fd, int events, void* p);
		int wait(epoll_event* events, int maxevents, int timeout);
		~Cepoll();
	private:
		int m_fd = -1;
};
