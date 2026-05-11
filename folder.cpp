#include "Folder.h"
#include <iostream>
#include <windows.h>
#include <string>
using namespace std;

static void setColor(int color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

Folder::Folder(const string& name, Node* parent)
    : Node(name, parent)
{
    head = nullptr;
}

Folder::~Folder()
{
    ChildNode* temp = head;
    while (temp != nullptr)
    {
        ChildNode* nextNode = temp->next;
        delete temp->data;
        delete temp;
        temp = nextNode;
    }
}

void Folder::addNode(Node* node)
{
    ChildNode* temp = head;
    while (temp != nullptr)
    {
        if (temp->data->getName() == node->getName())
        {
            cout << "Error: Name already exists" << endl;
            return;
        }
        temp = temp->next;
    }

    ChildNode* newNode = new ChildNode;
    newNode->data = node;
    newNode->next = nullptr;

    if (head == nullptr)
    {
        head = newNode;
        return;
    }

    temp = head;
    while (temp->next != nullptr)
        temp = temp->next;
    temp->next = newNode;
}

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

void Folder::removeNode(const string& name)
{
    ChildNode* temp = head;
    ChildNode* prev = nullptr;

    while (temp != nullptr)
    {
        if (temp->data->getName() == name)
        {
            if (prev == nullptr)
                head = temp->next;
            else
                prev->next = temp->next;

            delete temp->data;
            delete temp;
            cout << "Deleted successfully\n";
            return;
        }
        prev = temp;
        temp = temp->next;
    }
    cout << "Node not found" << endl;
}

void Folder::list() const {
    ChildNode* temp = head;
    while (temp != nullptr) {
        if (temp->data->isFolder()) {
            setColor(FOREGROUND_BLUE | FOREGROUND_INTENSITY);
            temp->data->display();
        }
        else {
            setColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
            temp->data->display();
        }
        setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        cout << endl;
        temp = temp->next;
    }
}

void Folder::open()
{
    cout << "Opened Folder: " << name << endl;
}

void Folder::display() const
{
    cout << "Folder: " << name;
}

bool Folder::isFolder() const
{
    return true;
}

bool Folder::searchIn(const string& targetName, const string& path) {
    bool foundAny = false;
    ChildNode* temp = head;
    while (temp != nullptr) {
        string fullPath = path + "/" + temp->data->getName();
        if (temp->data->getName() == targetName) {
            cout << "Found at: " << fullPath << endl;
            foundAny = true;
        }
        if (temp->data->isFolder()) {
            Folder* sub = (Folder*)temp->data;
            if (sub->searchIn(targetName, fullPath))
                foundAny = true;
        }
        temp = temp->next;
    }
    return foundAny;
}
