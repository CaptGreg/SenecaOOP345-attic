#include <string>
#include <vector>

void csvParse(std::string& s, std::string delimiter, std::vector<std::string>& csvData)  // matches exact delimiter string, not a character member of delimiter
{
  size_t pos = 0;
  while ((pos = s.find(delimiter)) != std::string::npos) {
    std::string token = s.substr(0, pos);
    csvData.push_back(token);
    s.erase(0, pos + delimiter.length());
  }
  csvData.push_back(s);
}

/*
 usage:
       struct Point {
          std::string city;
          double latitude;
          double longitude;
      };

      std::vector<Point> items;

      std::fstream in("cities.txt", std::ios::in);
      if(in) {
        std::string line;
        getline(in,line);  // skip header line

        for(;;) {
          if(not getline(in,line))
            break;
          std::vector<std::string> csvData;
          csvParse(line, std::string(","),  csvData);
          // CSV format: Country,City,Accent City,Region,Latitude,Longitude   <--HEADER LINE
          //             0       1    2           3      4        5
          // ca,toronto,Toronto,08,43.6666667,-79.4166667
          // 0  1       2       3  4          5
          Point point;
          point.city = std::move(csvData[2]);
          point.latitude  = std::stod( std::move(csvData[4]) );
          point.longitude = std::stod( std::move(csvData[5]) );

          items.push_back(std::move(point));
        }
        in.close();
      }
*/
