#pragma once

#include "stdafx.h"

class IDiceRoller
{
public:
    virtual std::string performCommand(std::string senderHandle, std::string senderName, std::string command) = 0;
    virtual bool isAdmin(std::string personID) = 0;
    virtual void setAdminName(std::string adminID) = 0;
    virtual void dispose() = 0;
};