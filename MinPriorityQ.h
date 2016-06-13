#pragma once
#include <vector>
#include <map>
using namespace std;
template<typename KeyType, typename compare = less<KeyType>>
class minPriorityQ {
public:
	typedef unsigned int _uInt;
	minPriorityQ() {
		_heap.push_back(KeyType());//insert at 0th position so that actual elements start at 1
	}
	minPriorityQ(const vector<KeyType>& dataArray) {
		_heap.push_back(KeyType());//insert at 0th position so that actual elements start at 1
		for (KeyType Key : dataArray) {
			push(Key);
		}
	}
	/*
	Inserts the element at the end and floats it up
	*/
	void push(KeyType key) {
		_heap.push_back(key);
		swim_up(_heap.size()-1);
	}
	KeyType min() {
		if (_heap.size() < 2) {//One is the element at 0th position which should not be considered
			throw("Underflow error");
		}
		return _heap.at(1);
	}
	/*
	Removes the minimum element
	Don't return the element to provide strong guarntee
		If the function returns the template type and the user defined type 
		raises exception while coping(during the return statement) the heap
		will be left in bad state (element removed but not returned)
	*/
	void extractMin() {
		if (_heap.size() < 2) {//One is the element at 0th position which should not be considered
			throw("Underflow error");
		}
		//First swap the first and last element
		swap(_heap.at(1), _heap.back());
		_heap.pop_back();
		//restore the min heap variant
		sink_down(1);
	}
	/*
	changes the key if it is lesser and restores the min priority queue invariant
	*/
	void decreaseKey(_uInt elementIndex, KeyType newKey) {
		if (elementIndex<1 || elementIndex>_heap.size() - 1)
			throw("Element out of range");

		if (newKey >= _heap.at(elementIndex))
			throw("New key is not lesser than earlier");

		_heap.at(elementIndex) = newKey;
		swim_up(elementIndex);
	}
	size_t size() {
		return _heap.size();
	}
	bool empty() {
		return !(_heap.size() > 1);
	}
	
private:
	vector<KeyType> _heap;

	unsigned int parent(_uInt elementIndex) {
		if (_heap.size() > 2)
			return elementIndex / 2;
		else
			return 1;
	}
	unsigned int left(_uInt elementIndex) {
		return 2 * elementIndex; 
	}
	unsigned int right(_uInt elementIndex) {
		return 2 * elementIndex + 1;
	}

	/*
	Takes the element's index as input which needs to be put in right place
	Keeps moving the element down until the childern's are bigger
	*/
	void sink_down(_uInt elementIndex) {
		_uInt leftIndex = left(elementIndex);
		_uInt rightIndex = right(elementIndex);
		_uInt smallestIndex = leftIndex;
		while (leftIndex <= _heap.size()-1 && _heap.at(leftIndex) < _heap.at(elementIndex) ||
			rightIndex <= _heap.size()-1 && _heap.at(rightIndex) < _heap.at(elementIndex)) {

			//First check if left side is smallest
			if (leftIndex <= _heap.size() - 1 && _heap.at(leftIndex) < _heap.at(elementIndex))
				smallestIndex = leftIndex;
			else
				smallestIndex = elementIndex;

			//check the smallest against right element
			if (rightIndex <= _heap.size() - 1 && _heap.at(rightIndex) < _heap.at(smallestIndex))
				smallestIndex = rightIndex;

			swap(_heap.at(elementIndex), _heap.at(smallestIndex));
			leftIndex = left(smallestIndex);
			rightIndex = right(smallestIndex);
			elementIndex = smallestIndex;
		}
	}
	/*
	Takes the element's index as input which needs to be put in right place
	Keeps moving the element up until the parent is lesser
	*/
	void swim_up(_uInt elementIndex) {
		_uInt parentIndex = parent(elementIndex);
		while (_heap[elementIndex] < _heap[parentIndex]) {
			swap(_heap[elementIndex], _heap[parentIndex]);
			elementIndex = parentIndex;
			parentIndex = parent(elementIndex);
		}
	}
};
