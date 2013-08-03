#pragma once

#include <iostream>
#include "lua.hpp"

class LuaScript
{
public:
    LuaScript(void);
    LuaScript(FILE* errorLog);
    ~LuaScript(void);

    void loadLua();
    int runScript(const char* filename);
    bool isScriptRun();
    void close();

    void callVoidFunction(const char* name);
    int callVoidFunctionInt(const char* name);

    void registerFunction(const char* name, lua_CFunction f);
    void openLibrary(const char* libName, lua_CFunction openf);

    int getResultInt();
    double getResultDouble();
    std::string getResultString();
    bool getResultBool();
    
    

protected:
    lua_State *L;  //global lya state pointer
    FILE* logFile;
    bool hasScriptBeenRun;
    void defaultValues();

    int report (lua_State *L, int status);
    bool hasGlobalFunction(const char* name);

};

