// http://www.cplusplus.com/reference/list/list/emplace_back/

// list::emplace_back
#include <iostream>
#include <list>

int main ()
{
  std::list< std::pair<int,char> > mylist;

  mylist.emplace_back(10,'a');
  mylist.emplace_back(20,'b');
  mylist.emplace_back(30,'c');

  std::cout << "mylist contains:";
  for (auto& x: mylist)
    std::cout << " (" << x.first << "," << x.second << ")";
  std::cout << "\n";

  mylist.push_back(std::make_pair(40,'d'));
  mylist.push_back(std::make_pair(50,'e'));
  mylist.push_back(std::make_pair(60,'f'));

  std::cout << "mylist contains:";
  for (auto x: mylist)
    std::cout << " (" << x.first << "," << x.second << ")";
  std::cout << "\n";


  return 0;
}
