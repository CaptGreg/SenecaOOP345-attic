 // Thread Class - Lambda Expression
 // thread_id_lambda.cpp

 #include <iostream>
 #include <thread>
 #include <vector>

 const int NT = 10;

 int main() {
     // create a vector of not-joinable threads
     std::vector<std::thread> threads;

     // launch the execution of each thread
#if 0
     for (int i = 0; i < NT; i++) {
         threads.push_back(std::thread([=]() {
             std::cout << i << " Thread id = " <<
              std::this_thread::get_id() << "\n"; 
         })); 
     }
#else
     for (int i = 0; i < NT; i++) {
         auto f = [=]() {
             std::cout << i 
                       << " Thread id = " 
                       << std::this_thread::get_id() 
                       << "\n"; 
         };

         threads.push_back( std::thread(f) );
     }
#endif


     // synchronize their execution here
     for (auto& thread : threads)
         thread.join();
 }
