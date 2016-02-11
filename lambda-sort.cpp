#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main()
{
        const int N=5;
        vector<long double> v(N);

        for(int i = N - 1; i >= 0; i--) v[i] = (N-i) + 0.1 * (N-i);
        cout<<"data           : "; for(auto i: v)  cout<<i<<" "; cout << "\n";

        sort(v.begin(),v.end(),[](int i, int j) -> bool{ return (i < j);}); // inline ascending
        cout<<"ascending sort : "; for(auto i: v)  cout<<i<< " "; cout << "\n";

        auto descending = [](int i, int j) -> bool{ return (i > j);};
        sort(v.begin(),v.end(),descending);
	cout<<"descending sort: "; for(auto i: v)  cout<<i<<" "; cout << "\n";

        auto ascending = [](int i, int j) -> bool{ return (i < j);};
        auto b=v.begin(), e=v.end();
        sort(b,e,ascending);
        cout <<"ascending sort : "; for(auto i: v)  cout<<i<<" "; cout << "\n";

	// more lambda

	cout << [](double m,double X,double b)-> float { return m*X + b; } (2,10,4) << "\n";
	auto line = [](double m,double X,double b) -> float{ return m*X + b; };
	cout << line(2,10,4) << "\n";

	cout << [](double a,double X,double b,double Y,double D) -> float { return a*X + b*Y - D; } (2,10,3,5,4) << "\n";
	auto plane = [](double a,double X,double b,double Y,double D) -> float { return a*X + b*Y - D; };
	cout << plane(2,10,3,5,4) << "\n";

        return 0;
}
