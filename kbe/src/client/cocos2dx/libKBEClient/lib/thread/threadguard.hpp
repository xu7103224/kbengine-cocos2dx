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

/*
	�߳��ػ��壺
		�����߳�֮����⾺�������������⡣
	�÷�:
		��һ�����ж��廥�����Ա
		ThreadMutex tm;
		����Ҫ�����ĵط�:
		void XXCLASS::foo(void)
		{
			ThreadGuard tg(this->tm);
			����Ĵ��붼�ǰ�ȫ��
			...
		}
*/
#ifndef __THREADGUARD_H__
#define __THREADGUARD_H__
	
// common include
#include "thread/threadmutex.hpp"
//#define NDEBUG
#include <assert.h>

// windows include	
#if KBE_PLATFORM == PLATFORM_WIN32
#else
// linux include
#include <errno.h>
#endif
	
namespace KBEngine{ namespace thread{

class ThreadGuard
{
public:
	explicit ThreadGuard(ThreadMutex* mutexPtr):mutexPtr_(mutexPtr)
	{
		mutexPtr_->lockMutex();
	}

	virtual ~ThreadGuard(void) 
	{ 
		mutexPtr_->unlockMutex();
	}	
	
protected:
	ThreadMutex* mutexPtr_;
};

}
}
#endif