#pragma once

#include <libsystem/utils/List.h>

#include "kernel/node/Node.h"

struct FsSocket
{
    FsNode node;
    List *pending;
} ;

FsNode *socket_create();
