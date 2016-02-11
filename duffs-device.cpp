void send( short *to, short  *from, int count ) // Tom Duff's device
{
  int n=(count+7)/8;
  switch(count%8){
  case 0:  do {  *to = *from++;
  case 7:    *to = *from++;
  case 6:    *to = *from++;
  case 5:    *to = *from++;
  case 4:    *to = *from++;
  case 3:    *to = *from++;
  case 2:    *to = *from++;
  case 1:    *to = *from++;
           } while(--n>0);
  }
}

void *memcpy(void *dst, void *src, int count) // GB Tom Duff's device applied to memcpy
{
  char *d   = (char*) dst;
  char *s   = (char*) src;
  int n=(count+7)/8;
  switch(count%8){
  case 0:  do {  *d++ = *s++;
  case 7:    *d++ = *s++;
  case 6:    *d++ = *s++;
  case 5:    *d++ = *s++;
  case 4:    *d++ = *s++;
  case 3:    *d++ = *s++;
  case 2:    *d++ = *s++;
  case 1:    *d++ = *s++;
           } while(--n>0);
  }
  return dst;
}

// GB see /usr/include/boost/numeric/ublas/detail/duff.hpp
//
//  Copyright (c) 2000-2002
//  Joerg Walter, Mathias Koch
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
//  The authors gratefully acknowledge the support of
//  GeNeSys mbH & Co. KG in producing this work.
//

#ifndef _BOOST_UBLAS_DUFF_
#define _BOOST_UBLAS_DUFF_

#define DD_SWITCH(n, d, r, expr) \
    { \
        unsigned r = ((n) + (d) - 1) / (d); \
        switch ((n) % (d))  { \
        case 0: do { expr;
#define DD_CASE_I(i, expr) \
        case (i): expr;
#define DD_WHILE(r) \
            } while (-- (r) > 0); \
        } \
    }

#define DD_1T(n, d, r, expr) \
    DD_WHILE(r)
#define DD_2T(n, d, r, expr) \
    DD_CASE_I(1, expr) \
    DD_1T(n, d, r, expr)
#define DD_3T(n, d, r, expr) \
    DD_CASE_I(2, expr) \
    DD_2T(n, d, r, expr)
#define DD_4T(n, d, r, expr) \
    DD_CASE_I(3, expr) \
    DD_3T(n, d, r, expr)
#define DD_5T(n, d, r, expr) \
    DD_CASE_I(4, expr) \
    DD_4T(n, d, r, expr)
#define DD_6T(n, d, r, expr) \
    DD_CASE_I(5, expr) \
    DD_5T(n, d, r, expr)
#define DD_7T(n, d, r, expr) \
    DD_CASE_I(6, expr) \
    DD_6T(n, d, r, expr)
#define DD_8T(n, d, r, expr) \
    DD_CASE_I(7, expr) \
    DD_7T(n, d, r, expr)

#define DD(n, d, r, expr) \
    DD_SWITCH(n, d, r, expr) \
    DD_##d##T(n, d, r, expr)

#endif

int main()
{
}
