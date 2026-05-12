#include "AudioFile.h"
#include <iostream>
#include <cstdlib>
using namespace std;

AudioFile::AudioFile(const string& name, Node* parent)
    : File(name + ".mpg", parent) 
{
    
    cout << "audio is recortding" << endl;
    
    #ifdef _WIN32
        string command = "ffmpeg -f dshow -i audio=\"Microphone\" -t 5 " + this->getName() + " -y";
    #else
        string command = "ffmpeg -f alsa -i default -t 5 " + this->getName() + " -y";
    #endif
    
    system(command.c_str());
    cout << "Saved: " << this->getName() << endl;
}

AudioFile::~AudioFile() 
{
    
}

void AudioFile::open()
{
    cout << "it is playing" << getName() << endl;
    string command = "ffplay -nodisp -autoexit " + getName();
    system(command.c_str());
    cout << "the playback has been done"<<endl;
}

void AudioFile::display() const {
    cout << "audio " << name;
}
