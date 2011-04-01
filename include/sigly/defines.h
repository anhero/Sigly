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
#ifndef SIGLY_SIGLY_DEFINES_H_
#define SIGLY_SIGLY_DEFINES_H_

#if defined(SIGLY_PURE_ISO) || (!defined(WIN32) && !defined(__GNUG__) && !defined(SIGLY_USE_POSIX_THREADS))
#	define _SIGLY_SINGLE_THREADED
#elif defined(WIN32)
#	define _SIGLY_HAS_WIN32_THREADS
#	include <windows.h>
#elif defined(__GNUG__) || defined(SIGLY_USE_POSIX_THREADS)
#	define _SIGLY_HAS_POSIX_THREADS
#	include <pthread.h>
#else
#	define _SIGLY_SINGLE_THREADED
#endif

#ifndef SIGLY_DEFAULT_MT_POLICY
#	ifdef _SIGLY_SINGLE_THREADED
#		define SIGLY_DEFAULT_MT_POLICY SingleThreaded
#	else
#		define SIGLY_DEFAULT_MT_POLICY MultiThreadedLocal
#	endif
#endif

#endif // SIGLY_SIGLY_DEFINES_H_