// http://mmlviewer.books24x7.com/book/id_35754/viewer.asp?bookid=35754&chunkid=876691173

// Hero's Inventory 2.0
// Demonstrates vectors

#include <iostream>
#include <string>
#include <vector>

using namespace std;

int main()
{
    vector<string> inventory;
    cout << "Your capacity is " << inventory.capacity() << " items.\n";

    inventory.push_back("sword");
    cout << "After one insertion, your capacity is " << inventory.capacity() << " items.\n";
    inventory.push_back("armor");
    cout << "After two insertions, your capacity is " << inventory.capacity() << " items.\n";
    inventory.push_back("shield");
    cout << "After three insertions, your capacity is " << inventory.capacity() << " items.\n";

    cout << "You have " << inventory.size() << " items.\n";

    cout << "\nYour items:\n";
    for (unsigned int i = 0; i < inventory.size(); ++i)
    {
        cout << inventory[i] << "\n";
    }

    cout << "\nYou trade your sword for a battle axe.";
    inventory[0] = "battle axe";
    cout << "\nYour items:\n";
    for (unsigned int i = 0; i < inventory.size(); ++i)
    {
        cout << inventory[i] << "\n";
    }

    cout << "\nThe item name '" << inventory[0] << "' has ";
    cout << inventory[0].size() << " letters in it.\n";

    cout << "\nYour shield is destroyed in a fierce battle.";
    inventory.pop_back();
    cout << "\nYour items:\n";
    for (unsigned int i = 0; i < inventory.size(); ++i)
    {
        cout << inventory[i] << "\n";
    }

    cout << "\nYou were robbed of all of your possessions by a thief.";
    inventory.clear();
    if (inventory.empty())
    {
        cout << "\nYou have nothing.\n";
    }
    else
    {
        cout << "\nYou have at least one item.\n";
    }

    cout << "You have " << inventory.size() << " items.\n";
    cout << "Your capacity is " << inventory.capacity() << " items.\n";

    return 0;
}
