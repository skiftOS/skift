#pragma once

#include "abi/Handle.h"
#include "abi/Filesystem.h"

#include <libsystem/list.h>

struct Connection;

typedef struct Socket
{
    Handle handle;
    List *connections;
} Socket;

Socket *socket_open(const char *path, OpenFlag flags);

struct Connection *socket_connect(const char *path);

struct Connection *socket_accept(Socket *socket);

void socket_close(Socket *socket);
