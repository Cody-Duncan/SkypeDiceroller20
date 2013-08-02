#pragma once

#include <iostream>
#include "lua.hpp"

class LuaScript
{
public:
    LuaScript(void);
    LuaScript(const char* filename);
    LuaScript(FILE* errorLog);
    LuaScript(const char* filename, FILE* errorLog);
    ~LuaScript(void);

    int initialize(const char* filename);
    bool isInitialized();
    void close();

    void callVoidFunction(const char* name);
    int callVoidFunctionInt(const char* name);

    int getResultInt();
    double getResultDouble();
    std::string getResultString();
    bool getResultBool();
    
    

private:
    lua_State *L;  //global lya state pointer
    FILE* logFile;
    bool isInit;
    void defaultValues();

    int report (lua_State *L, int status);
    bool hasGlobalFunction(const char* name);

};

