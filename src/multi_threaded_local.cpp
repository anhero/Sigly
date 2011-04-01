#include <sigly/multi_threaded_local.h>

#if defined(_SIGLY_HAS_WIN32_THREADS) || defined(_SIGLY_HAS_POSIX_THREADS)
namespace sigly {
#ifdef _SIGLY_HAS_WIN32_THREADS
MultiThreadedLocal::MultiThreadedLocal() {
	InitializeCriticalSection(&m_critsec);
}

MultiThreadedLocal::MultiThreadedLocal(const MultiThreadedLocal&) {
	InitializeCriticalSection(&m_critsec);
}

MultiThreadedLocal::~MultiThreadedLocal() {
	DeleteCriticalSection(&m_critsec);
}

void MultiThreadedLocal::lock() {
	EnterCriticalSection(&m_critsec);
}

void MultiThreadedLocal::unlock() {
	LeaveCriticalSection(&m_critsec);
}
#endif // _SIGLY_HAS_WIN32_THREADS

#ifdef _SIGLY_HAS_POSIX_THREADS
MultiThreadedLocal::MultiThreadedLocal() {
	pthread_mutex_init(&m_mutex, NULL);
}

MultiThreadedLocal::MultiThreadedLocal(const MultiThreadedLocal&) {
	pthread_mutex_init(&m_mutex, NULL);
}

MultiThreadedLocal::~MultiThreadedLocal() {
	pthread_mutex_destroy(&m_mutex);
}

void MultiThreadedLocal::lock() {
	pthread_mutex_lock(&m_mutex);
}

void MultiThreadedLocal::unlock() {
	pthread_mutex_unlock(&m_mutex);
}
#endif
}
#endif // defined(_SIGLY_HAS_WIN32_THREADS) || defined(_SIGLY_HAS_POSIX_THREADS)