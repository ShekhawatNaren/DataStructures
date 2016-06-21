#pragma once

template<class T> class Node;

template<class T>
class SinglyList {
	class const_iterator;
public:
	SinglyList():_head(nullptr) {}
	bool empty() {
		return _head == nullptr;
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
	void remove(T data) {
		Node<T>* temp = nullptr;
		Node<T>* prev = _head;
		if (_head != nullptr) {
			if (_head->_data == data) {
				_head = _head->_next;
				delete prev;
				prev = nullptr;
			}else{
				temp = _head->_next;
			}
		}

		while (temp != nullptr) {
			if (temp->_data == data) {
				erase_after(prev);
				break;
			}else{
				temp = temp->_next;
				prev = prev->_next;
			}
		}
	}

	~SinglyList() {
		while (_head != nullptr) {
			Node<T>* temp = _head;
			_head = _head->_next;
			delete temp;
			temp = nullptr;
		}
	}
	const_iterator cbegin() const {
		return const_iterator(_head);
	}
	const_iterator cend() const{
		return const_iterator(nullptr);
	}
private:
	Node<T>* _head;
	void erase_after(Node<T>* prev) {
		if (prev->_next != nullptr) {
			Node<T>* temp = prev->_next;
			prev->_next = prev->_next->_next;
			delete temp;
			temp = nullptr;
		}
	}
public:
	class const_iterator {
	public:
		const_iterator(Node<T>* curr) {
			_current = curr;
		}
		bool operator != (const const_iterator& rhs) {
			return this->_current != rhs._current;
		}
		const_iterator& operator++() {
			_current = _current->_next;
			return *this;
		}
		const T& operator*() const {
			return _current->_data;
		}
	private:
		Node<T>* _current;
	};
};

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
