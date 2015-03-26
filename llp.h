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

// NOTE with respect to C++ casting and c++ const
//
//  void sub(const int arg)
//      obj.write( (char*) &arg, sizeof(arg));  <== compiles
//
//  void sub(const int arg)
//      obj.write(reinterpret_cast<char*> (&arg), sizeof(arg)); <== error - violates 'const'
//
//  void sub(int arg)                                           <== remove const
//      obj.write(reinterpret_cast<char*> (&arg), sizeof(arg)); <== compiles

#include <fstream>
#include <iostream>
#include <string>

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
        obj.read(reinterpret_cast<char*> (&headvalue), sizeof(headvalue));
    }
    void writeHead(streampos& headvalue)
    {
        obj.seekp(0, ios::beg);
        obj.write(reinterpret_cast<char*> (&headvalue), sizeof(headvalue));
    }
    streampos addNode(const streampos link, const size_t length, const void* data)
    {
        obj.seekg(0, ios::end);
        streampos n = obj.tellg();
        // cout << "addNode: " << n << endl;

        obj.write( (char*) &link,   sizeof(link)   );

        obj.write( (char*) &length, sizeof(length) );
        obj.write( (char*) data,    length         );

        // obj.write(reinterpret_cast<char*> (&link),   sizeof(link));     // const issues

        // obj.write(reinterpret_cast<char*> (&length), sizeof(length));   // const issues
        // obj.write(reinterpret_cast<char*> (data),    length);           // const issues

        return n;
    }
public:
    LLPersistence(const char *filename) 
    {
        // cout << "ctor\n";
        // memory linked list:
        //        head = nullptr

        // if file does not exist, create file, initialize head
        // else if file does exist, read head

        if( (size_t) streamGetFileSize(filename) >= sizeof(head) ) {
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

    void llpDelete(streampos n)
    {
        // TODO: figure out a delete method
    }

    void clear()
    {
        // memory linked list:
        //        for(Node *n  = head;  n;  n = next) {
        //            next = n->link;
        //            delete n;
        //        }
        streampos link;
        for(streampos n = head; n; n = link) {
            obj.seekg(n, ios::beg);
            // obj.read((char*)&link, sizeof(link));
            obj.read(reinterpret_cast<char*> (&link), sizeof(link));
            llpDelete(n);
        }
        head = 0;
        writeHead(head);
    }

    void push(const void *data, const size_t length) 
    {
        // memory linked list:
        //        Node *n  = new Node
        //        n-> link = head
        //        n-> data = data
        //        head     = n

        head = addNode(head, length, data);
        writeHead(head);
    }

    void pop(void *& data, size_t& length) 
    {
        // memory linked list, page 130 of text:
        //  if(head) {
        //      Node *n = head;
        //      data = head->data;
        //      head = head->link;
        //      delete n;
        //  }
        //  return data;

        length = 0;

        if(head) {
            streampos n = head;

            // read node n
            obj.seekg(n, ios::beg);

            // read link
            streampos link;
            obj.read((char*) &link, sizeof(link));

            // read length
            obj.read((char*) &length, sizeof(length));

            // read data
            data = new char[length];
            obj.read((char*) data, length);

            // update head
            writeHead(link);

            llpDelete(n);
        }
    }


    void print() 
    {
        // memory linked list:
        //        for(Node *n  = head;  n;  n = n->link)
        //            cout << n-> data << end;
        for(streampos n = head, link; n; n = link) {
            // cout << "n: " << n << endl;
            obj.seekg(n, ios::beg);
            // obj.read((char*)&link, sizeof(link));
            obj.read(reinterpret_cast<char*> (&link), sizeof(link));
            // cout << "print: read link = " << link << endl;

            size_t length;
            obj.read((char*)&length, sizeof(length));
            // cout << "print: read length = " << length << endl;

            // allocate space
            char *data = new char[length];
            // read it
            obj.read(data, length);
            // print it
                cout << "print: " << length << " bytes [";
                cout.write(data,length);
                cout << "]" << endl;
            // free space
            delete [] data;

            // n = n->link
            n = link;
            // cout << endl;
        }
    }
    int countWhileLoop() 
    {
        // memory linked list:
        //        for(Node *n  = head;  n;  n = n->link)
        //            count++;
        int ret = 0;
        streampos n = head;
        // readHead(n);
        while(n) {
            ret++;
            obj.seekg(n, ios::beg);
            obj.read((char*)&n, sizeof(n));
        }
        return ret;
    }
    int countForLoop() 
    {
        // memory linked list:
        //        for(Node *n  = head;  n;  n = n->link)
        //            count++;
        int ret = 0;
        for(streampos n = head, link; n; n = link) {
            ret++;
            obj.seekg(n, ios::beg);
            obj.read((char*)&link, sizeof(link));
        }
        return ret;
    }
    int count() 
    {
        // return countWhileLoop();
        return countForLoop();
    }
};
