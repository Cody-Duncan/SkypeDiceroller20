#pragma once
#include "luascript.h"
#include "IDiceRoller.h"
class DiceRollerLuaScript : public LuaScript, public IDiceRoller
{
public:
    DiceRollerLuaScript(void);
    DiceRollerLuaScript(const char* filename);
    DiceRollerLuaScript(FILE* errorLog);
    DiceRollerLuaScript(const char* filename, FILE* errorLog);
    ~DiceRollerLuaScript(void);

    static const std::string funcName_PerformRoll;
    static const std::string funcName_SetAdminName;
    static const std::string funcName_IsAdmin;

    //IDiceRoller interface methods
    public:
        std::string performRoll(std::string sender, std::string command);
        void setAdminName(std::string adminName);
        bool isAdmin(std::string checkName);
        void dispose();

};



