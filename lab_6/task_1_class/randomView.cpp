#include <iostream>   
#include <vector>  
#include <string>
#include <ctime>
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
	for (int i = 0; i < n; i++) {
		if (inp == 0)
			cin >> LS[i];
		else
			LS[i] = rand() % 3 + 1;
		cout << LS[i] << " ";
	}
	cout << endl;

	cout << "LO" << endl;
	for (int i = 0; i < m; i++) {
		if (inp == 0)
			cin >> LO[i];
		else
			LO[i] = rand() % 3 + 1;
		cout << LO[i] << " ";
	}
	cout << endl;

	bool isSecretNornal = true;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
					//not write down                         // not read up
			if (((LS[i] < LO[j] && A[i][j] == "W") || (LS[i] > LO[j] && A[i][j] == "R"))) {
				isSecretNornal = false;
				goto m;
			}
		}
	}
m:
	cout << "Критерию безопасности Белла - Лападулы";
	if (isSecretNornal)
		cout << " есть" << endl;
	else
		cout << " нет" << endl;
}