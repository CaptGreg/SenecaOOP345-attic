//
//  TFSpinLock.h
//  CeloxicaTest
//
//  Created by Tom Fewster on 25/04/2013.
//

#ifndef __TFObjectPool__TFSpinLock__
#define __TFObjectPool__TFSpinLock__

#include <iostream>
#include <atomic>

class TFSpinLock {
	typedef enum {TFSpinLockLocked, TFSpinLockUnlocked} TFSpinLockState;
	std::atomic<TFSpinLockState> m_state = ATOMIC_VAR_INIT(TFSpinLockUnlocked);

public:
	TFSpinLock() : m_state(TFSpinLockUnlocked) {}
	void lock() {
		while (m_state.exchange(TFSpinLockLocked, std::memory_order_acquire) == TFSpinLockLocked);
	}
	
	void unlock() {
		m_state.store(TFSpinLockUnlocked, std::memory_order_release);
	}
};

class TFSpinLockAuto {
	TFSpinLock &m_lock;
public:
	explicit TFSpinLockAuto(TFSpinLock &lock) : m_lock(lock) { m_lock.lock(); }
	~TFSpinLockAuto() { m_lock.unlock(); }
};

#endif /* defined(__TFObjectPool__TFSpinLock__) */
