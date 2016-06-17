#pragma once

#define NOMINMAX

#include <array>
#include <boost/format.hpp>
#include <boost/ptr_container/ptr_container.hpp>
#include <boost/utility.hpp>
#include <cassert>
#include <cerrno>
#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <fstream>
#include <iostream>
#include <list>
#include <map>
#include <set>
#include <sha1.h>
#include <socket.h>
#include <sstream>
#include <stream_int.h>
#include <string>
#include <sys/stat.h>
#include <vector>

#include "bt_misc.h"
#include "database.h"
#include "find_ptr.h"
#include "sql_query.h"
#include "to_array.h"
#include "xcc_z.h"

typedef unsigned char byte;
