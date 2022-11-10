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

string getRandomAccess() {
	static vector<string> tmp = {"o", "i", "w", "r", "t", "g"};
	return tmp[rand()%6];
}

vector<vector<string>> generateGraph() {
	int n = 5;
	vector<vector<string>> r = vector<vector<string>>(n, vector<string>(n, " "));
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			if (i != j)
				r[i][j] = getRandomAccess();

	return r;
}

vector<vector<string>> badExsample() {
	int n = 5;
	vector<vector<string>> r = vector<vector<string>>(n, vector<string>(n, " "));
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			if (i != j)
				if (i == 0 || j == 0) {
					r[i][j] = "r";
				}
				else {
					r[i][j] = getRandomAccess();
				}
				

	return r;
}

bool contains(string s, char c) {
	for (auto i : s)
		if (i == c)
			return true;
	return false;
}

bool isHaveTgWay(vector<vector<string>>& matr, int x, int y) {
	vector<bool> visited(matr.size(), false);
	queue<int> q;
	q.push(x); visited[x] = true;

	while (!q.empty()) {
		int v = q.front(); q.pop();

		if (v == y) {
			return true;
		}
			
		for (int j = 0; j < matr.size(); j++) {
			if (!visited[j]) {
				if (contains(matr[v][j], 't') || contains(matr[v][j], 'g') || contains(matr[j][v], 't') || contains(matr[j][v], 'g')) {
					q.push(j);
					visited[j] = true;
				}
			}	
		}
	}
	return false;
}

bool possibleAccess(vector<vector<string>>& matr, string a, int x, int y) {
	vector<char> rules;
	for (auto i : a)
		rules.push_back(i);

	//Первый критерий
	vector<vector<int>> neededRules(rules.size(), vector<int>());
	for (int r = 0; r < rules.size(); r++) {  // для каждого правила

		for (int i = 0; i < matr.size(); i++) {
			for (int j = 0; j < matr.size(); j++) {
				if (contains(matr[i][j], rules[r])) { // проверяем, существует ли оно в матрице и запоминаем, кто им обладает
					neededRules[r].push_back(i);
					break;
				}
			}
		}
	}
	for (auto i : neededRules) {
		if (i.empty())
			return false;
	}
	
	// второй критерий
	vector<bool> canReachRules(rules.size());
	for (int r = 0; r < rules.size(); r++) { // для каждого правила
		bool f = false;
		
		for (int k = 0; k < neededRules[r].size(); k++) {
			if (isHaveTgWay(matr, x, neededRules[r][k])) { // ищем наличие tg пути хотя бы до одного обладателя этого правила
				f = true;
				break;
			}
		}

		if (!f) {
			return false; 
		}
	}

	return true;
}

int main()
{
	setlocale(LC_ALL, "RUS");
	srand(time(0));

	//vector<vector<string>> matr = generateGraph();
	vector<vector<string>> matr = badExsample();

	for (auto i : matr) {
		for (auto j : i)
			cout << j << ' ';
		cout << endl;
	}
		
	while (true) {
		string tmpStr;
		int i, j;
		cin >> tmpStr >> i >> j;
		cout << possibleAccess(matr, tmpStr, i, j) << endl;
	}
}
