#include "stdafx.h"
#include "DiceRollerLuaScript.h"

//names of functions in the lua script that will be called
const std::string DiceRollerLuaScript::funcName_PerformCommand = "performCommand";
const std::string DiceRollerLuaScript::funcName_SetAdminName = "setAdminName";
const std::string DiceRollerLuaScript::funcName_IsAdmin = "isAdmin";
const std::string DiceRollerLuaScript::commandName_load = "load";


DiceRollerLuaScript::DiceRollerLuaScript(void)
{
}

DiceRollerLuaScript::DiceRollerLuaScript(FILE* errorLog)
    :LuaScript(errorLog)
{
}

DiceRollerLuaScript::~DiceRollerLuaScript(void)
{
}

void DiceRollerLuaScript::dispose()
{
    closeLogFile();
    close();
}

std::string DiceRollerLuaScript::performCommand(std::string senderID, std::string senderDisplayName, std::string command)
{
    if((int)command.find("//") ==0 && command.length() >= 3)		//starts with //, has more than just //
    {
        const char* performCommand = funcName_PerformCommand.c_str();

        //check if the command is "//load scriptFilename"
        if(command.compare(2, commandName_load.size(), commandName_load) == 0)
        {
            std::string scriptFilename = command.substr(command.find(' ')+1);
            runScript(scriptFilename.c_str());
        }
        //check if performCommand() exists in lua script
        else if(hasGlobalFunction(performCommand) ) 
        {
            lua_getglobal(L, performCommand);                //put function onto stack
            lua_pushstring(L, senderID.c_str());             //first arg,   sender Handle (skype name)
            lua_pushstring(L, senderDisplayName.c_str());    //second arg,  sender Name   (skype fullname/display name)
            lua_pushstring(L, command.c_str());              //third arg,   chat message / command
            lua_call(L, 3, 1);                               //call function, 3 args, 1 return

            return getResultString();
        }
        else
        {
            fprintf(stderr, "function \"%s\" not found\n", performCommand);
            fprintf(logFile, "function \"%s\" not found\n", performCommand);
        }
    }

    return "";
}

void DiceRollerLuaScript::setAdminName(std::string adminID)
{
    const char* name = funcName_SetAdminName.c_str();

    if(hasGlobalFunction(name) )
    {
        lua_getglobal(L, name); //put function onto stack
        lua_pushstring(L, adminID.c_str());
        lua_call(L, 1, 0);      //call function
    }
    else
    {
        fprintf(stderr, "function \"%s\" not found\n", name);
        fprintf(logFile, "function \"%s\" not found\n", name);
    }
}

bool DiceRollerLuaScript::isAdmin(std::string personID)
{
    const char* name = funcName_IsAdmin.c_str();

    if(hasGlobalFunction(name) )
    {
        lua_getglobal(L, name); //put function onto stack
        lua_pushstring(L, personID.c_str());
        lua_call(L, 1, 1);      //call function

        return getResultBool();
    }
    else
    {
        fprintf(stderr, "function \"%s\" not found\n", name);
        fprintf(logFile, "function \"%s\" not found\n", name);
    }

    
}