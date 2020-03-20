// https://en.wikipedia.org/wiki/AddressSanitizer

// asan = google's address sanitizer

// To compile: 
//      g++   -O -g -fsanitize=address asan-global-buffer-overflow.cpp -o asan-global-buffer-overflow && ./asan-global-buffer-overflow
//  clang++   -O -g -fsanitize=address asan-global-buffer-overflow.cpp -o asan-global-buffer-overflow && ./asan-global-buffer-overflow
//  g++-9.2   -O -g -fsanitize=address asan-global-buffer-overflow.cpp -o asan-global-buffer-overflow && ./asan-global-buffer-overflow
//  clang++-9 -O -g -fsanitize=address asan-global-buffer-overflow.cpp -o asan-global-buffer-overflow && ./asan-global-buffer-overflow
//  clang++-9 -O -g -fsanitize=address asan-global-buffer-overflow.cpp -o asan-global-buffer-overflow && ./asan-global-buffer-overflow | asan_symbolize-9


// Q. How to make sense out of the output?
//
// A1. Make sure you have installed llvm (including llvm-symbolizer).
//    Export the following variable
//      export ASAN_SYMBOLIZER_PATH=/usr/bin/llvm-symbolizer
//      (replace with your correct path to the llvm-symbolizer command).
//    Now run your executable (a.out for now) as
//      ASAN_OPTIONS=symbolize=1 a.out
//
// A2. ./a.out 2>&1 | asan_symbolize

int global_array[100] = {-1};
int main(int argc, char **argv) {
  return global_array[argc + 100];  // BOOM
}
