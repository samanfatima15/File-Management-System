
#include "CommandManageer.h"
#include "AudioFile.h"
#include "ZipFile.h"
#include "Textfile.h"
#include "PrivateFile.h"
#include "Config.h"
#include <iostream>
#include <sstream>
#include <filesystem>
#include <cctype>
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#undef byte
#include <windows.h>
using namespace std;

namespace fs = std::filesystem;

static void setColor(int color) 
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

CommandManager::CommandManager() 
{
    fs::create_directories(VFS_ROOT);
    root = new Folder("root", nullptr);
    current = root;
}

CommandManager::~CommandManager() {
    delete root;
}

void CommandManager::showHelp() {
    setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE); // white
    cout << " These are the available commands:    " << endl;
    cout << endl;
    cout << "  ls             (to list files/folders)" << endl;
    cout << "  mkdir <name>      (create a folder)" << endl;
    cout << "  cd <name>             (change folder) " << endl;
    cout << "  rm <name>        ( delete file/folder)" << endl;
    cout << "  rename <old> <new>   ( rename file/folder)" << endl;
    cout << "  search <name>        ( find file/folder everywhere)" << endl;
    cout << "  touch <type> <name>    (create file) " << endl;
    cout << "  open <name>        (open file or folder)" << endl;
    cout << "  unzip <name>       (extract zip file)" << endl;
    cout << "  help               (show this menu)" << endl;
    cout << "  exit  (to quit)" << endl;
    cout << "                " << endl;
}

void CommandManager::listContents() {
    cout << "\n[" << current->getName() << "]" << endl;
    current->list();
}

void CommandManager::makeFolder(const string& name) 
{
    if (name.empty()) 
    {
        setColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
        cout << "Usage: mkdir <folder_name>" << endl;
        setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        return;
    }
    if (current->find(name)) {
        setColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
        cout << "Error: '" << name << "' already exists" << endl;
        setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        return;
    }
    current->addNode(new Folder(name, current));
    setColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    cout << "Folder '" << name << "' created" << endl;
    setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}

void CommandManager::changeFolder(const string& name) {
    if (name.empty()) {
        setColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
        cout << "Usage: cd <folder_name> or cd .." << endl;
        setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        return;
    }

    if (name == "..") {
        if (current->getParent())
            current = (Folder*)current->getParent();
        else {
            setColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
            cout << "Already at root" << endl;
            setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        }
        return;
    }

    Node* found = current->find(name);
    if (!found) {
        setColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
        cout << "Error: '" << name << "' not found" << endl;
        setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    }
    else if (!found->isFolder()) {
        setColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
        cout << "there is a error because  '" << name << "' is a file, not a folder. hey user pls use 'open' to view files" << endl;
        setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    }
    else {
        current = (Folder*)found;
    }
}

void CommandManager::deleteNode(const string& name) {
    if (name.empty()) {
        setColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
        cout << "Usage: rm <file_or_folder_name>" << endl;
        cout << "Note: Use full filename with extension (e.g., rm mynotes.txt)" << endl;
        setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        return;
    }
    if (name == "root") {
        setColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
        cout << "you cannot delete a root folder" << endl;
        setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        return;
    }
    current->removeNode(name);
}

void CommandManager::renameNode(const string& oldName, const string& newName) {
    if (oldName.empty() || newName.empty()) {
        setColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
        cout << "Usage: rename <old_name> <new_name>" << endl;
        setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        return;
    }

    Node* target = current->find(oldName);
    if (!target) {
        setColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
        cout << "we have a error" << oldName << "is not found" << endl;
        setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        return;
    }

    if (oldName == newName) {
        setColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
        cout << "Name is the same" << endl;
        setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        return;
    }

    if (current->find(newName)) {
        setColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
        cout << "Error: '" << newName << "' already exists" << endl;
        setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        return;
    }

    string oldPath = VFS_ROOT + target->getPath();
    string newPath = VFS_ROOT + target->getParent()->getPath() + "/" + newName;

    try {
        fs::rename(oldPath, newPath);
        target->setName(newName);
        setColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
        cout << "it is renamed '" << oldName << "' to '" << newName << "'" << endl;
        setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    }
    catch (const fs::filesystem_error& e) {
        setColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
        cout << "Error in the renaimming " << e.what() << endl;
        setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    }
}

void CommandManager::searchNode(const string& name) {
    if (name.empty()) {
        setColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
        cout << "usage: search <name>" << endl;
        setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        return;
    }
    cout << "we are seachering " << name << "" << endl;
    if (!root->searchNode(name, "")) {
        setColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
        cout << "Not found." << endl;
        setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    }
}

void CommandManager::makeFile(const string& type, const string& name) {
    if (type.empty() || name.empty()) {
        setColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
        cout << "Usage: touch <type> <name>" << endl;
        cout << "Types: txt, private, audio, zip" << endl;
        setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        return;
    }

    if (type == "txt") {
        string fullName = name + ".txt";
        if (current->find(fullName)) {
            setColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
            cout << "there is an error '" << fullName << "beacuse it already exists" << endl;
            setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
            return;
        }
        current->addNode(new TxtFile(name, current));
        setColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
        cout << "Text file '" << fullName << "is  created" << endl;
        cout << "to edit the file==open  " << fullName << endl;
        setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    }
    else if (type == "private") {
        string fullName = name + ".priv";
        if (current->find(fullName)) {
            setColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
            cout << "there is aan error as  '" << fullName << "' already exists" << endl;
            setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
            return;
        }
        string pass;
        cout << "enter the pasword for the private file: ";
        cin >> pass;
        cin.ignore();
        current->addNode(new PrivateFile(name, current, pass));
        setColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
        cout << "Private file '" << fullName << "' created" << endl;
        cout << "for editing= open " << fullName << endl;
        setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    }
    else if (type == "audio") {
        string fullName = name + ".mpg";
        if (current->find(fullName)) {
            setColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
            cout << "there is an error'" << fullName << "as it already exists" << endl;
            setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
            return;
        }
        current->addNode(new AudioFile(name, current));
        setColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
        cout << "the audio file   '" << fullName << "is  created now " << endl;
        cout << "for the oplay of the audio file :open " << fullName << endl;
        setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    }
    else if (type == "zip") {
        Node* toZip = current->find(name);
        if (!toZip) {
            setColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
            cout << "there is an error '" << name << "is not found can only zip the file and the folders" << endl;
            setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
            return;
        }
        string zipName = name + "-zip.zip";
        if (current->find(zipName)) {
            setColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
            cout << "there is an error'" << zipName << "as this exxists" << endl;
            setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
            return;
        }
        string origType = toZip->isFolder() ? "folder" : "file";
        current->addNode(new ZipFile(name, current, name, origType));
        setColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
        cout << "the zip file '" << zipName << "is created now" << endl;
        setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    }
    else {
        setColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
        cout << "Error: Unknown type '" << type << "'" << endl;
        cout << "the valid types are here: txt, private, audio, zip" << endl;
        setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    }
}

  
void CommandManager::unzipFile(const string& name) {
    if (name.empty()) {
        setColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
        cout << "Usage: unzip <zip_filename>" << endl;
        setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        return;
    }

    Node* found = current->find(name);
    if (!found) {
        setColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
        cout << "there is a error'" << name << "' not found" << endl;
        setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        return;
    }

    if (found->isFolder()) {
        setColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
        cout << "we have a error '" << name << "' is a folder, not a zip file" << endl;
        setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        return;
    }

    ZipFile* zip = dynamic_cast<ZipFile*>(found);
    if (!zip) {
        setColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
        cout << "Error: '" << name << "' is not a zip file" << endl;
        setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        return;
    }

    // Restore the real file/folder on disk (content is written)
    zip->unzipToDisk(current);

    // Original full name (e.g., "report.txt") and unzipped name (e.g., "report.txt-unzipped")
    string origFullName = zip->getOriginalName();
    string unzippedName = origFullName + "-unzipped";

    if (current->find(unzippedName)) {
        setColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
        cout << "there is and error  '" << unzippedName << "' it already exists in memory" << endl;
        setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        return;
    }

    // Create appropriate node type
    if (zip->getOriginalType() == "folder") {
        current->addNode(new Folder(unzippedName, current));
        setColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
        cout << "Unzipped folder: " << unzippedName << endl;
    }
    else {
        // Check file extension
        size_t dot = origFullName.find_last_of('.');
        string ext = (dot != string::npos) ? origFullName.substr(dot) : "";

        if (ext == ".txt") {
            // Use the new TxtFile constructor that takes the full name (with "-unzipped")
            // IMPORTANT: This constructor does NOT append ".txt"
            current->addNode(new TxtFile(unzippedName, current, true));
            setColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
            cout << "Unzipped text file. Use 'open " << unzippedName << "' to edit." << endl;
        }
        else if (ext == ".priv") {
            current->addNode(new File(unzippedName, current));
            cout << "Private file is restored and password is not restored you can open it with the  external editor"<<endl;
        }
        else if (ext == ".mpg") {
            string baseName = unzippedName.substr(0, unzippedName.find_last_of('.'));
            current->addNode(new AudioFile(baseName, current));
        }
        else {
            current->addNode(new File(unzippedName, current));
        }
    }

    setColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    cout << "Unzipped as: " << unzippedName << endl;
    setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}

void CommandManager::openNode(const string& name) {
    if (name.empty()) {
        setColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
        cout << "Usage: open <file_or_folder_name>" << endl;
        cout << "for the files use the full names with the extension" << endl;
        setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        return;
    }

    Node* found = current->find(name);
    if (!found) {
        setColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
        cout << "there is and error " << name << "as this is not founcd" << endl;
        cout << " you ca use the ls to the see the file and folers" << endl;
        setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        return;
    }
    found->open();
}

void CommandManager::run() {
    setColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    cout << "    Welcome to FILE MANAGEMENT SYSTEM !      " << endl;
    cout << "      " << endl;
    setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    showHelp();

    string input;
    while (true) {
        setColor(FOREGROUND_RED | FOREGROUND_GREEN);
        cout << "\n[" << current->getPath() << "]";
        setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        getline(cin, input);

        size_t start = input.find_first_not_of(" \t");
        if (start == string::npos) continue;
        input = input.substr(start);

        stringstream ss(input);
        string cmd;
        ss >> cmd;

        if (cmd == "exit" || cmd == "quit") {
            setColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
            cout << "Goodbye dear user Files are  saved in the  " << VFS_ROOT << endl;
            setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
            break;
        }
        else if (cmd == "help" || cmd == "?") {
            showHelp();
        }
        else if (cmd == "ls") {
            listContents();
        }
        else if (cmd == "mkdir") {
            string n;
            ss >> n;
            makeFolder(n);
        }
        else if (cmd == "cd") {
            string n;
            ss >> n;
            changeFolder(n);
        }
        else if (cmd == "rm") {
            string n;
            ss >> n;
            deleteNode(n);
        }
        else if (cmd == "rename") {
            string oldN, newN;
            ss >> oldN >> newN;
            renameNode(oldN, newN);
        }
        else if (cmd == "search") {
            string n;
            ss >> n;
            searchNode(n);
        }
        else if (cmd == "touch") {
            string t, n;
            ss >> t >> n;
            makeFile(t, n);
        }
        else if (cmd == "open") {
            string n;
            ss >> n;
            openNode(n);
        }
        else if (cmd == "unzip") {
            string n;
            ss >> n;
            unzipFile(n);
        }
        else if (!cmd.empty()) {
            setColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
            cout << "this is an unknown command'" << cmd << "'" << endl;
            cout << "pls try the help for the command as you are entering the wrog commands we will give you a menu" << endl;
            setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        }
    }
}
