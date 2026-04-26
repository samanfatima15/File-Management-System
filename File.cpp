#include "File.h"

// Constructor
File::File(const string& name, Node* parent)
    : Node(name, parent)
{
}

// Destructor
File::~File()
{
    // nothing inside file, so nothing to delete
}

// Display file
void File::display() const
{
    cout << "File: " << name << endl;
}
void File::open()
{
    cout << "Opening File: " << name << endl;
}


// Type check
bool File::isFolder() const
{
    return false;
}