
// http://mmlviewer.books24x7.com/book/id_35754/viewer.asp?bookid=35754&chunkid=0892366498

// Hero's Inventory 3.0
// Demonstrates iterators

#include <iostream>
#include <string>
#include <list>

using namespace std;

int main()
{
    list<string> inventory;
    list<string>::iterator myIterator;
    inventory.push_back("sword");
    inventory.push_back("armor");
    inventory.push_back("shield");

    list<string>::iterator iter;
    list<string>::const_iterator const_iter;

    cout << "Your items:\n";
    for (iter = inventory.begin(); iter != inventory.end(); ++iter)
    {
        cout << *iter << "\n";
    }

    cout << "\nYou trade your sword for a battle axe.";
    myIterator = inventory.begin();
    *myIterator = "battle axe";
    cout << "\nYour items:\n";
    for (iter = inventory.begin(); iter != inventory.end(); ++iter)
    {
        cout << *iter << "\n";
    }

    cout << "\nThe item name '" << *myIterator << "' has ";
    cout << (*myIterator).size() << " letters in it.\n";

    cout << "\nThe item name '" << *myIterator << "' has ";
    cout << myIterator->size() << " letters in it.\n";

    cout << "\nYou recover a crossbow from a slain enemy.";
    inventory.insert(inventory.begin(), "crossbow");

    cout << "\nYour items:\n";
    for (iter = inventory.begin(); iter != inventory.end(); ++iter)
    {
        cout << *iter << "\n";
    }

    cout << "\nYour armor is destroyed in a fierce battle.";
    iter = inventory.begin(); iter++; iter++;
    inventory.erase(iter);
    cout << "\nYour items:\n";
    for (iter = inventory.begin(); iter != inventory.end(); ++iter)
    {
        cout << *iter << "\n";
    }

    cout << "for (list<string>::iterator iter = inventory.begin(); iter != inventory.end(); ++iter)\n";
    for (list<string>::iterator iter = inventory.begin(); iter != inventory.end(); ++iter)
    {
        cout << *iter << "\n";
    }

    cout << "for (auto iter = inventory.begin(); iter != inventory.end(); ++iter)\n";
    for (auto iter = inventory.begin(); iter != inventory.end(); ++iter)
    {
        cout << *iter << "\n";
    }

    cout << "for (auto e : inventory )\n";
    for (auto e : inventory )
    {
        cout << e << "\n";
    }


    return 0;
}
