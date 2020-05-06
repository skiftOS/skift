/* Copyright © 2018-2020 N. Van Bossuyt.                                      */
/* This code is licensed under the MIT License.                               */
/* See: LICENSE.md                                                            */

#include <libsystem/cstring.h>

#include "kernel/filesystem/Filesystem.h"

static Result null_FsOperationRead(FsNode *node, FsHandle *handle, void *buffer, size_t size, size_t *read)
{
    __unused(node);
    __unused(handle);
    __unused(buffer);
    __unused(size);

    *read = 0;

    return SUCCESS;
}

static Result null_FsOperationWrite(FsNode *node, FsHandle *handle, const void *buffer, size_t size, size_t *written)
{
    __unused(node);
    __unused(handle);
    __unused(buffer);

    *written = size;

    return SUCCESS;
}

void null_initialize(void)
{
    FsNode *null_device = __create(FsNode);

    fsnode_init(null_device, FILE_TYPE_DEVICE);

    FSNODE(null_device)->read = (FsOperationRead)null_FsOperationRead;
    FSNODE(null_device)->write = (FsOperationWrite)null_FsOperationWrite;

    Path *null_device_path = path_create("/dev/null");
    filesystem_link_and_take_ref(null_device_path, null_device);
    path_destroy(null_device_path);
}