/*
This source file is part of KBEngine
For the latest info, see http://www.kbengine.org/

Copyright (c) 2008-2012 KBEngine.

KBEngine is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

KBEngine is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.
 
You should have received a copy of the GNU Lesser General Public License
along with KBEngine.  If not, see <http://www.gnu.org/licenses/>.
*/


#ifndef __KBE_DEBUG_HPP__
#define __KBE_DEBUG_HPP__

#include <assert.h>
#include <time.h>	
#include <stdarg.h> 
#include <list> 
#if defined( __WIN32__ ) || defined( WIN32 ) || defined( _WIN32 )
#pragma warning(disable:4819)
#endif
//#include "boost/format.hpp"
#include "cstdkbe/tasks.hpp"
#include "cstdkbe/singleton.hpp"
#include "thread/threadmutex.hpp"
#include "network/common.hpp"
#include "network/address.hpp"

namespace KBEngine{

namespace Mercury{
	class Channel;
	class Bundle;
	class EventDispatcher;
	class NetworkInterface;
	class Packet;
}

/** 
	֧��uft-8�����ַ������ 
*/
void vutf8printf(FILE *out, const char *str, va_list* ap);
void utf8printf(FILE *out, const char *str, ...);


#define	KBELOG_UNKNOWN		0x00000000
#define	KBELOG_PRINT		0x00000001
#define	KBELOG_ERROR		0x00000002
#define	KBELOG_WARNING		0x00000004
#define	KBELOG_DEBUG		0x00000008
#define	KBELOG_INFO			0x00000010
#define	KBELOG_CRITICAL		0x00000020
#define KBELOG_SCRIPT		0x00000040

#define KBELOG_TYPES KBELOG_UNKNOWN | KBELOG_PRINT | KBELOG_ERROR | KBELOG_WARNING | KBELOG_DEBUG | KBELOG_INFO | KBELOG_CRITICAL | KBELOG_SCRIPT

const char KBELOG_TYPE_NAME[][255] = {
	" UNKNOWN",
	"        ",
	"   ERROR",
	" WARNING",
	"   DEBUG",
	"    INFO",
	"CRITICAL",
	"  SCRIPT",
};

inline const char* KBELOG_TYPE_NAME_EX(uint32 CTYPE)
{									
	if(CTYPE < 0 || ((CTYPE) & (KBELOG_TYPES)) <= 0)
	{
		return " UNKNOWN";
	}
	
	switch(CTYPE)
	{
	case KBELOG_PRINT:
		return "        ";
	case KBELOG_ERROR:
		return "   ERROR";
	case KBELOG_WARNING:
		return " WARNING";
	case KBELOG_DEBUG:
		return "   DEBUG";
	case KBELOG_INFO:
		return "    INFO";
	case KBELOG_CRITICAL:
		return "CRITICAL";
	case KBELOG_SCRIPT:
		return "  SCRIPT";
	};

	return " UNKNOWN";
}

class DebugHelper : public Task, 
					public Singleton<DebugHelper>
{
public:
	DebugHelper();

	~DebugHelper();
	
	static bool isInit() { return getSingletonPtr() != 0; }

	static void initHelper(COMPONENT_TYPE componentType);

	void setFile(std::string funcname, std::string file, uint32 line){
		_currFile = file;
		_currLine = line;
		_currFuncName = funcname;
	}
	
	std::string getLogName();

	void lockthread();
	void unlockthread();

	/** 
		ͬ����־��messagelog
	*/
	void sync();
	bool process();
    
	void pNetworkInterface(Mercury:: NetworkInterface* networkInterface);
	void pDispatcher(Mercury:: EventDispatcher* dispatcher);

	//void print_msg(boost::format& fmt);
	void print_msg(std::string s);

	//void debug_msg(boost::format& fmt);
	void debug_msg(std::string s);

	//void error_msg(boost::format& fmt);
	void error_msg(std::string s);

	//void info_msg(boost::format& fmt);
	void info_msg(std::string s);

	//void warning_msg(boost::format& fmt);
	void warning_msg(std::string s);

	//void critical_msg(boost::format& fmt);
	void critical_msg(std::string s);
	
	//void script_msg(boost::format& fmt);
	void script_msg(std::string s);

	void backtrace_msg();

	void onMessage(uint32 logType, const char * str, uint32 length);

	void registerMessagelog(Mercury::MessageID msgID, Mercury::Address* pAddr);
	void unregisterMessagelog(Mercury::MessageID msgID, Mercury::Address* pAddr);

	void changeLogger(std::string name);

	void clearBufferedLog();

	void setScriptMsgType(int msgtype);

	void shouldWriteToSyslog(bool v = true);
private:
	FILE* _logfile;
	std::string _currFile, _currFuncName;
	uint32 _currLine;
	Mercury::Address messagelogAddr_;
	KBEngine::thread::ThreadMutex logMutex;
	std::list< Mercury::Bundle* > bufferedLogPackets_;
	bool syncStarting_;
	Mercury:: NetworkInterface* pNetworkInterface_;
	Mercury:: EventDispatcher* pDispatcher_;

	int scriptMsgType_;
};

/*---------------------------------------------------------------------------------
	������Ϣ����ӿ�
---------------------------------------------------------------------------------*/
#define SCRIPT_MSG(m)					DebugHelper::getSingleton().script_msg((m))									// ����κ���Ϣ
#define PRINT_MSG(m)					DebugHelper::getSingleton().print_msg((m))									// ����κ���Ϣ
#define ERROR_MSG(m)					DebugHelper::getSingleton().error_msg((m))									// ���һ������
#define DEBUG_MSG(m)					DebugHelper::getSingleton().debug_msg((m))									// ���һ��debug��Ϣ
#define INFO_MSG(m)						DebugHelper::getSingleton().info_msg((m))									// ���һ��info��Ϣ
#define WARNING_MSG(m)					DebugHelper::getSingleton().warning_msg((m))								// ���һ��������Ϣ
#define CRITICAL_MSG(m)					DebugHelper::getSingleton().setFile(__FUNCTION__, \
										__FILE__, __LINE__); \
										DebugHelper::getSingleton().critical_msg((m))

/*---------------------------------------------------------------------------------
	���Ժ�
---------------------------------------------------------------------------------*/
#ifdef KBE_USE_ASSERTS
void myassert(const char* exp, const char * func, const char * file, unsigned int line);
#define KBE_ASSERT(exp) if(!(exp))myassert(#exp, __FUNCTION__, __FILE__, __LINE__);
#define KBE_REAL_ASSERT assert(0);
#else
#define KBE_ASSERT(exp) NULL;
#define KBE_REAL_ASSERT
#endif

#ifdef _DEBUG
#define KBE_VERIFY KBE_ASSERT
#else
#define KBE_VERIFY(exp) (exp)
#endif

#define KBE_EXIT(msg) {														\
			CRITICAL_MSG(msg);												\
			KBE_REAL_ASSERT	}												\


}

#endif // __KBE_DEBUG_HPP__