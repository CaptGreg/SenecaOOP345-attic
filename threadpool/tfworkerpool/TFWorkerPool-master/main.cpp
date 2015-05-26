//
//  main.cpp
//  TFWorkerPool
//
//  Created by Tom Fewster on 09/05/2013.
//  Copyright (c) 2013 Tom Fewster. All rights reserved.
//

#include <chrono>
#include "TFLog.h"
#include "TFWorkerDispatch.h"
#include "TFSpinLock.h"

int main(int argc, const char * argv[])
{
	TFWorkerDispatch dispatch;
	dispatch.terminationTimeout(std::chrono::milliseconds(1000));
	for (int p = 0; p < 20; p++) {
		if (!dispatch.queueTask([=]() {
				cout_sync << "Task " << p << " starting";
				uint32_t j = 0;
				for (uint32_t i = 0; i < 100000000; i++) {
					j = i * i;
				}
				cout_sync << "Task " << p << " completed";
			})) {
			cout_sync << "Adding task to queue failed";
		}
	}

    return 0;
}

