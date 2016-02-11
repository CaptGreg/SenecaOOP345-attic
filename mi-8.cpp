// 
#include<iostream>
using namespace std;
#include<cstdio>
#ifdef __GNUC__
// char getch() { char c; cin >> c; return c; }
void clrscr() {}
#else
 #include<conio.h>
#endif
// #define gets(b) cin.getline(b,sizeof(b))
#define gets(b) cin >> b

 /*************************************************************************///------------------------------  item  ---------------------------------///*************************************************************************/
class item
    {
       private:
        char part_name[20];
        float part_price;

       public:
        void get_data();
        void show_data();
    };

 /*************************************************************************///------------------------------  sales  --------------------------------///*************************************************************************/
class sales
    {
       private:
        float sales_fig[3];

       public:
        void get_data();
        void show_data();
    };

 /*************************************************************************///---------------------------  hardware_item  ---------------------------///*************************************************************************/
class hardware_item:public item,public sales
    {
       private:
        char category[10];
        char manufacturers[10];

       public:
        void get_data();
        void show_data();
    };

 /*************************************************************************///--------------------------  software_item  ----------------------------///*************************************************************************/
class software_item:public item,public sales
    {
       private:
        char category[10];
        char operating_system[10];

       public:
        void get_data();
        void show_data();
    };


 /*************************************************************************///----------------------------  get_data( )  ----------------------------///*************************************************************************/
void item::get_data()
    {
       cout<<"\t Enter part name = ";
       gets(part_name);
       // cin >> part_name;

       cout<<"\t Enter part price = ";
       cin>>part_price;
    }

 /*************************************************************************///----------------------------  show_data( )  ---------------------------///*************************************************************************/
void item::show_data()
    {
       cout<<"\t Part name = "<<part_name<<"\n";
       cout<<"\t Part price = "<<part_price<<"\n";
    }


 /*************************************************************************///----------------------------  get_data( )  ----------------------------///*************************************************************************/
void sales::get_data()
    {
       cout<<"\n Enter the Sales figures for three months : "<<"\n";

       for(int count=0;count<3;count++)
      {
         cout<<"\t Sales for month  "<<count+1<<" = ";
         cin>>sales_fig[count];
      }
    }

 /*************************************************************************///----------------------------  show_data( )  ---------------------------///*************************************************************************/
void sales::show_data()
    {
       cout<<"\n Sales figures for three months : "<<"\n";

       for(int count=0;count<3;count++)
       cout<<"\t Sales for month  "<<count+1<<" = "<<sales_fig[count]<<"\n";
    }


 /*************************************************************************///----------------------------  get_data( )  ----------------------------///*************************************************************************/
void hardware_item::get_data()
    {
       item::get_data();

       cout<<"\t Enter the category = ";
       gets(category);
       // cin >> category;

       cout<<"\t Enter the manufacturers = ";
       gets(manufacturers);
       // cin >> manufacturers;

       sales::get_data();
    }

 /*************************************************************************///----------------------------  show_data( )  ---------------------------///*************************************************************************/
void hardware_item::show_data()
    {
       item::show_data();

       cout<<"\t Category = "<<category<<"\n";
       cout<<"\t Manufacturers = "<<manufacturers<<"\n";

       sales::show_data();
    }


 /*************************************************************************///----------------------------  get_data( )  ----------------------------///*************************************************************************/
void software_item::get_data()
    {
       item::get_data();

       cout<<"\t Enter the category = ";
       gets(category);
       // cin >> category;

       cout<<"\t Enter the operating system = ";
       gets(operating_system);
       // cin >> operating_system;

       sales::get_data();
    }

 /*************************************************************************///----------------------------  show_data( )  ---------------------------///*************************************************************************/
void software_item::show_data()
    {
       item::show_data();

       cout<<"\t Category = "<<category<<"\n";
       cout<<"\t Operating system = "<<operating_system<<"\n";

       sales::show_data();
    }

int main (int argc, char**argv)
    {
       cout << "\n" << argv[0] << "\n" << "\n";
       clrscr();

       hardware_item h;
       software_item s;

       cout<<"\n ********* Hardware Item ********"<<"\n";
       h.get_data();

       cout<<"\n ********* Software Item ********"<<"\n";
       s.get_data();

       // getch();
       // clrscr();

       cout<<"\n ********* Hardware Item ********"<<"\n";
       h.show_data();

       cout<<"\n ********* Software Item ********"<<"\n";
       s.show_data();

       // getch();
       return 0;
    }
