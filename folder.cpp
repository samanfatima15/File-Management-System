#include "Folder.h"
#include "PrivateFile.h"
#include "Config.h"
#include <filesystem>
#include <iostream>
using namespace std;

namespace fs = std::filesystem;

Folder::Folder(const string& name, Node* parent)
    : Node(name, parent), head(nullptr) {
}

Folder::~Folder() {
    ChildNode* curr = head;
    while (curr) {
        ChildNode* next = curr->next;
        delete curr->data;
        delete curr;
        curr = next;
    }
}

void Folder::addNode(Node* node) {
    // Check for duplicate names
    ChildNode* curr = head;
    while (curr) {
        if (curr->data->getName() == node->getName()) {
            cout << "i have error '" << node->getName() << "as this allready exists"<<endl;
            delete node;
            return;
        }
        curr = curr->next;
    }

    // If adding a folder, create real directory on disk
    if (node->isFolder()) {
        string realPath = VFS_ROOT + node->getPath();
        fs::create_directories(realPath);
    }

    // Create new linked list node (add at beginning)
    ChildNode* newNode = new ChildNode;
    newNode->data = node;
    newNode->next = head;
    head = newNode;
}

Node* Folder::find(const string& name) {
    ChildNode* curr = head;
    while (curr) {
        if (curr->data->getName() == name)
            return curr->data;
        curr = curr->next;
    }
    return nullptr;
}

void Folder::removeNode(const string& name) {
    ChildNode* curr = head;
    ChildNode* prev = nullptr;

    while (curr) {
        if (curr->data->getName() == name) 
        {
            File* filePtr = dynamic_cast<File*>(curr->data);
            if (filePtr && filePtr->isPrivate()) {
                PrivateFile* privFile = dynamic_cast<PrivateFile*>(filePtr);
                if (privFile) {
                    cout << "as this is a private file"<<endl;
                    string password;
                    cout << "so you have to enter the password for delete ";
                    getline(cin, password);

                    if (!privFile->verifyPassword(password)) {
                        cout << "as you have entered the wrong password we cannot give you access"<<endl;
                        return;
                    }
                    cout << "as the password is correct the access is granted to you"<<endl;
                }
            }
            string realPath = VFS_ROOT + curr->data->getPath();
            if (curr->data->isFolder()) {
                fs::remove_all(realPath);
            }
            else {
                fs::remove(realPath);
                string keyPath = realPath + ".key";
                if (fs::exists(keyPath)) {
                    fs::remove(keyPath);
                }
            }

            // Removing from the linked lists
            if (prev == nullptr) {
                head = curr->next;
            }
            else {
                prev->next = curr->next;
            }

            delete curr->data;
            delete curr;
            cout << "it is deleted" << name << "' successfully.\n";
            return;
        }
        prev = curr;
        curr = curr->next;
    }
    cout << "'" << name << "' not found"<<endl;
}

void Folder::list() const {
    ChildNode* curr = head;
    if (!curr) {
        cout << "empty" <<endl;
        return;
    }

    while (curr) {
        if (curr->data->isFolder())
            cout << "  it is folder ";
        else
            cout << "  it is file  ";
        cout << curr->data->getName() << endl;
        curr = curr->next;
    }
}

void Folder::open() 
{
    
}

void Folder::display() const
{
    cout << "folder " << name;
}

bool Folder::isFolder() const
{
    return true;
}

bool Folder::searchNode(const string& target, string path) 
{
    // Checking the current folder
    if (name == target) {
        cout << "the folder is found " << path << "/" << name << endl;
        return true;
    }// Searching through children
    ChildNode* curr = head;
    while (curr) {
        if (curr->data->getName() == target) {
            string type = curr->data->isFolder() ? "folder" : "file";
            cout << "Found " << type << ": " << path << "/" << name << "/" << target << endl;
            return true;
        }

        // If child is a folder,then we have choose reccursion to search as it searches by calling the function
        if (curr->data->isFolder()) {
            Folder* subFolder = (Folder*)curr->data;
            string newPath = path + "/" + name;
            if (subFolder->searchNode(target, newPath))
                return true;
        }
        curr = curr->next;
    }
    return false;
}
