// http://www.math.ucla.edu/~wittman/10a.1.10w/ccc/ch23/index.html

#include <map>
#include <queue>
#include <iostream>

using namespace std;

/**
   A utility class representing distance to a given city.
*/
class DistanceToCity 
{
public:
   DistanceToCity();
   DistanceToCity(string n, int d);
   bool operator<(const DistanceToCity& right) const;
   string get_name() const;
   int get_distance() const;
private:
   string name;
   int distance;
};

DistanceToCity::DistanceToCity()
{
   name = "";
   distance = 0;
}

DistanceToCity::DistanceToCity(string n, int d)
{
   name = n;
   distance = d;
}

bool DistanceToCity::operator<(const DistanceToCity& right) const
{
   return right.distance < distance;
}

inline string DistanceToCity::get_name() const { return name; }

inline int DistanceToCity::get_distance() const { return distance; }

/**
   A framework for finding shortest paths
   using Dijkstra's shortest path algorithm.
*/
class DistanceFinder 
{
public:
   /**
      Set the distance between two cities.
      @param from originating city
      @param to destination city
      @param distance distance between cities
   */
   void set_distance(string from, string to, int distance);

   /**
      Produce map of shortest distances.
      @param start originating city
      @param shortest map of shortest distances from start
   */
   void find_distance(string start, map<string, int>& shortest);

private:
   typedef multimap<string, DistanceToCity> CityMap;
   typedef CityMap::iterator Citr;
   CityMap cities;
};

void DistanceFinder::set_distance(string from, string to, int distance)
{
   cities.insert(CityMap::value_type(from, DistanceToCity(to, 
      distance)));
}

void DistanceFinder::find_distance(string start, 
   map<string, int>& shortest)
{
   priority_queue<DistanceToCity> que;
   que.push(DistanceToCity(start, 0));

   while (!que.empty()) 
   {
      DistanceToCity new_city = que.top();
      que.pop();
      if (shortest.count(new_city.get_name()) == 0)
      {
         int d = new_city.get_distance();
         shortest[new_city.get_name()] = d;
         Citr p = cities.lower_bound(new_city.get_name());
         Citr stop = cities.upper_bound(new_city.get_name());
         while (p != stop)
         {
            DistanceToCity next_destination = (*p).second;
            int total_distance = d + next_destination.get_distance();
            que.push(DistanceToCity(next_destination.get_name(),
							total_distance));
            ++p;
         }
      }
   }
}

int main()
{
   DistanceFinder d;
   d.set_distance("Pendleton", "Phoenix", 4);
   d.set_distance("Pendleton", "Pueblo", 8);
   d.set_distance("Pensacola", "Phoenix", 5);
   d.set_distance("Peoria", "Pittsburgh", 5);
   d.set_distance("Peoria", "Pueblo", 3);
   d.set_distance("Phoenix", "Peoria", 4);
   d.set_distance("Phoenix", "Pittsburgh", 10);
   d.set_distance("Phoenix", "Pueblo", 3);
   d.set_distance("Pierre", "Pendleton", 2);
   d.set_distance("Pittsburgh", "Pensacola", 4);
   d.set_distance("Princeton", "Pittsburgh", 2);
   d.set_distance("Pueblo", "Pierre", 3);

   map<string, int> shortest;
   d.find_distance("Pierre", shortest);
   map<string, int>::iterator current = shortest.begin();
   map<string, int>::iterator stop = shortest.end();
   while (current != stop)
   {
      pair<string, int> p = *current;
      cout << "distance to " << p.first << " is " << p.second << "\n";
      ++current;
   }
   return 0;
}

