// lambda's need at least the [] square brackets

int main()
{
  auto f = [] () -> void { };  // OK
  auto g = [] ()         { };  // OK
  auto h = []            { };  // OK
  auto i = []    -> void { };  // OK

#ifdef SHOW_SYNTAX_ERRORS
  auto j =    ()         { };  // syntax error
  auto k =               { };  // syntax error
#endif
}
