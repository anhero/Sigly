#include <sigly/multi_threaded_global.h>

#if defined(_SIGLY_HAS_WIN32_THREADS) || defined(_SIGLY_HAS_POSIX_THREADS)
namespace sigly {
#ifdef _SIGLY_HAS_WIN32_THREADS
MultiThreadedGlobal::MultiThreadedGlobal() {
	static bool isInitialized = false;

	if(!isInitialized) {
		InitializeCriticalSection(get_critsec());
		isInitialized = true;
	}
}

MultiThreadedGlobal::MultiThreadedGlobal(const MultiThreadedGlobal&) {
}

MultiThreadedGlobal::~MultiThreadedGlobal() {
}

void MultiThreadedGlobal::lock() {
	EnterCriticalSection(get_critsec());
}

void MultiThreadedGlobal::unlock() {
	LeaveCriticalSection(get_critsec());
}

CRITICAL_SECTION* MultiThreadedGlobal::get_critsec() {
	static CRITICAL_SECTION g_critsec;
	return &g_critsec;
}
#endif // _SIGLY_HAS_WIN32_THREADS

#ifdef _SIGLY_HAS_POSIX_THREADS
MultiThreadedGlobal::MultiThreadedGlobal() {
	pthread_mutex_init(get_mutex(), NULL);
}

MultiThreadedGlobal::MultiThreadedGlobal(const MultiThreadedGlobal&) {
}

MultiThreadedGlobal::~MultiThreadedGlobal() {
}

void MultiThreadedGlobal::lock() {
	pthread_mutex_lock(get_mutex());
}

void MultiThreadedGlobal::unlock() {
	pthread_mutex_unlock(get_mutex());
}

pthread_mutex_t* MultiThreadedGlobal::get_mutex() {
	static pthread_mutex_t g_mutex;
	return &g_mutex;
}
#endif // _SIGLY_HAS_POSIX_THREADS
}

#endif // defined(_SIGLY_HAS_WIN32_THREADS) || defined(_SIGLY_HAS_POSIX_THREADS)