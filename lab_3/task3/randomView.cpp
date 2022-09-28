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

bool isPerest(string s) {
	vector<int> v;
	for (int i = 0; i < s.size(); i++) {
		v.push_back(s[i]);
	}
	sort(begin(v), end(v));
	if (v[0] == 65) {
		for (int i = 1; i < s.size(); i++) {
			if (v[i - 1] + 1 != v[i]) {
				return false;
			}
		}
	}
	else {
		return false;
	}
	return true;
}

bool isSemiPerest(string s) {
	bool flag = false;
	for (int i = 0; i < s.size(); i++) {
		string tmp = s;
		tmp.erase(i, 1);
		flag = isPerest(tmp);
		if (flag) {
			return true;
		}
	}
	return false;
}

bool getNextString(string& s, int nowSize) {
	bool f = true;
	for (int i = 0; i < s.size(); i++) {
		f = s[i] == 64 + nowSize && f;
	}
	if (f) {
		return false;
	}

	int i = 0;
	string newString = "";
	while (true) {
		if (s[i] == 64 + nowSize) {
			newString += "A";
			i++;
		}
		else {
			newString += char(s[i] + 1) + s.substr(i+1);
			break;
		}
	}
	s = newString;
	return true;
}

bool isContains(string s, string smallS) {
	bool f = false;
	for (int i = 0; i < s.size() - smallS.size() + 1; i++) {
		f = s.substr(i, smallS.size()) == smallS;
		if (f) {
			return true;
		}
	}
	return false;
}

bool isHyperSemiPerest(string s, int n) {
	n = n + 1;
	bool f = true;
	for (int i = 0; i < s.size() + 1 - n; i++) {
		f = isSemiPerest(s.substr(i, n));
		if (!f) {
			return false;
		}
	}
	return true;
}

int main()
{
	int n; string tmp;
	ifstream input("input.txt");
	ofstream output("output.txt");
	getline(input, tmp);
	n = stoi(tmp);
	string s1, s2;
	getline(input, s1);
	getline(input, s2);
	int minSize = min(s1.size(), s2.size());

	int nowSize = minSize - 1;
	while (true) {
		nowSize++;
		string s = "";
		for (int i = 0; i < nowSize; i++) {
			s += "A";
		}
		do {
			if (isContains(s, s1) && isContains(s, s2)) {
				if (s == "ABCACB") {
					cout << (1);
				}
				if (isHyperSemiPerest(s, n)) {
					output << s ;
					return 0;
				}
			}
			
		} while (getNextString(s, nowSize));
	}
}
