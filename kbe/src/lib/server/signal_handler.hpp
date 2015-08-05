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

#ifndef __SIGNAL_HANDLER_H__
#define __SIGNAL_HANDLER_H__

// common include
#include "cstdkbe/cstdkbe.hpp"
#include "cstdkbe/tasks.hpp"
#include "cstdkbe/singleton.hpp"
// windows include	
#if KBE_PLATFORM == PLATFORM_WIN32
#else
// linux include
#endif
	
namespace KBEngine{
class ServerApp;

class SignalHandler
{
public:
	virtual void onSignalled(int sigNum) = 0;
};

class SignalHandlers : public Singleton<SignalHandlers>, public Task
{
public:
	SignalHandlers();
	~SignalHandlers();
	
	SignalHandler* addSignal(int sigNum, 
		SignalHandler* pSignalHandler, int flags = 0);
	
	SignalHandler* delSignal(int sigNum);
	
	void clear();
	
	void onSignalled(int sigNum);
	
	virtual bool process();

	void attachApp(ServerApp* app);

	ServerApp* getApp(){ return papp_; }
private:
	typedef std::map<int, SignalHandler*> SignalHandlerMap;
	SignalHandlerMap singnalHandlerMap_;
	
	std::vector<int> signalledVec_;
	ServerApp* papp_;
};

#define g_kbeSignalHandlers SignalHandlers::getSingleton()
}
#endif