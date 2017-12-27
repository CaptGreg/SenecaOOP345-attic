#include <iostream>
using namespace std;

const wchar_t* chineseWCharDigits = L"零一二三四五六七八九"; // All 10 Chinese     digits 0 through 9
const wchar_t* koreanWCharDigits  = L"공일이삼사오육칠팔구"; // All 10 Sino-Korean digits 0 through 9

const std::wstring chineseWString = chineseWCharDigits;
const std::wstring koreanWString  = koreanWCharDigits;

template <class T>
bool testBit(T target, int bit)
{
    T mask = (T(1) << bit);
    return target & mask;
}

template <class T>
void printBits(T target)
{
    for(int bit = 8 * sizeof(target) -1; bit >=0; bit-- ) 
        wcout << "01"[testBit(target, bit)];
        // wcout << koreanWString [testBit(target, bit)];
        // wcout << chineseWString [testBit(target, bit)];
}

int main(int argc, char* argv[])
{

  std::ios::sync_with_stdio(false);  // NEED THIS!
  std::locale::global( std::locale( "" ) ) ;
  std::wcout.imbue( std::locale() ) ;

  unsigned short A = 0, B = 0;

  if(argc > 1)
    A = atoi(argv[1]);
    
  if(argc > 2)
    B = atoi(argv[2]);
    
  wcout << "A  =  "; printBits(A); wcout << " (" << A << ")\n";
  wcout << "B  =  "; printBits(B); wcout << " (" << B << ")\n";
  wcout << "\n";
    
  // wcout << "A&B= "; printBits(A&B); wcout << " (" << A&B << ")\n";
  decltype(A) op = A&B;
  wcout << "A&B = "; printBits(op); wcout << " (" << op << ")\n";
  wcout << "\n";

  op = A|B;
  wcout << "A|B = "; printBits(op); wcout << " (" << op << ")\n";
  wcout << "\n";

  op = A^B;
  wcout << "A^B = "; printBits(op); wcout << " (" << op << ")\n";
  wcout << "\n";
    
  op = ~A;
  wcout << "~A  = "; printBits(op); wcout << " (" << op << ")\n";
  wcout << "\n";
    
  op = ~B;
  wcout << "~B  = "; printBits(op); wcout << " (" << op << ")\n";
  wcout << "\n";

  op = A>>1;
  wcout << "A>>1 = "; printBits(op); wcout << " (" << op << ")\n";
  wcout << "\n";
    
  op = B>>1;
  wcout << "B>>1 = "; printBits(op); wcout << " (" << op << ")\n";
  wcout << "\n";

  op = A<<1;
  wcout << "A<<1 = "; printBits(op); wcout << " (" << op << ")\n";
  wcout << "\n";
    
  op = B<<1;
  wcout << "B<<1 = "; printBits(op); wcout << " (" << op << ")\n";

}
    
