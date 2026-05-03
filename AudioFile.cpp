#include "AudioFile.h"
#include <iostream>
#include <cstdlib>
using namespace std;

AudioFile::AudioFile(const string& name, Node* parent) : File(name + ".mpg", parent)
{
    cout << "Recording has been started u can start speaking in your microphone" << endl;
 // Uses ffmpeg to record 5 seconds of audio from the default microphone into the file
 //(-f selects input format: alsa/Linux or dshow/Windows, -i default mic, -t 5 duration).
    string command = "ffmpeg -f dshow -i audio=\"Microphone\" -t 5 " + this->getName() + " -y";
    system(command.c_str());

    cout << "Recording complete & file saved as= " << this->getName() << endl;
}
AudioFile::~AudioFile() {}


void AudioFile::open() {
    cout << "Playing: " << getName() << endl;

    // ffmpeg plays the audio file to default speakers
    string command = "ffplay -nodisp -autoexit " + getName();
    system(command.c_str());

    cout << "Playback finished." << endl;
}

void AudioFile::display() const {
    cout << "Audio=  " << name << endl;
}