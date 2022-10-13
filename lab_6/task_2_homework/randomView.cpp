#include <iostream>   
#include <vector>  
#include <set> 
#include <string>
#include <ctime>
#include <algorithm>
using namespace std;

string getRandomRule()
{
	if (rand() % 2 == 0)
		return "R";
	else
		return "W";
}


int main()
{
	setlocale(LC_ALL, "RUS");
	srand(time(0));
	int n, m;
	cout << "Введите число субъектов" << endl;
	cin >> n;
	cout << "Введите число объектов" << endl;
	cin >> m;
	cout << "Введите 0 для ручного ввода" << endl;
	int inp = 0;
	cin >> inp;

	vector <vector<string>> A(n, vector<string>(m));
	vector <int> LS(n); 
	vector <int> LO(m);	
	
	cout << "A:" << endl;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			if (inp == 0)
				cin >> A[i][j];
			else
				A[i][j] = getRandomRule();
		}
	}
		
	cout << "Вывод матрицы A" << endl;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			cout << A[i][j] << ' ';
		}	
		cout << endl;
	}

	cout << "LS" << endl;
	LS = {1, 2, 3};
	random_shuffle(LS.begin(), LS.end());
	for (int i = 0; i < n; i++) {
		cout << LS[i] << " ";
	}
	cout << endl;

	LO = { 1,1,1 };
	for (int i = 0; i < m; i++) {
		
		int minR = 4, maxW = -1;
		for (int j = 0; j < n; j++) {
			if (A[j][i] == "R") {
				if (minR > LS[j]) {
					minR = LS[j];
				}
			}
		}
		for (int j = 0; j < n; j++) {
			if (A[j][i] == "W") {
				if (maxW < LS[j]) {
					maxW = LS[j];
				}
			}
		}
		if (minR < maxW) {
			goto m;
		}
		else {
			if (maxW != -1) {
				LO[i] = maxW;
			}
			else {
				LO[i] = minR;
			}
		}
	}
	cout << "LO" << endl;
	for (int i = 0; i < m; i++) {
		if (inp == 0)
			cin >> LO[i];
		else
			LO[i] = rand() % 3 + 1;
		cout << LO[i] << " ";
	}
	cout << endl;
	return 0;
m:
	cout << "Система не может удолетворить критерию" << endl;
}