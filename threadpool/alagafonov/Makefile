all : ThreadPool

ThreadPool : ThreadPoolMain.cpp ThreadPool.cpp ThreadPool.h
	clang++ -std=c++11 ThreadPoolMain.cpp ThreadPool.cpp -o ThreadPool -pthread
