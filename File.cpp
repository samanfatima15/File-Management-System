#include "File.h"
#include "Config.h"
#include <iostream>
#include <fstream>
using namespace std;

File::File(const string& name, Node* parent)
    : Node(name, parent)
{
    string fullPath = getDiskPath();
    ofstream ofs(fullPath);
    ofs.close();
}

File::File(const string& name, Node* parent, bool skipDiskCreate)
    : Node(name, parent)
{
    if (!skipDiskCreate) {
        string fullPath = getDiskPath();
        ofstream ofs(fullPath);
        ofs.close();
    }
}

File::~File() {}

bool File::isFolder() const {
    return false;
}

void File::display() const {
    cout << "file:" << name;
}

void File::open() {
    cout << "it is opening" << name << endl;
}

string File::getDiskPath() const {
    return VFS_ROOT + getPath();
}
