#if 0
// GO
package main

import "fmt"

type student struct {
  name string
  class string
  grades string
}

func main() {

  var students map[int]student
  students = make(map[int]student)
  students[1] = student{"Walker", "calculus", "A"}

  for k, v := range students {
    fmt.Println("key=",k, " name=" , v.name)
  }
}
#endif

#include <iostream>
#include <map>
#include <string>
using namespace std;

int main()
{
  struct student {
    string name;
    string course;   // 'class' is a reserved word. use 'course' instead
    string grades;
  };

  map<int,student> students;
  students.insert( make_pair(1,student{"Walker", "calculus", "A"}) );

  for(auto e : students)
    std::cout << "key=" << e.first << " name=" << e.second.name << std::endl;
}
