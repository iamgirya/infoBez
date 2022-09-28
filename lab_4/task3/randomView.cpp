#include <math.h>
#include <tuple>
#include <vector>
#include <stdio.h>
#include <algorithm>
#include <queue>
#include <string>
#include <iostream>
#include <fstream>
using namespace std;
typedef long long ll;

class Tuple
{
	int i, j, k;
public:
	Tuple(int i, int j, int k);
	~Tuple();

	int get(int h) {
		if (h == 0)
			return i;
		if (h == 1)
			return j;
		return k;
	}

private:

};

Tuple::Tuple(int i, int j, int k)
{
	this->i = i;
	this->j = j;
	this->k = k;
}

Tuple::~Tuple()
{
}

int main()
{
	ifstream input("onlyone.in");
	ofstream output("onlyone.out");
	int n; input >> n;
	vector<Tuple> v;
	for (int i = 0; i < n; i++) {
		int t1, t2, t3;
		input >> t1 >> t2 >> t3;
		v.push_back(Tuple(t1,t2,t3));
	}

	int count = 0;
	for (int i = 0; i < n; i ++)
		for (int j = 0; j < i; j++) {
			int ravCount = 0;
			for (int k = 0; k < 3; k++) {
				if (v[i].get(k) == v[j].get(k)) {
					ravCount++;
				}
			}
			if (ravCount == 1) {
				count++;
			}
		}
	output << count;

}

