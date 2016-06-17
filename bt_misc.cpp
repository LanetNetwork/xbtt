
#include <boost/algorithm/string.hpp>
#include <sys/stat.h>
#include <algorithm>
#include <cstdio>
#include <ctime>
#include <socket.h>

#include <syslog.h>

#include "inc/bt_misc.h"

std::string generate_random_string(int l)
{
	std::string v;
	while (l--)
		v += "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"[rand() % 62];
	return v;
}


static int hex_decode(char v)
{
	if (v >= '0' && v <= '9')
		return v - '0';
	if (v >= 'A' && v <= 'F')
		return v - 'A' + 10;
	if (v >= 'a' && v <= 'f')
		return v - 'a' + 10;
	return -1;
}


std::string hex_decode(str_ref v)
{
	std::string r;
	r.resize(v.size() >> 1);
	for (size_t i = 0; (ssize_t)(i + 2) <= (ssize_t)v.size(); i += 2)
	{
		int a = hex_decode(v[i]);
		r[i >> 1] = a << 4 | hex_decode(v[i + 1]);
	}
	return r;
}


std::string hex_encode(int l, int v)
{
	std::string r;
	r.resize(l);
	while (l--)
	{
		r[l] = "0123456789abcdef"[v & 0xf];
		v >>= 4;
	}
	return r;
}


std::string n(long long v)
{
	return std::to_string(v);
}


std::string hex_encode(data_ref v)
{
	std::string r;
	r.reserve(v.size() << 1);
	for (int i : v)
		r += hex_encode(2, i);
	return r;
}


std::string uri_decode(str_ref v)
{
	std::string r;
	r.reserve(v.size());
	for (size_t i = 0; (ssize_t)i < (ssize_t)v.size(); i++)
	{
		char c = v[i];
		switch (c)
		{
			case '%':
			{
				if ((ssize_t)(i + 2) >= (ssize_t)v.size())
					return std::string();
				int l = v[++i];
				r += hex_decode(l) << 4 | hex_decode(v[++i]);
				break;
			}
			case '+':
				r += ' ';
				break;
			default:
				r += c;
		}
	}
	return r;
}


std::string uri_encode(str_ref v)
{
	std::string r;
	r.reserve(v.size());
	for (char c : v)
	{
		if (isalpha(c & 0xff) || isdigit(c & 0xff))
			r += c;
		else
		{
			switch (c)
			{
				case ' ':
					r += '+';
					break;
				case '-':
				case ',':
				case '.':
				case '@':
				case '_':
					r += c;
					break;
				default:
					r += "%" + hex_encode(2, c);
			}
		}
	}
	return r;
}


bool is_private_ipa(int a)
{
	return (ntohl(a) & 0xff000000) == 0x0a000000
		|| (ntohl(a) & 0xff000000) == 0x7f000000
		|| (ntohl(a) & 0xfff00000) == 0xac100000
		|| (ntohl(a) & 0xffff0000) == 0xc0a80000;
}


static std::string peer_id2a(const std::string& name, const std::string& peer_id, int i)
{
	for (size_t j = i; j < peer_id.size(); j++)
	{
		if (!isalnum(peer_id[j]))
			return name + peer_id.substr(i, j - i);
	}
	return name + peer_id.substr(i);
}


std::string peer_id2a(const std::string& v)
{
	if (v.length() != 20)
		return std::string();
	if (v[7] == '-')
	{
		switch (v[0])
		{
			case '-':
				if (v[1] == 'A' && v[2] == 'Z')
					return peer_id2a("Azureus ", v, 3);
				if (v[1] == 'B' && v[2] == 'C')
					return peer_id2a("BitComet ", v, 3);
				if (v[1] == 'U' && v[2] == 'T')
					return peer_id2a("uTorrent ", v, 3);
				if (v[1] == 'T' && v[2] == 'S')
					return peer_id2a("TorrentStorm ", v, 3);
				break;
			case 'A':
				return peer_id2a("ABC ", v, 1);
			case 'M':
				return peer_id2a("Mainline ", v, 1);
			case 'S':
				return peer_id2a("Shadow ", v, 1);
			case 'T':
				return peer_id2a("BitTornado ", v, 1);
			case 'X':
				if (v[1] == 'B' && v[2] == 'T')
					return peer_id2a("XBT Client ", v, 3) + (v.find_first_not_of("0123456789ABCDEFGHIJKLMNOPQRSTUVWYXZabcdefghijklmnopqrstuvwyxz", 8) == std::string::npos ? "" : " (fake)");
				break;
		}
	}
	switch (v[0])
	{
		case '-':
			if (v[1] == 'G' && v[2] == '3')
				return "G3";
			break;
		case 'S':
			if (v[1] == 5 && v[2] == 7 && v[3] >= 0 && v[3] < 10)
				return "Shadow 57" + n(v[3]);
			break;
		case 'e':
			if (v[1] == 'x' && v[2] == 'b' && v[3] == 'c' && v[4] >= 0 && v[4] < 10 && v[5] >= 0 && v[5] < 100)
				return "BitComet " + n(v[4]) + '.' + n(v[5] / 10) + n(v[5] % 10);
	}
	return "Unknown";
}


std::string duration2a(float v)
{
	char d[32];
	if (v > 31557600)
		sprintf(d, "%.1f years", v / 31557600);
	else if (v > 2629800)
		sprintf(d, "%.1f months", v / 2629800);
	else if (v > 604800)
		sprintf(d, "%.1f weeks", v / 604800);
	else if (v > 86400)
		sprintf(d, "%.1f days", v / 86400);
	else if (v > 3600)
		sprintf(d, "%.1f hours", v / 3600);
	else if (v > 60)
		sprintf(d, "%.1f minutes", v / 60);
	else
		sprintf(d, "%.1f seconds", v);
	return d;
}


void xbt_syslog(const std::string& v)
{
	syslog(LOG_ERR, "%s", v.c_str());
}
