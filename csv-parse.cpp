// https://stackoverflow.com/questions/14265581/parse-split-a-string-in-c-using-string-delimiter-standard-c
// GB - first example works fine.  
// GB - Mileage varies with other examples :-(
// GB - hacked to store tokens in string table (std::vector<std::string>) csvData 
// GB - NOTE: destroys input string

#include <iostream>
#include <string>
#include <vector>

void csvParse(std::string& s, std::string delimiter, std::vector<std::string>& csvData)
{
  size_t pos = 0;
  while ((pos = s.find(delimiter)) != std::string::npos) {
    std::string token = s.substr(0, pos);
    csvData.push_back(token);
    s.erase(0, pos + delimiter.length());
  }
  csvData.push_back(s);
}

void csvPrint(std::vector<std::string>& csvData)
{
  std::cout << "CSV data = ";
  for(auto e: csvData) {
    std::cout << "[" << e << "] ";
  }
  std::cout << "\n";
}

int main()
{
  std::string s = "scott>=anything >=tiger>= meat>= mushroom >=chloroform>=frog>=flies";
  std::string delimiter = ">=";
  std::vector<std::string> csvData;

  csvParse(s, delimiter, csvData);

  csvPrint(csvData);
}
