#pragma once
#include "stdafx.h"

#define LOGGER_ENABLED 1
#define LOGFILE(fmt, ...) WriteLog(NULL, fmt, ##__VA_ARGS__)

BOOL WriteLog(const char *file_name, const char *fmt, ...);