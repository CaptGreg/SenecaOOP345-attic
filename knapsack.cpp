// http://www.cplusplus.com/forum/general/75356/

// I've got this code from the pseudo-code here:
// http://books.google.com/books?id=QrvsNy9paOYC&pg=PA235&lpg=PA235&dq=knapsack+problem+branch+and+bound+C%2B%2B&source=bl&ots=e6ok2kODMN&sig=Yh5__d3iAFa5rEkaCoBJ2JAWybk&hl=en&sa=X&ei=k1EDULDrHIfKqgHqtYyxDA&ved=0CEsQ6AEwAA#v=onepage&q=knapsack%20problem%20branch%20and%20bound%20C%2B%2B&f=false

#include <queue>
#include <iostream>
using namespace std;

struct node
{
  int level;
  int profit;
  int weight;
  float bound;

  bool operator<(const node& right) const
  {
    return bound < right.bound;
  }
};

float bound(node u, int n, int W, int p[], int w[])
{
  int j, k;
  int totweight;
  float result;

  if (u.weight >= W)
  {
    return 0;
  }
  else
  {
    result = u.profit;
    j = u.level + 1;
    totweight = u.weight;
    while (j <= n && totweight + w[j] <= W)
    {
      totweight = totweight + w[j];
      result = result + p[j];
      j++;
    }
    k = j;
    if (k <= n)
    {
      result = result + (W - totweight) * p[k]/w[k];
    }
    return result;
  }
}


void knapsack(int n, int p[], int w[], int W, int& maxProfit)
{
  priority_queue<node> Q;
  node u, v;
  Q.empty();

  v.level = 0;
  v.profit = 0;
  v.weight = 0;

  maxProfit = 0;

  v.bound = bound(v, n, W, p, w);
  Q.push(v);

  while (Q.size() > 0)
  {
    Q.pop();
    if (v.bound > maxProfit)
    {
      u.level = v.level + 1;
      u.weight = v.weight + w[u.level];
      u.profit = v.profit + p[u.level];
      if (u.weight <= W && u.profit > maxProfit)
      {
        maxProfit = u.profit;
      }
      u.bound = bound(u, n, W, p, w);
      if (u.bound > maxProfit)
      {
        Q.push(u);
      }
      u.weight = v.weight;
      u.profit = v.profit;
      u.bound = bound(u, n, W, p, w);
      if (u.bound > maxProfit)
      {
        Q.push(u);
      }
    }
  }
}


int main()
{
  int maxProfit;
  int n = 4;
  int W = 7;
  int p[4] = {2, 5, 3, 4};
  int w[4] = {2, 40, 18, 28};

  knapsack(n, p, w, W, maxProfit);

  cout << maxProfit << endl;
}
