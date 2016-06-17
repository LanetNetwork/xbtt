#include <unistd.h>

#include "epoll.h"

Cepoll::~Cepoll()
{
	if (m_fd != -1)
		close(m_fd);
}


int Cepoll::create()
{
	return m_fd = epoll_create(1);
}


int Cepoll::ctl(int op, int fd, int events, void* p)
{
	epoll_event e;
	e.data.ptr = p;
	e.events = events;
	return epoll_ctl(m_fd, op, fd, &e);
}


int Cepoll::wait(epoll_event* events, int maxevents, int timeout)
{
	return epoll_wait(m_fd, events, maxevents, timeout);
}
