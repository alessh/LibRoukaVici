#include <dlfcn.h>
#include <unistd.h>

int testVibrate(void* handle, void* lib)
{
  int (*vibrate)(void*, char, char);
  *(void**)(&vibrate) = dlsym(lib, "Vibrate");
  if (vibrate(handle, 0, 255)) goto error;
  if (vibrate(handle, 0, 0)) goto error;
  return 0;
error:
  return 1;
}

int main(int argc, const char** argv)
{
  int ret = 0;
  void* (*initrvici)();
  void (*stoprvici)(void*);

  #if __linux
    void* lib = dlopen("libroukavici.so", RTLD_LAZY);
  #elif __APPLE__
    void* lib = dlopen("libroukavici.dylib", RTLD_LAZY);
  #endif

  if (!lib)
    {
      return 1;
    }
  *(void**)(&initrvici) = dlsym(lib, "InitRVici");
  *(void**)(&stoprvici) = dlsym(lib, "StopRVici");

  // Init the lib
  void* handle = initrvici();
  if (testVibrate(handle, lib)){
    ret = 1;
  }
  stoprvici(handle);
  return ret;
}
