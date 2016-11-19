#include "stdafx.h"
#include <vector>
#include <unordered_map>
#include <list>

using namespace std;

template<size_t Capacity>
class LRUCache {
public:
	bool lookup(int isbn, int *price) {
		auto iter = cache_map_.find(isbn);
		if (iter == cache_map_.end()) {
			return false;
		}
		*price = iter->second.price;
		moveToFront(isbn, iter);

		return true;
	}
	void insert(int isbn, int p) {
		auto iter = cache_map_.find(isbn);
		if (iter == cache_map_.end()) {
			if (cache_map_.size() == Capacity) {
				//If the max capacity of the cache has reached we need to remove
				//Least Recently Used entry from the table
				cache_map_.erase(lru_queue_.back());
				lru_queue_.pop_back();
			}
			lru_queue_.emplace_front(isbn);
			cache_map_[isbn] = { p, lru_queue_.begin() };
		}
		else {
			//found the isbn in table
			moveToFront(isbn, iter);
		}
	}
	bool erase(int isbn) {
		auto it = cache_map_.find(isbn);
		if (it == cache_map_.end()) {
			return false;
		}
		else {
			lru_queue_.erase(it->second.listIter);
			cache_map_.erase(it);
			return true;
		}
	}
private:
	struct CacheVal {
		int price;
		list<int>::iterator listIter;
	};
	typedef unordered_map<int, CacheVal> Table;
	void moveToFront(int isbn,typename const Table::iterator& iter) {
		//remove it from the queue 
		lru_queue_.erase(iter->second.listIter);

		//insert it in the begginging of the queue
		lru_queue_.emplace_front(isbn);
		iter->second.listIter = lru_queue_.begin();
	}
	Table cache_map_;
	list<int> lru_queue_;
};

int main() {
	LRUCache<5> myCache;
	myCache.insert(452337, 200);
	myCache.insert(452338, 100);
	myCache.insert(452340, 555);
	myCache.insert(452352, 3330);
	myCache.insert(589456, 454);
	myCache.insert(875424, 7000);

	myCache.erase(452340);
	int price;
	myCache.lookup(452352, &price);
	return 0;
}
