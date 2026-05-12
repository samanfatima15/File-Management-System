#include "Folder.h"
#include "PrivateFile.h"
#include "Config.h"
#include <filesystem>
#include <iostream>
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#undef byte
#include <windows.h>
using namespace std;

namespace fs = std::filesystem;

static void setColor(int color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

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
    ChildNode* curr = head;
    while (curr) {
        if (curr->data->getName() == node->getName()) {
            setColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
            cout << "Error: '" << node->getName() << "' already exists\n";
            setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
            delete node;
            return;
        }
        curr = curr->next;
    }

    if (node->isFolder()) {
        string realPath = VFS_ROOT + node->getPath();
        fs::create_directories(realPath);
    }

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
        if (curr->data->getName() == name) {
            // Check if it's a private file and verify password
            File* filePtr = dynamic_cast<File*>(curr->data);
            if (filePtr && filePtr->isPrivate()) {
                PrivateFile* privFile = dynamic_cast<PrivateFile*>(filePtr);
                if (privFile) {
                    setColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
                    cout << "This is a private file.\n";
                    setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
                    string password;
                    cout << "Enter password to delete: ";
                    getline(cin, password);
                    if (!privFile->verifyPassword(password)) {
                        setColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
                        cout << "Wrong password. Access denied.\n";
                        setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
                        return;
                    }
                    setColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
                    cout << "Password correct. Access granted.\n";
                    setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
                }
            }

            // Delete real file/directory from disk
            string realPath = VFS_ROOT + curr->data->getPath();
            if (curr->data->isFolder())
                fs::remove_all(realPath);
            else {
                fs::remove(realPath);
                string keyPath = realPath + ".key";
                if (fs::exists(keyPath))
                    fs::remove(keyPath);
            }

            // Remove from linked list
            if (prev == nullptr)
                head = curr->next;
            else
                prev->next = curr->next;

            delete curr->data;
            delete curr;
            setColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
            cout << "Deleted '" << name << "' successfully.\n";
            setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
            return;
        }
        prev = curr;
        curr = curr->next;
    }
    setColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
    cout << "'" << name << "' not found\n";
    setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}

void Folder::list() const {
    ChildNode* curr = head;
    if (!curr) {
        cout << "  (empty)\n";
        return;
    }
    while (curr) {
        if (curr->data->isFolder()) {
            setColor(FOREGROUND_BLUE | FOREGROUND_INTENSITY);
        }
        else {
            setColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
        }
        // This will print "[TXT]", "[PRIVATE]", "[AUDIO]", "[FILE]", etc.
        curr->data->display();
        setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        cout << endl;
        curr = curr->next;
    }
}

void Folder::open() {}

void Folder::display() const {
    cout << "[FOLDER] " << name;
}

bool Folder::isFolder() const {
    return true;
}

bool Folder::searchNode(const string& target, string path) {
    if (name == target) {
        cout << "Found folder: " << path << "/" << name << endl;
        return true;
    }
    ChildNode* curr = head;
    while (curr) {
        if (curr->data->getName() == target) {
            string type = curr->data->isFolder() ? "folder" : "file";
            cout << "Found " << type << ": " << path << "/" << name << "/" << target << endl;
            return true;
        }
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
