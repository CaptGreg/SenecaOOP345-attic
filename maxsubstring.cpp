// an interview question:  find max substring without duplicate chars.
// random solutions from web, one for length of substring, the other for the substring.
// not all web solutions work.

#include <algorithm>       // copy_if
#include <chrono>
#include <cstring>         // isalpha
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
using namespace std;

class Timer {
  decltype(std::chrono::high_resolution_clock::now()) start, stop;
public:
  void Start() { start = std::chrono::high_resolution_clock::now(); }
  void Stop()  { stop  = std::chrono::high_resolution_clock::now(); }
  uint64_t millisecs() { return std::chrono::duration_cast<std::chrono::duration<int,std::milli> >(stop-start).count(); }
  uint64_t microsecs() { return std::chrono::duration_cast<std::chrono::duration<int,std::micro> >(stop-start).count(); }
  uint64_t nanosecs () { return std::chrono::duration_cast<std::chrono::duration<int,std::nano>  >(stop-start).count(); }
};

// https://duckduckgo.com/?q=c%2B%2B+find+the+longest+sub-string+wothout+a+repeating+character&t=canonical&atb=v1-1&ia=web&iax=qa
int lengthOfLongestSubstring(string& s) { // GB fast
    size_t i = 0, max = 0;
    int offset = -1;
    int arr[256];
    fill_n(arr, 256, -1);
    for (i = 0; i < s.length(); i++) {
        int si = (int) s[i];
        offset = std::max(offset,arr[si]); // -1, unless seen before
        max = std::max(max,i-offset);
        arr[si] = i;
    } 

    return max;
}

string findMaxSubstring(string& s) { // GB adapt lengthOfLongestSubstringa to return substring
    size_t i = 0, max = 0;
    int offset = -1;
    size_t pos = 0;   // GB
    int arr[256];
    fill_n(arr, 256, -1);
    for (i = 0; i < s.length(); i++) {
        int si = (int) s[i];
        offset = std::max(offset,arr[si]); // -1, unless seen before
        // max = std::max(max,i-offset);   // GB save position of max string
        if(i-offset > max) {
          max = i-offset;
          pos = i;                    // GB
        }
        arr[si] = i;
    } 

    return max? s.substr(pos+1-max, max): "";
}

// https://www.geeksforgeeks.org/print-longest-substring-without-repeating-characters/
// Function to find and print longest 
// substring without repeating characters. 
string findLongestSubstring(string& str)   // GB slow
{ 
    int i; 
    int n = str.length(); 
  
    // starting point of current substring. 
    int st = 0; 
  
    // length of current substring. 
    int currlen; 
  
    // maximum length substring without repeating  
    // characters. 
    int maxlen = 0; 
  
    // starting index of maximum length substring. 
    int start; 
  
    // Hash Map to store last occurrence of each 
    // already visited character. 
    unordered_map<char, int> pos; 
  
    // Last occurrence of first character is index 0; 
    pos[str[0]] = 0; 
  
    for (i = 1; i < n; i++) { 
  
        // If this character is not present in hash, 
        // then this is first occurrence of this 
        // character, store this in hash. 
        if (pos.find(str[i]) == pos.end()) 
            pos[str[i]] = i; 
  
        else { 
            // If this character is present in hash then 
            // this character has previous occurrence, 
            // check if that occurrence is before or after 
            // starting point of current substring. 
            if (pos[str[i]] >= st) { 
  
                // find length of current substring and 
                // update maxlen and start accordingly. 
                currlen = i - st; 
                if (maxlen < currlen) { 
                    maxlen = currlen; 
                    start = st; 
                } 
  
                // Next substring will start after the last 
                // occurrence of current character to avoid 
                // its repetition. 
                st = pos[str[i]] + 1; 
            } 
  
            // Update last occurrence of 
            // current character. 
            pos[str[i]] = i; 
        } 
    } 
  
    // Compare length of last substring with maxlen and 
    // update maxlen and start accordingly. 
    if (maxlen < i - st) { 
        maxlen = i - st; 
        start = st; 
    } 
  
    // The required longest substring without 
    // repeating characters is from str[start] 
    // to str[start+maxlen-1]. 
    return str.substr(start, maxlen); 
}

int main(int argc, char*argv[])
{
  int r = EXIT_SUCCESS;

  // string s = "oandamakesmoneyfromcurrencyexchange";
  string s = "oandamakesmoney";
  cout << "'" << s << "' = (" << lengthOfLongestSubstring(s) << ") '" << "\n"; // findLongestSubstring(s) << "'\n";

  // test data from https://stackoverflow.com/questions/9734474/find-longest-substring-without-repeating-characters
  string input[] = { { "" }, { "a" }, { "ab" }, { "aab" }, { "abb" },
            { "aabc" }, { "abbc" }, { "aabbccdefgbc" },
            { "abcdeafghicabcdefghijklmnop" },
            { "abcdeafghicabcdefghijklmnopqrabcdx" },
            { "zxxaabcdeafghicabcdefghijklmnopqrabcdx" },
            {"aaabcdefgaaa"}};
  for (auto s : input)
    cout << "'" << s << "' = (" << lengthOfLongestSubstring(s) << ") '" << findLongestSubstring(s) << "'\n";

  if(argc == 1)
    return EXIT_SUCCESS;

  string file = argv[1];
  cout << "\n" << file << " example\n";
  fstream fs(file, ios::in);
  if(fs.is_open()) {
    string data { istreambuf_iterator<char>(fs), istreambuf_iterator<char>() }; // read file
    // string data;
    // copy(istreambuf_iterator<char>(fs), istreambuf_iterator<char>(), data.begin()); // read file
    fs.close();
    cout << data.size() << " chars read\n";
    string s;
    // s.reserve(data.size());
    // copy_if(data.begin(), data.end(), s.begin(), ::isalpha);
    // copy_if(data.begin(), data.begin() + 10000, s.begin(), ::isalpha);
    // cout << "copy_if complete\n";
    for(auto e : data) if(::isalpha(e)) s += e;
    cout << s.size() << " alpha chars\n";

    Timer t;
    t.Start();
    int length = lengthOfLongestSubstring(s);
    t.Stop();
    cout << "time for length: " << length << " " << 1e-9 * t.nanosecs() << " s.\n";

    t.Start();
    string result = findLongestSubstring(s);
    t.Stop();
    cout << "time for findLongestSubstring: '" << result << "' " << 1e-9 * t.nanosecs() << " s.\n";

    t.Start();
    result = findMaxSubstring(s);
    t.Stop();
    cout << "time for findMaxSubstring: '" << result << "' " << 1e-9 * t.nanosecs() << " s.\n";


    cout << "ALL DONE.\n";
  } else {
    cerr << "cannot open '" << file << "'\n";
    r = EXIT_FAILURE;
  }

  return r;
}
