// PriorityQ.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "MinPriorityQ.h"
#include <vector>
#include <iostream>

class mergeData {
public:
	bool operator< (const mergeData& rhs) {
		return key < rhs.key;
	}
	bool operator> (const mergeData& rhs) {
		return key > rhs.key;
	}
	mergeData() :key{ }, vecPos{ }, vectorsInd{ } {

	}
	mergeData(int k, int iv, int index) :key{ k }, vecPos{ iv }, vectorsInd{ index }{}
public:
	int key;
	int vecPos;
	int vectorsInd;
};

vector<int> kWayMerge(const vector<vector<int>>& listOfSortedVectors) {
	minPriorityQ<mergeData> pq;
	vector<int> mergedVec;
	int vecPos = 0;
	for (auto v : listOfSortedVectors) {
		pq.push(mergeData(v[0], vecPos++, 0));
	}
	while (!pq.empty()) {
		mergeData data = pq.extractMin();
		mergedVec.push_back(data.key);
		if (listOfSortedVectors.at(data.vecPos).size() > data.vectorsInd + 1) {
			data.vectorsInd++;
			pq.push(mergeData(listOfSortedVectors.at(data.vecPos).at(data.vectorsInd), data.vecPos, data.vectorsInd));
		}
	}
	return mergedVec;
}
int main()
{
	vector<vector<int>> listOfSortedVectors{ { 4,5,17,18 },{1,7,8,9},{6,11,13,15},{2,12,14,20},{3,16,21,34},{10,19,22,26} };
	vector<int> mergedVec = kWayMerge(listOfSortedVectors);
	for (auto e : mergedVec) {
		cout << e << ",";
	}
    return 0;
}

