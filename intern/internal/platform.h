

#if defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)
#define SYSTEM_WINDOWS
#endif

#if defined(__POSIX__) || defined(__linux__)
#define SYSTEM_POSIX
#endif