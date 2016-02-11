// lambda's need at least the [] square brackets

int main()
{
  auto f = [] () -> void { };  // OK
  auto g = [] ()         { };  // OK
  auto h = []            { };  // OK

#ifdef SHOW_SYNTAX_ERRORS
  auto i = []    -> void { };  // syntax error, but it previously compiled
  auto j =    ()         { };  // syntax error
  auto k =               { };  // syntax error
#endif
}
