// http://msdn.microsoft.com/en-us/library/hskdteyh.aspx
// GB convert sloppy use of int to uint32_t
// GB DIY __cpuid + __cpuidex

// InstructionSet.cpp 
// Compile by using: cl /EHsc /W4 InstructionSet.cpp
// processor: x86, x64
// Uses the __cpuid intrinsic to get information about 
// CPU extended instruction set support.

#include <iostream>
#include <vector>
#include <bitset>
#include <array>
#include <string>
#include <memory.h>     // GB memcpy
#ifdef __MSVC__
  #include <intrin.h> 
#else // GB [clan]g++
  #include <mmintrin.h>
  #include <emmintrin.h>

  // GB DIY __cpuid + __cpuidex:
  // http://msdn.microsoft.com/en-us/library/vstudio/hskdteyh%28v=vs.100%29.aspx
  static inline void __cpuid(uint32_t r[4], uint32_t function_id)  // GB
  {
    asm volatile("cpuid":"=a"(*r),"=b"(*(r+1)), "=c"(*(r+2)),"=d"(*(r+3)):"a"(function_id));
  }
  static inline void __cpuidex(uint32_t r[4], uint32_t function_id, uint32_t ECXValue)  // GB
  {
    r[2] = ECXValue;   // The value of the ECX register before this intrinsic generates the cpuid instruction.
    asm volatile("cpuid":"=a"(*r),"=b"(*(r+1)), "=c"(*(r+2)),"=d"(*(r+3)):"a"(function_id));
  }
#endif

class InstructionSet
{
    // forward declarations
    class InstructionSet_Internal;

public:
    // getters
    static std::string Vendor(void) { return CPU_Rep.vendor_; }
    static std::string Brand(void)  { return CPU_Rep.brand_; }

    static bool SSE3(void)          { return CPU_Rep.f_1_ECX_[0];  }
    static bool PCLMULQDQ(void)     { return CPU_Rep.f_1_ECX_[1];  }
    static bool MONITOR(void)       { return CPU_Rep.f_1_ECX_[3];  }
    static bool SSSE3(void)         { return CPU_Rep.f_1_ECX_[9];  }
    static bool FMA(void)           { return CPU_Rep.f_1_ECX_[12]; }
    static bool CMPXCHG16B(void)    { return CPU_Rep.f_1_ECX_[13]; }
    static bool SSE41(void)         { return CPU_Rep.f_1_ECX_[19]; }
    static bool SSE42(void)         { return CPU_Rep.f_1_ECX_[20]; }
    static bool MOVBE(void)         { return CPU_Rep.f_1_ECX_[22]; }
    static bool POPCNT(void)        { return CPU_Rep.f_1_ECX_[23]; }
    static bool AES(void)           { return CPU_Rep.f_1_ECX_[25]; }
    static bool XSAVE(void)         { return CPU_Rep.f_1_ECX_[26]; }
    static bool OSXSAVE(void)       { return CPU_Rep.f_1_ECX_[27]; }
    static bool AVX(void)           { return CPU_Rep.f_1_ECX_[28]; }
    static bool F16C(void)          { return CPU_Rep.f_1_ECX_[29]; }
    static bool RDRAND(void)        { return CPU_Rep.f_1_ECX_[30]; }

    static bool MSR(void)           { return CPU_Rep.f_1_EDX_[5];  }
    static bool CX8(void)           { return CPU_Rep.f_1_EDX_[8];  }
    static bool SEP(void)           { return CPU_Rep.f_1_EDX_[11]; }
    static bool CMOV(void)          { return CPU_Rep.f_1_EDX_[15]; }
    static bool CLFSH(void)         { return CPU_Rep.f_1_EDX_[19]; }
    static bool MMX(void)           { return CPU_Rep.f_1_EDX_[23]; }
    static bool FXSR(void)          { return CPU_Rep.f_1_EDX_[24]; }
    static bool SSE(void)           { return CPU_Rep.f_1_EDX_[25]; }
    static bool SSE2(void)          { return CPU_Rep.f_1_EDX_[26]; }

    static bool FSGSBASE(void)      { return CPU_Rep.f_7_EBX_[0];  }
    static bool BMI1(void)          { return CPU_Rep.f_7_EBX_[3];  }
    static bool HLE(void)           { return CPU_Rep.isIntel_ && CPU_Rep.f_7_EBX_[4]; }
    static bool AVX2(void)          { return CPU_Rep.f_7_EBX_[5];  }
    static bool BMI2(void)          { return CPU_Rep.f_7_EBX_[8];  }
    static bool ERMS(void)          { return CPU_Rep.f_7_EBX_[9];  }
    static bool INVPCID(void)       { return CPU_Rep.f_7_EBX_[10]; }
    static bool RTM(void)           { return CPU_Rep.isIntel_ && CPU_Rep.f_7_EBX_[11]; }
    static bool AVX512F(void)       { return CPU_Rep.f_7_EBX_[16]; }
    static bool RDSEED(void)        { return CPU_Rep.f_7_EBX_[18]; }
    static bool ADX(void)           { return CPU_Rep.f_7_EBX_[19]; }
    static bool AVX512PF(void)      { return CPU_Rep.f_7_EBX_[26]; }
    static bool AVX512ER(void)      { return CPU_Rep.f_7_EBX_[27]; }
    static bool AVX512CD(void)      { return CPU_Rep.f_7_EBX_[28]; }
    static bool SHA(void)           { return CPU_Rep.f_7_EBX_[29]; }

    static bool PREFETCHWT1(void)   { return CPU_Rep.f_7_ECX_[0];  }

    static bool LAHF(void)          { return CPU_Rep.f_81_ECX_[0]; }
    static bool LZCNT(void)         { return CPU_Rep.isIntel_ && CPU_Rep.f_81_ECX_[5];  }
    static bool ABM(void)           { return CPU_Rep.isAMD_   && CPU_Rep.f_81_ECX_[5];  }
    static bool SSE4a(void)         { return CPU_Rep.isAMD_   && CPU_Rep.f_81_ECX_[6];  }
    static bool XOP(void)           { return CPU_Rep.isAMD_   && CPU_Rep.f_81_ECX_[11]; }
    static bool TBM(void)           { return CPU_Rep.isAMD_   && CPU_Rep.f_81_ECX_[21]; }

    static bool SYSCALL(void)       { return CPU_Rep.isIntel_ && CPU_Rep.f_81_EDX_[11]; }
    static bool MMXEXT(void)        { return CPU_Rep.isAMD_   && CPU_Rep.f_81_EDX_[22]; }
    static bool RDTSCP(void)        { return CPU_Rep.isIntel_ && CPU_Rep.f_81_EDX_[27]; }
    static bool _3DNOWEXT(void)     { return CPU_Rep.isAMD_   && CPU_Rep.f_81_EDX_[30]; }
    static bool _3DNOW(void)        { return CPU_Rep.isAMD_   && CPU_Rep.f_81_EDX_[31]; }

private:
    static const InstructionSet_Internal CPU_Rep;

    class InstructionSet_Internal
    {
    public:
        InstructionSet_Internal()
          : nIds_    { 0 },
            nExIds_  { 0 },
            isIntel_ { false },
            isAMD_   { false },
            f_1_ECX_ { 0 },
            f_1_EDX_ { 0 },
            f_7_EBX_ { 0 },
            f_7_ECX_ { 0 },
            f_81_ECX_{ 0 },
            f_81_EDX_{ 0 },
            data_    {},
            extdata_ {}
        {
            //uint32_t cpuInfo[4] = {-1};
            std::array<uint32_t, 4> cpui;

            // Calling __cpuid with 0x0 as the function_id argument
            // gets the number of the highest valid function ID.
            __cpuid(cpui.data(), 0);
            nIds_ = cpui[0];

            for (uint32_t i = 0; i <= nIds_; ++i) {
                __cpuidex(cpui.data(), i, 0);
                data_.push_back(cpui);
            }

            // Capture vendor string
            char vendor[0x20];
            memset(vendor, 0, sizeof(vendor));
            *reinterpret_cast<uint32_t*>(vendor) = data_[0][1];
            *reinterpret_cast<uint32_t*>(vendor + 4) = data_[0][3];
            *reinterpret_cast<uint32_t*>(vendor + 8) = data_[0][2];
            vendor_ = vendor;
            if (vendor_ == "GenuineIntel") {
                isIntel_ = true;
            } else if (vendor_ == "AuthenticAMD") {
                isAMD_ = true;
            }

            // load bitset with flags for function 0x00000001
            if (nIds_ >= 1) {
                f_1_ECX_ = data_[1][2];
                f_1_EDX_ = data_[1][3];
            }

            // load bitset with flags for function 0x00000007
            if (nIds_ >= 7) {
                f_7_EBX_ = data_[7][1];
                f_7_ECX_ = data_[7][2];
            }

            // Calling __cpuid with 0x80000000 as the function_id argument
            // gets the number of the highest valid extended ID.
            __cpuid(cpui.data(), 0x80000000);
            nExIds_ = cpui[0];

            char brand[0x40];
            memset(brand, 0, sizeof(brand));

            for (uint32_t i = 0x80000000; i <= nExIds_; ++i) {
                __cpuidex(cpui.data(), i, 0);
                extdata_.push_back(cpui);
            }

            // load bitset with flags for function 0x80000001
            if (nExIds_ >= 0x80000001) {
                f_81_ECX_ = extdata_[1][2];
                f_81_EDX_ = extdata_[1][3];
            }

            // Interpret CPU brand string if reported
            if (nExIds_ >= 0x80000004) {
                memcpy(brand, extdata_[2].data(), sizeof(cpui));
                memcpy(brand + 16, extdata_[3].data(), sizeof(cpui));
                memcpy(brand + 32, extdata_[4].data(), sizeof(cpui));
                brand_ = brand;
            }
        };

        uint32_t nIds_;
        uint32_t nExIds_;
        std::string vendor_;
        std::string brand_;
        bool isIntel_;
        bool isAMD_;
        std::bitset<32> f_1_ECX_;
        std::bitset<32> f_1_EDX_;
        std::bitset<32> f_7_EBX_;
        std::bitset<32> f_7_ECX_;
        std::bitset<32> f_81_ECX_;
        std::bitset<32> f_81_EDX_;
        std::vector<std::array<uint32_t, 4>> data_;
        std::vector<std::array<uint32_t, 4>> extdata_;
    };
};

// Initialize static member data
const InstructionSet::InstructionSet_Internal InstructionSet::CPU_Rep;


// Print out supported instruction set extensions
int main()
{
    auto& outstream = std::cout;

    auto support_message = [&outstream](bool pass, std::string feature, bool is_supported) -> void {
        // if(  pass &&  is_supported ) outstream << feature + " supported\n"; 
        if(  pass &&  is_supported ) outstream << feature + "\n"; 
        if( !pass && !is_supported ) outstream << feature + " not supported\n";
    };

    std::cout << InstructionSet::Vendor() + "\n";
    std::cout << InstructionSet::Brand()  + "\n";
    
    for(int pass = 1; pass >= 0 ; pass--) { // first pass: "supported", second pass: "not supported"
       support_message(pass, "3DNOW",       InstructionSet::_3DNOW());
       support_message(pass, "3DNOWEXT",    InstructionSet::_3DNOWEXT());
       support_message(pass, "ABM",         InstructionSet::ABM());
       support_message(pass, "ADX",         InstructionSet::ADX());
       support_message(pass, "AES",         InstructionSet::AES());
       support_message(pass, "AVX",         InstructionSet::AVX());
       support_message(pass, "AVX2",        InstructionSet::AVX2());
       support_message(pass, "AVX512CD",    InstructionSet::AVX512CD());
       support_message(pass, "AVX512ER",    InstructionSet::AVX512ER());
       support_message(pass, "AVX512F",     InstructionSet::AVX512F());
       support_message(pass, "AVX512PF",    InstructionSet::AVX512PF());
       support_message(pass, "BMI1",        InstructionSet::BMI1());
       support_message(pass, "BMI2",        InstructionSet::BMI2());
       support_message(pass, "CLFSH",       InstructionSet::CLFSH());
       support_message(pass, "CMPXCHG16B",  InstructionSet::CMPXCHG16B());
       support_message(pass, "CX8",         InstructionSet::CX8());
       support_message(pass, "ERMS",        InstructionSet::ERMS());
       support_message(pass, "F16C",        InstructionSet::F16C());
       support_message(pass, "FMA",         InstructionSet::FMA());
       support_message(pass, "FSGSBASE",    InstructionSet::FSGSBASE());
       support_message(pass, "FXSR",        InstructionSet::FXSR());
       support_message(pass, "HLE",         InstructionSet::HLE());
       support_message(pass, "INVPCID",     InstructionSet::INVPCID());
       support_message(pass, "LAHF",        InstructionSet::LAHF());
       support_message(pass, "LZCNT",       InstructionSet::LZCNT());
       support_message(pass, "MMX",         InstructionSet::MMX());
       support_message(pass, "MMXEXT",      InstructionSet::MMXEXT());
       support_message(pass, "MONITOR",     InstructionSet::MONITOR());
       support_message(pass, "MOVBE",       InstructionSet::MOVBE());
       support_message(pass, "MSR",         InstructionSet::MSR());
       support_message(pass, "OSXSAVE",     InstructionSet::OSXSAVE());
       support_message(pass, "PCLMULQDQ",   InstructionSet::PCLMULQDQ());
       support_message(pass, "POPCNT",      InstructionSet::POPCNT());
       support_message(pass, "PREFETCHWT1", InstructionSet::PREFETCHWT1());
       support_message(pass, "RDRAND",      InstructionSet::RDRAND());
       support_message(pass, "RDSEED",      InstructionSet::RDSEED());
       support_message(pass, "RDTSCP",      InstructionSet::RDTSCP());
       support_message(pass, "RTM",         InstructionSet::RTM());
       support_message(pass, "SEP",         InstructionSet::SEP());
       support_message(pass, "SHA",         InstructionSet::SHA());
       support_message(pass, "SSE",         InstructionSet::SSE());
       support_message(pass, "SSE2",        InstructionSet::SSE2());
       support_message(pass, "SSE3",        InstructionSet::SSE3());
       support_message(pass, "SSE4.1",      InstructionSet::SSE41());
       support_message(pass, "SSE4.2",      InstructionSet::SSE42());
       support_message(pass, "SSE4a",       InstructionSet::SSE4a());
       support_message(pass, "SSSE3",       InstructionSet::SSSE3());
       support_message(pass, "SYSCALL",     InstructionSet::SYSCALL());
       support_message(pass, "TBM",         InstructionSet::TBM());
       support_message(pass, "XOP",         InstructionSet::XOP());
       support_message(pass, "XSAVE",       InstructionSet::XSAVE());
       outstream << "\n";
    }
}
