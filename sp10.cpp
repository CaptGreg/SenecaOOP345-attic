#include <vector>
#include <string>
#include <memory>   // *_ptr family
#include <memory.h>  // memcpy
#include <iostream>

class ObjRaw { // simple rule-of-5 class using raw pointers
  size_t   bs;           // buffer size
  uint8_t* bp;           // buffer pointer

public:

  ObjRaw(int s) : bs(s), bp(new uint8_t(bs))  // CTOR
  { 
    std::cout <<  "ObjRaw(" << bs << ")\n"; 
  }

  ~ObjRaw()                                   // DTOR
  { 
    std::cout << "~ObjRaw(), bs=" << bs << "\n";
    delete [] bp; 
  }

  ObjRaw(const ObjRaw& rhs)                   // copy CTOR
  { 
    std::cout << "copy ctor rhs.bs=" << rhs.bs << "\n"; 
    bp = nullptr;                           // set up to use copy = operator
    *this = rhs;                            // call it
  }

  ObjRaw& operator= (const ObjRaw& rhs)       // copy = operator
  { 
    std::cout << "copy = operator bs,rhs.bs=" << bs << "," << rhs.bs << "\n";
    if(this!=&rhs) { 
      bs=rhs.bs; 
      delete [] bp;
      bp = bs? (uint8_t*) memcpy(new uint8_t(bs), rhs.bp, rhs.bs): nullptr; 
    }
    return *this;
  }

  ObjRaw(ObjRaw&& rhs)                        // move CTOR
  { 
    std::cout << "move ctor rhs.bs=" << rhs.bs << "\n"; 
    bp = nullptr;                          // set up to use move = operator
    *this = std::move(rhs);                // call it
  }

  ObjRaw&& operator= (ObjRaw&& rhs)           // move = operator
  { 
    std::cout << "move = operator bs,rhs.bs=" << bs << "," << rhs.bs << "\n";
    if(this!=&rhs) { 
      delete [] bp;
      bs = rhs.bs;
      bp = rhs.bp;
      rhs.bs = 0;
      rhs.bp = nullptr;
    }
    return std::move(*this);
  }

  void print(const std::string& l)           // print out buffers size
  {
    std::cout << l << ", bs=" << bs << "\n";
  }
};

void doWorkRawPtr ()
{
  std::cout << "doWorkRawPtr start\n";
  std::vector< std::shared_ptr<ObjRaw> >  v;
  std::shared_ptr<ObjRaw> p;

  v.push_back( std::shared_ptr<ObjRaw> (new ObjRaw(1e7)) );
  v.push_back( std::shared_ptr<ObjRaw> (new ObjRaw(2e7)) );

  v.front() -> print("v.front");
  v.back()  -> print("v.back");
  std::cout << "front use_count=" << v.front().use_count() << "\n";
  std::cout << "back  use_count=" << v.back ().use_count() << "\n";

  for(int i = 0; i < 1000; i++)
   v.push_back( v.front() );

  for(int i = 0; i < 1000; i++)
   v.push_back( v[1] );

  std::cout << "v.size()=" << v.size() << "\n";

  v.front() -> print("v.front");
  v.back()  -> print("v.back");
  std::cout << "front use_count=" << v.front().use_count() << "\n";
  std::cout << "back  use_count=" << v.back ().use_count() << "\n";

  // Same thing again, this time using a copy of the ptr
  p = v.front();
  p->print("v.front");
  std::cout << "front use_count=" << v.front().use_count() << "\n";
  p = v.back();
  std::cout << "front use_count=" << v.front().use_count() << "\n";
  std::cout << "back  use_count=" << v.back ().use_count() << "\n";
  p->print("v.back");

  std::cout << "doWorkRawPtr finish\n";
}

class ObjSmart { // simple rule-of-5 class using shared_ptr
  size_t                   bs;           // buffer size
  std::shared_ptr<uint8_t> bp;           // buffer pointer

public:

  ObjSmart(int s)                                 // CTOR
  : bs(s), 
    bp( std::make_shared  <uint8_t>  (bs) )  // same as
  //bp( std::shared_ptr   <uint8_t>  (new uint8_t(bs)) )
  { 
    std::cout <<  "ObjSmart(" << bs << ")\n"; 
  }

  ~ObjSmart()                                     // DTOR
  { 
    std::cout << "~ObjSmart(), bs=" << bs << "\n";
  }

  ObjSmart(const ObjSmart& rhs)                   // copy CTOR
  { 
    std::cout << "copy ctor rhs.bs=" << rhs.bs << "\n"; 
    bp.reset();                                // set up to use copy = operator
    *this = rhs;                               // call it
  }

  ObjSmart& operator= (const ObjSmart& rhs)       // copy = operator
  { 
    std::cout << "copy = operator bs,rhs.bs=" << bs << "," << rhs.bs << "\n";
    if(this!=&rhs) { 
      bs=rhs.bs; 
      if(bs) {
        bp = std::make_shared<uint8_t>( bs );   // same as
        // bp = std::shared_ptr<uint8_t>(new uint8_t(bs));
        memcpy( (void*) (bp.get()), (const void*) (rhs.bp.get()), rhs.bs);
      } else {
        bp.reset();
      }
    }
    return *this;
  }

  ObjSmart(ObjSmart&& rhs)                        // move CTOR
  { 
    std::cout << "move ctor rhs.bs=" << rhs.bs << "\n"; 
    bp.reset();                                // set up to use copy = operator
    *this = std::move(rhs);                    // call it
  }

  ObjSmart&& operator= (ObjSmart&& rhs)           // move = operator
  { 
    std::cout << "move = operator bs,rhs.bs=" << bs << "," << rhs.bs << "\n";
    if(this!=&rhs) { 
      bs = rhs.bs;
      bp = rhs.bp;
      rhs.bs = 0;
      rhs.bp.reset();
    }
    return std::move(*this);
  }

  void print(const std::string& l)           // print out buffers size
  {
    std::cout << l << ", bs=" << bs << "\n";
  }
};

void doWorkSmartPtr ()
{
  std::cout << "doWorkSmartPtr start\n";
  std::vector< std::shared_ptr<ObjSmart> >  v;
  std::shared_ptr<ObjSmart> p;

  v.push_back( std::shared_ptr<ObjSmart> (new ObjSmart(1e7)) );
  v.push_back( std::shared_ptr<ObjSmart> (new ObjSmart(2e7)) );

  v.front() -> print("v.front");
  v.back()  -> print("v.back");
  std::cout << "front use_count=" << v.front().use_count() << "\n";
  std::cout << "back  use_count=" << v.back ().use_count() << "\n";

  for(int i = 0; i < 1000; i++)
   v.push_back( v.front() );

  for(int i = 0; i < 1000; i++)
   v.push_back( v[1] );

  std::cout << "v.size()=" << v.size() << "\n";

  v.front() -> print("v.front");
  v.back()  -> print("v.back");
  std::cout << "front use_count=" << v.front().use_count() << "\n";
  std::cout << "back  use_count=" << v.back ().use_count() << "\n";

  // Same thing again, this time using a copy of the ptr
  p = v.front();
  p->print("v.front");
  std::cout << "front use_count=" << v.front().use_count() << "\n";
  p = v.back();
  std::cout << "front use_count=" << v.front().use_count() << "\n";
  std::cout << "back  use_count=" << v.back ().use_count() << "\n";
  p->print("v.back");

  std::cout << "doWorkSmartPtr finish\n";
}

int main()
{
  std::cout << "main:\n";

  std::cout << "++++++++++++++++++++++++++++++++ calling doWorkRawPtr\n";
  doWorkRawPtr();
  std::cout << "++++++++++++++++++++++++++++++++ doWorkRawPtr returned\n";
  std::cout << "++++++++++++++++++++++++++++++++ calling doWorkSmartPtr\n";
  doWorkSmartPtr();
  std::cout << "++++++++++++++++++++++++++++++++ doWorkSmartPtr returned\n";

  return 0;
}
