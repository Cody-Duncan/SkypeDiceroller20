#pragma once

#include <time.h>


/// <summary>
/// Gets the current date and time in YYYY-MM-DD.HH:mm:ss format
/// </summary>
/// <returns>current date and time</returns>
const std::string currentDateTime();

/// <summary>
/// Opens the specified log file.
/// </summary>
/// <param name="filename">The filename of the log file.</param>
/// <returns>a FILE* to the log file object</returns>
FILE* openLogFile(std::string filename);