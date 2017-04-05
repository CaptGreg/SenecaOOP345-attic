class X {
  int i;
public:
  X()
  {
    i         = 1; // compiles

    this->i   = 2; // compiles

    (*this).i = 3; // compiles

    this.i    = 4; // error: request for member ‘i’ in ‘(X*)this’, which is of pointer type ‘X*’ (maybe you meant to use ‘->’ ?)
  }
};

int main()
{
}

