//
//  TFWorkerDispatch.cpp
//  TFFIXEngine
//
//  Created by Tom Fewster on 09/04/2013.
//  Copyright (c) 2013 Tom Fewster. All rights reserved.
//

#include "TFWorkerDispatch.h"
#ifdef __APPLE__
#	include <mach/task.h>
#	include <mach/mach_init.h>
#endif

#include <algorithm>

TFWorkerDispatch::TFWorkerDispatch(size_t initialThreads) {
#ifdef __APPLE__
	if (semaphore_create(mach_task_self(), &m_semaphore, SYNC_POLICY_FIFO, 0) != -1) {
#else
	if (sem_init(&m_semaphore, 0, 0) != -1) {
#endif
		for (uint16_t i = 0; i < initialThreads; i++) {
			m_workers.push_back(new std::thread(&TFWorkerDispatch::run, this));
		}
	} else {
		std::cerr << "Error initilising semaphore: " << errno << std::endl;
	}
}

TFWorkerDispatch::~TFWorkerDispatch() {
	if (m_terminationTimeout.count() != 0) {
		static const std::chrono::microseconds interval(100);
		uint32_t counter = 0;
		while (true) {
			TFSpinLockAuto lock(m_lock);
			if ( m_tasks.size() == 0 || m_terminationTimeout.count() < (interval.count() * counter) ) {
				// worker queue is now empty
				break;
			}
			std::this_thread::sleep_for(interval);
			counter++;
		}
	}
	
	m_terminating = true;
	for (uint16_t i = 0; i < m_workers.size(); i++) {
#ifdef __APPLE__
		semaphore_signal(m_semaphore);
#else
		sem_post(&m_semaphore);
#endif
	}
	std::for_each(m_workers.begin(), m_workers.end(), [&](std::thread *t) {
		t->join();
		delete t;
	});
#ifdef __APPLE__
	semaphore_destroy(mach_task_self(), m_semaphore);
#else
	sem_destroy(&m_semaphore);
#endif
}

void TFWorkerDispatch::terminationTimeout(const std::chrono::milliseconds &duration) {
	m_terminationTimeout = duration;
}


bool TFWorkerDispatch::queueTask(std::function<void()> task) {
	if (!m_terminating) {
		m_lock.lock();
		m_tasks.push_back(task);
		m_lock.unlock();
#ifdef __APPLE__
		semaphore_signal(m_semaphore);
#else
		sem_post(&m_semaphore);
#endif
		return true;
	}
	return false;
}

void TFWorkerDispatch::run() {
	while (!m_terminating) {
#ifdef __APPLE__
		while (semaphore_wait(m_semaphore) == -1 && (errno == EINTR || errno == EAGAIN));
#else
		while (sem_wait(&m_semaphore) == -1 && (errno == EINTR || errno == EAGAIN));
#endif
		while (m_tasks.size() > 0) {
			m_lock.lock();
			std::function<void()> fn;
			if (m_tasks.size() > 0) {
				fn = m_tasks.front();
				m_tasks.pop_front();
			}
			m_lock.unlock();
			if (fn) {
				fn();
			}
		}
	}
}