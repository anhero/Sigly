#ifndef SIGLY_SIGLY_SINGLE_THREADED_H_
#define SIGLY_SIGLY_SINGLE_THREADED_H_

namespace sigly {
class SingleThreaded {
public:
	SingleThreaded();

	virtual ~SingleThreaded();

	virtual void lock();

	virtual void unlock();
};
}

#endif // SIGLY_SIGLY_SINGLE_THREADED_H_