// http://en.wikipedia.org/wiki/Binary_search_algorithm

const int KEY_NOT_FOUND = -1;
int binary_search(int A[], int key, int imin, int imax)
{
  // continue searching while [imin,imax] is not empty
  while (imax >= imin)
    {
      // calculate the midpoint for roughly equal partition
      auto midpoint = [] (int imin, int imax) { return (imin + imax) / 2; };
      int imid = midpoint(imin, imax);
      if(A[imid] == key)
        // key found at index imid
        return imid; 
      // determine which subarray to search
      else if (A[imid] < key)
        // change min index to search upper subarray
        imin = imid + 1;
      else         
        // change max index to search lower subarray
        imax = imid - 1;
    }
  // key was not found
  return KEY_NOT_FOUND;
}

int main()
{
}
