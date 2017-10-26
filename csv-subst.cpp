// compiles and runs fine with gnu g++-7

// http://www.bfilipek.com/2017/09/the-expressive-cpp17-challenge.html
// http://www.bfilipek.com/2017/10/expressive-cpp17-results.html
// http://coliru.stacked-crooked.com/a/8b3d4be71e01e14e

// GB
//  - uses getline operating on stringstream with a delimter to split fields

/* Name: Fernando B. Giannasi
 * 
 * Features of C++17 used:
 * - std::filesystem
 * - nodiscard and noreturn attributes
 * - structured bindings
 * - string_view
 * - Template argument deduction for class templates
 * - Init-statement for if/switch
 * - std::optional
 */

/*
sample code, just to start, it's run with the following commands:

g++ -std=c++1z -O2 -Wall $1 -lstdc++fs
./a.out $2 $3 $4 $5
echo comparing "correct_output.csv" with $5
cmp --silent correct_output.csv $5 || echo "files are different!" 

where 
  $1 - cpp file name
  $2 - name of input csv
  $3 - column name
  $4 - replacement string
  $5 - output file name

*/


#include <algorithm>        // std::find
#include <experimental/filesystem>
#include <fstream>
#include <iostream>
#include <optional>
#include <string>           // not needed with gnu g++-7, (<algorithm> includes these files?)
#include <string_view>      // not needed with gnu g++-7, (<algorithm> includes these files?)
#include <sstream>          // not needed with gnu g++-7, (<algorithm> includes these files?)
#include <utility>          // not needed with gnu g++-7, (<algorithm> includes these files?)
#include <vector>

using namespace std;
namespace fs = std::experimental::filesystem;

void writeSampleData(string filename) // Greg Blair
{
string sampleData =
R"foo(First Name,Last Name,Age,City,Eyes color,Species
John,Doe,32,Tokyo,Blue,Human
Flip,Helm,12,Canberra,Red,Unknown
Terdos,Bendarian,165,Cracow,Blue,Magic tree
Dominik,Elpos,33,Paris,Purple,Orc
Brad,Doe,42,Dublin,Blue,Human
Ewan,Grath,51,New Delhi,Green,Human)foo";

  fstream os(filename, ios::out|ios::trunc);
  if(os.is_open()) { 
    os << sampleData;
    os.close(); 
  }
}

[[nodiscard]] auto split_string(const string_view& input, const char delimiter) 
{
    stringstream ss {input.data()};
    vector<string> result;
    // result.reserve(count(begin(input), end(input), delimiter));
    
    for (string buffer; 
         getline(ss, buffer, delimiter);) 
            {result.push_back(move(buffer));}
    
    return result;
}

[[nodiscard]] optional<int> get_target_column(ifstream& input,
                                              const string_view& label,
                                              const char delimiter)
{
    string first_line;
    
    if (getline(input, first_line); // Init-statement for if/switch
        first_line.size() == 0) { throw runtime_error("Input file missing"); }
    input.seekg(0);
    
    auto tokens = split_string(first_line, delimiter);
        
    if (auto it = find(begin(tokens), end(tokens), label); // Init-statement for if/switch
        it == tokens.end()) {
            return {}; //return empty optional
    } else {
        return distance(begin(tokens), it);
    }
}

[[nodiscard]] auto get_file_handlers(const string_view& input, const string_view& output)
{
    ifstream in_file {input.data(), ios::in};
    if (!in_file.is_open()) { throw runtime_error("Unable to open input file"); }
    
    ofstream out_file {output.data(), ios::out | ios::trunc};
    if (!out_file.is_open()) { throw runtime_error("Unable to open output file");}
    
    return pair(move(in_file), move(out_file)); //Template argument deduction for class templates (no make_pair)
}

void do_work(ifstream& input,
             ofstream& output,
             int target_index,
             const string_view& new_value,
             const char delimiter)
{
    string buffer;
    
    getline(input, buffer); // for the header line
    output << buffer << "\n";
    
    while (getline(input, buffer)) {
        auto tokens = split_string(buffer, delimiter);
        tokens[target_index] = new_value.data();
        
        for (auto& i: tokens) {
            output << i;
            output << (i == tokens.back() ? '\n':delimiter);
        }
    }
}

[[noreturn]] void usage_terminate(const string_view& progname) noexcept
{
    cout << "Usage: " << progname << " [IN_FILE] [COLUMN] [NEW_VALUE] [OUT_FILE]" << "\n";
    abort();
}

int main(int argc, char* argv[])
{
    try {
        if (argc != 5) { throw runtime_error("Bad arguments"); }

        writeSampleData(argv[1]); // Greg Blair

        auto [in_file, out_file] = get_file_handlers(argv[1], argv[4]);

        string_view new_value = argv[3];
        auto target_index = get_target_column(in_file, argv[2], ',');
        if (target_index) {
            do_work(in_file, out_file, *target_index, new_value, ',');
        }
        else {
            throw runtime_error("Column name doesn’t exist in the input file");
        }
    } catch (runtime_error& e) {
        cout << e.what() << "\n";
        usage_terminate(argv[0]);
    }
    
    return 0;
}
