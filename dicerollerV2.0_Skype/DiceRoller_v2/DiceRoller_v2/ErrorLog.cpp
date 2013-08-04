#include "stdafx.h"
#include "ErrorLog.h"

const std::string currentDateTime()
{
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    localtime_s(&tstruct, &now);
    strftime(buf, sizeof(buf), "%Y-%B-%d.%X", &tstruct);

    return buf;
}


FILE* openLogFile(std::string filename)
{
    FILE* logFile = fopen("errorLog.txt", "a");
    fprintf(logFile, "Starting Log: %s\n", currentDateTime().c_str());
    return logFile;
}