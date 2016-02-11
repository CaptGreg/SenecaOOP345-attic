// http://danieltarbotton.com/2012/11/21/a-stdpromise-means-nothing-until-its-delivered/

// GB syntax error

#include <thread>
#include <future>

int f(int x) { return x+1; }

template<typename Function, typename Arg>
std::future<typename std::result_of<Function(Arg)>::type>
SimpleAsync(Function func, Arg arg)
{
    typedef std::result_of<Function(Arg)>::type return_val;
    // prom.cpp:10:13: error: missing 'typename' prior to dependent type name 'std::result_of<Function (Arg)>::type'
    //   (Arg)>::type'
    // typedef std::result_of<Function(Arg)>::type return_val;
    //         ^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    //         typename

    // http://www.cplusplus.com/reference/type_traits/result_of/
 
    std::promise<return_val> prom;
    std::future<return_val> fut = prom.get_future();
 
    std::thread thr([](std::promise<return_val>& prom, Function func, Arg arg)
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
