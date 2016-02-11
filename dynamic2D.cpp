// OOP345 week two compound types
// https://scs.senecac.on.ca/~oop345/pages/content/compt.html

// GB 
//    fix syntax error (nc,nr)
// +  change int table to double table to better illustrate table data
// +  versus table labels

 // Dynamically Allocated 2d Arrays
 // dynamic2D.cpp

 #include <iostream>
 #include <iomanip>

 int main () {
     int c, r;
     double **a;  // points to row addresses

     std::cout << "Number of rows : ";
     std::cin >> r;
     std::cout << "Number of columns : ";
     std::cin >> c;

     a = new double*[r]; // for row addresses

     for (int i = 0; i < r; i++)
         a[i] = new double[c]; // for elements
                            // of row i
     for (int i = 0; i < r; i++)
         for (int j = 0; j < c; j++)
             a[i][j] =  (r*c  - (i + 1) * (j + 1)) / 10.;

     std::cout << "   ";
     // for (int j = 0; j < nc; j++)
     for (int j = 0; j < c; j++)     // GB change nc to c
         std::cout << std::setw(4) << j + 1;
     std::cout << "\n";
     for (int i = 0; i < r; i++) {
         std::cout << std::setw(4) << i + 1;
         for (int j = 0; j < c; j++)
             std::cout << std::setw(4) << std::fixed << std::setprecision(1) << a[i][j];
         std::cout << "\n";
     }

     // for (int i = 0; i < nr; i++)
     for (int i = 0; i < r; i++) // GB change nr to r
         delete [] a[i]; // deallocate row i
     delete [] a; // deallocate pointers to rows
 }
