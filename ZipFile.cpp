#include "ZipFile.h"
#include "Config.h"
#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>
using namespace std;

namespace fs = std::filesystem;

ZipFile::ZipFile(const string& name, Node* parent, const string& origName, const string& origType)
    : File(name + "-zip.zip", parent), originalName(origName), originalType(origType)
{
    string sourcePath = VFS_ROOT + parent->getPath() + "/" + origName;
    string zipPath = VFS_ROOT + getPath();

    ofstream zipFile(zipPath);
    if (!zipFile.is_open()) {
        cout << "Error: Could not create zip file\n";
        return;
    }

    zipFile << "MYZIP_ARCHIVE\n";
    zipFile << "ORIGINAL_NAME:" << origName << "\n";
    zipFile << "TYPE:" << origType << "\n";

    if (origType == "file") {
        ifstream inFile(sourcePath);
        if (inFile.is_open()) {
            zipFile << "CONTENT_BEGIN\n";
            string line;
            while (getline(inFile, line))
                zipFile << line << "\n";
            zipFile << "CONTENT_END\n";
            inFile.close();
        }
        else {
            cout << "Warning: Source file not found\n";
        }
    }
    else if (origType == "folder") {
        zipFile << "FOLDER_CONTENT_BEGIN\n";
        for (const auto& entry : fs::recursive_directory_iterator(sourcePath)) {
            if (fs::is_regular_file(entry)) {
                string relativePath = fs::relative(entry.path(), sourcePath).string();
                zipFile << "FILE:" << relativePath << "\n";
                ifstream inFile(entry.path());
                if (inFile.is_open()) {
                    zipFile << "CONTENT_BEGIN\n";
                    string line;
                    while (getline(inFile, line))
                        zipFile << line << "\n";
                    zipFile << "CONTENT_END\n";
                    inFile.close();
                }
            }
        }
        zipFile << "FOLDER_CONTENT_END\n";
    }

    zipFile.close();
    cout << "Zipped successfully: " << this->getName() << endl;
}

ZipFile::~ZipFile() {}

void ZipFile::open() {
    cout << "Cannot open zip file directly. Use 'unzip' command.\n";
}

void ZipFile::display() const {
    cout << "[ZIP] " << name << " (contains: " << originalName << ")";
}

string ZipFile::getOriginalName() const {
    return originalName;
}

string ZipFile::getOriginalType() const {
    return originalType;
}

void ZipFile::unzipToDisk(Folder* currentFolder) {
    string zipPath = VFS_ROOT + getPath();
    ifstream zipFile(zipPath);
    if (!zipFile.is_open()) {
        cout << "Error: Cannot open zip file\n";
        return;
    }

    string line;
    getline(zipFile, line); // MYZIP_ARCHIVE
    getline(zipFile, line); // ORIGINAL_NAME:...
    getline(zipFile, line); // TYPE:...

    string targetPath = VFS_ROOT + currentFolder->getPath() + "/" + originalName + "-unzipped";

    if (originalType == "file") {
        while (getline(zipFile, line) && line != "CONTENT_BEGIN") {}
        ofstream outFile(targetPath);
        while (getline(zipFile, line) && line != "CONTENT_END")
            outFile << line << "\n";
        outFile.close();
        cout << "Unzipped file: " << originalName << "-unzipped\n";
    }
    else if (originalType == "folder") {
        fs::create_directories(targetPath);
        ofstream* outFile = nullptr;
        bool insideFile = false;

        while (getline(zipFile, line)) {
            if (line == "FOLDER_CONTENT_END") break;
            if (line.find("FILE:") == 0) {
                if (outFile) {
                    outFile->close();
                    delete outFile;
                    outFile = nullptr;
                }
                string relativePath = line.substr(5);
                string fullPath = targetPath + "/" + relativePath;
                fs::create_directories(fs::path(fullPath).parent_path());
                outFile = new ofstream(fullPath);
                insideFile = true;
            }
            else if (line == "CONTENT_BEGIN") {
            }
            else if (line == "CONTENT_END") {
                if (outFile) {
                    outFile->close();
                    delete outFile;
                    outFile = nullptr;
                }
                insideFile = false;
            }
            else if (insideFile && outFile) {
                *outFile << line << "\n";
            }
        }
        if (outFile) delete outFile;
        cout << "Unzipped folder: " << originalName << "-unzipped\n";
    }
    zipFile.close();
}
