#include "stdafx.h"
#include "DiceRollerLuaScript.h"

//names of functions in the lua script that will be called
const std::string DiceRollerLuaScript::funcName_PerformRoll = "performRoll";
const std::string DiceRollerLuaScript::funcName_SetAdminName = "setAdminName";
const std::string DiceRollerLuaScript::funcName_IsAdmin = "isAdmin";


DiceRollerLuaScript::DiceRollerLuaScript(void)
{
}

DiceRollerLuaScript::DiceRollerLuaScript(const char* filename)
    :LuaScript(filename)
{
}
DiceRollerLuaScript::DiceRollerLuaScript(FILE* errorLog)
    :LuaScript(errorLog)
{
}
DiceRollerLuaScript::DiceRollerLuaScript(const char* filename, FILE* errorLog)
    :LuaScript(filename, errorLog)
{
}


DiceRollerLuaScript::~DiceRollerLuaScript(void)
{
}

void DiceRollerLuaScript::dispose()
{
    close();
}

std::string DiceRollerLuaScript::performRoll(std::string sender, std::string command)
{
    if((int)command.find("//") ==0 && command.length() >= 3)		//starts with //, has more than just //
    {
        const char* name = funcName_PerformRoll.c_str();

        if(hasGlobalFunction(name) )
        {
            lua_getglobal(L, name); //put function onto stack
            lua_pushstring(L, sender.c_str());  //first arg
            lua_pushstring(L, command.c_str()); //second arg
            lua_call(L, 2, 1);      //call function, 2 args, 1 return

            return getResultString();
        }
        else
        {
            fprintf(stderr, "function \"%s\" not found\n", name);
            fprintf(logFile, "function \"%s\" not found\n", name);
        }
    }

    return "";
}

void DiceRollerLuaScript::setAdminName(std::string adminName)
{
    const char* name = funcName_SetAdminName.c_str();

    if(hasGlobalFunction(name) )
    {
        lua_getglobal(L, name); //put function onto stack
        lua_pushstring(L, adminName.c_str());
        lua_call(L, 1, 0);      //call function
    }
    else
    {
        fprintf(stderr, "function \"%s\" not found\n", name);
        fprintf(logFile, "function \"%s\" not found\n", name);
    }
}

bool DiceRollerLuaScript::isAdmin(std::string checkName)
{
    const char* name = funcName_IsAdmin.c_str();

    if(hasGlobalFunction(name) )
    {
        lua_getglobal(L, name); //put function onto stack
        lua_pushstring(L, checkName.c_str());
        lua_call(L, 1, 1);      //call function

        return getResultBool();
    }
    else
    {
        fprintf(stderr, "function \"%s\" not found\n", name);
        fprintf(logFile, "function \"%s\" not found\n", name);
    }

    
}