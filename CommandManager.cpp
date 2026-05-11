#include "CommandManager.h"
#include "AudioFile.h"
#include "ZipFile.h"
#include "Textfile.h"
#include "PrivateFile.h"
#include <iostream>
#include <windows.h>
using namespace std;

static void setColor(int color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

void CommandManager::help() {
    cout << endl << "Following are the available commands:" << endl;
    cout << endl;
    cout << endl;
    cout << "  ls     (to List contents of current folder)" << endl;
    cout << "  mkdir <name>      (Create a new folder)" << endl;
    cout << "  cd <name>      ( Change to folder (use '..' for parent))" << endl;
    cout << "  rm <name>         (Remove file/folder)" << endl;
    cout << "  rename <old> <new>   (Rename a node)" << endl;
    cout << "  search <name>     (Find node recursively)" << endl;
    cout << "  touch <type> <name>   (Create file (txt, private, audio, zip))" << endl;
    cout << "  open <name>      (Open file/folder)" << endl;
    cout << "  unzip <name>    ( Unzip a .zip file)" << endl;
    cout << "  exit        ( Quit the program)" << endl;
    cout << endl;
    cout << endl;
}

CommandManager::CommandManager() {
    root = new Folder("root", nullptr);
    current = root;
}

CommandManager::~CommandManager() {
    delete root;
}

void CommandManager::ls() {
    cout << endl << "[" << current->getName() << "]" << endl;
    current->list();
}

void CommandManager::mkdir(const string& name) {
    if (current->find(name) != nullptr) {
        setColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
        cout << "Error: " << name << " already exists." << endl;
        setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        return;
    }
    current->addNode(new Folder(name, current));
    setColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    cout << "Folder '" << name << "' created." << endl;
    setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}

void CommandManager::cd(const string& name) {
    if (name == "..") {
        if (current->getParent() == nullptr) {
            setColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
            cout << "Already at root." << endl;
            setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        }
        else {
            current = (Folder*)current->getParent();
        }
        return;
    }
    Node* found = current->find(name);
    if (found == nullptr) {
        setColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
        cout << "'" << name << "' not found." << endl;
        setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    }
    else if (!found->isFolder()) {
        setColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
        cout << "'" << name << "' is a file, not a folder." << endl;
        setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    }
    else {
        current = (Folder*)found;
    }
}

void CommandManager::rm(const string& name) {
    Node* found = current->find(name);
    if (found == root) {
        setColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
        cout << "Cannot delete root." << endl;
        setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        return;
    }
    current->removeNode(name);
}

void CommandManager::rename(const string& oldName, const string& newName) {
    Node* found = current->find(oldName);
    if (found == nullptr) {
        setColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
        cout << oldName << " not found." << endl;
        setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        return;
    }
    if (found == root) {
        setColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
        cout << "Cannot rename root folder." << endl;
        setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        return;
    }
    if (oldName == newName) {
        setColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
        cout << "Same name. No changes made." << endl;
        setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        return;
    }
    if (current->find(newName) != nullptr) {
        setColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
        cout << "Name '" << newName << "' already exists." << endl;
        setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        return;
    }
    found->setName(newName);
    setColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    cout << "Renamed " << oldName << " to " << newName << endl;
    setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}

void CommandManager::search(const string& name) {
    cout << "Searching for '" << name << "'..." << endl;
    bool found = current->searchIn(name, current->getPath());
    if (!found) {
        setColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
        cout << "'" << name << "' not found." << endl;
        setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    }
}

void CommandManager::touch(const string& type, const string& name) {
    if (type == "txt") {
        if (current->find(name + ".txt") != nullptr) {
            setColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
            cout << "Error: " << name << ".txt already exists." << endl;
            setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
            return;
        }
        current->addNode(new TxtFile(name, current));
        setColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
        cout << "Text file '" << name << ".txt' created." << endl;
        setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    }
    else if (type == "private") {
        if (current->find(name + ".priv") != nullptr) {
            setColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
            cout << "Error: " << name << ".priv already exists." << endl;
            setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
            return;
        }
        string passkey;
        cout << "Set a passkey for this file: ";
        cin >> passkey;
        current->addNode(new PrivateFile(name, current, passkey));
        setColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
        cout << "Private file '" << name << ".priv' created." << endl;
        setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    }
    else if (type == "audio") {
        if (current->find(name + ".mpg") != nullptr) {
            setColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
            cout << "Error: " << name << ".mpg already exists." << endl;
            setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
            return;
        }
        current->addNode(new AudioFile(name, current));
        setColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
        cout << "Audio file '" << name << ".mpg' created." << endl;
        setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    }
    else if (type == "zip") {
        Node* toZip = current->find(name);
        if (toZip == nullptr) {
            setColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
            cout << "Error: " << name << " not found. Can only zip existing nodes." << endl;
            setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
            return;
        }
        string zipName = name + "-zip.zip";
        if (current->find(zipName) != nullptr) {
            setColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
            cout << "Error: " << zipName << " already exists." << endl;
            setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
            return;
        }
        string origType = toZip->isFolder() ? "folder" : "file";
        current->addNode(new ZipFile(name, current, name, origType));
        setColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
        cout << "Zip file '" << zipName << "' created." << endl;
        setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    }
    else {
        setColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
        cout << "Unknown type: '" << type << "'" << endl;
        cout << "Valid types: txt, private, audio, zip" << endl;
        setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    }
}

void CommandManager::unzip(const string& zipName) {
    Node* found = current->find(zipName);
    if (found == nullptr) {
        setColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
        cout << zipName << " not found." << endl;
        setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        return;
    }
    if (found->isFolder()) {
        setColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
        cout << zipName << " is a folder, not a zip file." << endl;
        setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        return;
    }
    ZipFile* zf = dynamic_cast<ZipFile*>(found);
    if (zf == nullptr) {
        setColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
        cout << zipName << " is not a zip file." << endl;
        setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        return;
    }
    string unzippedName = zf->getOriginalName() + "-unzipped";
    if (zf->getOriginalType() == "folder")
        current->addNode(new Folder(unzippedName, current));
    else
        current->addNode(new File(unzippedName, current));
    setColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    cout << "Unzipped as: " << unzippedName << endl;
    setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}

void CommandManager::run() {
    string cmd;
    cout << endl;
    setColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    cout << "Welcome to File Management System!!" << endl;
    cout << endl;
    cout << "    by 24L0987 & 25L2539    " << endl;

    setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    help();
    while (true) {
        setColor(FOREGROUND_RED | FOREGROUND_GREEN);
        cout << endl << "[" << current->getPath() << "] > ";
        setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        cin >> cmd;
        if (cmd == "exit") break;
        else if (cmd == "help") help();
        else if (cmd == "ls") ls();
        else if (cmd == "mkdir") { string n; cin >> n; mkdir(n); }
        else if (cmd == "cd") { string n; cin >> n; cd(n); }
        else if (cmd == "rm") { string n; cin >> n; rm(n); }
        else if (cmd == "rename") { string o, n; cin >> o >> n; rename(o, n); }
        else if (cmd == "search") { string n; cin >> n; search(n); }
        else if (cmd == "touch") { string t, n; cin >> t >> n; touch(t, n); }
        else if (cmd == "open") {
            string n; cin >> n;
            Node* f = current->find(n);
            if (!f) {
                setColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
                cout << n << " not found." << endl;
                setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
            }
            else {
                f->open();
            }
        }
        else if (cmd == "unzip") { string n; cin >> n; unzip(n); }
        else {
            setColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
            cout << "Unknown command." << endl;
            setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        }
    }
}
