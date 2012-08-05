// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#ifndef WINVER // Allow use of features specific to Windows XP or later.
#define WINVER 0x0501 // Change this to the appropriate value to target other versions of Windows.
#endif

#define WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Windows headers
#include <stdio.h>		//standard input/output
#include <tchar.h>		//international text mappings
#include <iostream>		//standard input/output streams
#include <string>
#include <vector>
#include <ctype.h>
#include <sstream>

//Active Template Library (ATL), a set of template-based C++ classes that simplify the 
//programming of Component Object Model (COM) objects.
#include <atlbase.h>
#include <atlstr.h>
#include <atlcom.h>

//import the Skype4COM library, rename a couple of commands
#import "Skype4COM.dll" rename("CreateEvent","CreatePluginEvent"), rename("SendMessage","SendChatMessage")




// TODO: reference additional headers your program requires here
