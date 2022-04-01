#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <mutex>
#include "RingBuffer.h"

std::mutex mtx;


void bufferReader(RingBuffer* buffer) {
    while (true) {
        mtx.lock();
        std::cout << buffer->get();
        mtx.unlock();
    }
}

void keyboardReader(RingBuffer* buffer) {
    while (true) {
        std::string input;
        std::getline(std::cin, input);
        
        for (int i = 0; i < input.size(); i++) {
            buffer->add(input[i]);
            //std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
        buffer->add('\n');
    }
}

void generatedReader(std::string genTxt, RingBuffer* buffer) {
    while (true) {
        for (int i = 0; i < genTxt.size(); i++) {
            buffer->add(genTxt[i]);
            std::this_thread::sleep_for(std::chrono::milliseconds(20));
        }
        buffer->add('\n');
    } 
}

int main()
{
    RingBuffer buffer(15);
    std::string c = "GENERERT TEKST!";

    std::thread keyboard(keyboardReader, &buffer);
    std::thread generated(generatedReader, c, &buffer);
    std::thread reader(bufferReader, &buffer);


    keyboard.join();
    generated.join();    
    reader.join();
}

