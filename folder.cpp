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
            cout << "Error: '" << node->getName() << "' already exists\n";
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
        if (curr->data->getName() == name) {

            // ===== PRIVATE FILE PASSWORD CHECK =====
            File* filePtr = dynamic_cast<File*>(curr->data);
            if (filePtr && filePtr->isPrivate()) {
                PrivateFile* privFile = dynamic_cast<PrivateFile*>(filePtr);
                if (privFile) {
                    cout << "\n[SECURITY] This is a protected private file.\n";
                    string password;
                    cout << "Enter passkey to delete: ";
                    getline(cin, password);

                    if (!privFile->verifyPassword(password)) {
                        cout << "[DENIED] Wrong passkey. Deletion canceled!\n";
                        return;
                    }
                    cout << "[GRANTED] Password correct. Deleting...\n";
                }
            }
            // ===== END OF PASSWORD CHECK =====

            // Delete real file/directory from disk
            string realPath = VFS_ROOT + curr->data->getPath();
            if (curr->data->isFolder()) {
                fs::remove_all(realPath);
            }
            else {
                fs::remove(realPath);
                // Also delete associated .key file for private files
                string keyPath = realPath + ".key";
                if (fs::exists(keyPath)) {
                    fs::remove(keyPath);
                }
            }

            // Remove from linked list
            if (prev == nullptr) {
                head = curr->next;
            }
            else {
                prev->next = curr->next;
            }

            delete curr->data;
            delete curr;
            cout << "Deleted '" << name << "' successfully.\n";
            return;
        }
        prev = curr;
        curr = curr->next;
    }
    cout << "'" << name << "' not found\n";
}

void Folder::list() const {
    ChildNode* curr = head;
    if (!curr) {
        cout << "  (empty)\n";
        return;
    }

    while (curr) {
        if (curr->data->isFolder())
            cout << "  [FOLDER] ";
        else
            cout << "  [FILE]   ";
        cout << curr->data->getName() << endl;
        curr = curr->next;
    }
}

void Folder::open() {
    // Do nothing - folder navigation happens in CommandManager
}

void Folder::display() const {
    cout << "[FOLDER] " << name;
}

bool Folder::isFolder() const {
    return true;
}

bool Folder::searchNode(const string& target, string path) {
    // Check current folder
    if (name == target) {
        cout << "Found folder: " << path << "/" << name << endl;
        return true;
    }

    // Search through children
    ChildNode* curr = head;
    while (curr) {
        if (curr->data->getName() == target) {
            string type = curr->data->isFolder() ? "folder" : "file";
            cout << "Found " << type << ": " << path << "/" << name << "/" << target << endl;
            return true;
        }

        // If child is a folder, search inside it recursively
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
