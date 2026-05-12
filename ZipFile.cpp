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
        cout << "there is an error as we could not create a zip file\n";
        return;
    }

    zipFile << "MYZIP_ARCHIVE\n";
    zipFile << "originalname" << origName << "\n";
    zipFile << "type" << origType << "\n";

    if (origType == "file") {
        ifstream inFile(sourcePath);
        if (inFile.is_open()) {
            zipFile << "the content here is begin"<<endl;
            string line;
            while (getline(inFile, line))
                zipFile << line << "\n";
            zipFile << "CONTENT_END\n";
            inFile.close();
        }
        else {
            cout << "we have a error as the source file is not found"<<endl;
        }
    }
    else if (origType == "folder") {
        zipFile << "foldercontent has begin"<<endl;
        for (const auto& entry : fs::recursive_directory_iterator(sourcePath)) {
            if (fs::is_regular_file(entry)) {
                string relativePath = fs::relative(entry.path(), sourcePath).string();
                zipFile << "file:" << relativePath << "\n";
                ifstream inFile(entry.path());
                if (inFile.is_open()) {
                    zipFile << "content has begin"<<endl;
                    string line;
                    while (getline(inFile, line))
                        zipFile << line << "\n";
                    zipFile << "the content has been ended"<<endl;
                    inFile.close();
                } 
            }
        }
        zipFile << "folder content has endded"<<endl;
    }

    zipFile.close();
    cout << "it is zipped succesfuly: " << this->getName() << endl;
}

ZipFile::~ZipFile() {}

void ZipFile::open() {
    cout << "we cannot open the zip file for this use the unzip command"<<endl;
}

void ZipFile::display() const {
    cout << "ZIP " << name << " (contains: " << originalName << ")";
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
        cout << "there is error we cannot open the zip file"<<endl;
        return;
    }

    string line;
    getline(zipFile, line); 
    getline(zipFile, line); 
    getline(zipFile, line); 

    string targetPath = VFS_ROOT + currentFolder->getPath() + "/" + originalName + "-unzipped";

    if (originalType == "file") {
        while (getline(zipFile, line) && line != "content has begin") {}
        ofstream outFile(targetPath);
        while (getline(zipFile, line) && line != "content is ended ")
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
