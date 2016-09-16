// g++ -Wall -std=c++14  cpp14.cpp -o cpp14 -pthread && ./cpp14

// https://gcc.gnu.org/projects/cxx1y.html
// https://gcc.gnu.org/onlinedocs/libstdc++/manual/status.html

// these code snippets were taken from
// http://cpprocks.com/spotlight-on-libraries-relying-on-c14/

// It appeaars g++ 5.2.1 does not have the header files for the new library features.

#if 0
// GB no <sprout/string.hpp> g++ 5.2.1 

#include <sprout/string.hpp>
void sprout()
{

  using namespace sprout;

  SPROUT_STATIC_CONSTEXPR auto x = to_string("hello ");
  SPROUT_STATIC_CONSTEXPR auto y = to_string("world");
  SPROUT_STATIC_CONSTEXPR auto z = x + y;
  static_assert(z == "hello world", "Should be: hello world.");

  SPROUT_STATIC_CONSTEXPR auto input = sprout::array<int, 10>{ {1, 2, 3, 4, 5, 6, 7, 8, 9, 10} };

  // count odd elements
    SPROUT_STATIC_CONSTEXPR auto result = sprout::count_if(begin(input), end(input), bind2nd(modulus<>(), 2));
    static_assert(result == 5, "counted 5 elements of odd number from input.");

  // find min and max elements
    SPROUT_STATIC_CONSTEXPR auto result = sprout::minmax_element(begin(input), end(input));
    static_assert(*result.first == 1, "min element is 1.");
    static_assert(*result.second == 10, "max element is 10.");
}
#endif

#if 0
// GB no <mettle.hpp> g++ 5.2.1 

#include <mettle.hpp>
using namespace mettle;

suite<> basic("a basic suite", [](auto &_) {

    _.test("a test", []() {
        expect(true, equal_to(true));
    });

    _.skip_test("a skipped test", []() {
        expect(3, any(1, 2, 4));
    });

    for(int i = 0; i < 4; i++) {
        _.test("test number " + std::to_string(i), [i]() {
           expect(i % 2, less(2));
        });
    }

});
#endif

#if 0
// #include <video.hpp> // GB include file not secified.  Take a guess.
#include <opencv2/opencv.hpp>
void video ()
{
  // A parallel implementation of a box_filter using Video++.

  image2d<int> A(1000, 1000);
  image2d<int> B(A.domain());

  auto nbh = box_nbh2d<int, 3, 3>(A);

  // Parallel Loop over pixels of in and out.
  pixel_wise(A, B) << [&] (int& a, int& b) {
      int sum = int;

      // Loop over neighbors wrt nbh to compute a sum.
      nbh(a) < [&] (int& n) sum += n;

      // Write the sum to the output image.
      b = (sum / 3);
  };

  // Load JPG image in a vpp image using OpenCV imread.
  image2d<vuchar3> img = from_opencv<vuchar3>(cv::imread("image.jpg"));

  // Write a vpp image using OpenCV imwrite.
  cv::imwrite("in.jpg", to_opencv(img));
}
#endif

#include <iostream>

using namespace std;

int main()
{
  auto min11 = [] (int  a, int  b) -> int  { return a < b? a: b; };
  auto min14 = [] (auto a, auto b) -> auto { return a < b? a: b; };
  

  cout << "min11(1,2)=" << min11(1,2) << "\n";
  cout << "min14(1,2)=" << min14(1,2) << "\n";
  cout << "min14(1.5,2.5)=" << min14(1.5,2.5) << "\n";
}
