
#include <libsystem/io/File.h>
#include <libsystem/io/Filesystem.h>
#include <libsystem/io/Stream.h>
#include <libfile/ZipArchive.h>
#include <libutils/Path.h>

int main(int argc, char **argv)
{
    if (argc == 1)
    {
        stream_format(err_stream, "%s: Missing archive operand(s)\n", argv[0]);
        return PROCESS_FAILURE;
    }

    for(int i=1;i<argc;i++)
    {
      File file{argv[i]};
      if(!file.exist())
      {
        stream_format(err_stream, "%s: File does not exist", argv[i]);
      }

      ZipArchive archive(file);

      if (!archive.valid())
      {
          stream_format(err_stream, "%s: Failed to read zip archive", argv[i]);
          return PROCESS_FAILURE;
      }

      
    }

    return PROCESS_SUCCESS;
}
