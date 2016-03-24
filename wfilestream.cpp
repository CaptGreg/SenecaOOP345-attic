#include <locale>
#include <iostream>
#include <iomanip>
#include <fstream> 
#include <iosfwd>   // wofstream
#include <cstdlib>  // rand
using namespace std;

const wchar_t* wCharDigits_Chinese = L"零一二三四五六七八九"; // All 10 Chinese     digits 0 through 9
const wchar_t* wCharDigits_Korean  = L"공일이삼사오육칠팔구"; // All 10 Sino-Korean digits 0 through 9
// Correct Arabic numbers: [from right to left from 0 to 10 comma separated] 
// صفر، واحد، إثنان، ثلاثة، اربعة، خمسة، ستة، سبعة، ثمانية، تسعة، عشرة 
// Chinese: google translate
// Korean http://www.omniglot.com/language/numbers/korean.htm
// Sarout fixed the Arabic numbers

const std::wstring wString_Chinese = wCharDigits_Chinese;
const std::wstring wString_Korean  = wCharDigits_Korean;

/////////////////////////////////////////////////////////
//                 No thought here.                    //
// Wide character lanugages found on Google Translate. //
/////////////////////////////////////////////////////////

// macro SUPPORT defines and initializes two arrays
//  1. - a wide character array - const wchar_t *wCharDigits_##LANGUAGE[]
//  2. - a wide string    array - const std::wstring wString_##LANGUAGE[]
#define SUPPORT(LANGUAGE, ZERO, ONE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN) \
const wchar_t *wCharDigits_##LANGUAGE[] \
  = { ZERO, ONE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN }; \
const std::wstring wString_##LANGUAGE[] \
  = { ZERO, ONE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN };

SUPPORT( Amharic,
  L"ዜሮ", L"አንድ", L"ሁለት", L"ሶስት", L"አራት",
  L"አምስት", L"ስድስት", L"ሰባት", L"ስምት", L"ዘጠኝ",
  L"አስር" )

SUPPORT( Arabic,
  L"صفر",  L"واحد", L"إثنان", L"ثلاثة",   L"اربعة", 
  L"خمسة", L"ستة",  L"سبعة",  L"ثمانية", L"تسعة", 
  L"عشرة" )

SUPPORT( Belarusian,
  L"нуль", L"адзін", L"два", L"тры", L"чатыры",
  L"пяць", L"шэсць", L"сем", L"восем", L"дзевяць",
  L"дзесяць" )

SUPPORT( Bengali,
  L"শূন্য", L"এক", L"দুই", L"তিন", L"চার",
  L"পাঁচ", L"ছয়", L"সাত", L"আট", L"নয়",
  L"দশ" )

SUPPORT( Georgian,
  L"ნული", L"ერთ-ერთი", L"ორ", L"სამ", L"ოთხ",
  L"ხუთ", L"ექვსი", L"შვიდი", L"რვა", L"ცხრა",
  L"ათი" )

SUPPORT( Gujarati,
  L"શૂન્ય", L"એક", L"બે", L"ત્રણ", L"ચાર",
  L"પાંચ", L"છ", L"સાત", L"આઠ", L"નવ",
  L"દસ" )

SUPPORT( Hebrew,
  L"אֶפֶס", L"אֶחָד", L"דוּ", L"שְׁלוֹשָׁה", L"אַרְבָּעָה",
  L"חָמֵשׁ", L"שֵׁשׁ", L"שֶׁבַע", L"שמונה", L"תֵשַׁע",
  L"עשר" )

SUPPORT( Greek,
  L"μηδέν", L"ένας", L"δύο", L"τρία", L"τέσσερα",
  L"πέντε", L"έξι", L"επτά", L"οκτώ", L"εννέα",
  L"δέκα" )

SUPPORT( Hindi,
  L"शून्य", L"एक", L"दो", L"तीन", L"चार",
  L"पंज", L"छक्का", L"सात", L"आठ", L"नौ",
  L"दस" )

SUPPORT( Kannada,
  L"ಶೂನ್ಯ", L"ಒಂದು", L"ಎರಡು", L"ಮೂರು", L"ನಾಲ್ಕು",
  L"ಐದು", L"ಆರು", L"ಏಳು", L"ಎಂಟು", L"ಒಂಬತ್ತು",
  L"ಹತ್ತು" )

SUPPORT( Khmer,
  L"សូន្យ", L"មួយ", L"ពីរ", L"បី", L"បួននាក់",
  L"ប្រាំនាក់", L"ប្រាំមួយ", L"ចំនួនប្រាំពីរ", L"ប្រាំបី", L"ប្រាំបួន",
  L"ទាំងដប់" )

SUPPORT( Kazakh,
  L"нөл", L"бір", L"екі", L"үш", L"төрт",
  L"бес", L"алты", L"Жеті", L"сегіз", L"тоғыз",
  L"он" )

SUPPORT( Kyrgyz,
  L"нөл", L"бир", L"эки", L"үч", L"төрт",
  L"беш", L"алты", L"жети", L"сегиз", L"тогуз",
  L"он" )

SUPPORT( Lao,
  L"ສູນ", L"ຫນຶ່ງ", L"ສອງ", L"ສາມ", L"ສີ່",
  L"ຫ້າ", L"ຫົກ", L"ເຈັດ", L"ແປດ", L"ເກົ້າ",
  L"ສິບ" )

SUPPORT( Macedonian,
  L"нула", L"еден", L"две", L"три", L"четири",
  L"пет", L"шест", L"седум", L"осум", L"девет",
  L"десет" )

SUPPORT( Marathi,
  L"शून्य", L"एक", L"दोन", L"तीन", L"चार",
  L"पाच", L"सहा", L"सात", L"आठ", L"नऊ",
  L"दहा" )

SUPPORT( Mongolian,
  L"тэг", L"нэг", L"хоёр", L"гурван", L"дөрвөн",
  L"таван", L"зургаан", L"Долоо", L"найман", L"есөн",
  L"арван")

SUPPORT( Myanmar_Burmese,
  L"သုည", L"တစ်", L"နှစ်", L"သုံး", L"လေး",
  L"ငါး", L"ခွောကျ", L"ခုနှစ်", L"ရှစ်", L"ကိုး",
  L"တကျိပ်" )

SUPPORT( Nepali,
  L"शून्य", L"एक", L"दुई", L"तीन", L"चार",
  L"पाँच", L"छ", L"सात", L"आठ", L"नौ",
  L"दस" )

SUPPORT( Punjabi,
  L"ਜ਼ੀਰੋ", L"ਇੱਕ", L"ਦੋ", L"ਤਿੰਨ", L"ਚਾਰ",
  L"ਪੰਜ", L"ਛੇ", L"ਸੱਤ", L"ਅੱਠ", L"ਨੌ",
  L"ਦਸ" )

SUPPORT( Serbian,
  L"нула", L"један", L"два", L"три", L"четири",
  L"пет", L"шест", L"седам", L"осам", L"девет",
  L"десет" )

SUPPORT( Sinhala,
  L"ශුන්ය", L"එක", L"දෙක", L"තුන්", L"සිව්",
  L"පහ", L"හය", L"හතක්", L"අට", L"නවය",
  L"දස" )

SUPPORT( Tajik,
  L"нол", L"як", L"ду", L"се", L"чаҳор",
  L"панҷ", L"шаш", L"Ҳафт", L"ҳашт", L"нӯҳ",
  L"даҳ" )

SUPPORT( Tamil,
  L"பூஜ்யம்", L"ஒரு", L"இரண்டு", L"மூன்று", L"நான்கு",
  L"ஐந்து", L"ஆறு", L"ஏழு", L"எட்டு", L"ஒன்பது",
  L"பத்து" )

SUPPORT( Telugu, // our friends from Andre Pradesh
  L"సున్నా", L"ఒకటి", L"ఇద్దరు", L"మూడు", L"నాలుగు",
  L"ఐదు", L"ఆరు", L"ఏడు", L"ఎనిమిది", L"తొమ్మిది",
  L"పది" )

SUPPORT( Thai,
  L"ศูนย์", L"หนึ่ง", L"สอง", L"สาม", L"สี่",
  L"ห้า", L"หก", L"เจ็ด", L"แปด", L"เก้า",
  L"สิบ" )

SUPPORT( Vietnamese,
  L"số không", L"một", L"số hai", L"số ba", L"bốn",
  L"năm", L"sáu", L"bảy", L"tám", L"chín",
  L"mười" )

SUPPORT( Ukranian,
  L"нуль", L"один", L"два", L"три", L"чотири",
  L"п'ять", L"шість", L"сім", L"вісім", L"дев'ять",
  L"десять" )

SUPPORT( Yoruba,
  L"odo", L"ọkan", L"meji", L"mẹta", L"mẹrin",
  L"marun", L"mefa", L"meje", L"mẹjọ", L"mẹsan",
  L"mẹwa")

///////////////////////////////////////////////////
// End Google Translate wide character languages //
///////////////////////////////////////////////////

void WideCharSetup()
{
#ifdef __MSVC
  _setmode(_fileno(stdout), _O_WTEXT); // or __O_U16TEXT); // or _O_WTEXT
#endif

  std::locale::global( std::locale( "" ) ) ;
  std::wcout.imbue( std::locale() ) ;
  // std::wcin.imbue( std::locale() ) ;       // not using wcin
  // std::wcerr.imbue( std::locale() ) ;      // not using wcerr

  // test it
  #define P(L,D) wcout << #L " wchar_t characters for 0 to 9: " << D << " ==> " << wcslen(D) << " wchar_t\n";
  P(Chinese, wCharDigits_Chinese)
  P(Korean, wCharDigits_Korean)
//P(Arabic, arabicWCharDigits)

  // wcout << "Chinese wchar_t characters for 0 to 9: " << wCharDigits_Chinese << "\n";
  // wcout << "Korean  wchar_t characters for 0 to 9: " << wCharDigits_Korean << "\n";
  // wcout << "Arabic  wchar_t characters for 0 to 9: " << arabicWCharDigits << "\n";
  wcout << "\n";

  wcout << "Chinese std::wstring characters for 0 to 9: " << wString_Chinese << "\n";
  wcout << "Korean  std::wstring characters for 0 to 9: " << wString_Korean << "\n";
  wcout << "\n";


  for(int i = 0; i <= 9; i++) 
    wcout << "Ascii digit " << i 
          << " is Chinese "   << wCharDigits_Chinese[i] 
          << ", Sino-Korean " << wCharDigits_Korean[i] 
          << ", Arabic " << std::setw(8) << wCharDigits_Arabic[i] 
          << "\n";

  // works!
}

template<typename T> // a function template
void WriteTestFile(string& file, T& digits) // need T& so this works with 'const std::wstring'
{

  // write a test file with 1 million wide characters
  size_t size = 1000000;

  std::wofstream wos(file, wios::out | wios::trunc);
  if(wos.is_open()) {
    for(size_t i = 0; i < size; i++) {
       wchar_t wc;

       wc =  digits[rand() % 10];
       wos << wc;
    }
    wos.close();
  }
}

// void Translate(const wchar_t* digits, string& fileIn, string& fileOut)
template<typename T> // a function template
void Translate(T& digits, string& fileIn, string& fileOut)
{
  std::wifstream wis(fileIn,  wios::in);
  std::ofstream  os (fileOut, ios::out | ios::trunc);

  if(wis.is_open() && os.is_open()) {
    wchar_t wc;
    while( wis >> wc ) {
      for(size_t i = 0; i < wcslen(digits); i++) {
        if(wc == digits[i]) {
          char c = '0' + i;
          os << c;
          break;
        }
      }
    }
  } else {
    throw string("problem creating '") + fileOut + "' or opening '" + fileIn + "'\n";
  }

  if(wis) wis.close();
  if(os)  os .close();
}


int main(int argc, char* argv[], char* envp[])
// int wmain(int argc,wchar_t*argv[],wchar_t*envp[]) // appears to be a Microsoft extension, not C++
{
  try {

    string rootFileName   =  string(argv[0]);

    string fileChinese    = rootFileName + ".dat.Chinese";
    string fileChineseOut = fileChinese  + ".ASCII";

    string fileKorean     = rootFileName + ".dat.Korean";
    string fileKoreanOut  = fileKorean   + ".ASCII";

    WideCharSetup();

    WriteTestFile(fileChinese, wCharDigits_Chinese);
    WriteTestFile(fileChinese, wString_Chinese);

    WriteTestFile(fileKorean,  wCharDigits_Korean );
    WriteTestFile(fileKorean,  wString_Korean );

    Translate(wCharDigits_Chinese, fileChinese, fileChineseOut);
    Translate(wCharDigits_Korean,  fileKorean,  fileKoreanOut);

  } catch(string& e) {
    cerr << e << "\n";
  }

  return 0;
}
