// We can 
//   1. create a function object instance called 'main'.  
//   2. assign a lambda function to a function pointer called 'main'.

// However, with either method the runtime startup code segfaults 
// prior to 'main' executing.

#include <iostream>
#include <csignal>     // signal+SIGSEGV
#include <functional>  // std::function
using namespace std;

#if 0
  ///////////////////////////////////
  // F U N C T I O N   O B J E C T //
  ///////////////////////////////////

  const char* pMain = "main resolved by function object\n";

  class Main {
  public:
    // try with and without args
    #if 1
      int operator() ()
      {
        cout << "function object alive - no args\n";
        return 0;
      }
    #endif

    #if 1
      int operator() (int argc, char**argv)
      {
        cout << "function object alive\n";
        for(int arg = 0; arg < argc; arg++)
          cout << "argv[" << arg << "] -->" << argv[arg] << "<--\n";
        return 0;
      }
    #endif

    #if 0
      // if above 'int operator() (int argc, char**argv)' is defined:
      // error: ‘int Main::operator()(int, char**)’ cannot be overloaded
      int operator() (int argc, char*argv[])
      {
        cout << "function object alive\n";
        for(int arg = 0; arg < argc; arg++)
          cout << "argv[" << arg << "] -->" << argv[arg] << "<--\n";
        return 0;
      }
    #endif
  };

  Main main;  

  // int main (int argc, char**argv) { Main m; m(argc,argv); } // Works.
#endif


#if 0
  /////////////////
  // L A M B D A //
  /////////////////

  const char* pMain = "main resolved by lambda function\n";

  #if 0
    // lambda 'main' link error: undefined reference to `main'
    auto main = [] () -> int { cout << "lambda alive\n"; return 0; };
  #endif

  #if 0
    auto Main = [] () -> int { cout << "lambda alive\n"; return 0; };
    function<int()> main = Main;
    // c-code function pointer generates same result
    // int (*main) () = Main;   
  #endif

  #if 0
    // lambda 'main' link error: undefined reference to `main'
    auto main = [] (int argc, char**argv) -> int
    {
      cout << "lambda alive\n";
      for(int arg = 0; arg < argc; arg++)
        cout << "argv[" << arg << "] -->" << argv[arg] << "<--\n";
      return 0;
    };
  #endif

  #if 1
    auto MainArgs = [] (int argc, char**argv) -> int
    {
      cout << "lambda alive\n";
      for(int arg = 0; arg < argc; arg++)
        cout << "argv[" << arg << "] -->" << argv[arg] << "<--\n";
      return 0;
    };
    function<int(int,char**)> main = MainArgs;
    // c-code function pointer generates same result
    // int (*main) (int,char**) = MainArgs;   
  #endif


#endif

#if 1
  ////////////////////////////////////////////
  // P L A I N   O R D I N A R Y    M A I N //
  ////////////////////////////////////////////


  #if 0 // error: conflicting declaration ... if both mains defined.

    const char* pMain = "main resolved by plain ordinary main()\n";
    int main ()
    {  cout << "main alive\n"; return 0; }

  #endif

  #if 0

    const char* pMain = "main resolved by plain ordinary main(int argc,char**argv)\n";
    int main (int argc, char**argv)
    {
      cout << "main alive\n";
      for(int arg = 0; arg < argc; arg++)
        cout << "argv[" << arg << "] -->" << argv[arg] << "<--\n";
      return 0;
    }

  #endif

  #if 1

    const char* pMain = "main resolved by a function pointer.\n"
                        "The function pointer is initialized by 'class SetupMain'.\n";
    int MainArgs (int argc, char**argv)
    {
      cout << "MainArgs(argc,argv) alive\n";
      for(int arg = 0; arg < argc; arg++)
        cout << "argv[" << arg << "] -->" << argv[arg] << "<--\n";
      return 0;
    }
    // function<int(int,char**)> main = MainArgs; // segfaults
    function<int(int,char**)> main;

    class SetupMain {
    public:
      // SetupMain (function<int(int,char**)> m) 
      SetupMain (int(*m)(int,char**))                 // same behavior
      {
        extern function<int(int,char**)> main;
        cout << "SetupMain alive\n";

        uint64_t* p; 
        // nm main-fo-lambda | grep -i main
        // 0000000000400c9a t _GLOBAL__sub_I_pMain
        //                  U __libc_start_main@@GLIBC_2.2.5
        // 0000000000603240 B main
        // 0000000000603098 D pMain
        // 0000000000603260 B set_up_main
        // 0000000000400b76 T _Z8MainArgsiPPc <-- 'int MainArgs(int, char**)'
        // 0000000000400e6e W _ZN9SetupMainC1EPFiiPPcE
        // 0000000000400e6e W _ZN9SetupMainC2EPFiiPPcE


        // man nm
        // ·   The symbol value, in the radix selected by options (see below), or
        //     hexadecimal by default.
        // ·   The symbol type.  At least the following types are used; others
        //     are, as well, depending on the object file format.  If lowercase,
        //     the symbol is usually local; if uppercase, the symbol is global
        //     (external).  There are however a few lowercase symbols that are
        //     shown for special global symbols ("u", "v" and "w").
        //
        //    "B"
        //    "b" The symbol is in the uninitialized data section (known as BSS).

        // p = 0x603240ULL;
        // error: invalid conversion from ‘long long unsigned int’ to ‘uint64_t* {aka long unsigned int*}’
        // so coerce it

        // p = main;
        p = nullptr;
        p += 0x603240ULL/sizeof(*p);
        cout << "SetupMain() pointer to main, p=" << p 
             << ", *p=0x" << hex << *p
             << "\n";

        main = m; 

        cout << "SetupMain() pointer to main, p=" << p 
             << ", *p=0x" << hex << *p
             << "\n";

        // SetupMain() pointer to main, p=0x603240, *p=0x0
        // SetupMain() pointer to main, p=0x603240, *p=0x400b76
        // printout agrees with expectedvalues for 'nm' namelist 


        // cout << 
        // main 
        // (void*) main 
        // reinterpret_cast<void*> (main)  
        // static_cast<void*> (main) 

        // m 
        // (void*) m 
        // reinterpret_cast<void*> (m)  
        // static_cast<void*> (m) 

        // << "\n";
      }
    } set_up_main( MainArgs ); 


  #endif

#endif

////////////////////////////////////////
// trap the SIGSEGV segment violation //
////////////////////////////////////////
class Startup {
public:
  Startup()
  {
    atexit( [] () { cout << "atexit callback: all done.\n"; } );
    cout << "Startup(): atexit callback registered.\n";

    // __sighandler_t {aka void (*)(int)}’
    auto cb =[] (int s) // -> sighandler_t
      { 
        switch(s) {
        case SIGSEGV: cout << "intercept signal callback signal " << dec << s << " (SEGSEGV="<<SIGSEGV<<")\n"; break; 
        case SIGABRT: cout << "intercept signal callback signal " << dec << s << " (SEGABRT="<<SIGABRT<<")\n"; break;
        default:      cout << "intercept signal callback signal " << dec << s << "\n";                         break;
        }
        exit(-99); 
      };

    signal(SIGSEGV, cb);
    signal(SIGABRT, cb);
    cout << "Startup(): SIGSEGV signal callback registered.\n";
    cout << "\n";
    cout << "Startup(): " << pMain;
    cout << "\n";

    // Try to print the value of 'main'.
    // Perhaps the function pointer is not intialized.
    extern function<int(int,char**)> main;
    // Cannot figure out cast.

    // cout << "Startup(): main=" << 

    // main 
    // error: no match for ‘operator<<’ (operand types are ‘std::basic_ostream<char>’ and ‘std::function<int(int, char**)>’)

    // (void*) main 
    // error: invalid cast from type ‘std::function<int(int, char**)>’ to type ‘void*’

    // reinterpret_cast<void*> (main)  
    // error: invalid cast from type ‘std::function<int(int, char**)>’ to type ‘void*’

    // static_cast<void*> (main) 
    //  error: invalid static_cast from type ‘std::function<int(int, char**)>’ to type ‘void*’

    // << "\n";

    extern function<int(int,char**)> main;
    uint64_t* p; 
    p = nullptr;
    p += 0x603240ULL/sizeof(*p);
    cout << "Startup(): pointer to main, p=" << p 
         << ", *p=0x" << hex << *p
         << "  <-- looks plausible, but still SIGSEGV!\n";

    cout << "Startup calling calling main();\n";
    char* argv[0]; argv[1] =(char*) "startup dummy path to main...";
    main(1,argv);
    // exit(0);
    cout << "Startup main returned\n";
    cout << "startup finished.  The c-runtime will now call main properly with the correct (argc,argv)\n";
  }

  ~Startup() { cout << "~Startup(): all done.\n"; }
} startup;


/*
++++++++++++++++++++++++++++++++++++++++++++++++++
Startup(): atexit callback registered.
Startup(): SIGSEGV signal callback registered.

Startup(): main resolved by function object

signal callback intercepted signal 11 (SEGSEGV=11)
**************************************************

~Startup(): all done.
atexit callback: all done.

++++++++++++++++++++++++++++++++++++++++++++++++++
Startup(): atexit callback registered.
Startup(): SIGSEGV signal callback registered.

Startup(): main resolved by lambda function

signal callback intercepted signal 11 (SEGSEGV=11)
**************************************************

~Startup(): all done.
atexit callback: all done.

++++++++++++++++++++++++++++++++++++++++++++++++++
Startup(): atexit callback registered.
Startup(): SIGSEGV signal callback registered.

Startup(): main resolved by plain ordinary 'int main()'

main alive
~Startup(): all done.
atexit callback: all done.

++++++++++++++++++++++++++++++++++++++++++++++++++
Startup(): atexit callback registered.
Startup(): SIGSEGV signal callback registered.

Startup(): main resolved by plain ordinary 'int main(int argc, char**argv)'
main alive
argv[0] -->./main-fo-lambda<--
~Startup(): all done.
atexit callback: all done.

++++++++++++++++++++++++++++++++++++++++++++++++++
*/
