
#ifndef FIDINVENTOR_THREAD_H
#define FIDINVENTOR_THREAD_H

#include <boost/thread/thread.hpp>
#include <boost/bind.hpp>

#define DeleteThreadSafe(thd) \
do{ \
if(thd && thd->working()) \
delete thd; \
}while(0);

namespace fid {

// main   - boost::thread::sleep(boost::get_system_time() + boost::posix_time::seconds(5));
// thread - boost::this_thread::sleep(boost::posix_time::seconds(2));


class Thread {
public:
	
protected:
	boost::thread 	*m_thread;
	int		running;	// for thread running able
	
	virtual void work() = 0;

	void doWork() {
		std::cout << "Thread doWork() call..." << std::endl;
		running = 1;
		work();
	}

public:
	Thread(): m_thread(NULL), running(0) {}
	virtual ~Thread() {
		std::cout << "~Thread() call..." << std::endl;
		catchUp();
		delete m_thread;
	}

	inline void catchUp() {
		std::cout << "Thread catchUp() call..." << std::endl;
		if(m_thread != NULL) {
			std::cout << "Thread catchUp() join..." << std::endl;
			m_thread->join();
		}
	}

	inline int working() {
		return running;
	}

	void run() {
		std::cout << "Thread run() call..." << std::endl;
		m_thread = new boost::thread(boost::bind(&Thread::doWork, boost::ref(*this)));
		//boost::this_thread::sleep(boost::posix_time::seconds(1));
		std::cout << "new Thread : " << m_thread << std::endl;
		
	}
};

class TestThread: public Thread {
	void work() {
		std::cout << "TestThread work() call..." << std::endl;
		while(1) {
			if(count >5)	
				break;
			std::cout << "TestThread run... " << count++ << " count" << std::endl;
			boost::this_thread::sleep(boost::posix_time::seconds(1));
		}
	}
private:
	int count;
};

inline int deleteThreadSafe(Thread *thd) {
	do {
		if(thd && thd->working()) {
			std::cout << "delete Thread : " << thd << std::endl;
			delete thd;
			return 1;
		}
	} while(0);
	return 0;
}

}

#endif
