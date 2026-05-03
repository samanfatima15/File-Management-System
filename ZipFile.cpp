#include "ZipFile.h"
#include <iostream>
using namespace std;


ZipFile::ZipFile(const string& name, Node* parent, const string& origName, const string& origType) : File(name + "-zip.zip", parent)
{
    originalName = origName;
    originalType = origType;
    cout << "Zipped successfully, created: " << this->getName() << endl;
}

ZipFile::~ZipFile() {
void ZipFile::open() {
    cout << "Zipped files cannot be opened. thyve to be Unzip first." << endl;
}

void ZipFile::display() const {
    cout << "Zip " << name << " contains: " << originalName << endl;
}

string ZipFile::getOriginalName() const {
    return originalName;
}

string ZipFile::getOriginalType() const {
    return originalType;
}