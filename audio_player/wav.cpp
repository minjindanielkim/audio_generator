#include <iostream>
#include <fstream>
#include <cmath>
using namespace std;

// Riff Chunk
const string chunkID = "RIFF";
const string chunkSize = "----"; // 4 dashes since it needs 4 bytes for the field size
const string format = "WAVE";

// FMT sub-chunk
const string subChunk1_ID = "fmt "; // there could be different information stored that is worth 4 bytes, so there a space is neccessary
const int subChunk1_size = 16;
const int audioFormat = 1;
const int numChannels = 2; // right and left speaker doing different things at different times
const int sampleRate = 44100;
const int byteRate = sampleRate * numChannels * (subChunk1_size / 8); // dividing the number of bytes and bites
const int blockAlign = numChannels * (subChunk1_size / 8);
const int bits_per_sample = 16;

// data sub-chunk
const string subChunk2_ID = "data";
const string subChunk2_size = "----";

const int duration = 2;
const int maxAmplitude = 32760;
const double frequency = 250;

void write_as_bytes(ofstream  &file, int value, int byte_size) {
    file.write(reinterpret_cast<const char*>(&value), byte_size); // calling the file we are sending, and using the write function from ofstream 
}


int main() {
    ofstream wav;
    wav.open("test.wav", ios::binary);
    if(wav.is_open()) {
        wav << chunkID;
        wav << chunkSize;
        wav << format;

        wav << subChunk1_ID;
        write_as_bytes(wav, subChunk1_size, 4);
        write_as_bytes(wav, audioFormat, 2);
        write_as_bytes(wav, numChannels, 2); 
        write_as_bytes(wav, sampleRate, 4);
        write_as_bytes(wav, byteRate, 4);
        write_as_bytes(wav, blockAlign, 2);                
        write_as_bytes(wav, bits_per_sample, 2);

        wav << subChunk2_ID;
        wav << subChunk2_size;

        int beginAudio = wav.tellp();

        for(int i = 0; i < sampleRate * duration; i++) {
            // respect the max amplitude 
            // operate as a wave
            double amplitude = (double)i / sampleRate * maxAmplitude;
            double value = sin((2 * 3.14 * i * frequency) / sampleRate);

            double channel1 = amplitude * value / 2; 
            double channel2 = (maxAmplitude - amplitude) * value; 

            write_as_bytes(wav, channel1, 2);
            write_as_bytes(wav, channel2, 2);
        }   
        int endAudio = wav.tellp();
        wav.seekp(beginAudio - 4);
        write_as_bytes(wav, endAudio - beginAudio, 4);
        wav.seekp(4, ios::beg);
        write_as_bytes(wav, endAudio - 8, 4);
    }
    wav.close();
    return 0;
}