// http://danieltarbotton.com/2012/11/21/a-stdpromise-means-nothing-until-its-delivered/

// GB syntax error

#include <thread>
#include <future>

int f(int x) { return x+1; }

template<typename FUNCTION, typename ARG>
std::future<typename std::result_of<FUNCTION(ARG)>::type>
SimpleAsync(FUNCTION func, ARG arg)
{
//  typedef std::result_of<FUNCTION(ARG)>::type return_val;

//  promise.cpp: In function ‘std::future<typename std::result_of<FUNCTION(ARG)>::type> SimpleAsync(FUNCTION, ARG)’:
// promise.cpp:14:13: error: need ‘typename’ before ‘std::result_of<FUNCTION(ARG)>::type’ because ‘std::result_of<FUNCTION(ARG)>’ is a dependent scope
//   typedef std::result_of<FUNCTION(ARG)>::type return_val;
//           ^

    // typedef typename std::result_of<FUNCTION(ARG)>::type return_val;  // syntax error says need typename, OK add typename
    typedef std::result_of<FUNCTION(ARG)> return_val;

    // http://www.cplusplus.com/reference/type_traits/result_of/
 
    std::promise<return_val> prom;
    std::future<return_val> fut = prom.get_future();
 
    std::thread thr([](std::promise<return_val>& prom, FUNCTION func, ARG arg)
    {
        try
        {
            prom.set_value(func(arg));
        }
        catch (const std::exception& e)
        {
            prom.set_exception(std::current_exception());
        }
    }, std::move(prom), func, arg);
 
    thr.detach();
 
    return std::move(fut);
}

int main(int argc, char**argv)
{
  // auto SimpleAsync(Function func, Arg arg) 
  auto sa = SimpleAsync(f, 9); 
}
