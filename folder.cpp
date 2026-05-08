#include "Folder.h"
#include <iostream>
#include <windows.h>   
#include <string>
using namespace std;

void setColor(int color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

Folder::Folder(const string& name, Node* parent)
    : Node(name, parent)
{
    head = nullptr;   // empty folder initially
}

Folder::~Folder()
{
    ChildNode* temp = head;

    while (temp != nullptr)
    {
        ChildNode* nextNode = temp->next;

        delete temp->data; // delete file/folder object
        delete temp;       // delete list node

        temp = nextNode;
    }
}

void Folder::addNode(Node* node)
{
    // check duplicate names
    ChildNode* temp = head;

    while (temp != nullptr)
    {
        if (temp->data->getName() == node->getName())
        {
            cout << "Error: Name already exists\n";
            return;
        }
        temp = temp->next;
    }

    // create new linked list node
    ChildNode* newNode = new ChildNode;
    newNode->data = node;
    newNode->next = nullptr;

    // if empty folder
    if (head == nullptr)
    {
        head = newNode;
        return;
    }

    // go to end
    temp = head;
    while (temp->next != nullptr)
    {
        temp = temp->next;
    }

    temp->next = newNode;
}

// Find node by name
Node* Folder::find(const string& name)
{
    ChildNode* temp = head;

    while (temp != nullptr)
    {
        if (temp->data->getName() == name)
            return temp->data;

        temp = temp->next;
    }

    return nullptr;
}

// Remove node
void Folder::removeNode(const string& name)
{
    ChildNode* temp = head;
    ChildNode* prev = nullptr;

    while (temp != nullptr)
    {
        if (temp->data->getName() == name)
        {
            // if first node
            if (prev == nullptr)
            {
                head = temp->next;
            }
            else
            {
                prev->next = temp->next;
            }

            delete temp->data;
            delete temp;

            cout << "Deleted successfully\n";
            return;
        }

        prev = temp;
        temp = temp->next;
    }

    cout << "Node not found\n";
}

void Folder::list() const {
    ChildNode* temp = head;
    while (temp != nullptr) {
        if (temp->data->isFolder()) {
            setColor(FOREGROUND_BLUE | FOREGROUND_INTENSITY);  
            cout << "[Folder] ";
        } 
        else {
            setColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);  
            cout << "[File] ";
        }
       
        cout << temp->data->getName();
        setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        
        cout << endl;
        temp = temp->next;
    }
}

// Open folder
void Folder::open()
{
    cout << "Opened Folder: " << name << "\n";
}

// Display folder info
void Folder::display() const
{
    cout << "Folder: " << name << "\n";
}

// Type check
bool Folder::isFolder() const
{
    return true;
}

void Folder::searchIn(const string& targetName, const string& path) {
    ChildNode* temp = head;
    while (temp != nullptr) {
        string fullPath = path + "/" + temp->data->getName();
        if (temp->data->getName() == targetName)
            cout << "Found at: " << fullPath << endl;
        if (temp->data->isFolder())
            ((Folder*)temp->data)->searchIn(targetName, fullPath);
        temp = temp->next;
    }
}
