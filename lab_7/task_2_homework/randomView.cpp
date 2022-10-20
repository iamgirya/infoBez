#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <string>
#include <ctime>
#include <regex>
using namespace std;

string ruleSetGen()
{
	string M = "RW";


	int n = pow(2, M.size());
	static vector <string> fullRuleSet;

	if (fullRuleSet.size() == 0) {
		for (int i = 0; i < n; i++) {
			string buff = "";
			for (int j = 0; j < M.size(); j++) {
				if (i & (1 << j)) {
					buff += M[j];
				}
			}
			fullRuleSet.push_back(buff);
		}
		fullRuleSet[0] = "_";
	}
	fullRuleSet.push_back("_");
	string ruleSet = fullRuleSet[rand() % 4];
	return ruleSet;
}

vector < vector <string> > getRuleMatr() {
	int n, m;
	vector <string> tmp;
	string s;

	ifstream access_matr("access_matr.txt");
	while (getline(access_matr, s))
	{
		tmp.push_back(s);
	}
	access_matr.close();

	n = stoi(tmp[0]);
	m = stoi(tmp[1]);
	tmp.erase(tmp.begin(), tmp.begin() + 2);

	vector < vector <string> > ruleMatr = vector < vector <string> >();

	for (int i = 0; i < n; i++) {
		ruleMatr.push_back(vector<string>());
		string ss = "";
		for (int j = 0; j < tmp[i].size(); j++) {
			if (tmp[i][j] != ' ') {
				ss += tmp[i][j];
			}
			else {
				ruleMatr[i].push_back(ss);
				ss = "";
			}
		}
	}
	return ruleMatr;
}

bool isCanRead(string s) {
	for (int i = 0; i < s.size(); i++) {
		if (s[i] == 'R') {
			return true;
		}
	}
	return false;
}

bool isCanWrite(string s) {
	for (int i = 0; i < s.size(); i++) {
		if (s[i] == 'W') {
			return true;
		}
	}
	return false;
}

bool isEmptyRule(string s) {
	return !(isCanWrite(s) || isCanRead(s));
}

bool isFullRule(string s) {
	return isCanWrite(s) && isCanRead(s);
}

vector<int> task_a(vector < vector <string> > ruleMatr) // список объектов, не доступных для всех субъектов
{
	vector <int> answer = vector <int>();
	for (int j = 0; j < ruleMatr[0].size(); j++) {
		bool f = true;
		for (int i = 0; i < ruleMatr.size(); i++) {
			if (!isEmptyRule(ruleMatr[i][j])) {
				f = false;
				break;
			}
		}
		if (f) {
			answer.push_back(j+1);
		}
	}
	return answer;
}

vector<int> task_b(vector < vector <string> > ruleMatr) // список пустых субъектов
{
	vector <int> answer = vector <int>();
	for (int i = 0; i < ruleMatr.size(); i++) {
		bool f = true;
		for (int j = 0; j < ruleMatr[i].size(); j++) {
			if (!isEmptyRule(ruleMatr[i][j])) {
				f = false;
				break;
			}
		}
		if (f) {
			answer.push_back(i + 1);
		}
	}
	return answer;
}

vector<int> task_c(vector < vector <string> > ruleMatr) // список полных субъектов
{
	vector <int> answer = vector <int>();
	for (int i = 0; i < ruleMatr.size(); i++) {
		bool f = true;
		for (int j = 0; j < ruleMatr[i].size(); j++) {
			if (isEmptyRule(ruleMatr[i][j])) {
				f = false;
				break;
			}
		}
		if (f) {
			answer.push_back(i + 1);
		}
	}
	return answer;
}

vector<vector<int>> task_d(vector < vector <string> > ruleMatr) // список список субъектов записи по объектам
{
	vector<vector<int>> answer = vector<vector<int>>();
	for (int j = 0; j < ruleMatr[0].size(); j++) {
		answer.push_back(vector<int>());
		bool f = true;
		for (int i = 0; i < ruleMatr.size(); i++) {
			if (isCanWrite(ruleMatr[i][j])) {
				answer[j].push_back(i + 1);
			}
		}
	}
	return answer;
}

vector<int> task_e(vector < vector <string> > ruleMatr) // список одиноких субъектов
{
	vector <int> answer = vector <int>();
	for (int i = 0; i < ruleMatr.size(); i++) {
		bool f = true;
		bool fullRule = false;
		for (int j = 0; j < ruleMatr[i].size(); j++) {
			if (isFullRule(ruleMatr[i][j])) {
				if (fullRule) {
					f = false;
					break;
				}
				fullRule = true;
			} else if (isCanWrite(ruleMatr[i][j])) {
				f = false;
				break;
			}
		}
		if (f) {
			answer.push_back(i + 1);
		}
	}
	return answer;
}

void coutVector(vector<int> v) {
	for (int i = 0; i < v.size(); i++) {
		cout << v[i] << ' ';
	}
	cout << endl << endl;
}

void coutVectors(vector<vector<int>> v) {
	for (int i = 0; i < v.size(); i++) {
		for (int j = 0; j < v[i].size(); j++) {
			cout << v[i][j] << ' ';
		}
		cout << endl;
	}
	cout << endl;
}

void main()
{
	setlocale(LC_ALL, "RUS");
	srand(time(0));
	int n; // число субъектов(программ обработчиков) n
	int m; // число объектов(содержат информацию) m 
	int k; // число генераций команд
	n = rand() % 5 + 1;
	m = rand() % 5 + 1;

	vector < vector <string> > RW(n, vector <string>(m));
	vector <int> LS(n); // уровни допуска субъектов s
	vector <int> LO(m); // уровень секретности объектов o

	cout << "генерация rw" << endl;
	for (int i = 0; i < n; i++)
		for (int j = 0; j < m; j++)
			RW[i][j] = ruleSetGen();

	ofstream access_matr;
	access_matr.open("access_matr.txt", fstream::in | fstream::out | fstream::trunc);
	if (access_matr.is_open())
	{
		access_matr << n << endl;
		access_matr << m << endl;
		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < m; j++)
			{
				access_matr << RW[i][j] << " ";
				cout << RW[i][j] << " ";
			}
			access_matr << endl;
			cout << endl;
		}
	}
	access_matr.close();

	vector < vector <string> > ruleMatr =  getRuleMatr();
	cout << "Список пустых объектов" << endl;
	coutVector(task_a(ruleMatr));
	cout << "Список пустых субъектов" << endl;
	coutVector(task_b(ruleMatr));
	cout << "Список полных объектов" << endl;
	coutVector(task_c(ruleMatr));
	cout << "Список редакторов по объектам" << endl;
	coutVectors(task_d(ruleMatr));
	cout << "Список единственных редакторов" << endl;
	coutVector(task_e(ruleMatr));

}