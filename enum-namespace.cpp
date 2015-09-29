// GB - We had a question about namespaces and enums
//    - No surprises here.  It works exactly like you think it should.

#include <iostream>
using namespace std;

namespace ns1 {
  enum enum1 { V1 = 10, V2, V3 = 20, V4 };
}
namespace ns2 {
  enum enum2 { V1 = 400, V2, V3 = 500, V4 };
}

int main()
{
  cout << ns1::V1 << "\n";
  cout << ns1::V2 << "\n";
  cout << ns1::V3 << "\n";
  cout << ns1::V4 << "\n";

  cout << ns2::V1 << "\n";
  cout << ns2::V2 << "\n";
  cout << ns2::V3 << "\n";
  cout << ns2::V4 << "\n";

  cout << ns1::enum1::V1 << "\n";
  cout << ns1::enum1::V2 << "\n";
  cout << ns1::enum1::V3 << "\n";
  cout << ns1::enum1::V4 << "\n";

  cout << ns2::enum2::V1 << "\n";
  cout << ns2::enum2::V2 << "\n";
  cout << ns2::enum2::V3 << "\n";
  cout << ns2::enum2::V4 << "\n";

#define P(X) cout << #X "=" << X << "\n";
  P( ns1::V1 )
  P( ns1::V2 )
  P( ns1::V3 )
  P( ns1::V4 )

  P( ns2::V1 )
  P( ns2::V2 )
  P( ns2::V3 )
  P( ns2::V4 )

  P( ns1::enum1::V1 )
  P( ns1::enum1::V2 )
  P( ns1::enum1::V3 )
  P( ns1::enum1::V4 )

  P( ns2::enum2::V1 )
  P( ns2::enum2::V2 )
  P( ns2::enum2::V3 )
  P( ns2::enum2::V4 )
}
