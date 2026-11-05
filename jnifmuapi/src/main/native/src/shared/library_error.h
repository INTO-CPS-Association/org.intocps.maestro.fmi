#ifndef FMI_LIBRARY_ERROR_H
#define FMI_LIBRARY_ERROR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <dlfcn.h>
#endif

/* The caller owns the returned message. */
static char *createLibraryError(const char *path, const char *reason) {
    size_t size = strlen(path) + strlen(reason) + 32;
    char *message = (char *)malloc(size);
    if (message != NULL) {
        snprintf(message, size, "Could not load \"%s\": %s", path, reason);
    }
    return message;
}

static char *lastLibraryError(const char *path) {
#ifdef _WIN32
    DWORD code = GetLastError();
    WCHAR wide[1024];
    char reason[4096];
    char detail[4160];
    if (FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                       NULL, code, 0, wide, sizeof(wide) / sizeof(wide[0]), NULL) == 0) {
        strcpy(reason, "System error description unavailable");
    } else if (WideCharToMultiByte(CP_UTF8, 0, wide, -1, reason, sizeof(reason), NULL, NULL) == 0) {
        strcpy(reason, "System error description could not be converted to UTF-8");
    }
    snprintf(detail, sizeof(detail), "Windows error %lu: %s", (unsigned long)code, reason);
    return createLibraryError(path, detail);
#else
    const char *reason = dlerror();
    return createLibraryError(path, reason != NULL ? reason : "No dynamic-loader diagnostic available");
#endif
}

#endif
