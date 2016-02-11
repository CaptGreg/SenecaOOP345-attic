// http://msdn.microsoft.com/en-us/library/dd293665.aspx

// MemoryBlock.h
// #pragma once
#include <iostream>
#include <algorithm>

class MemoryBlock
{
public:

   // Simple constructor that initializes the resource.
   explicit MemoryBlock(size_t length)
      : _length(length)
      , _data(new int[length])
   {
      std::cout << "In MemoryBlock(size_t). length = "
                << _length << "." << "\n";
   }

   // Destructor.
   ~MemoryBlock()
   {
      std::cout << "In ~MemoryBlock(). length = "
                << _length << ".";
      
      if (_data != NULL)
      {
         std::cout << " Deleting resource.";
         // Delete the resource.
         delete[] _data;
      }

      std::cout << "\n";
   }

   // Copy constructor.
   MemoryBlock(const MemoryBlock& other)
      : _length(other._length)
      , _data(new int[other._length])
   {
      std::cout << "In MemoryBlock(const MemoryBlock&). length = " 
                << other._length << ". Copying resource.\n";

      std::copy(other._data, other._data + _length, _data);
   }

   // Copy assignment operator.
   MemoryBlock& operator=(const MemoryBlock& other)
   {
      std::cout << "In operator=(const MemoryBlock&). length = " 
                << other._length << ". Copying resource." << "\n";

      if (this != &other)
      {
         // Free the existing resource.
         delete[] _data;

         _length = other._length;
         _data = new int[_length];
         std::copy(other._data, other._data + _length, _data);
      }
      return *this;
   }

   // Move assignment operator.
   MemoryBlock& operator=(MemoryBlock&& other)
   {
      std::cout << "In operator=(MemoryBlock&&). length = " 
                << other._length << "." << "\n";
   
      if (this != &other)
      {
         // Free the existing resource.
         delete[] _data;
   
         // Copy the data pointer and its length from the 
         // source object.
         _data = other._data;
         _length = other._length;
   
         // Release the data pointer from the source object so that
         // the destructor does not free the memory multiple times.
         other._data = NULL;
         other._length = 0;
      }
      return *this;
   }

   MemoryBlock(MemoryBlock&& other) // Move constructor.
      : _data(NULL)
      , _length(0)
   {
      std::cout << "In MemoryBlock(MemoryBlock&&). length = " 
                << other._length << ". Moving resource." << "\n";
   
      // Copy the data pointer and its length from the 
      // source object.
      _data = other._data;
      _length = other._length;
   
      // Release the data pointer from the source object so that
      // the destructor does not free the memory multiple times.
      other._data = NULL;
      other._length = 0;
   }
   // Retrieves the length of the data resource.
   size_t Length() const
   {
      return _length;
   }

private:
   size_t _length; // The length of the resource.
   int* _data; // The resource.
};



// rvalue-references-move-semantics.cpp
// compile with: /EHsc
#include <vector>

using namespace std;

int main(int argc,char**argv)
{
   // Create a vector object and add a few elements to it.
   vector<MemoryBlock> v;
   v.push_back(MemoryBlock(25));
   v.push_back(MemoryBlock(75));

   // Insert a new element into the second position of the vector.
   v.insert(v.begin() + 1, MemoryBlock(50));
}
