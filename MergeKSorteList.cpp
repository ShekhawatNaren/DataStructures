#include "stdafx.h"
#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
#include <functional> 
using namespace std;

struct IteratorForSortedArray {
	bool operator >(const IteratorForSortedArray& that) const {
		return *current > *that.current;
	}
	vector<int>::const_iterator current;
	vector<int>::const_iterator end;
};
vector<int> MergeKSortedList(const vector<vector<int>>& sorted_arrays) {
	vector<int> result;
	std::priority_queue<IteratorForSortedArray,vector<IteratorForSortedArray>,greater<>> pq;
	for (const vector<int>& sorted_arr : sorted_arrays) {
		if(!sorted_arr.empty())
			pq.emplace(IteratorForSortedArray{ sorted_arr.cbegin(), sorted_arr.cend() });
	}
	while (!pq.empty()) {
		auto el = pq.top();
		pq.pop();
		
		if (el.current != el.end) {
			result.push_back(*el.current);
			if(std::next(el.current)!=el.end)
				pq.emplace(IteratorForSortedArray{ std::next(el.current), el.end });
		}
	}

	return result;
}
int main()
{
	vector<vector<int>> sorted_arrays{ { 57,131,493 } ,{ 339,418,452 }, { 190,442 }};
	vector<int> res = MergeKSortedList(sorted_arrays);
	for_each(res.cbegin(), res.cend(), [](int a) {cout << a << " "; });
	cout << endl;
	return 0;
}
