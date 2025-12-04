#ifndef COM_GITHUB_JEFFMANZIONE_INTERN_INTERNAL_PLATFORM_H_
#define COM_GITHUB_JEFFMANZIONE_INTERN_INTERNAL_PLATFORM_H_

#if defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)
#define SYSTEM_WINDOWS
#endif

#if defined(__POSIX__) || defined(__linux__) || defined(__unix__) || \
    (defined(__APPLE__) && defined(__MACH__))
#define SYSTEM_POSIX
#endif

#endif /* COM_GITHUB_JEFFMANZIONE_INTERN_INTERNAL_PLATFORM_H_ */