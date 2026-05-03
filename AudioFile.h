#pragma once
#ifndef AUDIOFILE_H
#define AUDIOFILE_H

#include "File.h"
#include <string>
using namespace std;

class AudioFile : public File {
public:
    AudioFile(const string& name, Node* parent);
    ~AudioFile();

    void open() override;
    void display() const override;
};

#endif