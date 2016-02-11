
// http://mmlviewer.books24x7.com/book/id_35754/viewer.asp?bookid=35754&chunkid=0892366498

// Hero's Inventory 3.0
// Demonstrates iterators

#include <iostream>
#include <string>
#include <vector>

using namespace std;

int main()
{
    vector<string> inventory;
    vector<string>::iterator myIterator;
    inventory.push_back("sword");
    inventory.push_back("armor");
    inventory.push_back("shield");

    vector<string>::iterator iter;
    vector<string>::const_iterator const_iter;

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
    inventory.erase((inventory.begin() + 2));
    cout << "\nYour items:\n";
    for (iter = inventory.begin(); iter != inventory.end(); ++iter)
    {
        cout << *iter << "\n";
    }
    cout << "for (vector<string>::iterator it = inventory.begin(); it != inventory.end(); ++it)\n";
    for (vector<string>::iterator it = inventory.begin(); it != inventory.end(); ++it)
    {
        cout << *it << "\n";
    }
    cout << "for (auto it = inventory.begin(); it != inventory.end(); ++it)\n";
    for (auto it = inventory.begin(); it != inventory.end(); ++it)
    {
        cout << *it << "\n";
    }
    cout << "for (auto element : inventory)\n";
    for (auto element : inventory)
    {
        cout << element << "\n";
    }

    return 0;
}
