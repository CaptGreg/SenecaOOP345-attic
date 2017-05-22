// https://en.wikipedia.org/wiki/AddressSanitizer

// asan = google's address sanitizer

// To compile: 
//      g++ -O -g -fsanitize=address asan-heap-buffer-overflow.cpp -o asan-heap-buffer-overflow && ./asan-heap-buffer-overflow
//  clang++ -O -g -fsanitize=address asan-heap-buffer-overflow.cpp -o asan-heap-buffer-overflow && ./asan-heap-buffer-overflow
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

int main(int argc, char **argv) {
  int *array = new int[100];
  array[0] = 0;
  int res = array[argc + 100];  // BOOM
  delete [] array;
  return res;
}
