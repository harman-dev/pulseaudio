#ifndef LTDL_H
#define LTDL_H

#include <dlfcn.h>
#include <stdlib.h>

typedef void * lt_dlhandle;
typedef void * lt_ptr;
typedef struct { const char *name; void *address; } lt_dlsymlist;

#define lt_dlsym dlsym
#define lt_dlclose dlclose
#define lt_dlerror dlerror
#define lt_dlinit() 0
#define lt_dlexit() 0

#define LTDL_SET_PRELOADED_SYMBOLS()
#define LT_UNUSED __attribute__ ((unused))


static const lt_dlsymlist *lt_preloaded_symbols;

static char *
lt_dlgetsearchpath(void)
{
    return getenv("LD_LIBRARY_PATH");
}

static int
lt_dlsetsearchpath(char*ld_library_path)
{
    return setenv("LD_LIBRARY_PATH", ld_library_path, 1);
}

static lt_dlhandle lt_dlopenext(char *name)
{
    char buf[256];
	lt_dlhandle handle;
	
    sprintf(buf, PA_DLSEARCHPATH"/%s.so", name);
    handle = dlopen(buf, RTLD_NOW);
	if(!handle){
		fprintf(stderr, "dlopen(%s) faild : %s\n",name, dlerror());
	}
	return handle;
}

static int lt_dlforeachfile (LT_UNUSED const char *search_path, LT_UNUSED int (*func) (const char *filename, void * data), LT_UNUSED void * data)
{
    return 0;
}

#endif /* LTDL_H */
