int main(int argc, char**argv)
{
   class ABC {
   public:
     virtual void member() = 0;
   };

   // ABC abc; // error: variable type 'ABC' is an abstract class

   ABC* abcPtr;
   
   class Derived : public ABC {
   };

   class Concrete : public ABC {
   public:
    void member() { }
   };

   
   // Derived derived;
   Concrete concrete;

}
