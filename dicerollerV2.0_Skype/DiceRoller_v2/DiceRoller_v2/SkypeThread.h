//SkypeThread.h
//SkypeThread.h and SkypeThread.cpp code is courtesy of chilopod
//C++ SKYPE4COMLib multithreading example
//http://forum.skype.com/index.php?showtopic=195851
//Modification done by Cody Duncan

#include "stdafx.h"
#include "DiceRoller.h"


//Contains type information used to describe a method or property on a dispinterface.
static _ATL_FUNC_INFO OnAttachmentStatusInfo = {CC_STDCALL, VT_EMPTY, 1, {VT_I4} };
static _ATL_FUNC_INFO OnMessageStatusInfo = {CC_STDCALL,VT_EMPTY,2,{VT_DISPATCH|VT_BYREF,VT_I4}};


class SkypeThread : public IDispEventSimpleImpl<1, SkypeThread, &__uuidof(SKYPE4COMLib::_ISkypeEvents)>
{
	public:
		BEGIN_SINK_MAP(SkypeThread)
			SINK_ENTRY_INFO(1, __uuidof(SKYPE4COMLib::_ISkypeEvents), 4, OnAttachmentStatus, &OnAttachmentStatusInfo)
			SINK_ENTRY_INFO(1, __uuidof(SKYPE4COMLib::_ISkypeEvents), 11, OnMessageStatus, &OnMessageStatusInfo)
		END_SINK_MAP()

		SkypeThread();
		~SkypeThread();
		void __stdcall OnAttachmentStatus(SKYPE4COMLib::TAttachmentStatus Status);
		void __stdcall OnMessageStatus (SKYPE4COMLib::IChatMessage* pMessage ,SKYPE4COMLib::TChatMessageStatus Status);
		void ConnectSkype();
		static DWORD WINAPI ProcessThread(LPVOID parm);
		

	protected:
		SKYPE4COMLib::ISkypePtr m_spSkype;
		DWORD m_dwThreadId;
		BOOL m_bEventsConnected;
		void Stop();

	private:
		IDiceRoller* roll;
		static const string CLOSE_MESSAGE;
		static const string ABOUT_MESSAGE;

        void earlyDispose();

};