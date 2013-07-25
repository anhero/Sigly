/*
 Copyright (c) 2011 Anhero Inc.
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 
 sigly - event/delegate (signal/slot) mechanism implementation for C++
 
 https://github.com/anhero/Sigly
 
 Library is written in C++03 and does not use C++11 features.
 
 To use the library, add the sigly.h file to the list of the system include directories or
 to your project.
 
 You can specify which multi-thread policy you want by defining a macro before including sigly or
 by defining it directly in the define.h file. The macros are described in the following quick
 documentation.
 
 Originally written by Sarah Thompson (sarah@telergy.com) 2002.
 
 QUICK DOCUMENTATION
 
 (see also the full documentation at http://sigslot.sourceforge.net/)
 
 #define switches
 SIGLY_PURE_ISO				- Define this to force ISO C++ compliance. This also disables
 all of the thread safety support on platforms where it is
 available.
 
 SIGLY_USE_POSIX_THREADS		- Force use of Posix threads when using a C++ compiler other than
 gcc on a platform that supports Posix threads. (When using gcc,
 this is the default - use SIGLY_PURE_ISO to disable this if
 necessary)
 
 SIGLY_DEFAULT_MT_POLICY		- Where thread support is enabled, this defaults to MultiThreadedGlobal.
 Otherwise, the default is SingleThreaded. #define this yourself to
 override the default. In pure ISO mode, anything other than
 SingleThreaded will cause a compiler error.
 
 PLATFORM NOTES
 
 Win32						- On Win32, the WIN32 symbol must be #defined. Most mainstream
 compilers do this by default, but you may need to define it
 yourself if your build environment is less standard. This causes
 the Win32 thread support to be compiled in and used automatically.
 
 Unix/Linux/BSD, etc.		- If you're using gcc, it is assumed that you have Posix threads
 available, so they are used automatically. You can override this
 (as under Windows) with the SIGLY_PURE_ISO switch. If you're using
 something other than gcc but still want to use Posix threads, you
 need to #define SIGLY_USE_POSIX_THREADS.
 
 ISO C++						- If none of the supported platforms are detected, or if
 SIGLY_PURE_ISO is defined, all multithreading support is turned off,
 along with any code that might cause a pure ISO C++ environment to
 complain. Before you ask, gcc -ansi -pedantic won't compile this
 library, but gcc -ansi is fine. Pedantic mode seems to throw a lot of
 errors that aren't really there. If you feel like investigating this,
 please contact the author.
 
 
 THREADING MODES
 
 SingleThreaded				- Your program is assumed to be single threaded from the point of view
 of signal/slot usage (i.e. all objects using signals and slots are
 created and destroyed from a single thread). Behaviour if objects are
 destroyed concurrently is undefined (i.e. you'll get the occasional
 segmentation fault/memory exception).
 
 MultiThreadedGlobal			- Your program is assumed to be multi threaded. Objects using signals and
 slots can be safely created and destroyed from any thread, even when
 connections exist. In MultiThreadedGlobal mode, this is achieved by a
 single global mutex (actually a critical section on Windows because they
 are faster). This option uses less OS resources, but results in more
 opportunities for contention, possibly resulting in more context switches
 than are strictly necessary.
 
 MultiThreadedLocal			- Behaviour in this mode is essentially the same as MultiThreadedGlobal,
 except that each signal, and each object that inherits HasSlots, all
 have their own mutex/critical section. In practice, this means that
 mutex collisions (and hence context switches) only happen if they are
 absolutely essential. However, on some platforms, creating a lot of
 mutexes can slow down the whole OS, so use this option with care.
 */
#ifndef SIGLY_H__
#define SIGLY_H__

#include <set>
#include <list>

#if defined(SIGLY_PURE_ISO) || (!defined(WIN32) && !defined(__GNUG__) && !defined(SIGLY_USE_POSIX_THREADS))
#       define _SIGLY_SINGLE_THREADED
#elif defined(WIN32)
#       define _SIGLY_HAS_WIN32_THREADS
#       include <windows.h>
#elif defined(__GNUG__) || defined(SIGLY_USE_POSIX_THREADS)
#       define _SIGLY_HAS_POSIX_THREADS
#       include <pthread.h>
#else
#       define _SIGLY_SINGLE_THREADED
#endif

#ifndef SIGLY_DEFAULT_MT_POLICY
#       ifdef _SIGLY_SINGLE_THREADED
#               define SIGLY_DEFAULT_MT_POLICY SingleThreaded
#       else
#               define SIGLY_DEFAULT_MT_POLICY MultiThreadedLocal
#       endif
#endif


namespace sigly {
	
	class SingleThreaded {
	public:
		SingleThreaded() {
		}
		
		virtual ~SingleThreaded() {
		}
		
		virtual void lock() {
		}
		
		virtual void unlock() {
		}
	};
	
#ifdef _SIGLY_HAS_WIN32_THREADS
	// The multi threading policies only get compiled in if they are enabled.
	class MultiThreadedGlobal {
	public:
		MultiThreadedGlobal() {
			static bool isinitialised = false;
			
			if (!isinitialised) {
				InitializeCriticalSection(get_critsec());
				isinitialised = true;
			}
		}
		
		MultiThreadedGlobal(const MultiThreadedGlobal &) {
		}
		
		virtual ~MultiThreadedGlobal() {
		}
		
		virtual void lock() {
			EnterCriticalSection(get_critsec());
		}
		
		virtual void unlock() {
			LeaveCriticalSection(get_critsec());
		}
		
	private:
		CRITICAL_SECTION *get_critsec() {
			static CRITICAL_SECTION g_critsec;
			return &g_critsec;
		}
	};
	
	class MultiThreadedLocal {
	public:
		MultiThreadedLocal() {
			InitializeCriticalSection(&m_critsec);
		}
		
		MultiThreadedLocal(const MultiThreadedLocal &) {
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
	class MultiThreadedGlobal {
	public:
		MultiThreadedGlobal() {
			pthread_mutex_init(get_mutex(), NULL);
		}
		
		MultiThreadedGlobal(const MultiThreadedGlobal &) {
		}
		
		virtual ~MultiThreadedGlobal() {
		}
		
		virtual void lock() {
			pthread_mutex_lock(get_mutex());
		}
		
		virtual void unlock() {
			pthread_mutex_unlock(get_mutex());
		}
		
	private:
		pthread_mutex_t *get_mutex() {
			static pthread_mutex_t g_mutex;
			return &g_mutex;
		}
	};
	
	class MultiThreadedLocal {
	public:
		MultiThreadedLocal() {
			pthread_mutex_init(&m_mutex, NULL);
		}
		
		MultiThreadedLocal(const MultiThreadedLocal &) {
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
	
	template<class mt_policy>
	class lock_block {
	public:
		mt_policy *m_mutex;
		
		lock_block(mt_policy *mtx)
		: m_mutex(mtx) {
			m_mutex->lock();
		}
		
		~lock_block() {
			m_mutex->unlock();
		}
	};
	
	template<class mt_policy>
	class HasSlots;
	
	template<class mt_policy>
	class _connection_base0 {
	public:
		virtual ~_connection_base0() {}
		virtual HasSlots<mt_policy>* getdest() const = 0;
		virtual void shoot() = 0;
		virtual _connection_base0 *clone() = 0;
		virtual _connection_base0 *duplicate(HasSlots<mt_policy>* pnewdest) = 0;
	};
	
	template<class arg1_type, class mt_policy>
	class _connection_base1 {
	public:
		virtual ~_connection_base1() {}
		virtual HasSlots<mt_policy>* getdest() const = 0;
		virtual void shoot(arg1_type) = 0;
		virtual _connection_base1<arg1_type, mt_policy>* clone() = 0;
		virtual _connection_base1<arg1_type, mt_policy>* duplicate(HasSlots<mt_policy>* pnewdest) = 0;
	};
	
	template<class arg1_type, class arg2_type, class mt_policy>
	class _connection_base2 {
	public:
		virtual ~_connection_base2() {}
		virtual HasSlots<mt_policy>* getdest() const = 0;
		virtual void shoot(arg1_type, arg2_type) = 0;
		virtual _connection_base2<arg1_type, arg2_type, mt_policy>* clone() = 0;
		virtual _connection_base2<arg1_type, arg2_type, mt_policy>* duplicate(HasSlots<mt_policy>* pnewdest) = 0;
	};
	
	template<class arg1_type, class arg2_type, class arg3_type, class mt_policy>
	class _connection_base3 {
	public:
		virtual ~_connection_base3() {}
		virtual HasSlots<mt_policy>* getdest() const = 0;
		virtual void shoot(arg1_type, arg2_type, arg3_type) = 0;
		virtual _connection_base3<arg1_type, arg2_type, arg3_type, mt_policy>* clone() = 0;
		virtual _connection_base3<arg1_type, arg2_type, arg3_type, mt_policy>* duplicate(HasSlots<mt_policy>* pnewdest) = 0;
	};
	
	template<class arg1_type, class arg2_type, class arg3_type, class arg4_type, class mt_policy>
	class _connection_base4 {
	public:
		virtual ~_connection_base4() {}
		virtual HasSlots<mt_policy>* getdest() const = 0;
		virtual void shoot(arg1_type, arg2_type, arg3_type, arg4_type) = 0;
		virtual _connection_base4<arg1_type, arg2_type, arg3_type, arg4_type, mt_policy>* clone() = 0;
		virtual _connection_base4<arg1_type, arg2_type, arg3_type, arg4_type, mt_policy>* duplicate(HasSlots<mt_policy>* pnewdest) = 0;
	};
	
	template < class arg1_type, class arg2_type, class arg3_type, class arg4_type,
	class arg5_type, class mt_policy >
	class _connection_base5 {
	public:
		virtual ~_connection_base5() {}
		virtual HasSlots<mt_policy>* getdest() const = 0;
		virtual void shoot(arg1_type, arg2_type, arg3_type, arg4_type,
		                   arg5_type) = 0;
		virtual _connection_base5 < arg1_type, arg2_type, arg3_type, arg4_type,
		arg5_type, mt_policy > * clone() = 0;
		virtual _connection_base5 < arg1_type, arg2_type, arg3_type, arg4_type,
		arg5_type, mt_policy > * duplicate(HasSlots<mt_policy>* pnewdest) = 0;
	};
	
	template < class arg1_type, class arg2_type, class arg3_type, class arg4_type,
	class arg5_type, class arg6_type, class mt_policy >
	class _connection_base6 {
	public:
		virtual ~_connection_base6() {}
		virtual HasSlots<mt_policy>* getdest() const = 0;
		virtual void shoot(arg1_type, arg2_type, arg3_type, arg4_type, arg5_type,
		                   arg6_type) = 0;
		virtual _connection_base6 < arg1_type, arg2_type, arg3_type, arg4_type,
		arg5_type, arg6_type, mt_policy > * clone() = 0;
		virtual _connection_base6 < arg1_type, arg2_type, arg3_type, arg4_type,
		arg5_type, arg6_type, mt_policy > * duplicate(HasSlots<mt_policy>* pnewdest) = 0;
	};
	
	template < class arg1_type, class arg2_type, class arg3_type, class arg4_type,
	class arg5_type, class arg6_type, class arg7_type, class mt_policy >
	class _connection_base7 {
	public:
		virtual ~_connection_base7() {}
		virtual HasSlots<mt_policy>* getdest() const = 0;
		virtual void shoot(arg1_type, arg2_type, arg3_type, arg4_type, arg5_type,
		                   arg6_type, arg7_type) = 0;
		virtual _connection_base7 < arg1_type, arg2_type, arg3_type, arg4_type,
		arg5_type, arg6_type, arg7_type, mt_policy > * clone() = 0;
		virtual _connection_base7 < arg1_type, arg2_type, arg3_type, arg4_type,
		arg5_type, arg6_type, arg7_type, mt_policy > * duplicate(HasSlots<mt_policy>* pnewdest) = 0;
	};
	
	template < class arg1_type, class arg2_type, class arg3_type, class arg4_type,
	class arg5_type, class arg6_type, class arg7_type, class arg8_type, class mt_policy >
	class _connection_base8 {
	public:
		virtual ~_connection_base8() {}
		virtual HasSlots<mt_policy>* getdest() const = 0;
		virtual void shoot(arg1_type, arg2_type, arg3_type, arg4_type, arg5_type,
		                   arg6_type, arg7_type, arg8_type) = 0;
		virtual _connection_base8 < arg1_type, arg2_type, arg3_type, arg4_type,
		arg5_type, arg6_type, arg7_type, arg8_type, mt_policy > * clone() = 0;
		virtual _connection_base8 < arg1_type, arg2_type, arg3_type, arg4_type,
		arg5_type, arg6_type, arg7_type, arg8_type, mt_policy > * duplicate(HasSlots<mt_policy>* pnewdest) = 0;
	};
	
	template<class mt_policy>
	class _signal_base : public mt_policy {
	public:
		virtual void slot_disconnect(HasSlots<mt_policy>* pslot) = 0;
		virtual void slot_duplicate(const HasSlots<mt_policy>* poldslot, HasSlots<mt_policy>* pnewslot) = 0;
	};
	
	template<class  mt_policy = SIGLY_DEFAULT_MT_POLICY>
	class HasSlots : public mt_policy {
	private:
		typedef typename std::set<_signal_base<mt_policy> *> sender_set;
		typedef typename sender_set::const_iterator const_iterator;
		
	public:
		HasSlots(): mt_policy(), active(true) {
		}
		
		HasSlots(const HasSlots &hs): mt_policy(hs) {
			lock_block<mt_policy> lock(this);
			const_iterator it = hs.m_senders.begin();
			const_iterator itEnd = hs.m_senders.end();
			
			while (it != itEnd) {
				(*it)->slot_duplicate(&hs, this);
				m_senders.insert(*it);
				++it;
			}
		}
		
		void signalConnect(_signal_base<mt_policy>* sender) {
			lock_block<mt_policy> lock(this);
			m_senders.insert(sender);
		}
		
		void signalDisconnect(_signal_base<mt_policy>* sender) {
			lock_block<mt_policy> lock(this);
			m_senders.erase(sender);
		}
		
		virtual ~HasSlots() {
			disconnectAll();
		}
		
		void disconnectAll() {
			lock_block<mt_policy> lock(this);
			const_iterator it = m_senders.begin();
			const_iterator itEnd = m_senders.end();
			
			while (it != itEnd) {
				(*it)->slot_disconnect(this);
				++it;
			}
			
			m_senders.erase(m_senders.begin(), m_senders.end());
		}
		
		void deactivateSlots() {
			active = false;
		}
		
		void activateSlots() {
			active = true;
		}
		
		bool areSlotsActive() const {
			return active;
		}
		
	private:
		sender_set m_senders;
		bool active;
	};
	
	template<class mt_policy>
	class _signal_base0 : public _signal_base<mt_policy> {
	public:
		typedef typename std::list<_connection_base0<mt_policy> *>  connections_list;
		typedef typename connections_list::const_iterator const_iterator;
		typedef typename connections_list::iterator iterator;
		
		_signal_base0() {
			;
		}
		
		_signal_base0(const _signal_base0 &s)
		: _signal_base<mt_policy>(s) {
			lock_block<mt_policy> lock(this);
			const_iterator  it = s.m_connected_slots.begin();
			const_iterator itEnd = s.m_connected_slots.end();
			
			while (it != itEnd) {
				(*it)->getdest()->signalConnect(this);
				m_connected_slots.push_back((*it)->clone());
				
				++it;
			}
		}
		
		~_signal_base0() {
			disconnectAll();
		}
		
		void disconnectAll() {
			lock_block<mt_policy> lock(this);
			const_iterator it  = m_connected_slots.begin();
			const_iterator itEnd = m_connected_slots.end();
			
			while (it != itEnd) {
				(*it)->getdest()->signalDisconnect(this);
				delete *it;
				
				++it;
			}
			
			m_connected_slots.erase(m_connected_slots.begin(), m_connected_slots.end());
		}
		
		void disconnect(HasSlots<mt_policy>* pclass) {
			lock_block<mt_policy> lock(this);
			iterator it = m_connected_slots.begin();
			iterator itEnd = m_connected_slots.end();
			
			while (it != itEnd) {
				if ((*it)->getdest() == pclass) {
					delete *it;
					m_connected_slots.erase(iterator(it++));
					pclass->signalDisconnect(this);
					continue;
				}
				
				++it;
			}
		}
		
		void slot_disconnect(HasSlots<mt_policy>* pslot) {
			lock_block<mt_policy> lock(this);
			iterator it = m_connected_slots.begin();
			iterator itEnd = m_connected_slots.end();
			
			while (it != itEnd) {
				iterator itNext = it;
				++itNext;
				
				if ((*it)->getdest() == pslot) {
					delete *it;
					m_connected_slots.erase(it);
				}
				
				it = itNext;
			}
		}
		
		void slot_duplicate(const HasSlots<mt_policy>* oldtarget, HasSlots<mt_policy>* newtarget) {
			lock_block<mt_policy> lock(this);
			iterator it = m_connected_slots.begin();
			iterator itEnd = m_connected_slots.end();
			
			while (it != itEnd) {
				if ((*it)->getdest() == oldtarget) {
					m_connected_slots.push_back((*it)->duplicate(newtarget));
				}
				
				++it;
			}
		}
		
	protected:
		connections_list m_connected_slots;
	};
	
	template<class arg1_type, class mt_policy>
	class _signal_base1 : public _signal_base<mt_policy> {
	public:
		typedef typename std::list<_connection_base1<arg1_type, mt_policy> *>  connections_list;
		typedef typename connections_list::const_iterator const_iterator;
		typedef typename connections_list::iterator iterator;
		
		_signal_base1() {
			;
		}
		
		_signal_base1(const _signal_base1<arg1_type, mt_policy>& s)
		: _signal_base<mt_policy>(s) {
			lock_block<mt_policy> lock(this);
			const_iterator it = s.m_connected_slots.begin();
			const_iterator itEnd = s.m_connected_slots.end();
			
			while (it != itEnd) {
				(*it)->getdest()->signalConnect(this);
				m_connected_slots.push_back((*it)->clone());
				
				++it;
			}
		}
		
		void slot_duplicate(const HasSlots<mt_policy>* oldtarget, HasSlots<mt_policy>* newtarget) {
			lock_block<mt_policy> lock(this);
			iterator it = m_connected_slots.begin();
			iterator itEnd = m_connected_slots.end();
			
			while (it != itEnd) {
				if ((*it)->getdest() == oldtarget) {
					m_connected_slots.push_back((*it)->duplicate(newtarget));
				}
				
				++it;
			}
		}
		
		~_signal_base1() {
			disconnectAll();
		}
		
		void disconnectAll() {
			lock_block<mt_policy> lock(this);
			const_iterator it = m_connected_slots.begin();
			const_iterator itEnd = m_connected_slots.end();
			
			while (it != itEnd) {
				(*it)->getdest()->signalDisconnect(this);
				delete *it;
				
				++it;
			}
			
			m_connected_slots.erase(m_connected_slots.begin(), m_connected_slots.end());
		}
		
		void disconnect(HasSlots<mt_policy>* pclass) {
			lock_block<mt_policy> lock(this);
			iterator it = m_connected_slots.begin();
			iterator itEnd = m_connected_slots.end();
			
			while (it != itEnd) {
				if ((*it)->getdest() == pclass) {
					delete *it;
					m_connected_slots.erase(iterator(it++));
					pclass->signalDisconnect(this);
					continue;
				}
				
				++it;
			}
		}
		
		void slot_disconnect(HasSlots<mt_policy>* pslot) {
			lock_block<mt_policy> lock(this);
			iterator it = m_connected_slots.begin();
			iterator itEnd = m_connected_slots.end();
			
			while (it != itEnd) {
				iterator itNext = it;
				++itNext;
				
				if ((*it)->getdest() == pslot) {
					delete *it;
					m_connected_slots.erase(it);
				}
				
				it = itNext;
			}
		}
		
		
	protected:
		connections_list m_connected_slots;
	};
	
	template<class arg1_type, class arg2_type, class mt_policy>
	class _signal_base2 : public _signal_base<mt_policy> {
	public:
		typedef typename std::list<_connection_base2<arg1_type, arg2_type, mt_policy> *>
		connections_list;
		typedef typename connections_list::const_iterator const_iterator;
		typedef typename connections_list::iterator iterator;
		
		_signal_base2() {
			;
		}
		
		_signal_base2(const _signal_base2<arg1_type, arg2_type, mt_policy>& s)
		: _signal_base<mt_policy>(s) {
			lock_block<mt_policy> lock(this);
			const_iterator it = s.m_connected_slots.begin();
			const_iterator itEnd = s.m_connected_slots.end();
			
			while (it != itEnd) {
				(*it)->getdest()->signalConnect(this);
				m_connected_slots.push_back((*it)->clone());
				
				++it;
			}
		}
		
		void slot_duplicate(const HasSlots<mt_policy>* oldtarget, HasSlots<mt_policy>* newtarget) {
			lock_block<mt_policy> lock(this);
			iterator it = m_connected_slots.begin();
			iterator itEnd = m_connected_slots.end();
			
			while (it != itEnd) {
				if ((*it)->getdest() == oldtarget) {
					m_connected_slots.push_back((*it)->duplicate(newtarget));
				}
				
				++it;
			}
		}
		
		~_signal_base2() {
			disconnectAll();
		}
		
		void disconnectAll() {
			lock_block<mt_policy> lock(this);
			const_iterator it = m_connected_slots.begin();
			const_iterator itEnd = m_connected_slots.end();
			
			while (it != itEnd) {
				(*it)->getdest()->signalDisconnect(this);
				delete *it;
				
				++it;
			}
			
			m_connected_slots.erase(m_connected_slots.begin(), m_connected_slots.end());
		}
		
		void disconnect(HasSlots<mt_policy>* pclass) {
			lock_block<mt_policy> lock(this);
			iterator it = m_connected_slots.begin();
			iterator itEnd = m_connected_slots.end();
			
			while (it != itEnd) {
				if ((*it)->getdest() == pclass) {
					delete *it;
					m_connected_slots.erase(iterator(it++));
					pclass->signalDisconnect(this);
					continue;
				}
				
				++it;
			}
		}
		
		void slot_disconnect(HasSlots<mt_policy>* pslot) {
			lock_block<mt_policy> lock(this);
			iterator it = m_connected_slots.begin();
			iterator itEnd = m_connected_slots.end();
			
			while (it != itEnd) {
				iterator itNext = it;
				++itNext;
				
				if ((*it)->getdest() == pslot) {
					delete *it;
					m_connected_slots.erase(it);
				}
				
				it = itNext;
			}
		}
		
	protected:
		connections_list m_connected_slots;
	};
	
	template<class arg1_type, class arg2_type, class arg3_type, class mt_policy>
	class _signal_base3 : public _signal_base<mt_policy> {
	public:
		typedef std::list<_connection_base3<arg1_type, arg2_type, arg3_type, mt_policy> *>
		connections_list;
		
		typedef typename connections_list::const_iterator const_iterator;
		typedef typename connections_list::iterator iterator;
		_signal_base3() {
			;
		}
		
		_signal_base3(const _signal_base3<arg1_type, arg2_type, arg3_type, mt_policy>& s)
		: _signal_base<mt_policy>(s) {
			lock_block<mt_policy> lock(this);
			const_iterator it = s.m_connected_slots.begin();
			const_iterator itEnd = s.m_connected_slots.end();
			
			while (it != itEnd) {
				(*it)->getdest()->signalConnect(this);
				m_connected_slots.push_back((*it)->clone());
				
				++it;
			}
		}
		
		void slot_duplicate(const HasSlots<mt_policy>* oldtarget, HasSlots<mt_policy>* newtarget) {
			lock_block<mt_policy> lock(this);
			iterator it = m_connected_slots.begin();
			iterator itEnd = m_connected_slots.end();
			
			while (it != itEnd) {
				if ((*it)->getdest() == oldtarget) {
					m_connected_slots.push_back((*it)->duplicate(newtarget));
				}
				
				++it;
			}
		}
		
		~_signal_base3() {
			disconnectAll();
		}
		
		void disconnectAll() {
			lock_block<mt_policy> lock(this);
			const_iterator it = m_connected_slots.begin();
			const_iterator itEnd = m_connected_slots.end();
			
			while (it != itEnd) {
				(*it)->getdest()->signalDisconnect(this);
				delete *it;
				
				++it;
			}
			
			m_connected_slots.erase(m_connected_slots.begin(), m_connected_slots.end());
		}
		
		void disconnect(HasSlots<mt_policy>* pclass) {
			lock_block<mt_policy> lock(this);
			iterator it = m_connected_slots.begin();
			iterator itEnd = m_connected_slots.end();
			
			while (it != itEnd) {
				if ((*it)->getdest() == pclass) {
					delete *it;
					m_connected_slots.erase(iterator(it++));
					pclass->signalDisconnect(this);
					continue;
				}
				
				++it;
			}
		}
		
		void slot_disconnect(HasSlots<mt_policy>* pslot) {
			lock_block<mt_policy> lock(this);
			iterator it = m_connected_slots.begin();
			iterator itEnd = m_connected_slots.end();
			
			while (it != itEnd) {
				iterator itNext = it;
				++itNext;
				
				if ((*it)->getdest() == pslot) {
					delete *it;
					m_connected_slots.erase(it);
				}
				
				it = itNext;
			}
		}
		
	protected:
		connections_list m_connected_slots;
	};
	
	template<class arg1_type, class arg2_type, class arg3_type, class arg4_type, class mt_policy>
	class _signal_base4 : public _signal_base<mt_policy> {
	public:
		typedef std::list < _connection_base4 < arg1_type, arg2_type, arg3_type,
		arg4_type, mt_policy > * >  connections_list;
		typedef typename connections_list::const_iterator const_iterator;
		typedef typename connections_list::iterator iterator;
		
		_signal_base4() {
			;
		}
		
		_signal_base4(const _signal_base4<arg1_type, arg2_type, arg3_type, arg4_type, mt_policy>& s)
		: _signal_base<mt_policy>(s) {
			lock_block<mt_policy> lock(this);
			const_iterator it = s.m_connected_slots.begin();
			const_iterator itEnd = s.m_connected_slots.end();
			
			while (it != itEnd) {
				(*it)->getdest()->signalConnect(this);
				m_connected_slots.push_back((*it)->clone());
				
				++it;
			}
		}
		
		void slot_duplicate(const HasSlots<mt_policy>* oldtarget, HasSlots<mt_policy>* newtarget) {
			lock_block<mt_policy> lock(this);
			iterator it = m_connected_slots.begin();
			iterator itEnd = m_connected_slots.end();
			
			while (it != itEnd) {
				if ((*it)->getdest() == oldtarget) {
					m_connected_slots.push_back((*it)->duplicate(newtarget));
				}
				
				++it;
			}
		}
		
		~_signal_base4() {
			disconnectAll();
		}
		
		void disconnectAll() {
			lock_block<mt_policy> lock(this);
			const_iterator it = m_connected_slots.begin();
			const_iterator itEnd = m_connected_slots.end();
			
			while (it != itEnd) {
				(*it)->getdest()->signalDisconnect(this);
				delete *it;
				
				++it;
			}
			
			m_connected_slots.erase(m_connected_slots.begin(), m_connected_slots.end());
		}
		
		void disconnect(HasSlots<mt_policy>* pclass) {
			lock_block<mt_policy> lock(this);
			iterator it = m_connected_slots.begin();
			iterator itEnd = m_connected_slots.end();
			
			while (it != itEnd) {
				if ((*it)->getdest() == pclass) {
					delete *it;
					this->m_connected_slots.erase(iterator(it++));
					pclass->signalDisconnect(this);
					continue;
				}
				
				++it;
			}
		}
		
		void slot_disconnect(HasSlots<mt_policy>* pslot) {
			lock_block<mt_policy> lock(this);
			iterator it = m_connected_slots.begin();
			iterator itEnd = m_connected_slots.end();
			
			while (it != itEnd) {
				iterator itNext = it;
				++itNext;
				
				if ((*it)->getdest() == pslot) {
					delete *it;
					m_connected_slots.erase(it);
				}
				
				it = itNext;
			}
		}
		
	protected:
		connections_list m_connected_slots;
	};
	
	template < class arg1_type, class arg2_type, class arg3_type, class arg4_type,
	class arg5_type, class mt_policy >
	class _signal_base5 : public _signal_base<mt_policy> {
	public:
		typedef std::list < _connection_base5 < arg1_type, arg2_type, arg3_type,
		arg4_type, arg5_type, mt_policy > * >  connections_list;
		typedef typename connections_list::const_iterator const_iterator;
		typedef typename connections_list::iterator iterator;
		
		_signal_base5() {
			;
		}
		
		_signal_base5(const _signal_base5 < arg1_type, arg2_type, arg3_type, arg4_type,
		              arg5_type, mt_policy > & s)
		: _signal_base<mt_policy>(s) {
			lock_block<mt_policy> lock(this);
			const_iterator it = s.m_connected_slots.begin();
			const_iterator itEnd = s.m_connected_slots.end();
			
			while (it != itEnd) {
				(*it)->getdest()->signalConnect(this);
				m_connected_slots.push_back((*it)->clone());
				
				++it;
			}
		}
		
		void slot_duplicate(const HasSlots<mt_policy>* oldtarget, HasSlots<mt_policy>* newtarget) {
			lock_block<mt_policy> lock(this);
			iterator it = m_connected_slots.begin();
			iterator itEnd = m_connected_slots.end();
			
			while (it != itEnd) {
				if ((*it)->getdest() == oldtarget) {
					m_connected_slots.push_back((*it)->duplicate(newtarget));
				}
				
				++it;
			}
		}
		
		~_signal_base5() {
			disconnectAll();
		}
		
		void disconnectAll() {
			lock_block<mt_policy> lock(this);
			const_iterator it = m_connected_slots.begin();
			const_iterator itEnd = m_connected_slots.end();
			
			while (it != itEnd) {
				(*it)->getdest()->signalDisconnect(this);
				delete *it;
				
				++it;
			}
			
			m_connected_slots.erase(m_connected_slots.begin(), m_connected_slots.end());
		}
		
		void disconnect(HasSlots<mt_policy>* pclass) {
			lock_block<mt_policy> lock(this);
			iterator it = m_connected_slots.begin();
			iterator itEnd = m_connected_slots.end();
			
			while (it != itEnd) {
				if ((*it)->getdest() == pclass) {
					delete *it;
					m_connected_slots.erase(iterator(it++));
					pclass->signalDisconnect(this);
					continue;
				}
				
				++it;
			}
		}
		
		void slot_disconnect(HasSlots<mt_policy>* pslot) {
			lock_block<mt_policy> lock(this);
			iterator it = m_connected_slots.begin();
			iterator itEnd = m_connected_slots.end();
			
			while (it != itEnd) {
				iterator itNext = it;
				++itNext;
				
				if ((*it)->getdest() == pslot) {
					delete *it;
					m_connected_slots.erase(it);
				}
				
				it = itNext;
			}
		}
		
	protected:
		connections_list m_connected_slots;
	};
	
	template < class arg1_type, class arg2_type, class arg3_type, class arg4_type,
	class arg5_type, class arg6_type, class mt_policy >
	class _signal_base6 : public _signal_base<mt_policy> {
	public:
		typedef std::list < _connection_base6 < arg1_type, arg2_type, arg3_type,
		arg4_type, arg5_type, arg6_type, mt_policy > * >  connections_list;
		typedef typename connections_list::const_iterator const_iterator;
		typedef typename connections_list::iterator iterator;
		
		_signal_base6() {
			;
		}
		
		_signal_base6(const _signal_base6 < arg1_type, arg2_type, arg3_type, arg4_type,
		              arg5_type, arg6_type, mt_policy > & s)
		: _signal_base<mt_policy>(s) {
			lock_block<mt_policy> lock(this);
			const_iterator it = s.m_connected_slots.begin();
			const_iterator itEnd = s.m_connected_slots.end();
			
			while (it != itEnd) {
				(*it)->getdest()->signalConnect(this);
				m_connected_slots.push_back((*it)->clone());
				
				++it;
			}
		}
		
		void slot_duplicate(const HasSlots<mt_policy>* oldtarget, HasSlots<mt_policy>* newtarget) {
			lock_block<mt_policy> lock(this);
			iterator it = m_connected_slots.begin();
			iterator itEnd = m_connected_slots.end();
			
			while (it != itEnd) {
				if ((*it)->getdest() == oldtarget) {
					m_connected_slots.push_back((*it)->duplicate(newtarget));
				}
				
				++it;
			}
		}
		
		~_signal_base6() {
			disconnectAll();
		}
		
		void disconnectAll() {
			lock_block<mt_policy> lock(this);
			const_iterator it = m_connected_slots.begin();
			const_iterator itEnd = m_connected_slots.end();
			
			while (it != itEnd) {
				(*it)->getdest()->signalDisconnect(this);
				delete *it;
				
				++it;
			}
			
			m_connected_slots.erase(m_connected_slots.begin(), m_connected_slots.end());
		}
		
		void disconnect(HasSlots<mt_policy>* pclass) {
			lock_block<mt_policy> lock(this);
			iterator it = m_connected_slots.begin();
			iterator itEnd = m_connected_slots.end();
			
			while (it != itEnd) {
				if ((*it)->getdest() == pclass) {
					delete *it;
					m_connected_slots.erase(iterator(it++));
					pclass->signalDisconnect(this);
					continue;
				}
				
				++it;
			}
		}
		
		void slot_disconnect(HasSlots<mt_policy>* pslot) {
			lock_block<mt_policy> lock(this);
			iterator it = m_connected_slots.begin();
			iterator itEnd = m_connected_slots.end();
			
			while (it != itEnd) {
				iterator itNext = it;
				++itNext;
				
				if ((*it)->getdest() == pslot) {
					delete *it;
					m_connected_slots.erase(it);
				}
				
				it = itNext;
			}
		}
		
	protected:
		connections_list m_connected_slots;
	};
	
	template < class arg1_type, class arg2_type, class arg3_type, class arg4_type,
	class arg5_type, class arg6_type, class arg7_type, class mt_policy >
	class _signal_base7 : public _signal_base<mt_policy> {
	public:
		typedef std::list < _connection_base7 < arg1_type, arg2_type, arg3_type,
		arg4_type, arg5_type, arg6_type, arg7_type, mt_policy > * >  connections_list;
		typedef typename connections_list::const_iterator const_iterator;
		typedef typename connections_list::iterator iterator;
		
		_signal_base7() {
			;
		}
		
		_signal_base7(const _signal_base7 < arg1_type, arg2_type, arg3_type, arg4_type,
		              arg5_type, arg6_type, arg7_type, mt_policy > & s)
		: _signal_base<mt_policy>(s) {
			lock_block<mt_policy> lock(this);
			const_iterator it = s.m_connected_slots.begin();
			const_iterator itEnd = s.m_connected_slots.end();
			
			while (it != itEnd) {
				(*it)->getdest()->signalConnect(this);
				m_connected_slots.push_back((*it)->clone());
				
				++it;
			}
		}
		
		void slot_duplicate(const HasSlots<mt_policy>* oldtarget, HasSlots<mt_policy>* newtarget) {
			lock_block<mt_policy> lock(this);
			iterator it = m_connected_slots.begin();
			iterator itEnd = m_connected_slots.end();
			
			while (it != itEnd) {
				if ((*it)->getdest() == oldtarget) {
					m_connected_slots.push_back((*it)->duplicate(newtarget));
				}
				
				++it;
			}
		}
		
		~_signal_base7() {
			disconnectAll();
		}
		
		void disconnectAll() {
			lock_block<mt_policy> lock(this);
			const_iterator it = m_connected_slots.begin();
			const_iterator itEnd = m_connected_slots.end();
			
			while (it != itEnd) {
				(*it)->getdest()->signalDisconnect(this);
				delete *it;
				
				++it;
			}
			
			m_connected_slots.erase(m_connected_slots.begin(), m_connected_slots.end());
		}
		
		void disconnect(HasSlots<mt_policy>* pclass) {
			lock_block<mt_policy> lock(this);
			iterator it = m_connected_slots.begin();
			iterator itEnd = m_connected_slots.end();
			
			while (it != itEnd) {
				if ((*it)->getdest() == pclass) {
					delete *it;
					m_connected_slots.erase(iterator(it++));
					pclass->signalDisconnect(this);
					continue;
				}
				
				++it;
			}
		}
		
		void slot_disconnect(HasSlots<mt_policy>* pslot) {
			lock_block<mt_policy> lock(this);
			iterator it = m_connected_slots.begin();
			iterator itEnd = m_connected_slots.end();
			
			while (it != itEnd) {
				iterator itNext = it;
				++itNext;
				
				if ((*it)->getdest() == pslot) {
					delete *it;
					m_connected_slots.erase(it);
				}
				
				it = itNext;
			}
		}
		
	protected:
		connections_list m_connected_slots;
	};
	
	template < class arg1_type, class arg2_type, class arg3_type, class arg4_type,
	class arg5_type, class arg6_type, class arg7_type, class arg8_type, class mt_policy >
	class _signal_base8 : public _signal_base<mt_policy> {
	public:
		typedef std::list < _connection_base8 < arg1_type, arg2_type, arg3_type,
		arg4_type, arg5_type, arg6_type, arg7_type, arg8_type, mt_policy > * >
		connections_list;
		typedef typename connections_list::const_iterator const_iterator;
		typedef typename connections_list::iterator iterator;
		
		_signal_base8() {
			;
		}
		
		_signal_base8(const _signal_base8 < arg1_type, arg2_type, arg3_type, arg4_type,
		              arg5_type, arg6_type, arg7_type, arg8_type, mt_policy > & s)
		: _signal_base<mt_policy>(s) {
			lock_block<mt_policy> lock(this);
			const_iterator it = s.m_connected_slots.begin();
			const_iterator itEnd = s.m_connected_slots.end();
			
			while (it != itEnd) {
				(*it)->getdest()->signalConnect(this);
				m_connected_slots.push_back((*it)->clone());
				
				++it;
			}
		}
		
		void slot_duplicate(const HasSlots<mt_policy>* oldtarget, HasSlots<mt_policy>* newtarget) {
			lock_block<mt_policy> lock(this);
			iterator it = m_connected_slots.begin();
			iterator itEnd = m_connected_slots.end();
			
			while (it != itEnd) {
				if ((*it)->getdest() == oldtarget) {
					m_connected_slots.push_back((*it)->duplicate(newtarget));
				}
				
				++it;
			}
		}
		
		~_signal_base8() {
			disconnectAll();
		}
		
		void disconnectAll() {
			lock_block<mt_policy> lock(this);
			const_iterator it = m_connected_slots.begin();
			const_iterator itEnd = m_connected_slots.end();
			
			while (it != itEnd) {
				(*it)->getdest()->signalDisconnect(this);
				delete *it;
				
				++it;
			}
			
			m_connected_slots.erase(m_connected_slots.begin(), m_connected_slots.end());
		}
		
		void disconnect(HasSlots<mt_policy>* pclass) {
			lock_block<mt_policy> lock(this);
			iterator it = m_connected_slots.begin();
			iterator itEnd = m_connected_slots.end();
			
			while (it != itEnd) {
				if ((*it)->getdest() == pclass) {
					delete *it;
					m_connected_slots.erase(iterator(it++));
					pclass->signalDisconnect(this);
					continue;
				}
				
				++it;
			}
		}
		
		void slot_disconnect(HasSlots<mt_policy>* pslot) {
			lock_block<mt_policy> lock(this);
			iterator it = m_connected_slots.begin();
			iterator itEnd = m_connected_slots.end();
			
			while (it != itEnd) {
				iterator itNext = it;
				++itNext;
				
				if ((*it)->getdest() == pslot) {
					delete *it;
					m_connected_slots.erase(it);
				}
				
				it = itNext;
			}
		}
		
	protected:
		connections_list m_connected_slots;
	};
	
	
	template<class dest_type, class mt_policy>
	class _connection0 : public _connection_base0<mt_policy> {
	public:
		_connection0() {
			this->pobject = NULL;
			this->pmemfun = NULL;
		}
		
		_connection0(dest_type *pobject, void (dest_type::*pmemfun)()) {
			m_pobject = pobject;
			m_pmemfun = pmemfun;
		}
		
		virtual _connection_base0<mt_policy>* clone() {
			return new _connection0<dest_type, mt_policy>(*this);
		}
		
		virtual _connection_base0<mt_policy>* duplicate(HasSlots<mt_policy>* pnewdest) {
			return new _connection0<dest_type, mt_policy>((dest_type *)pnewdest, m_pmemfun);
		}
		
		virtual void shoot() {
			(m_pobject->*m_pmemfun)();
		}
		
		virtual HasSlots<mt_policy>* getdest() const {
			return m_pobject;
		}
		
	private:
		dest_type *m_pobject;
		void (dest_type::* m_pmemfun)();
	};
	
	template<class dest_type, class arg1_type, class mt_policy>
	class _connection1 : public _connection_base1<arg1_type, mt_policy> {
	public:
		_connection1() {
			this->pobject = NULL;
			this->pmemfun = NULL;
		}
		
		_connection1(dest_type *pobject, void (dest_type::*pmemfun)(arg1_type)) {
			m_pobject = pobject;
			m_pmemfun = pmemfun;
		}
		
		virtual _connection_base1<arg1_type, mt_policy>* clone() {
			return new _connection1<dest_type, arg1_type, mt_policy>(*this);
		}
		
		virtual _connection_base1<arg1_type, mt_policy>* duplicate(HasSlots<mt_policy>* pnewdest) {
			return new _connection1<dest_type, arg1_type, mt_policy>((dest_type *)pnewdest, m_pmemfun);
		}
		
		virtual void shoot(arg1_type a1) {
			(m_pobject->*m_pmemfun)(a1);
		}
		
		virtual HasSlots<mt_policy>* getdest() const {
			return m_pobject;
		}
		
	private:
		dest_type *m_pobject;
		void (dest_type::* m_pmemfun)(arg1_type);
	};
	
	template<class dest_type, class arg1_type, class arg2_type, class mt_policy>
	class _connection2 : public _connection_base2<arg1_type, arg2_type, mt_policy> {
	public:
		_connection2() {
			this->pobject = NULL;
			this->pmemfun = NULL;
		}
		
		_connection2(dest_type *pobject, void (dest_type::*pmemfun)(arg1_type,
		                                                            arg2_type)) {
			m_pobject = pobject;
			m_pmemfun = pmemfun;
		}
		
		virtual _connection_base2<arg1_type, arg2_type, mt_policy>* clone() {
			return new _connection2<dest_type, arg1_type, arg2_type, mt_policy>(*this);
		}
		
		virtual _connection_base2<arg1_type, arg2_type, mt_policy>* duplicate(HasSlots<mt_policy>* pnewdest) {
			return new _connection2<dest_type, arg1_type, arg2_type, mt_policy>((dest_type *)pnewdest, m_pmemfun);
		}
		
		virtual void shoot(arg1_type a1, arg2_type a2) {
			(m_pobject->*m_pmemfun)(a1, a2);
		}
		
		virtual HasSlots<mt_policy>* getdest() const {
			return m_pobject;
		}
		
	private:
		dest_type *m_pobject;
		void (dest_type::* m_pmemfun)(arg1_type, arg2_type);
	};
	
	template<class dest_type, class arg1_type, class arg2_type, class arg3_type, class mt_policy>
	class _connection3 : public _connection_base3<arg1_type, arg2_type, arg3_type, mt_policy> {
	public:
		_connection3() {
			this->pobject = NULL;
			this->pmemfun = NULL;
		}
		
		_connection3(dest_type *pobject, void (dest_type::*pmemfun)(arg1_type,
		                                                            arg2_type, arg3_type)) {
			m_pobject = pobject;
			m_pmemfun = pmemfun;
		}
		
		virtual _connection_base3<arg1_type, arg2_type, arg3_type, mt_policy>* clone() {
			return new _connection3<dest_type, arg1_type, arg2_type, arg3_type, mt_policy>(*this);
		}
		
		virtual _connection_base3<arg1_type, arg2_type, arg3_type, mt_policy>* duplicate(HasSlots<mt_policy>* pnewdest) {
			return new _connection3<dest_type, arg1_type, arg2_type, arg3_type, mt_policy>((dest_type *)pnewdest, m_pmemfun);
		}
		
		virtual void shoot(arg1_type a1, arg2_type a2, arg3_type a3) {
			(m_pobject->*m_pmemfun)(a1, a2, a3);
		}
		
		virtual HasSlots<mt_policy>* getdest() const {
			return m_pobject;
		}
		
	private:
		dest_type *m_pobject;
		void (dest_type::* m_pmemfun)(arg1_type, arg2_type, arg3_type);
	};
	
	template < class dest_type, class arg1_type, class arg2_type, class arg3_type,
	class arg4_type, class mt_policy >
	class _connection4 : public _connection_base4 < arg1_type, arg2_type,
	arg3_type, arg4_type, mt_policy > {
	public:
		_connection4() {
			this->pobject = NULL;
			this->pmemfun = NULL;
		}
		
		_connection4(dest_type *pobject, void (dest_type::*pmemfun)(arg1_type,
		                                                            arg2_type, arg3_type, arg4_type)) {
			m_pobject = pobject;
			m_pmemfun = pmemfun;
		}
		
		virtual _connection_base4<arg1_type, arg2_type, arg3_type, arg4_type, mt_policy>* clone() {
			return new _connection4<dest_type, arg1_type, arg2_type, arg3_type, arg4_type, mt_policy>(*this);
		}
		
		virtual _connection_base4<arg1_type, arg2_type, arg3_type, arg4_type, mt_policy>* duplicate(HasSlots<mt_policy>* pnewdest) {
			return new _connection4<dest_type, arg1_type, arg2_type, arg3_type, arg4_type, mt_policy>((dest_type *)pnewdest, m_pmemfun);
		}
		
		virtual void shoot(arg1_type a1, arg2_type a2, arg3_type a3,
		                   arg4_type a4) {
			(m_pobject->*m_pmemfun)(a1, a2, a3, a4);
		}
		
		virtual HasSlots<mt_policy>* getdest() const {
			return m_pobject;
		}
		
	private:
		dest_type *m_pobject;
		void (dest_type::* m_pmemfun)(arg1_type, arg2_type, arg3_type,
		                              arg4_type);
	};
	
	template < class dest_type, class arg1_type, class arg2_type, class arg3_type,
	class arg4_type, class arg5_type, class mt_policy >
	class _connection5 : public _connection_base5 < arg1_type, arg2_type,
	arg3_type, arg4_type, arg5_type, mt_policy > {
	public:
		_connection5() {
			this->pobject = NULL;
			this->pmemfun = NULL;
		}
		
		_connection5(dest_type *pobject, void (dest_type::*pmemfun)(arg1_type,
		                                                            arg2_type, arg3_type, arg4_type, arg5_type)) {
			m_pobject = pobject;
			m_pmemfun = pmemfun;
		}
		
		virtual _connection_base5 < arg1_type, arg2_type, arg3_type, arg4_type,
		arg5_type, mt_policy > * clone() {
			return new _connection5 < dest_type, arg1_type, arg2_type, arg3_type, arg4_type,
			arg5_type, mt_policy > (*this);
		}
		
		virtual _connection_base5 < arg1_type, arg2_type, arg3_type, arg4_type,
		arg5_type, mt_policy > * duplicate(HasSlots<mt_policy>* pnewdest) {
			return new _connection5 < dest_type, arg1_type, arg2_type, arg3_type, arg4_type,
			arg5_type, mt_policy > ((dest_type *)pnewdest, m_pmemfun);
		}
		
		virtual void shoot(arg1_type a1, arg2_type a2, arg3_type a3, arg4_type a4,
						   arg5_type a5) {
			(m_pobject->*m_pmemfun)(a1, a2, a3, a4, a5);
		}
		
		virtual HasSlots<mt_policy>* getdest() const {
			return m_pobject;
		}
		
	private:
		dest_type *m_pobject;
		void (dest_type::* m_pmemfun)(arg1_type, arg2_type, arg3_type, arg4_type,
		                              arg5_type);
	};
	
	template < class dest_type, class arg1_type, class arg2_type, class arg3_type,
	class arg4_type, class arg5_type, class arg6_type, class mt_policy >
	class _connection6 : public _connection_base6 < arg1_type, arg2_type,
	arg3_type, arg4_type, arg5_type, arg6_type, mt_policy > {
	public:
		_connection6() {
			this->pobject = NULL;
			this->pmemfun = NULL;
		}
		
		_connection6(dest_type *pobject, void (dest_type::*pmemfun)(arg1_type,
		                                                            arg2_type, arg3_type, arg4_type, arg5_type, arg6_type)) {
			m_pobject = pobject;
			m_pmemfun = pmemfun;
		}
		
		virtual _connection_base6 < arg1_type, arg2_type, arg3_type, arg4_type,
		arg5_type, arg6_type, mt_policy > * clone() {
			return new _connection6 < dest_type, arg1_type, arg2_type, arg3_type, arg4_type,
			arg5_type, arg6_type, mt_policy > (*this);
		}
		
		virtual _connection_base6 < arg1_type, arg2_type, arg3_type, arg4_type,
		arg5_type, arg6_type, mt_policy > * duplicate(HasSlots<mt_policy>* pnewdest) {
			return new _connection6 < dest_type, arg1_type, arg2_type, arg3_type, arg4_type,
			arg5_type, arg6_type, mt_policy > ((dest_type *)pnewdest, m_pmemfun);
		}
		
		virtual void shoot(arg1_type a1, arg2_type a2, arg3_type a3, arg4_type a4,
						   arg5_type a5, arg6_type a6) {
			(m_pobject->*m_pmemfun)(a1, a2, a3, a4, a5, a6);
		}
		
		virtual HasSlots<mt_policy>* getdest() const {
			return m_pobject;
		}
		
	private:
		dest_type *m_pobject;
		void (dest_type::* m_pmemfun)(arg1_type, arg2_type, arg3_type, arg4_type,
		                              arg5_type, arg6_type);
	};
	
	template < class dest_type, class arg1_type, class arg2_type, class arg3_type,
	class arg4_type, class arg5_type, class arg6_type, class arg7_type, class mt_policy >
	class _connection7 : public _connection_base7 < arg1_type, arg2_type,
	arg3_type, arg4_type, arg5_type, arg6_type, arg7_type, mt_policy > {
	public:
		_connection7() {
			this->pobject = NULL;
			this->pmemfun = NULL;
		}
		
		_connection7(dest_type *pobject, void (dest_type::*pmemfun)(arg1_type,
		                                                            arg2_type, arg3_type, arg4_type, arg5_type, arg6_type, arg7_type)) {
			m_pobject = pobject;
			m_pmemfun = pmemfun;
		}
		
		virtual _connection_base7 < arg1_type, arg2_type, arg3_type, arg4_type,
		arg5_type, arg6_type, arg7_type, mt_policy > * clone() {
			return new _connection7 < dest_type, arg1_type, arg2_type, arg3_type, arg4_type,
			arg5_type, arg6_type, arg7_type, mt_policy > (*this);
		}
		
		virtual _connection_base7 < arg1_type, arg2_type, arg3_type, arg4_type,
		arg5_type, arg6_type, arg7_type, mt_policy > * duplicate(HasSlots<mt_policy>* pnewdest) {
			return new _connection7 < dest_type, arg1_type, arg2_type, arg3_type, arg4_type,
			arg5_type, arg6_type, arg7_type, mt_policy > ((dest_type *)pnewdest, m_pmemfun);
		}
		
		virtual void shoot(arg1_type a1, arg2_type a2, arg3_type a3, arg4_type a4,
						   arg5_type a5, arg6_type a6, arg7_type a7) {
			(m_pobject->*m_pmemfun)(a1, a2, a3, a4, a5, a6, a7);
		}
		
		virtual HasSlots<mt_policy>* getdest() const {
			return m_pobject;
		}
		
	private:
		dest_type *m_pobject;
		void (dest_type::* m_pmemfun)(arg1_type, arg2_type, arg3_type, arg4_type,
		                              arg5_type, arg6_type, arg7_type);
	};
	
	template < class dest_type, class arg1_type, class arg2_type, class arg3_type,
	class arg4_type, class arg5_type, class arg6_type, class arg7_type,
	class arg8_type, class mt_policy >
	class _connection8 : public _connection_base8 < arg1_type, arg2_type,
	arg3_type, arg4_type, arg5_type, arg6_type, arg7_type, arg8_type, mt_policy > {
	public:
		_connection8() {
			this->pobject = NULL;
			this->pmemfun = NULL;
		}
		
		_connection8(dest_type *pobject, void (dest_type::*pmemfun)(arg1_type,
		                                                            arg2_type, arg3_type, arg4_type, arg5_type, arg6_type,
		                                                            arg7_type, arg8_type)) {
			m_pobject = pobject;
			m_pmemfun = pmemfun;
		}
		
		virtual _connection_base8 < arg1_type, arg2_type, arg3_type, arg4_type,
		arg5_type, arg6_type, arg7_type, arg8_type, mt_policy > * clone() {
			return new _connection8 < dest_type, arg1_type, arg2_type, arg3_type, arg4_type,
			arg5_type, arg6_type, arg7_type, arg8_type, mt_policy > (*this);
		}
		
		virtual _connection_base8 < arg1_type, arg2_type, arg3_type, arg4_type,
		arg5_type, arg6_type, arg7_type, arg8_type, mt_policy > * duplicate(HasSlots<mt_policy>* pnewdest) {
			return new _connection8 < dest_type, arg1_type, arg2_type, arg3_type, arg4_type,
			arg5_type, arg6_type, arg7_type, arg8_type, mt_policy > ((dest_type *)pnewdest, m_pmemfun);
		}
		
		virtual void shoot(arg1_type a1, arg2_type a2, arg3_type a3, arg4_type a4,
						   arg5_type a5, arg6_type a6, arg7_type a7, arg8_type a8) {
			(m_pobject->*m_pmemfun)(a1, a2, a3, a4, a5, a6, a7, a8);
		}
		
		virtual HasSlots<mt_policy>* getdest() const {
			return m_pobject;
		}
		
	private:
		dest_type *m_pobject;
		void (dest_type::* m_pmemfun)(arg1_type, arg2_type, arg3_type, arg4_type,
		                              arg5_type, arg6_type, arg7_type, arg8_type);
	};
	
	template<class mt_policy = SIGLY_DEFAULT_MT_POLICY>
	class Signal0 : public _signal_base0<mt_policy> {
	public:
		typedef typename _signal_base0<mt_policy>::connections_list::const_iterator const_iterator;
		Signal0() {
			;
		}
		
		Signal0(const Signal0<mt_policy>& s)
		: _signal_base0<mt_policy>(s) {
			;
		}
		
		template<class desttype>
		void connect(desttype *pclass, void (desttype::*pmemfun)()) {
			lock_block<mt_policy> lock(this);
			_connection0<desttype, mt_policy>* conn =
			new _connection0<desttype, mt_policy>(pclass, pmemfun);
			this->m_connected_slots.push_back(conn);
			pclass->signalConnect(this);
		}
		
		void shoot() {
			lock_block<mt_policy> lock(this);
			const_iterator it = this->m_connected_slots.begin();
			const_iterator itEnd = this->m_connected_slots.end();
			
			while (it != itEnd) {
				if ((*it)->getdest()->areSlotsActive()) {
					(*it)->shoot();
				}
				++it;
			}
		}
		
		void operator()() {
			lock_block<mt_policy> lock(this);
			const_iterator it = this->m_connected_slots.begin();
			const_iterator itEnd = this->m_connected_slots.end();
			
			while (it != itEnd) {
				if ((*it)->getdest()->areSlotsActive()) {
					(*it)->shoot();
				}
				++it;
			}
		}
	};
	
	template<class arg1_type, class mt_policy = SIGLY_DEFAULT_MT_POLICY>
	class Signal1 : public _signal_base1<arg1_type, mt_policy> {
	public:
		typedef typename _signal_base1<arg1_type, mt_policy>::connections_list::const_iterator const_iterator;
		Signal1() {
			;
		}
		
		Signal1(const Signal1<arg1_type, mt_policy>& s)
		: _signal_base1<arg1_type, mt_policy>(s) {
			;
		}
		
		template<class desttype>
		void connect(desttype *pclass, void (desttype::*pmemfun)(arg1_type)) {
			lock_block<mt_policy> lock(this);
			_connection1<desttype, arg1_type, mt_policy>* conn =
			new _connection1<desttype, arg1_type, mt_policy>(pclass, pmemfun);
			this->m_connected_slots.push_back(conn);
			pclass->signalConnect(this);
		}
		
		void shoot(arg1_type a1) {
			lock_block<mt_policy> lock(this);
			const_iterator  it = this->m_connected_slots.begin();
			const_iterator itEnd = this->m_connected_slots.end();
			
			while (it != itEnd) {
				if ((*it)->getdest()->areSlotsActive()) {
					(*it)->shoot(a1);
				}
				++it;
			}
		}
		
		void operator()(arg1_type a1) {
			lock_block<mt_policy> lock(this);
			const_iterator it = this->m_connected_slots.begin();
			const_iterator itEnd = this->m_connected_slots.end();
			
			while (it != itEnd) {
				if ((*it)->getdest()->areSlotsActive()) {
					(*it)->shoot(a1);
				}
				++it;
			}
		}
	};
	
	template<class arg1_type, typename arg2_type, typename mt_policy = SIGLY_DEFAULT_MT_POLICY>
	class Signal2 : public _signal_base2<arg1_type, arg2_type, mt_policy> {
	public:
		typedef typename _signal_base2<arg1_type, arg2_type, mt_policy>::connections_list::const_iterator const_iterator;
		Signal2() {
			;
		}
		
		Signal2(const Signal2<arg1_type, arg2_type, mt_policy>& s)
		: _signal_base2<arg1_type, arg2_type, mt_policy>(s) {
			;
		}
		
		template<class desttype>
		void connect(desttype *pclass, void (desttype::*pmemfun)(arg1_type,
		                                                         arg2_type)) {
			lock_block<mt_policy> lock(this);
			_connection2<desttype, arg1_type, arg2_type, mt_policy>* conn = new
			_connection2<desttype, arg1_type, arg2_type, mt_policy>(pclass, pmemfun);
			this->m_connected_slots.push_back(conn);
			pclass->signalConnect(this);
		}
		
		void shoot(arg1_type a1, arg2_type a2) {
			lock_block<mt_policy> lock(this);
			const_iterator it = this->m_connected_slots.begin();
			const_iterator itEnd = this->m_connected_slots.end();
			
			while (it != itEnd) {
				if ((*it)->getdest()->areSlotsActive()) {
					(*it)->shoot(a1, a2);
				}
				++it;
			}
		}
		
		void operator()(arg1_type a1, arg2_type a2) {
			lock_block<mt_policy> lock(this);
			const_iterator it = this->m_connected_slots.begin();
			const_iterator itEnd = this->m_connected_slots.end();
			
			while (it != itEnd) {
				if ((*it)->getdest()->areSlotsActive()) {
					(*it)->shoot(a1, a2);
				}
				++it;
			}
		}
	};
	
	template<class arg1_type, typename arg2_type, typename arg3_type, typename mt_policy = SIGLY_DEFAULT_MT_POLICY>
	class Signal3 : public _signal_base3<arg1_type, arg2_type, arg3_type, mt_policy> {
	public:
		typedef typename _signal_base3<arg1_type, arg2_type, arg3_type, mt_policy>::connections_list::const_iterator const_iterator;
		Signal3() {
			;
		}
		
		Signal3(const Signal3<arg1_type, arg2_type, arg3_type, mt_policy>& s)
		: _signal_base3<arg1_type, arg2_type, arg3_type, mt_policy>(s) {
			;
		}
		
		template<class desttype>
		void connect(desttype *pclass, void (desttype::*pmemfun)(arg1_type,
		                                                         arg2_type, arg3_type)) {
			lock_block<mt_policy> lock(this);
			_connection3<desttype, arg1_type, arg2_type, arg3_type, mt_policy>* conn =
			new _connection3<desttype, arg1_type, arg2_type, arg3_type, mt_policy>(pclass,
																				   pmemfun);
			this->m_connected_slots.push_back(conn);
			pclass->signalConnect(this);
		}
		
		void shoot(arg1_type a1, arg2_type a2, arg3_type a3) {
			lock_block<mt_policy> lock(this);
			const_iterator it = this->m_connected_slots.begin();
			const_iterator itEnd = this->m_connected_slots.end();
			
			while (it != itEnd) {
				if ((*it)->getdest()->areSlotsActive()) {
					(*it)->shoot(a1, a2, a3);
				}
				++it;
			}
		}
		
		void operator()(arg1_type a1, arg2_type a2, arg3_type a3) {
			lock_block<mt_policy> lock(this);
			const_iterator it = this->m_connected_slots.begin();
			const_iterator itEnd = this->m_connected_slots.end();
			
			while (it != itEnd) {
				if ((*it)->getdest()->areSlotsActive()) {
					(*it)->shoot(a1, a2, a3);
				}
				++it;
			}
		}
	};
	
	template<class arg1_type, class arg2_type, class arg3_type, class arg4_type, class mt_policy = SIGLY_DEFAULT_MT_POLICY>
	class Signal4 : public _signal_base4 < arg1_type, arg2_type, arg3_type,
	arg4_type, mt_policy > {
	public:
		typedef typename _signal_base4<arg1_type, arg2_type, arg3_type, arg4_type, mt_policy>::connections_list::const_iterator const_iterator;
		Signal4() {
			;
		}
		
		Signal4(const Signal4<arg1_type, arg2_type, arg3_type, arg4_type, mt_policy>& s)
		: _signal_base4<arg1_type, arg2_type, arg3_type, arg4_type, mt_policy>(s) {
			;
		}
		
		template<class desttype>
		void connect(desttype *pclass, void (desttype::*pmemfun)(arg1_type,
		                                                         arg2_type, arg3_type, arg4_type)) {
			lock_block<mt_policy> lock(this);
			_connection4<desttype, arg1_type, arg2_type, arg3_type, arg4_type, mt_policy>*
			conn = new _connection4 < desttype, arg1_type, arg2_type, arg3_type,
			arg4_type, mt_policy > (pclass, pmemfun);
			this->m_connected_slots.push_back(conn);
			pclass->signalConnect(this);
		}
		
		void shoot(arg1_type a1, arg2_type a2, arg3_type a3, arg4_type a4) {
			lock_block<mt_policy> lock(this);
			const_iterator it = this->m_connected_slots.begin();
			const_iterator itEnd = this->m_connected_slots.end();
			
			while (it != itEnd) {
				if ((*it)->getdest()->areSlotsActive()) {
					(*it)->shoot(a1, a2, a3, a4);
				}
				++it;
			}
		}
		
		void operator()(arg1_type a1, arg2_type a2, arg3_type a3, arg4_type a4) {
			lock_block<mt_policy> lock(this);
			const_iterator it = this->m_connected_slots.begin();
			const_iterator itEnd = this->m_connected_slots.end();
			
			while (it != itEnd) {
				if ((*it)->getdest()->areSlotsActive()) {
					(*it)->shoot(a1, a2, a3, a4);
				}
				++it;
			}
		}
	};
	
	template < class arg1_type, class arg2_type, class arg3_type, class arg4_type,
	class arg5_type, class mt_policy = SIGLY_DEFAULT_MT_POLICY >
	class Signal5 : public _signal_base5 < arg1_type, arg2_type, arg3_type,
	arg4_type, arg5_type, mt_policy > {
	public:
		typedef typename _signal_base5<arg1_type, arg2_type, arg3_type, arg4_type, arg5_type, mt_policy>::connections_list::const_iterator const_iterator;
		Signal5() {
			;
		}
		
		Signal5(const Signal5 < arg1_type, arg2_type, arg3_type, arg4_type,
		        arg5_type, mt_policy > & s)
		: _signal_base5 < arg1_type, arg2_type, arg3_type, arg4_type,
		arg5_type, mt_policy > (s) {
			;
		}
		
		template<class desttype>
		void connect(desttype *pclass, void (desttype::*pmemfun)(arg1_type,
		                                                         arg2_type, arg3_type, arg4_type, arg5_type)) {
			lock_block<mt_policy> lock(this);
			_connection5 < desttype, arg1_type, arg2_type, arg3_type, arg4_type,
			arg5_type, mt_policy > * conn = new _connection5 < desttype, arg1_type, arg2_type,
			arg3_type, arg4_type, arg5_type, mt_policy > (pclass, pmemfun);
			this->m_connected_slots.push_back(conn);
			pclass->signalConnect(this);
		}
		
		void shoot(arg1_type a1, arg2_type a2, arg3_type a3, arg4_type a4,
		           arg5_type a5) {
			lock_block<mt_policy> lock(this);
			const_iterator it = this->m_connected_slots.begin();
			const_iterator itEnd = this->m_connected_slots.end();
			
			while (it != itEnd) {
				if ((*it)->getdest()->areSlotsActive()) {
					(*it)->shoot(a1, a2, a3, a4, a5);
				}
				++it;
			}
		}
		
		void operator()(arg1_type a1, arg2_type a2, arg3_type a3, arg4_type a4,
		                arg5_type a5) {
			lock_block<mt_policy> lock(this);
			const_iterator it = this->m_connected_slots.begin();
			const_iterator itEnd = this->m_connected_slots.end();
			
			while (it != itEnd) {
				if ((*it)->getdest()->areSlotsActive()) {
					(*it)->shoot(a1, a2, a3, a4, a5);
				}
				++it;
			}
		}
	};
	
	
	template < class arg1_type, class arg2_type, class arg3_type, class arg4_type,
	class arg5_type, class arg6_type, class mt_policy = SIGLY_DEFAULT_MT_POLICY >
	class Signal6 : public _signal_base6 < arg1_type, arg2_type, arg3_type,
	arg4_type, arg5_type, arg6_type, mt_policy > {
	public:
		typedef typename _signal_base6 < arg1_type, arg2_type, arg3_type,
		arg4_type, arg5_type, arg6_type, mt_policy >::connections_list::const_iterator const_iterator;
		Signal6() {
			;
		}
		
		Signal6(const Signal6 < arg1_type, arg2_type, arg3_type, arg4_type,
		        arg5_type, arg6_type, mt_policy > & s)
		: _signal_base6 < arg1_type, arg2_type, arg3_type, arg4_type,
		arg5_type, arg6_type, mt_policy > (s) {
			;
		}
		
		template<class desttype>
		void connect(desttype *pclass, void (desttype::*pmemfun)(arg1_type,
		                                                         arg2_type, arg3_type, arg4_type, arg5_type, arg6_type)) {
			lock_block<mt_policy> lock(this);
			_connection6 < desttype, arg1_type, arg2_type, arg3_type, arg4_type,
			arg5_type, arg6_type, mt_policy > * conn =
			new _connection6 < desttype, arg1_type, arg2_type, arg3_type,
			arg4_type, arg5_type, arg6_type, mt_policy > (pclass, pmemfun);
			this->m_connected_slots.push_back(conn);
			pclass->signalConnect(this);
		}
		
		void shoot(arg1_type a1, arg2_type a2, arg3_type a3, arg4_type a4,
		           arg5_type a5, arg6_type a6) {
			lock_block<mt_policy> lock(this);
			const_iterator it = this->m_connected_slots.begin();
			const_iterator itEnd = this->m_connected_slots.end();
			
			while (it != itEnd) {
				if ((*it)->getdest()->areSlotsActive()) {
					(*it)->shoot(a1, a2, a3, a4, a5, a6);
				}
				++it;
			}
		}
		
		void operator()(arg1_type a1, arg2_type a2, arg3_type a3, arg4_type a4,
		                arg5_type a5, arg6_type a6) {
			lock_block<mt_policy> lock(this);
			const_iterator it = this->m_connected_slots.begin();
			const_iterator itEnd = this->m_connected_slots.end();
			
			while (it != itEnd) {
				if ((*it)->getdest()->areSlotsActive()) {
					(*it)->shoot(a1, a2, a3, a4, a5, a6);
				}
				++it;
			}
		}
	};
	
	template < class arg1_type, class arg2_type, class arg3_type, class arg4_type,
	class arg5_type, class arg6_type, class arg7_type, class mt_policy = SIGLY_DEFAULT_MT_POLICY >
	class Signal7 : public _signal_base7 < arg1_type, arg2_type, arg3_type,
	arg4_type, arg5_type, arg6_type, arg7_type, mt_policy > {
	public:
		typedef typename _signal_base7 < arg1_type, arg2_type, arg3_type,
		arg4_type, arg5_type, arg6_type, arg7_type, mt_policy >::connections_list::const_iterator const_iterator;
		Signal7() {
			;
		}
		
		Signal7(const Signal7 < arg1_type, arg2_type, arg3_type, arg4_type,
		        arg5_type, arg6_type, arg7_type, mt_policy > & s)
		: _signal_base7 < arg1_type, arg2_type, arg3_type, arg4_type,
		arg5_type, arg6_type, arg7_type, mt_policy > (s) {
			;
		}
		
		template<class desttype>
		void connect(desttype *pclass, void (desttype::*pmemfun)(arg1_type,
		                                                         arg2_type, arg3_type, arg4_type, arg5_type, arg6_type,
		                                                         arg7_type)) {
			lock_block<mt_policy> lock(this);
			_connection7 < desttype, arg1_type, arg2_type, arg3_type, arg4_type,
			arg5_type, arg6_type, arg7_type, mt_policy > * conn =
			new _connection7 < desttype, arg1_type, arg2_type, arg3_type,
			arg4_type, arg5_type, arg6_type, arg7_type, mt_policy > (pclass, pmemfun);
			this->m_connected_slots.push_back(conn);
			pclass->signalConnect(this);
		}
		
		void shoot(arg1_type a1, arg2_type a2, arg3_type a3, arg4_type a4,
		           arg5_type a5, arg6_type a6, arg7_type a7) {
			lock_block<mt_policy> lock(this);
			const_iterator it = this->m_connected_slots.begin();
			const_iterator itEnd = this->m_connected_slots.end();
			
			while (it != itEnd) {
				if ((*it)->getdest()->areSlotsActive()) {
					(*it)->shoot(a1, a2, a3, a4, a5, a6, a7);
				}
				++it;
			}
		}
		
		void operator()(arg1_type a1, arg2_type a2, arg3_type a3, arg4_type a4,
		                arg5_type a5, arg6_type a6, arg7_type a7) {
			lock_block<mt_policy> lock(this);
			const_iterator it = this->m_connected_slots.begin();
			const_iterator itEnd = this->m_connected_slots.end();
			
			while (it != itEnd) {
				if ((*it)->getdest()->areSlotsActive()) {
					(*it)->shoot(a1, a2, a3, a4, a5, a6, a7);
				}
				++it;
			}
		}
	};
	
	template < class arg1_type, class arg2_type, class arg3_type, class arg4_type,
	class arg5_type, class arg6_type, class arg7_type, class arg8_type, class mt_policy = SIGLY_DEFAULT_MT_POLICY >
	class Signal8 : public _signal_base8 < arg1_type, arg2_type, arg3_type,
	arg4_type, arg5_type, arg6_type, arg7_type, arg8_type, mt_policy > {
	public:
		typedef typename _signal_base8 < arg1_type, arg2_type, arg3_type,
		arg4_type, arg5_type, arg6_type, arg7_type, arg8_type, mt_policy >::connections_list::const_iterator const_iterator;
		Signal8() {
			;
		}
		
		Signal8(const Signal8 < arg1_type, arg2_type, arg3_type, arg4_type,
		        arg5_type, arg6_type, arg7_type, arg8_type, mt_policy > & s)
		: _signal_base8 < arg1_type, arg2_type, arg3_type, arg4_type,
		arg5_type, arg6_type, arg7_type, arg8_type, mt_policy > (s) {
			;
		}
		
		template<class desttype>
		void connect(desttype *pclass, void (desttype::*pmemfun)(arg1_type,
		                                                         arg2_type, arg3_type, arg4_type, arg5_type, arg6_type,
		                                                         arg7_type, arg8_type)) {
			lock_block<mt_policy> lock(this);
			_connection8 < desttype, arg1_type, arg2_type, arg3_type, arg4_type,
			arg5_type, arg6_type, arg7_type, arg8_type, mt_policy > * conn =
			new _connection8 < desttype, arg1_type, arg2_type, arg3_type,
			arg4_type, arg5_type, arg6_type, arg7_type,
			arg8_type, mt_policy > (pclass, pmemfun);
			this->m_connected_slots.push_back(conn);
			pclass->signalConnect(this);
		}
		
		void shoot(arg1_type a1, arg2_type a2, arg3_type a3, arg4_type a4,
		           arg5_type a5, arg6_type a6, arg7_type a7, arg8_type a8) {
			lock_block<mt_policy> lock(this);
			const_iterator it = this->m_connected_slots.begin();
			const_iterator itEnd = this->m_connected_slots.end();
			
			while (it != itEnd) {
				if ((*it)->getdest()->areSlotsActive()) {
					(*it)->shoot(a1, a2, a3, a4, a5, a6, a7, a8);
				}
				++it;
			}
		}
		
		void operator()(arg1_type a1, arg2_type a2, arg3_type a3, arg4_type a4,
		                arg5_type a5, arg6_type a6, arg7_type a7, arg8_type a8) {
			lock_block<mt_policy> lock(this);
			const_iterator it = this->m_connected_slots.begin();
			const_iterator itEnd = this->m_connected_slots.end();
			
			while (it != itEnd) {
				if ((*it)->getdest()->areSlotsActive()) {
					(*it)->shoot(a1, a2, a3, a4, a5, a6, a7, a8);
				}
				++it;
			}
		}
	};
	
} // namespace sigly

#endif // SIGLY_H__
