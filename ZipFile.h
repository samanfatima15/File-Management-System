
#pragma once
#ifndef ZIPFILE_H
#define ZIPFILE_H

#include "File.h"
#include <string>
using namespace std;

class ZipFile : public File {
private:
    string originalName;    
    string originalType;     
public:
    ZipFile(const string& name, Node* parent, const string& origName, const string& origType);
    ~ZipFile();

    void open() override;
    void display() const override;

   
    string getOriginalName() const;
    string getOriginalType() const;
};

#endif