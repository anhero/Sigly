// Copyright (c) 2011 Anhero Inc.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
#ifndef SIGLY_SIGLY_MULTI_THREADED_LOCAL_H_
#define SIGLY_SIGLY_MULTI_THREADED_LOCAL_H_

#include <sigly/defines.h>

namespace sigly {
#ifdef _SIGLY_HAS_WIN32_THREADS
// The multi threading policies only get compiled in if they are enabled.
class MultiThreadedLocal {
public:
	MultiThreadedLocal() {
		InitializeCriticalSection(&m_critsec);
	}
	
	MultiThreadedLocal(const MultiThreadedLocal&) {
		InitializeCriticalSection(&m_critsec);
	}
	
	virtual ~MultiThreadedLocal() {
		DeleteCriticalSection(&m_critsec);
	}
	
	virtual void lock() {
		EnterCriticalSection(&m_critsec);
	}
	
	virtual void unlock() {
		LeaveCriticalSection(&m_critsec);
	}
	
private:
	CRITICAL_SECTION m_critsec;
};
#endif // _SIGLY_HAS_WIN32_THREADS

#ifdef _SIGLY_HAS_POSIX_THREADS
// The multi threading policies only get compiled in if they are enabled.
class MultiThreadedLocal {
public:
	MultiThreadedLocal() {
		pthread_mutex_init(&m_mutex, NULL);
	}
	
	MultiThreadedLocal(const MultiThreadedLocal&) {
		pthread_mutex_init(&m_mutex, NULL);
	}
	
	virtual ~MultiThreadedLocal() {
		pthread_mutex_destroy(&m_mutex);
	}
	
	virtual void lock() {
		pthread_mutex_lock(&m_mutex);
	}
	
	virtual void unlock() {
		pthread_mutex_unlock(&m_mutex);
	}
	
private:
	pthread_mutex_t m_mutex;
};
#endif // _SIGLY_HAS_POSIX_THREADS
}
#endif // SIGLY_SIGLY_MULTI_THREADED_LOCAL_H_