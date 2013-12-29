#pragma once
#include "luascript.h"
#include "IDiceRoller.h"
class DiceRollerLuaScript : public LuaScript, public IDiceRoller
{
public:
    DiceRollerLuaScript(void);
    DiceRollerLuaScript(FILE* errorLog);
    ~DiceRollerLuaScript(void);

    static const std::string funcName_PerformCommand;
    static const std::string funcName_SetAdminName;
    static const std::string funcName_IsAdmin;
    static const std::string commandName_load;

    //IDiceRoller interface methods
    public:
        std::string performCommand(std::string senderID, std::string senderDisplayName, std::string command);
        bool isAdmin(std::string personID);
        void setAdminName(std::string adminID);
        void dispose();

};



