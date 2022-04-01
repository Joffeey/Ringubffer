#pragma once
class RingBuffer
{
public:
	RingBuffer(int capacity);
	~RingBuffer();

	void add(char val);
	char get();

private:
	char * buffer;

	int bufferSize;
	int in = 0;
	int out = 0;
};

std::mutex door1;
std::mutex door2;


RingBuffer::RingBuffer(int capacity) {
	buffer = new char[capacity];
	bufferSize = capacity;
}


RingBuffer::~RingBuffer() {
	if (buffer != nullptr) {
		delete[] buffer;
	}
}

void RingBuffer::add(char val) {
	while ((in + 1) % bufferSize == out);
	door1.lock();
	buffer[in] = val;
	in = (in + 1) % bufferSize;
	door1.unlock();
}

char RingBuffer::get() {
	while (in == out);
	door2.lock();
	int consumed = buffer[out];
	out = (out + 1) % bufferSize;
	door2.unlock();
	return consumed;
	
}

