//
//  TFWorkerDispatch.h
//  TFFIXEngine
//
//  Created by Tom Fewster on 09/04/2013.
//  Copyright (c) 2013 Tom Fewster. All rights reserved.
//

#ifndef __TFFIXEngine__TFWorkerDispatch__
#define __TFFIXEngine__TFWorkerDispatch__

#include "TFSpinLock.h"
#ifdef __APPLE__
#	include <mach/semaphore.h>
#else
#	include <semaphore.h>
#endif
#include <thread>
#include <functional>
#include <deque>
#include <vector>

class TFWorkerDispatch {
private:
	TFWorkerDispatch(const TFWorkerDispatch &);
	TFWorkerDispatch &operator=(const TFWorkerDispatch &rhs);

	void run();

	TFSpinLock m_lock;
#ifdef __APPLE__
	semaphore_t m_semaphore;
#else
	sem_t m_semaphore;
#endif

	std::chrono::milliseconds m_terminationTimeout = std::chrono::milliseconds(0);

	std::vector<std::thread *> m_workers;
	std::deque<std::function<void()>> m_tasks;

	std::atomic<bool> m_terminating = ATOMIC_VAR_INIT(false);
public:
	TFWorkerDispatch(size_t initialThreads = std::thread::hardware_concurrency());
	~TFWorkerDispatch();

	bool queueTask(std::function<void()> task);

	// configuration methods
	// How long do we wait for out worker queue to subside before forcing termination?
	std::chrono::milliseconds terminationTimeout() const { return m_terminationTimeout; }
	void terminationTimeout(const std::chrono::milliseconds &duration);
};

#endif /* defined(__TFFIXEngine__TFWorkerDispatch__) */
