#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <utility>
#include <tuple>
#include <algorithm>
#include <cstring>
#include <math.h>

//prepare to reuse old code from CS 240 Project 2 -> I did a max heap in that one, so some stuff may need to be converted



class MinHeap {
private:
	struct Contestant {
		int id;
		int score;
	};
	int max_size;
	const int STARTING_INDEX = 1;
	Contestant* min_heap;
	int heap_size;
	int* handle_array;
	bool isLeaf(int);
	void siftdown(int);
	unsigned int right(int base) {
		return (2 * base) + 1;
	};
	unsigned int left(int base) {
		return (2 * base);
	};
public:
	MinHeap(int);
	~MinHeap();
	//Contestant* make_min_heap(Contestant& min_heap);
	void addElement(Contestant, std::ofstream&); //done
	bool empty() {
		return this->heap_size == 0;
	};
	void findContestant(int, std::ofstream&); //done
	void insertContestant(int, int, std::ofstream&);
	void eliminateWeakest(std::ofstream&); //done
	void earnPoints(int, int, std::ofstream&); //done
	void losePoints(int, int, std::ofstream&); //done
	void showContestants(std::ofstream&); //done
	void showHandles(std::ofstream&); //done
	void showLocation(int, std::ofstream&); //done
	void crownWinner(std::ofstream&);
};

bool MinHeap::isLeaf(int position) {
	int size_var = this->heap_size;
	if (position == 1 && size_var == 2) {
		return false;
	}
	if (position >= ((size_var / 2)) && position <= size_var) {
		return true;
	}
	return false;
}

void MinHeap::siftdown(int position) {
	int num_contestants = this->heap_size;
	if ((position >= 1) && (position <= num_contestants)) {
		if (!isLeaf(position)) {
			int test_leaf = left(position);
			int right_check = right(position);
			if (right_check < num_contestants && (this->min_heap[right_check].score < this->min_heap[test_leaf].score)) {
				test_leaf = right_check;
			}
			if (this->min_heap[position].score > this->min_heap[test_leaf].score) {
				Contestant temp = this->min_heap[position];
				this->min_heap[position] = this->min_heap[test_leaf];
				this->handle_array[this->min_heap[test_leaf].id] = position; //updates handle array regarding test leaf's location
				this->min_heap[test_leaf] = temp;
				this->handle_array[this->min_heap[position].id] = test_leaf; //updates handle array regarding position's location
			}
			siftdown(test_leaf);
		}
	}
}

MinHeap::MinHeap(int size) {
	this->max_size = size;
	this->heap_size = 0;
	this->min_heap = new Contestant[this->max_size + 1];
	this->handle_array = new int[this->max_size + 1];
	for (int i = 1; i <= this->max_size; i++) {
		handle_array[i] = -1;
	}
}

MinHeap::~MinHeap() {
	delete[] min_heap;
	delete[] handle_array;
}

void MinHeap::addElement(Contestant element, std::ofstream& output) {
	if (this->handle_array[element.id] != -1) {
		output << "Contestant <" << element.id << "> is already in the extended array: cannot insert." << std::endl;
		return;
	}
	if (this->heap_size == this->max_size) {
		output << "Contestant <" << element.id << "> could not be inserted because the extended heap is full." << std::endl;
		return;
	}
	this->min_heap[heap_size + 1] = element;
	this->heap_size += 1;
	this->handle_array[element.id] = this->heap_size;
	for (int pos = heap_size; pos >= 1; pos--) {
		siftdown(pos);
	}
	output << "Contestant <" << element.id << "> inserted with initial score <" << element.score << ">." << std::endl;
}


void MinHeap::insertContestant(int id, int score, std::ofstream& output) {
	Contestant applicant;
	applicant.id = id;
	applicant.score = score;
	addElement(applicant, output);
}

void MinHeap::eliminateWeakest(std::ofstream& output)
{
	if (empty()) {
		output << "No Contestant can be eliminated since the extended heap is empty" << std::endl;
		return;
	}
	Contestant elim = this->min_heap[this->STARTING_INDEX];
	this->min_heap[this->STARTING_INDEX] = this->min_heap[this->heap_size];
	this->handle_array[this->min_heap[this->STARTING_INDEX].id] = 1;
	this->min_heap[this->heap_size] = elim;
	this->handle_array[this->min_heap[heap_size].id] = -1;
	this->heap_size--;
	siftdown(this->STARTING_INDEX);
	output << "Contestant <" << elim.id << "> with current lowest score <" << elim.score << "> eliminated." << std::endl;
}

void MinHeap::findContestant(int id, std::ofstream& output) {
	if (this->handle_array[id] != -1 && id <= this->max_size) {
		Contestant selected = this->min_heap[this->handle_array[id]];
		output << "Contestant <" << id << "> is in the extended heap with score <" << selected.score << ">." << std::endl;
	}
	else {
		output << "Contestant <" << id << "> is not in the extended heap." << std::endl;
	}
}

void MinHeap::earnPoints(int id, int points, std::ofstream& output) {
	if (this->handle_array[id] != -1 && id <= this->max_size) {
		this->min_heap[this->handle_array[id]].score += points;
		output << "Contestant <" << id << ">'s score in increased by <" << points << "> points to <" << this->min_heap[this->handle_array[id]].score << ">." << std::endl;
		for (int pos = heap_size; pos >= 1; pos--) {
			siftdown(pos);
		}
	}
	else {
		output << "Contestant <" << id << "> is not in the extended heap." << std::endl;
	}
}

void MinHeap::losePoints(int id, int points, std::ofstream& output) {
	if (this->handle_array[id] != -1 && id <= this->max_size) {
		this->min_heap[this->handle_array[id]].score -= points;
		output << "Contestant <" << id << ">'s score in decreased by <" << points << "> points to <" << this->min_heap[this->handle_array[id]].score << ">." << std::endl;
		for (int pos = heap_size; pos >= 1; pos--) {
			siftdown(pos);
		}
	}
	else {
		output << "Contestant <" << id << "> is not in the extended heap." << std::endl;
	}
}

void MinHeap::showContestants(std::ofstream& output) {
	for (int i = 1; i <= this->heap_size; i++) {
			output << "Contestant <" << this->min_heap[i].id << "> in extended heap location <" << i << "> with score <" << this->min_heap[i].score << ">." << std::endl;
	}
}

void MinHeap::showHandles(std::ofstream& output) {
	for (int i = 1; i <= this->max_size; i++) {
		if (this->handle_array[i] != -1) {
			output << "Contestant <" << i << "> stored in extended heap location <" << this->handle_array[i] << ">." << std::endl;
		}
		else {
			output << "There is no Contestant <" << i << "> in the extended heap. handle[<" << i << ">] = -1." << std::endl;
		}
	}
}

void MinHeap::showLocation(int id, std::ofstream& output) {
	if (this->handle_array[id] != -1 && id <= this->max_size) {
		output << "Contestant <" << id << "> stored in extended heap location <" << this->handle_array[id] << ">." << std::endl;
	}
	else {
		output << "There is no Contestant <" << id << "> in the extended heap. handle[<" << id << ">] = -1." << std::endl;
	}
}

void MinHeap::crownWinner(std::ofstream& output)
{
	while (this->heap_size > 1) {
		eliminateWeakest(output);
	}
	output << "Contestant <" << this->min_heap[this->STARTING_INDEX].id << "> wins with score " << this->min_heap[this->STARTING_INDEX].score << "!" << std::endl;
}


int main(int argc, char* argv[]) {
	char * input_file;
	char * output_file;
	input_file = argv[1];
	output_file = argv[2];
	std::ifstream input(input_file);
	std::ofstream output(output_file);
	int heap_size;
	input >> heap_size;
	auto heap = MinHeap(heap_size);
	while (input.peek() != EOF) {
		std::string next_command;
		std::string param1;
		std::string param2;
		input >> next_command;
		output << next_command;
		if (next_command == "insertContestant") {
			input >> param1;
			input >> param2;
			output << " " << param1;
			output << " " << param2;
			output << std::endl;
			param1.erase(0, 1); //moves past the < present at the beginning
			param2.erase(0, 1);
			int id = std::stoi(param1);
			int score = std::stoi(param2);
			heap.insertContestant(id, score, output);
		}
		if (next_command == "findContestant") {
			input >> param1;
			output << " " << param1;
			output << std::endl;
			param1.erase(0, 1); //moves past the < present at the beginning
			int id = std::stoi(param1);
			heap.findContestant(id, output);
		}
		if (next_command == "eliminateWeakest") {
			output << std::endl;
			heap.eliminateWeakest(output);
		}
		if (next_command == "earnPoints") {
			input >> param1;
			input >> param2;
			output << " " << param1;
			output << " " << param2;
			output << std::endl;
			param1.erase(0, 1); //moves past the < present at the beginning
			param2.erase(0, 1);
			int id = std::stoi(param1);
			int points = std::stoi(param2);
			heap.earnPoints(id, points, output);
		}
		if (next_command == "losePoints") {
			input >> param1;
			input >> param2;
			output << " " << param2;
			output << " " << param1;
			output << std::endl;
			param1.erase(0, 1); //moves past the < present at the beginning
			param2.erase(0, 1);
			int id = std::stoi(param1);
			int points = std::stoi(param2);
			heap.losePoints(id, points, output);
		}
		if (next_command == "showContestants") {
			output << std::endl;
			heap.showContestants(output);
		}
		if (next_command == "showHandles") {
			output << std::endl;
			heap.showHandles(output);
		}
		if (next_command == "showLocation") {
			input >> param1;
			output << " " << param1;
			output << std::endl;
			param1.erase(0, 1); //moves past the < present at the beginning
			int id = std::stoi(param1);
			heap.showLocation(id, output);
		}
		if (next_command == "crownWinner") {
			output << std::endl;
			heap.crownWinner(output);
		}
	
	}
	input.close();
	output.close();
}

/*
Still to do:

Implement Heap Structure:
Implement Heapify
Create array of input lines to run commands, using a series of if statements.
Finish all the functions.
*/