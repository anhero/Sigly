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
#ifndef SIGLY_SIGLY_MULTI_THREADED_GLOBAL_H_
#define SIGLY_SIGLY_MULTI_THREADED_GLOBAL_H_

#include <sigly/defines.h>

#if defined(_SIGLY_HAS_WIN32_THREADS) || defined(_SIGLY_HAS_POSIX_THREADS)
namespace sigly {
// The multi threading policies only get compiled in if they are enabled.
class MultiThreadedGlobal {
public:
	MultiThreadedGlobal();

	MultiThreadedGlobal(const MultiThreadedGlobal&);

	virtual ~MultiThreadedGlobal();

	virtual void lock();

	virtual void unlock();

private:
#ifdef _SIGLY_HAS_WIN32_THREADS
	CRITICAL_SECTION* get_critsec();
#endif // _SIGLY_HAS_WIN32_THREADS

#ifdef _SIGLY_HAS_POSIX_THREADS
	pthread_mutex_t* get_mutex();
#endif // _SIGLY_HAS_POSIX_THREADS

};
}
#endif // defined(_SIGLY_HAS_WIN32_THREADS) || defined(_SIGLY_HAS_POSIX_THREADS)

#endif // SIGLY_SIGLY_MULTI_THREADED_GLOBAL_H_