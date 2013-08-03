#include "stdafx.h"
#include "LuaScript.h"

#include <string>
#include <stdio.h>
#include <fstream>
#include <time.h>

LuaScript::LuaScript(void)
     : hasScriptBeenRun(false), logFile(NULL)
{
}
LuaScript::LuaScript(FILE* errorLog)
     : hasScriptBeenRun(false), logFile(errorLog)
{
}

LuaScript::~LuaScript(void)
{
}


int LuaScript::report (lua_State *L, int status) {
    const char *msg;
    if (status) 
    {
        msg = lua_tostring(L, -1);                          //get the error message pushed on
        if (msg == NULL) 
            msg = "(error with no message)"; 
        fprintf(stderr, "status=%d, %s\n", status, msg);
        fprintf(logFile, "status=%d, %s\n", status, msg);
        lua_pop(L, 1);                                      //get rid of error message from stack
    }
    return status;
}


/// <summary>
/// Initializes lua_State object, loads libraries.
/// </summary>
/// <param name="filename">The filename.</param>
/// <returns>0 if successful, other values for errors</returns>
void LuaScript::loadLua()
{
    L = luaL_newstate(); //use this for lua (version > 5.1) to create lua_State object
    luaL_openlibs(L);    //open standard lua libraries
}

/// <summary>
/// Runs the specified script
/// </summary>
/// <param name="filename">The script's filename</param>
/// <returns>0 if successful, other if there was error. Output and log file will report errors</returns>
int LuaScript::runScript(const char* filename)
{
    int err = 0;

    //load the script file
    std::cerr << "-- Loading file: " << filename << std::endl;
    err = luaL_loadfile(L, filename);
    report(L, err);

    if ( err == 0 ) 
    {
        // execute Lua script (to load functions)
        err = lua_pcall(L, 0, LUA_MULTRET, 0);
        report(L, err);
    }

    if(err == 0)
        hasScriptBeenRun = true;

    return err;
}


/// <summary>
/// Calls the specified function in the lua script if the function takes no parameters and has a return type of void.
/// </summary>
/// <param name="name">The name of the function to call.</param>
void LuaScript::callVoidFunction(const char* name)
{
    if(hasGlobalFunction(name))
    {
        lua_getglobal(L, name); //put function onto stack
        lua_call(L, 0, 0);      //call function
    }
    else
    {
        fprintf(stderr, "function \"%s\" not found\n", name);
        fprintf(logFile, "function \"%s\" not found\n", name);
    }
}

/// <summary>
/// Calls the specified function in the lua script if the function takes no parameters and has a return type of int.
/// </summary>
/// <param name="name">The name of the function to call.</param>
/// <returns>int result of function call</returns>
int LuaScript::callVoidFunctionInt(const char* name)
{
    if(hasGlobalFunction(name))
    {
        lua_getglobal(L, name); //put function onto stack
        lua_call(L, 0, 1);      //call function
    }
    else
    {
        fprintf(stderr, "function \"%s\" not found\n", name);
        fprintf(logFile, "function \"%s\" not found\n", name);
    }

    return getResultInt();
}

void LuaScript::registerFunction(const char* name, lua_CFunction f)
{
    lua_register(L, name, f);
}


/// <summary>
/// Opens the library.
/// </summary>
/// <param name="openf">A lua_CFunction that creates the library function list and calls luaL_newlib to load that list.</param>
void LuaScript::openLibrary(const char* libName, lua_CFunction openf)
{
    luaL_requiref(L, libName, openf, 1);
}

/// <summary>
/// Closes the lua script.
/// </summary>
void LuaScript::close()
{
    lua_close(L);
    hasScriptBeenRun = false;
}


/// <summary>
/// Determines whether the lua script has been run.
/// </summary>
/// <returns>true if the script has been run, false otherwise.</returns>
bool LuaScript::isScriptRun()
{
    return hasScriptBeenRun;
}

/// <summary>
/// Determines whether the function exists in global namespace.
/// </summary>
/// <param name="name">The name of the function</param>
/// <returns>Whether the function exists</returns>
bool LuaScript::hasGlobalFunction(const char* name)
{
    lua_getglobal(L, name); //put function onto stack
    bool result = lua_isfunction(L,lua_gettop(L));
    lua_pop(L, 1);
    return result;
}


/// <summary>
/// Gets the last function's return value
/// </summary>
/// <returns>int return value</returns>
int LuaScript::getResultInt()
{
    if (!lua_isnumber(L, -1))
        return 0;
    int result = lua_tointeger(L, -1);

    lua_pop(L, 1);  /* pop returned value */
    return result;
}

/// <summary>
/// Gets the last function's return value
/// </summary>
/// <returns>int return value</returns>
double LuaScript::getResultDouble()
{
    if (!lua_isnumber(L, -1))
        return 0.0;
    double result = lua_tonumber(L, -1);
    
    lua_pop(L, 1);  /* pop returned value */
    return result;
}

/// <summary>
/// Gets the last function's return value
/// </summary>
/// <returns>int return value</returns>
std::string LuaScript::getResultString()
{
    if (!lua_isstring(L, -1))
        return "";
    std::string result = lua_tostring(L, -1);

    lua_pop(L, 1);  /* pop returned value */
    return result;
}

/// <summary>
/// Gets the last function's return value
/// </summary>
/// <returns>int return value</returns>
bool LuaScript::getResultBool()
{
    if (!lua_isboolean(L, -1))
        return false;
    bool result = lua_toboolean(L, -1);

    lua_pop(L, 1);  /* pop returned value */
    return result;
}