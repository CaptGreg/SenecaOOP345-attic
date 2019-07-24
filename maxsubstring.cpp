// an interview question:  find max substring without duplicate chars.
// random solutions from web, one for length of substring, the other for the substring.
// not all web solutions work.

#include <iostream>
#include <string>
#include <unordered_map>
using namespace std;

// https://duckduckgo.com/?q=c%2B%2B+find+the+longest+sub-string+wothout+a+repeating+character&t=canonical&atb=v1-1&ia=web&iax=qa
int lengthOfLongestSubstring(string& s) {
    size_t i = 0, max = 0;
    int offset = -1;
    int arr[256];
    fill_n(arr, 256, -1);
    for (i = 0; i < s.length(); i++) {
        int si = (int) s[i];
        offset = std::max(offset,arr[si]);
        max = std::max(max,i-offset);
        arr[si] = i;
    } 
    return max;
}

// https://www.geeksforgeeks.org/print-longest-substring-without-repeating-characters/
// Function to find and print longest 
// substring without repeating characters. 
string findLongestSubstring(string& str) 
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

int main()
{
  string s = "oandamakesmoney";
  cout << "'" << s << "' = (" << lengthOfLongestSubstring(s) << ") '" << findLongestSubstring(s) << "'\n";

  // test data from https://stackoverflow.com/questions/9734474/find-longest-substring-without-repeating-characters
  string input[] = { { "" }, { "a" }, { "ab" }, { "aab" }, { "abb" },
            { "aabc" }, { "abbc" }, { "aabbccdefgbc" },
            { "abcdeafghicabcdefghijklmnop" },
            { "abcdeafghicabcdefghijklmnopqrabcdx" },
            { "zxxaabcdeafghicabcdefghijklmnopqrabcdx" },
            {"aaabcdefgaaa"}};
  for (auto s : input) {
    cout << "'" << s << "' = (" << lengthOfLongestSubstring(s) << ") '" << findLongestSubstring(s) << "'\n";
  }
}
