#include "stdafx.h"
#include "SkypeThread.h"
#include "DiceRollerLuaScript.h"
#include "DiceRand_Lua.h"

const string SkypeThread::CLOSE_MESSAGE = "Thanks for Using DiceRoller v2.0. Shutting Down.";
const string SkypeThread::ABOUT_MESSAGE = 
"---------------------Credits--------------------" "\n"
"	Skype Diceroller V2.0 (C++)" "\n"
"	Original Code Author: Cody Duncan" "\n"
"	Code Assistance: Mr. Scott Bell , Skype Moderator TheUberOverlord" "\n"
"	Future Code Authors: " "\n"
"	Icon Artist: Donald Simon, Andrew Sinclair" "\n"
"	Testers: Roy Lee, Matt M., Chad(wick)" "\n"
"	Version 1.2, August 8, 2009 (Java)" "\n"
"	Version 2.0, June 1, 2011 (C++)" "\n"
"	This project was created independently by Cody Duncan, a Computer Science student at Northwest Missouri State University," "\n"
"	with moral support and concept help from The Fellowship of the Tower Gaming Society (an extremely enjoyable NWMSU campus organization)." "\n"
"	Links: Northwest Missouri State University:  http://www.nwmissouri.edu" "\n"
"	The Fellowship of the Tower Gaming Society:   http://catpages.nwmissouri.edu/m/FOT/" "\n"
"	You can contact the Fellowship of the Tower by emailing: fot@nwmissouri.edu" "\n"
"	Please support the Fellowship of the Tower, all donations are accepted." "\n"
"------------------------------------------------" "\n";


SkypeThread::SkypeThread() : m_bEventsConnected(0)
{
    HRESULT hr = m_spSkype.CreateInstance(__uuidof(SKYPE4COMLib::Skype));
    if (SUCCEEDED(hr)) {
        if (SUCCEEDED(DispEventAdvise(m_spSkype)))
            m_bEventsConnected = TRUE;
    }

    DiceRollerLuaScript* luaDiceRoller = new DiceRollerLuaScript();
    luaDiceRoller->loadLua();
    luaDiceRoller->openLibrary("DiceRand", luaopen_DiceRand);

    int err = luaDiceRoller->runScript("LuaDiceRoller.lua");
    if(!err)
    {
        roll = luaDiceRoller;
    }   

    //roll = new DiceRoller();
}

SkypeThread::~SkypeThread()
{
    if (m_bEventsConnected) {
        if (SUCCEEDED(DispEventUnadvise(m_spSkype)))
            m_bEventsConnected = FALSE;
    }
    m_spSkype = NULL;
}

//disposal before destructor call
void SkypeThread::earlyDispose()
{
    if(typeid(roll) == typeid(DiceRollerLuaScript*))
    {
        DiceRollerLuaScript* luaDiceRoller = dynamic_cast<DiceRollerLuaScript*> (roll);
        luaDiceRoller->dispose();
    }
    
    delete roll;
}
 
///Fired when an attachment status changes, typically fires when a connection is made.
void __stdcall SkypeThread::OnAttachmentStatus(SKYPE4COMLib::TAttachmentStatus Status)
{
    if (Status == SKYPE4COMLib::apiAttachSuccess) {
        std::cout << "API connected" << std::endl;
        try 
        {
            _bstr_t bstrCurrentUserHandle = m_spSkype->GetCurrentUserHandle();
            std::cout << "Logged in user " << (LPSTR)bstrCurrentUserHandle << std::endl;
        }
        catch (_com_error&) 
        {
            std::cout << "Failed to get logged in user handle" << std::endl;
        }
    }
    else if (Status == SKYPE4COMLib::apiAttachPendingAuthorization) {
        std::cout << "API connection pending authorization" << std::endl;
    }
    else if (Status == SKYPE4COMLib::apiAttachRefused) {
        std::cout << "API connection refused" << std::endl;
        ::PostThreadMessage(m_dwThreadId, WM_QUIT, 0, 0);
    }
    else if (Status == SKYPE4COMLib::apiAttachNotAvailable) {
        std::cout << "API not available" << std::endl;
    }
    else if (Status == SKYPE4COMLib::apiAttachAvailable) {
        std::cout << "API available" << std::endl;
        try 
        {
            std::cout << "Trying to reconnect ..." << std::endl;
            m_spSkype->Attach(5, VARIANT_FALSE);
        }
        catch (_com_error&) 
        {
            std::cout << "Failed to reconnect" << std::endl;
        }
    }
}

//Fired when a message is sent or recieved.
void __stdcall SkypeThread::OnMessageStatus (SKYPE4COMLib::IChatMessage* pMessage ,SKYPE4COMLib::TChatMessageStatus Status)
{
    std::string sender = (char*)pMessage->GetSender()->GetHandle();  //get sender
    std::string command = (char*)pMessage->GetBody();                //get the message (which may be a command)
    std::string result = "";                                         //result to output

    //check to ensure this is not exit or about command
    if( strcmp(command.c_str(), "//DICE-EXIT")  != 0 && 
        strcmp(command.c_str(), "//DICE-ABOUT") != 0)
    {
        //perform a roll on sent and read messages
        if(Status == SKYPE4COMLib::cmsSent || Status == SKYPE4COMLib::cmsRead )
        {
            result = roll->performRoll(sender, command);
            result += "\n";
        }

        //if the result is longer than 3 characters, send chat message of result
        if(result.length() > 3)
        {
            _bstr_t output = result.c_str();
            pMessage->GetChat()->SendChatMessage(output);
            Sleep(100);
        }
    }
    
    //deal with "exit" and "about" command
    if(Status == SKYPE4COMLib::cmsSending && roll->isAdmin(sender))
    {
        if(strcmp(command.c_str(), "//DICE-EXIT")==0)
        {
            std::cout << CLOSE_MESSAGE << endl;
            pMessage->GetChat()->SendChatMessage(CLOSE_MESSAGE.c_str());

            PostThreadMessage(m_dwThreadId, WM_QUIT, NULL, NULL);			//closes the thread
        }
        else if(strcmp(command.c_str(), "//DICE-ABOUT")==0)
        {
            std::cout << ABOUT_MESSAGE << endl;
            pMessage->GetChat()->SendChatMessage(ABOUT_MESSAGE.c_str());
        }
    }
}

//main loop
void SkypeThread::ConnectSkype()
{
    try 
    {
        m_dwThreadId = GetCurrentThreadId();
        m_spSkype->Attach(5, VARIANT_TRUE);

        _bstr_t username =  m_spSkype->GetCurrentUserHandle();
        this->roll->setAdminName((char*)username);

        MSG msg;
        while (GetMessage( &msg, NULL, 0, 0 )) 
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        earlyDispose();
        std::cout << "closing down connection" << std::endl;
        ExitThread(0);

    }
    catch (_com_error & err) 
    {
        std::cout << "Failed to run" << std::endl;
    }
}

DWORD WINAPI SkypeThread::ProcessThread(LPVOID parm)
{
    SkypeThread* pObject = reinterpret_cast<SkypeThread*>(parm);
    pObject->ConnectSkype();
    return 0;
}

void SkypeThread::Stop()
{
}


