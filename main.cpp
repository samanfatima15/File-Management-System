#include "Folder.h"
#include "File.h"   
#include <iostream>

using namespace std;

int main()
{
  
    Folder* root = new Folder("Root", nullptr);


    Node* file1 = new File("notes.txt", root);
    Node* file2 = new File("homework.cpp", root);

    Folder* docs = new Folder("Documents", root);


    root->addNode(file1);
    root->addNode(file2);
    root->addNode(docs);

    Node* file3 = new File("resume.pdf", docs);
    docs->addNode(file3);

    cout << "\n--- ROOT CONTENTS ---"<<endl;
    root->list();

    cout << "\n--- DOCUMENTS CONTENTS ---"<<endl;
    docs->list();

    cout << "\n--- SEARCH TEST ---"<<endl;
    Node* found = root->find("homework.cpp");
    if (found)
        cout << "Found: " << found->getName() << endl;
    else
        cout << "Not found"<<endl;

    cout << "\n--- DELETE TEST ---" << endl;;
    root->removeNode("notes.txt");

    cout << "\n--- ROOT AFTER DELETE ---" << endl;;
    root->list();

    delete root;

    return 0;
}
