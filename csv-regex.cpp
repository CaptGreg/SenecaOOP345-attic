#include <iostream>
#include <regex>

using namespace std;

/*
https://stackoverflow.com/questions/18144431/regex-to-split-a-csv

///////////////////////////////////////////////////////////////////////////
If you know that you won't have an empty field (,,) then this expression works well:

("[^"]*"|[^,]+)

///////////////////////////////////////////////////////////////////////////


This expression will:

    divide your sample text on the comma delimits
    will process empty values
    will ignore double quoted commas, providing double quotes are not nested
    trims the delimiting comma from the returned value
    trims surrounding quotes from the returned value

Regex: (?:^|,)(?=[^"]|(")?)"?((?(1)[^"]*|[^,"]*))"?(?=,|$)


///////////////////////////////////////////////////////////////////////////
Worked on this for a bit and came up with this solution:

(?:,|\n|^)("(?:(?:"")*[^"]*)*"|[^",\n]*|(?:\n|$))

Try it out here!

This solution handles "nice" CSV data like

"a","b",c,"d",e,f,,"g"

0: "a"
1: "b"
2: c
3: "d"#include <iostream>
///////////////////////////////////////////////////////////////////////////
#include <iosteam>
#include <iterator>
#include <string>
#include <regex>

int main()
{
  std::regex field_regex("(\"([^\"]*)\"|([^,]*))(,|$)");

  for (const std::string s : {
      "a,,hello,,o",
      "\"a\",,\"hello\",,\"o\"",
      ",,,,"})
  {
    std::cout << "parsing: " << s << "\n";
    std::cout << "======================================" << "\n";
    auto fieldNum = 0;
    for (auto it = std::sregex_iterator(s.begin(), s.end(), field_regex);
        it != std::sregex_iterator();
        ++it, ++fieldNum)
    {
      auto match = *it;
      auto extracted = match[2].length() ? match[2].str() : match[3].str();
      std::cout << "column[" << fieldNum << "]: " << extracted << "\n";
      if (match[4].length() == 0)
      {
        break;
      }
    }
    std::cout << "\n";
  }
}
4: e
5: f
6:
7: "g"

and uglier things like

"""test"" one",test' two,"""test"" 'three'","""test 'four'"""

0: """test"" one"
1: test' two
2: """test"" 'three'"
3: """test 'four'"""

Here's an explanation of how it works:

(?:,|\n|^)      # all values must start at the beginning of the file,  
                #   the end of the previous line, or at a comma  
(               # single capture group for ease of use; CSV can be either...  
  "             # ...(A) a double quoted string, beginning with a double quote (")  
    (?:         #        character, containing any number (0+) of  
      (?:"")*   #          escaped double quotes (""), or  
      [^"]*     #          non-double quote characters  
    )*          #        in any order and any number of times  
  "             #        and ending with a double quote character  

  |             # ...or (B) a non-quoted value  

  [^",\n]*      # containing any number of characters which are not  
                # double quotes ("), commas (,), or newlines (\n)  

  |             # ...or (C) a single newline or end-of-file character,  
                #           used to capture empty values at the end of  
  (?:\n|$)      #           the file or at the ends of lines  
)

///////////////////////////////////////////////////////////////////////////

I'm late to the party, but the following is the Regular Expression I use:

(?:,"|^")(""|[\w\W]*?)(?=",|"$)|(?:,(?!")|^(?!"))([^,]*?)(?=$|,)|(\r\n|\n)

This pattern has three capturing groups:

    Contents of a quoted cell
    Contents of an unquoted cell
    A new line

This pattern handles all of the following:

    Normal cell contents without any special features: one,2,three
    Cell containing a double quote (" is escaped to ""): no quote,"a ""quoted"" thing",end
    Cell contains a newline character: one,two\nthree,four
    Normal cell contents which have an internal quote: one,two"three,four
    Cell contains quotation mark followed by comma: one,"two ""three"", four",five

See this pattern in use.

///////////////////////////////////////////////////////////////////////////
If you have are using a more capable flavor of regex with named groups and lookbehinds, I prefer the following:

(?<quoted>(?<=,"|^")(?:""|[\w\W]*?)*(?=",|"$))|(?<normal>(?<=,(?!")|^(?!"))[^,]*?(?=(?<!")$|(?<!"),))|(?<eol>\r\n|\n)

///////////////////////////////////////////////////////////////////////////

(?:^"|,")(""|[\w\W]*?)(?=",|"$)|(?:^(?!")|,(?!"))([^,]*?)(?=$|,)|(\r\n|\n)

This slightly modified pattern handles lines where the first column is empty as long as you are not using Javascript. For some reason Javascript will omit the second column with this pattern. I was unable to correctly handle this edge-case.

///////////////////////////////////////////////////////////////////////////
("([^"]*)"|[^,]*)(,|$)

This matches a single column from the CSV line. The first portion "([^"]*)" of the expression is to match a quoted entry, the second part [^,]* is to match a non-quoted entry. Then either followed by a , or end of line $.


///////////////////////////////////////////////////////////////////////////

Here's the commented pattern (test it on Regexstorm.net):

(?<=\r|\n|^)(?!\r|\n|$)                       // Records start at the beginning of line (line must not be empty)
(?:                                           // Group for each value and a following comma or end of line (EOL) - required for quantifier (+?)
  (?:                                         // Group for matching one of the value formats before a comma or EOL
    "(?<Value>(?:[^"]|"")*)"|                 // Quoted value -or-
    (?<Value>(?!")[^,\r\n]+)|                 // Unquoted value -or-
    "(?<OpenValue>(?:[^"]|"")*)(?=\r|\n|$)|   // Open ended quoted value -or-
    (?<Value>)                                // Empty value before comma (before EOL is excluded by "+?" quantifier later)
  )
  (?:,|(?=\r|\n|$))                           // The value format matched must be followed by a comma or EOL
)+?                                           // Quantifier to match one or more values (non-greedy/as few as possible to prevent infinite empty values)
(?:(?<=,)(?<Value>))?                         // If the group of values above ended in a comma then add an empty value to the group of matched values
(?:\r\n|\r|\n|$)                              // Records end at EOL


Here's the raw pattern without all the comments or whitespace.

(?<=\r|\n|^)(?!\r|\n|$)(?:(?:"(?<Value>(?:[^"]|"")*)"|(?<Value>(?!")[^,\r\n]+)|"(?<OpenValue>(?:[^"]|"")*)(?=\r|\n|$)|(?<Value>))(?:,|(?=\r|\n|$)))+?(?:(?<=,)(?<Value>))?(?:\r\n|\r|\n|$)

///////////////////////////////////////////////////////////////////////////
*/

// https://www.softwaretestinghelp.com/regex-in-cpp/

#include <iostream>
#include <iterator>
#include <regex>
#include <string>

using namespace std;
 
// https://stackoverflow.com/questions/35085680/regex-pattern-to-extract-empty-field-from-csv-file
int main()
{
  const char* re = 

    R"RE((\"([^\"]*)\"|([^,]*))(,|$))RE"; 

    // R"RE(("[^"]*"|[^,]+)a)RE";     // fails (doesn't work)  because of null field, as expected

    // R"RE((?<=\r|\n|^)(?!\r|\n|$)(?:(?:"(?<Value>(?:[^"]|"")*)"|(?<Value>(?!")[^,\r\n]+)|"(?<OpenValue>(?:[^"]|"")*)(?=\r|\n|$)|(?<Value>))(?:,|(?=\r|\n|$)))+?(?:(?<=,)(?<Value>))?(?:\r\n|\r|\n|$))RE";
      // terminate called after throwing an instance of 'std::regex_error'
      // what():  Invalid special open parenthesis.

    // R"RE((?:^|,)(?=[^"]|(")?)"?((?(1)[^"]*|[^,"]*))"?(?=,|$))RE";     
      //  terminate called after throwing an instance of 'std::regex_error'
      //  what():  Invalid special open parenthesis.

    // R"RE((?:,"|^")(""|[\w\W]*?)(?=",|"$)|(?:,(?!")|^(?!"))([^,]*?)(?=$|,)|(\r\n|\n))RE";  // fails (doesn't work) 

    // R"RE((?<quoted>(?<=,"|^")(?:""|[\w\W]*?)*(?=",|"$))|(?<normal>(?<=,(?!")|^(?!"))[^,]*?(?=(?<!")$|(?<!"),))|(?<eol>\r\n|\n))RE";
      // terminate called after throwing an instance of 'std::regex_error'
      // what():  Invalid special open parenthesis.


  std::cout << "re=" << re << "\n";

  std::regex field_regex(re);

  for (const std::string s : {
      "a,b,hello,,o",
      R"("a","b","hello",,"o")",
      ",,,,",
      R"ugly("""test"" one",test' two,"""test"" 'three'","""test 'four'""",test","five)ugly"
      })
  {
    std::cout << "parsing: " << s << "\n";
    std::cout << "======================================" << "\n";
    auto fieldNum = 0;
    for (auto it = std::sregex_iterator(s.begin(), s.end(), field_regex);
        it != std::sregex_iterator();
        ++it, ++fieldNum)
    {
      auto match = *it;
      auto extracted = match[2].length() ? match[2].str() : match[3].str();  // auto is string
      std::cout << "column[" << fieldNum << "]: " << extracted << "\n";
      if (match[4].length() == 0)
      {
        break;
      }
    }
    std::cout << "\n";
  }
  return EXIT_SUCCESS;
}
