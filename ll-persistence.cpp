// Persistent Linked List stack
// The first record on disk is 
//      streampos head;

// head is followed by an unknown number of nodes
// Each node record looks like:
//      struct Node {
//          streampos link;
//          struct Data {
//              size_t    length;
//              void*     data;
//          }
//      }

#include <fstream>
#include <iostream>
#include <string>
#include <cstring>   // for strlen

using namespace std;

class LLPersistence {
private:
    fstream      obj;
    streampos    head;

private:
    streampos streamGetFileSize(const char *filename) // ifstream::pos_type also works with sizeof =  16
    {
        ifstream in(filename, ios::in | ios::binary | ios::ate); 
        if(in.is_open()) {
            return in.tellg(); 
        }
        return 0;
    }
    void readHead(streampos& headvalue)
    {
        obj.seekg(0, ios::beg);
        obj.read((char*)&headvalue, sizeof(headvalue));
    }
    void writeHead(streampos& headvalue)
    {
        obj.seekp(0, ios::beg);
        obj.write((char*)&headvalue, sizeof(headvalue));
    }
public:
    LLPersistence(const char *filename) 
    {
        // cout << "ctor\n";
        // memory linked list:
        //        head = nullptr

        // if file does not exist, create file, initialize head
        // else if file does exist, read head

        if(streamGetFileSize(filename) >= sizeof(head)) {
            obj.open(filename, ios::in | ios::out | ios::binary );
            if( obj.is_open() ) { 
                readHead(head);
            }
        } else {
            obj.open(filename, ios::trunc | ios::in | ios::out | ios::binary );
            if( obj.is_open() ) { 
                head = 0;
                writeHead(head);
            } else 
                throw string("ctor: cannot create file") + string(filename);
        }
    }

    ~LLPersistence() 
    {
        // cout << "dtor\n";
        obj.close();
    }

    void push(const void *p, const size_t l) 
    {
        // memory linked list:
        //        Node *n  = new Node
        //        n-> link = head
        //        n-> data = data
        //        head     = n

        obj.seekg(0, ios::end);
        streampos n = obj.tellg();
        // cout << "push: = " << n << "\n";

        obj.write((char*)&head, sizeof(head));

        obj.write((char*)&l, sizeof(l));
        obj.write((char*)p, l);

        writeHead(n);
        head = n;
    }

    void print() 
    {
        // memory linked list:
        //        for(Node *n  = head;  n;  n = n->link)
        //            cout << n-> data << end;
        readHead(head);
        streampos n = head;
        while(n) {
            // cout << "n: " << n << "\n";
            obj.seekg(n, ios::beg);
            streampos link;
            obj.read((char*)&link, sizeof(link));
            // cout << "print: read link = " << link << "\n";

            size_t length;
            obj.read((char*)&length, sizeof(length));
            // cout << "print: read length = " << length << "\n";

            // allocate space
            char *data = new char[length];
            // read it
            obj.read(data, length);
            // print it
                cout << "print: " << length << " bytes [";
                cout.write(data,length);
                cout << "]" << "\n";
            // free space
            delete [] data;

            // n = n->link
            n = link;
            // cout << "\n";
        }

    }
};

int main(int argc, char**argv) 
{
    const char *filename =  "llp.dat";

    LLPersistence llp(filename);

    char *p;

    p = const_cast<char *> ("one");
    llp.push( (void*) p, strlen(p) );

    p = const_cast<char *> ("two");
    llp.push( (void*) p, strlen(p) );

    p = const_cast<char *> ("three");
    llp.push( (void*) p, strlen(p) );

    p = const_cast<char *> ("four");
    llp.push( (void*) p, strlen(p) );

    p = const_cast<char *> ("five");
    llp.push( (void*) p, strlen(p) );

    // push args
    for(int i = 0; i < argc; i++)
        llp.push( (void*) argv[i], strlen(argv[i]) );

    llp.print();
}
