#pragma once

#include "stdafx.h"

class IDiceRoller
{
public:
    virtual std::string performRoll(std::string sender, std::string command) = 0;
    virtual bool isAdmin(std::string name) = 0;
    virtual void setAdminName(std::string adminName) = 0;

    virtual void dispose() = 0;

};