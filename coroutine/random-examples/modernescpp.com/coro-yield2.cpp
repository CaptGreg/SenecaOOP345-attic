// https://raw.githubusercontent.com/atulmehra/moderncpp-code-examples/master/cpp20_code_examples/coroutines/src/main.cpp
//
#include <iostream>
#include "templatized_generator.h"

#define TOPIC "Coroutines"

Couroutine_Example_Generator<int> Example_CoYield(int begin, int end)
{
  for (int i = begin; i <= end; ++i)
    co_yield i;
}

void Feature_Coroutines_Coyield_Example()
{
  const std::string FEATURE_EXAMPLE_NAME = "Modules_SimpleModuleExample";
  std::cout << "Start " << __func__ << std::endl;
  auto results = Example_CoYield(25, 35);

  for (int i = 0; i < 5; ++i)
  {
    std::cout << "This is result number " << results.current_value() << std::endl;
    results.move_next();
  }
  std::cout << "First loop has ended " << results.current_value() << std::endl;
  for (int i = 0; i < 5; ++i)
  {
    std::cout << "This is result number " << results.current_value() << std::endl;
    results.move_next();
  }
  std::cout << "Second loop has ended " << results.current_value() << std::endl;

  std::cout << "End " << __func__ << std::endl;
}

int main(int argc, char *argv[])
{
  std::cout << "Topic:" << TOPIC << std::endl;
  Feature_Coroutines_Coyield_Example();
}
