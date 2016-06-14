#pragma once

template<class T>
class Node {
public:
	Node() {
		_data{};
		_next = nullptr;
	}
	Node(T d) :_data{ d } {
		_next = nullptr;
	}
	~Node() {
		_next = nullptr;
	}
	T _data;
	Node* _next;
};

template<class T>
class SinglyList {
public:
	SinglyList() {
		_head = nullptr;
	}
	void push_front(T data) {
		if (_head == nullptr) {
			_head = new Node<T>(data);
		}else{
			Node<T>* newNode = new Node<T>(data);
			newNode->_next = _head;
			_head = newNode;
		}
	}
	Node<T>* findKthLastElement(unsigned int k) {
		if (_head == nullptr)
			return nullptr;
		Node<T>* temp = _head;
		Node<T>* kPrev = _head;
		while (temp->_next != nullptr) {
			temp = temp->_next;
			if(k==1){
				kPrev = kPrev->_next;
			}else{
				k--;
			}
		}
		if (k == 1 && kPrev != nullptr)
			return kPrev;
		else
			return nullptr;
	}
	~SinglyList() {
		while (_head != nullptr) {
			Node<T>* temp = _head;
			_head = _head->_next;
			delete temp;
			temp = nullptr;
		}
	}
private:
	Node<T>* _head;
};