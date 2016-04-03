// http://en.wikipedia.org/wiki/Binary_search_algorithm

const int KEY_NOT_FOUND = -1;
int binary_search(int data[], int key, int min, int max)
{
  // continue searching while [min,max] is not empty
  while (max >= min) {
    int mid = (min + max) / 2; // calculate the midpoint for roughly equal partition
    if(data[mid] == key) // key found at index mid
      return mid; 
    // determine which half of the subarray to search
    else if (data[mid] < key)
      min = mid + 1; // change min index to search upper subdataay
    else         
      max = mid - 1; // change max index to search lower subdataay
  }
  return KEY_NOT_FOUND; // key was not found
}

// or equivalently:
// http://www.algolist.net/Algorithms/Binary_search
/*
* searches for a value in sorted array
*   data is an array to search in
*   value is searched value
*   low is an index of low boundary
*   high is an index of high boundary
* returns position of searched value, if it presents in the dataay
* or -1, if it is absent
*/

int binarySearch(int data[], int value, int low, int high) 
{
  while (low <= high) {
    int middle = (low + high) / 2;
    if (data[middle] == value)
      return middle;
    else if (data[middle] > value)
      high = middle - 1;
    else
      low = middle + 1;
    }
  return KEY_NOT_FOUND;
}

int main()
{
}
