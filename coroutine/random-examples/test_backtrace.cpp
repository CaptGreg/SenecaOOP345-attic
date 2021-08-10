// https://gist.github.com/owt5008137/78e1fea9a0221ddf9ed540f4adacf358

/**
 * traceback for cpp
 *
 *  Created on: 2018-01-27
 *      Author: owent
 *
 *  Released under the MIT license
 * 
 *  @note Required flag -rdynamic or addr2line to get the function name when using gcc/clang in unix like system
 *  @note Using addr2line -Cfpe <exe_path> [func_addr...] for more detail when using gcc/clang
 *  @note Required flag /DEBUG /Zi to generate .pdb file when using MSVC
 */

#include <cstdlib>
#include <cstdio>
#include <string>

#ifndef BACKTRACE_MAX_FRAME_NUMBER
#define BACKTRACE_MAX_FRAME_NUMBER 100
#endif

#ifndef USING_WIN_DBG_ENG_COM
#define USING_WIN_DBG_ENG_COM 1
#endif
#ifndef USING_WIN_DBG_HELP
#define USING_WIN_DBG_HELP 1
#endif
#ifndef USING_GNU_UNWIND
#define USING_GNU_UNWIND 1
#endif
#ifndef USING_GNU_EXECINFO
#define USING_GNU_EXECINFO 1
#endif

#ifdef __GLIBCXX__
#include <cxxabi.h>
#define USING_LIBSTDCXX_ABI 1
#elif defined(_LIBCPP_ABI_VERSION)
#include <cxxabi.h>
#define USING_LIBCXX_ABI 1
#endif

#if defined(__cplusplus) && __cplusplus >= 201103L
#define USING_CXX11_NOEXCEPT noexcept
#else
#define USING_CXX11_NOEXCEPT
#endif

#ifdef USING_LIBUNWIND

#include <libunwind.h>

void print_trace() {
    unw_context_t unw_ctx;
    unw_cursor_t unw_cur;
    unw_proc_info_t unw_proc;
    unw_getcontext(&unw_ctx);
    unw_init_local(&unw_cur, &unw_ctx);

    char func_name_cache[256];
    unw_word_t unw_offset;
    int frame_id = 0;
#if defined(USING_LIBSTDCXX_ABI) || defined(USING_LIBCXX_ABI)
    int cxx_abi_status;
    char* realfunc_name;
#endif
    do {
        unw_get_proc_name(&unw_cur, func_name_cache, sizeof(func_name_cache), &unw_offset);
        unw_get_proc_info(&unw_cur, &unw_proc);
        const char* func_name = func_name_cache;
#if defined(USING_LIBSTDCXX_ABI) || defined(USING_LIBCXX_ABI)
        realfunc_name = abi::__cxa_demangle(func_name_cache, 0, 0, &cxx_abi_status);
        if (NULL != realfunc_name) {
            func_name = realfunc_name;
        }
#endif
        printf("Frame #%02d: (%s+0x%lx) [0x%lx]\n", frame_id, func_name, unw_offset, unw_proc.start_ip);

#if defined(USING_LIBSTDCXX_ABI) || defined(USING_LIBCXX_ABI)
        if (NULL != realfunc_name) {
            free(realfunc_name);
            realfunc_name = NULL;
        }
#endif

        int next_res = unw_step(&unw_cur);

        if (0 == next_res) {
            puts("All frames.");
            break;
        }

        if (UNW_EBADFRAME == next_res) {
            puts("Stop with UNW_EBADFRAME");
            break;
        }

        if (UNW_ESTOPUNWIND == next_res) {
            puts("Stop with UNW_ESTOPUNWIND");
            break;
        }

        if (UNW_EINVALIDIP == next_res) {
            puts("Stop with UNW_EINVALIDIP");
            break;
        }

        if (UNW_ENOINFO == next_res) {
            puts("Stop with UNW_ENOINFO");
            break;
        }
        ++ frame_id;
    } while(true);
}

#else

#if defined(_WIN32) && (!defined(__GNUC__) || (!defined(USING_GNU_UNWIND) || !USING_GNU_UNWIND))

#include <Windows.h>

#if defined(USING_WIN_DBG_HELP) && USING_WIN_DBG_HELP
#include <DbgHelp.h>
#endif

#if defined(USING_WIN_DBG_ENG_COM) && USING_WIN_DBG_ENG_COM
#include <DbgEng.h>
#endif

#ifdef _MSC_VER
    #if defined(USING_WIN_DBG_ENG_COM) && USING_WIN_DBG_ENG_COM
        #pragma comment(lib, "ole32.lib")
        #pragma comment(lib, "dbgeng.lib")
    #endif
    #if defined(USING_WIN_DBG_HELP) && USING_WIN_DBG_HELP
        #pragma comment(lib, "dbghelp.lib")
    #endif
#endif

#if defined(USING_WIN_DBG_ENG_COM) && USING_WIN_DBG_ENG_COM
    #ifdef __CRT_UUID_DECL // for __MINGW32__
        __CRT_UUID_DECL(IDebugClient,0x27fe5639,0x8407,0x4f47,0x83,0x64,0xee,0x11,0x8f,0xb0,0x8a,0xc8)
        __CRT_UUID_DECL(IDebugControl,0x5182e668,0x105e,0x416e,0xad,0x92,0x24,0xef,0x80,0x04,0x24,0xba)
        __CRT_UUID_DECL(IDebugSymbols,0x8c31e98c,0x983a,0x48a5,0x90,0x16,0x6f,0xe5,0xd6,0x67,0xa9,0x50)
    #elif defined(DEFINE_GUID) && !defined(BOOST_MSVC)
        DEFINE_GUID(IID_IDebugClient,0x27fe5639,0x8407,0x4f47,0x83,0x64,0xee,0x11,0x8f,0xb0,0x8a,0xc8);
        DEFINE_GUID(IID_IDebugControl,0x5182e668,0x105e,0x416e,0xad,0x92,0x24,0xef,0x80,0x04,0x24,0xba);
        DEFINE_GUID(IID_IDebugSymbols,0x8c31e98c,0x983a,0x48a5,0x90,0x16,0x6f,0xe5,0xd6,0x67,0xa9,0x50);
    #endif

template <class T>
class print_trace_com_holder {
private:
    T* holder_;

private:
    print_trace_com_holder(const print_trace_com_holder&);
    print_trace_com_holder& operator=(const print_trace_com_holder&);

public:
    print_trace_com_holder() USING_CXX11_NOEXCEPT : holder_(NULL) {}
    ~print_trace_com_holder() USING_CXX11_NOEXCEPT {
        if (holder_) {
            holder_->Release();
        }
    }

    T* operator->() const USING_CXX11_NOEXCEPT {
        return holder_;
    }

    PVOID* to_pvoid_ptr()  USING_CXX11_NOEXCEPT {
        return reinterpret_cast<PVOID*>(&holder_);
    }

    bool is_inited() const USING_CXX11_NOEXCEPT {
        return !!holder_;
    }
};
#endif

void print_trace() USING_CXX11_NOEXCEPT {
    void         * stack[ BACKTRACE_MAX_FRAME_NUMBER ];
    unsigned short frames;

    frames = CaptureStackBackTrace( 0, BACKTRACE_MAX_FRAME_NUMBER, stack, NULL );

#if !defined(_MSC_VER)
    for(unsigned short i = 0; i < frames; i++ ) {
        printf( "Frame %02i: () [0x%p]\n", i, stack[ i ]);
    }

#else // MSVC required flag: /DEBUG /Zi
    #if defined(USING_WIN_DBG_HELP) && USING_WIN_DBG_HELP
    SYMBOL_INFO  * symbol;
    HANDLE         process;
    DWORD64        displacement = 0;
    process = GetCurrentProcess();

    // SymSetOptions(SYMOPT_UNDNAME | SYMOPT_DEFERRED_LOADS | SYMOPT_LOAD_LINES);
    SymInitialize( process, NULL, TRUE );

    symbol               = ( SYMBOL_INFO * )malloc(sizeof( SYMBOL_INFO ) + (MAX_SYM_NAME + 1) * sizeof(TCHAR));
    symbol->MaxNameLen   = MAX_SYM_NAME;
    symbol->SizeOfStruct = sizeof( SYMBOL_INFO );

    for(unsigned short i = 0; i < frames; i++ ) {
        if(SymFromAddr( process, reinterpret_cast<ULONG64>(stack[i]), &displacement, symbol )) {
            printf( "Frame %02i: (%s) [0x%p]\n", i, &symbol->Name[0], symbol->Address );
        } else {
            printf( "Frame %02i: () [0x%p]\n", i, stack[i]);
        }
    }

    free(symbol);
    SymCleanup(process);
    #endif

    // ================== Using IDebugClient ==================
    #if defined(USING_WIN_DBG_ENG_COM) && USING_WIN_DBG_ENG_COM
    print_trace_com_holder<IDebugClient> dbg_cli;
    print_trace_com_holder<IDebugControl> dbg_ctrl;
    print_trace_com_holder<IDebugSymbols> dbg_sym;
    if (S_OK != ::DebugCreate(__uuidof(IDebugClient), dbg_cli.to_pvoid_ptr())) {
        return;
    }
    const bool res0 = (S_OK == dbg_cli->QueryInterface(
        __uuidof(IDebugControl),
        dbg_ctrl.to_pvoid_ptr()
    ));
    if (!res0) {
        return;
    }

    const bool res1 = (S_OK == dbg_cli->AttachProcess(
        0,
        ::GetCurrentProcessId(),
        DEBUG_ATTACH_NONINVASIVE | DEBUG_ATTACH_NONINVASIVE_NO_SUSPEND
    ));
    if (!res1) {
        return;
    }

    if (S_OK != dbg_ctrl->WaitForEvent(DEBUG_WAIT_DEFAULT, INFINITE)) {
        return;
    }

    // No cheking: QueryInterface sets the output parameter to NULL in case of error.
    dbg_cli->QueryInterface(__uuidof(IDebugSymbols), dbg_sym.to_pvoid_ptr());

    for(unsigned short i = 0; i < frames; i++ ) {
        if (!dbg_sym.is_inited()) {
            break;
        }

        const ULONG64 offset = reinterpret_cast<ULONG64>(stack[i]);

        char name[256];
        name[0] = '\0';
        ULONG size = 0;
        bool res = (S_OK == dbg_sym->GetNameByOffset(
            offset,
            name,
            sizeof(name),
            &size,
            0
        ));

        std::string result;
        if (!res && size != 0) {
            result.resize(size);
            res = (S_OK == dbg_sym->GetNameByOffset(
                offset,
                &result[0],
                static_cast<ULONG>(result.size()),
                &size,
                0
            ));
        } else if (res) {
            result = name;
        }

        printf( "Frame %02i: (%s) [0x%p]\n", i, result.c_str(), stack[i]);
    }
    #endif
#endif
}

#else

#if defined(USING_GNU_EXECINFO) && USING_GNU_EXECINFO
#include <execinfo.h>
#endif

#if defined(USING_GNU_UNWIND) && USING_GNU_UNWIND
#include <unwind.h>

struct print_trace_unwind_state_t {
    size_t frames_to_skip;
    _Unwind_Word* current;
    _Unwind_Word* end;
};

static _Unwind_Reason_Code print_trace_unwind_callback(::_Unwind_Context* context, void* arg) {
    // Note: do not write `::_Unwind_GetIP` because it is a macro on some platforms.
    // Use `_Unwind_GetIP` instead!
    print_trace_unwind_state_t* const state = reinterpret_cast<print_trace_unwind_state_t*>(arg);
    if (state->frames_to_skip) {
        --state->frames_to_skip;
        return _Unwind_GetIP(context) ? ::_URC_NO_REASON : ::_URC_END_OF_STACK;
    }

    *state->current = _Unwind_GetIP(context);

    ++state->current;
    if (!*(state->current - 1) || state->current == state->end) {
        return ::_URC_END_OF_STACK;
    }

    return ::_URC_NO_REASON;
}

#endif

/* Obtain a backtrace and print it to stdout. */

struct print_trace_symbol_group_t {
    std::string module_name;
    std::string func_name;
    std::string func_offset;
    std::string func_address;
};

static const char* print_trace_skip_space(const char* name) {
    if (NULL == name) {
        return name;
    }

    while (*name && (' ' == *name || '\r' == *name || '\t' == *name || '\r' == *name)) {
        ++name;
    }

    return name;
}

inline bool print_trace_is_number_char(char c) {
    return c >= '0' && c <= '9';
}

inline bool print_trace_is_ident_char(char c) {
    return '_' == c || '$' == c || print_trace_is_number_char(c) || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c & 0x80);// utf-8 or unicode
}

static const char* print_trace_get_ident_end(const char* name) {
    if (NULL == name) {
        return name;
    }

    while (*name && print_trace_is_ident_char(*name)) {
        ++ name;
    }

    return name;
}

static bool print_trace_pick_ident(const char* name, const char*& start, const char*& end, char& previous_c) {
    previous_c = 0;
    start = name;
    end = name;

    bool ret = false;
    if (NULL == name) {
        return false;
    }

    while(*name) {
        name = print_trace_skip_space(name);
        
        if (print_trace_is_ident_char(*name)) {
            start = name;
            end = print_trace_get_ident_end(name);
            ret = true;
            break;
        } else {
            previous_c = *name;
            ++name;
        }
    }

    return ret;
}

static void print_trace_pick_symbol_info (const char* name, print_trace_symbol_group_t& out) {
    out.module_name.clear();
    out.func_name.clear();
    out.func_offset.clear();
    out.func_address.clear();

    if (NULL == name || 0 == *name) {
        return;
    }

    name = print_trace_skip_space(name);
    while (name) {
        const char* start;
        char previous_c;

        if (print_trace_pick_ident(name, start, name, previous_c)) {
            if (print_trace_is_number_char(*start)) {
                if ('+' == previous_c) {
                    out.func_offset = "+";
                    out.func_offset.insert(out.func_offset.end(), start, name);
                } else {
                    out.func_address.assign(start, name);
                }
            } else {
                if (out.module_name.empty()) {
                    out.module_name.assign(start, name);
                } else {
                    out.func_name.assign(start, name);
                }
            }
        } else {
            break;
        }

        name = print_trace_skip_space(name);
    }
}

void print_trace () {
#if defined(USING_GNU_EXECINFO) && USING_GNU_EXECINFO
    {
        void *array[BACKTRACE_MAX_FRAME_NUMBER];
        size_t size;
        char **func_name_cache;

        size = backtrace (array, BACKTRACE_MAX_FRAME_NUMBER);
        func_name_cache = backtrace_symbols (array, size);

        for (size_t i = 0; i < size; i++) {
            print_trace_symbol_group_t symbol;
            print_trace_pick_symbol_info(func_name_cache[i], symbol);

    #if defined(USING_LIBSTDCXX_ABI) || defined(USING_LIBCXX_ABI)
            if (!symbol.func_name.empty()) {
                int cxx_abi_status;
                char* realfunc_name = abi::__cxa_demangle(symbol.func_name.c_str(), 0, 0, &cxx_abi_status);
                if (NULL != realfunc_name) {
                    symbol.func_name = realfunc_name;
                }

                if (NULL != realfunc_name) {
                    free(realfunc_name);
                }
            }
    #endif
            
            printf("Frame #%02d: (%s%s) [%s]\n", static_cast<int>(i), symbol.func_name.c_str(), symbol.func_offset.c_str(), symbol.func_address.c_str());
        }

        free (func_name_cache);
    }
#endif

#if defined(USING_GNU_UNWIND) && USING_GNU_UNWIND
    {
        _Unwind_Word stacks[BACKTRACE_MAX_FRAME_NUMBER];
        print_trace_unwind_state_t state;
        state.frames_to_skip = 0;
        state.current = stacks;
        state.end = stacks + BACKTRACE_MAX_FRAME_NUMBER;

        ::_Unwind_Backtrace(&print_trace_unwind_callback, &state);
        size_t frames_count = state.current - &stacks[0];
        for (size_t i = 0; i < frames_count; ++ i) {
            printf("Frame #%02d: () [0x%lx]\n", static_cast<int>(i), stacks[i]);
        }
    }
#endif
}
#endif

#endif

void func1(int times) {
    if (times > 0) {
        func1(times - 1);
        return;
    }

    print_trace();
}

class functor2 {
public:
    void func2(int times) {
        if (times & 0x01) {
            func2(times - 1);
        } else {
            func1(times - 1);
        }
    }
};

class functor3 {
public:
    static void func3(int times) {
        if (times & 0x01) {
            func3(times - 1);
        } else {
            functor2 f;
            f.func2(times - 1);
        }
    }
};

struct functor4 {
    void operator()(int times) {
        if (times & 0x01) {
            (*this)(times - 1);
        } else {
            functor3::func3(times - 1);
        }
    }
};

static void func5(int times) {
    if (times & 0x01) {
        func5(times - 1);
    } else {
        functor4 f;
        f(times - 1);
    }
}

void func6(int times) {
    if (times & 0x01) {
        func6(times - 1);
    } else {
        func5(times - 1);
    }
}

int main (int argc, char* argv[]) {
    int times = 15;
    if (argc > 1) {
        times = atoi(argv[1]);
    }
    func6(times);
    return 0;
}

