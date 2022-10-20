#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <string>
#include <ctime>
#include <regex>
using namespace std;


string ruleSetGen() // рандоматор прав доступа на чтение и запись
{
	string M = "RWOX";
	

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
	}
	string ruleSet = fullRuleSet[rand() % 15 + 1];
	return ruleSet;
}


string commandGen(int n, int m)
{
	int Sub_n = n;
	int Obj_m = m;

	string commandString = "";
	string ruleSet = "RWOX";

	int cmd1 = rand() % 6 + 0;
	string commandSet[6] =
	{
		"cco = 'O+' image_",
		"ccs = 'S+' image_",
		"cdo = 'O-' image_",
		"cds = 'S-' image_",
		"cer = ",
		"cdr = "
	};

	if (cmd1 == 0) {
		commandString += commandSet[cmd1];
		int rander = rand() % Obj_m + 1;
		commandString += to_string(rander);
	}
	if (cmd1 == 1) {
		commandString += commandSet[cmd1];
		int rander = rand() % Sub_n + 1;
		commandString += to_string(rander);
	}
	if (cmd1 == 2) {
		commandString += commandSet[cmd1];
		int rander = rand() % Obj_m + 1;
		commandString += to_string(rander);
	}
	if (cmd1 == 3) {
		//где N - строка
		commandString += commandSet[cmd1];
		int rander = rand() % Sub_n + 1;
		commandString += to_string(rander);
	}
	if (cmd1 == 4) {
		commandString += commandSet[cmd1];
		int RWOX_rnd = rand() % 3;

		int randN = rand() % Sub_n + 1;
		int randM = rand() % Obj_m + 1;

		commandString += "'";
		commandString += ruleSet[RWOX_rnd];
		commandString += "'";
		commandString += " image_";
		commandString += to_string(randN);
		commandString += " image_";
		commandString += to_string(randM);
	}
	if (cmd1 == 5) {
		commandString += commandSet[cmd1];
		int RWOX_rnd = rand() % 3;

		int randN = rand() % Sub_n + 1;
		int randM = rand() % Obj_m + 1;

		commandString += "'";
		commandString += ruleSet[RWOX_rnd];
		commandString += "'";
		commandString += " image_";
		commandString += to_string(randN);
		commandString += " image_";
		commandString += to_string(randM);
	}
	return commandString;
}

void writeGen(int count, int n, int m)
{
	ofstream output_prgrm;
	output_prgrm.open("prgrm.txt", fstream::in | fstream::out | fstream::trunc);
	if (output_prgrm.is_open())
	{
		for (int i = 0; i < count; i++)
		{
			output_prgrm << commandGen(n, m) << endl;
		}
	}
	output_prgrm.close();
}

void interpreter()
{
	int n, m;
	vector <string> temp_N_M_A;

	string s;
	ifstream environ_file("environ.txt");
	while (getline(environ_file, s))
	{
		temp_N_M_A.push_back(s);
	}
	environ_file.close();

	n = stoi(temp_N_M_A[0]);
	m = stoi(temp_N_M_A[1]);

	temp_N_M_A.erase(temp_N_M_A.begin(), temp_N_M_A.begin() + 2);

	vector < vector <string> > ruleMatr = vector < vector <string> >();
	string input = "";
	regex re("[ ]");

	for (int i = 0; i < n; i++) // субъекты
	{
		ruleMatr.push_back(vector<string>());
		input = temp_N_M_A[i];
		sregex_token_iterator first{ input.begin(), input.end(), re, -1 }, last;
		vector<string> tokens{ first, last };
		for (int j = 0; j < m; j++) // объекты
		{
			ruleMatr[i].push_back(tokens[j]);
		}
	}

	cout << "вывод для теста RW_FROM_FILE" << endl;

	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < m; j++)
			cout << ruleMatr[i][j] << ' ';
		cout << endl;
	}

	vector <string> temp_prgrm;

	string s2;
	ifstream prgrm_file("prgrm.txt");
	while (getline(prgrm_file, s))
	{
		temp_prgrm.push_back(s);
	}
	prgrm_file.close();

	int cmd_K = temp_prgrm.size();



	for (int i = 0; i < cmd_K; i++)
	{
		cout << temp_prgrm[i] << endl;
	}

	string input2;
	regex re2(" image_");

	vector < vector <string> > structed_CMD(cmd_K, vector <string>());

	for (int i = 0; i < cmd_K; i++)
	{
		input2 = temp_prgrm[i];
		sregex_token_iterator first{ input2.begin(), input2.end(), re2, -1 }, last;//the '-1' is what makes the regex split (-1 := what was not matched)
		vector<string> tokens{ first, last };
		int k = tokens.size();
		structed_CMD[i].resize(k);
		for (int d = 0; d < k; d++)
		{
			structed_CMD[i][d] = tokens[d];
		}
	}

	for (int i = 0; i < cmd_K; i++)
	{
		if (structed_CMD[i][0].find("cco") != string::npos)
		{
			cout << i << " " << "cco" << endl;
			int ind = stoi(structed_CMD[i][1]);
			ind--;
			if (!(ruleMatr.size() == 0 || ruleMatr[0].size() == 0))
			{
				if (ruleMatr[0].size() >= ind) {
					for (int d = 0; d < ruleMatr.size(); d++)
					{
						ruleMatr[d].insert(ruleMatr[d].begin() + ind, "R");
					}
				}
				else {
					for (int d = 0; d < ruleMatr.size(); d++)
					{
						ruleMatr[d].push_back("R");
					}
				}
			}
			else {
				cout << "выполнить команду невозможно, команда -" << structed_CMD[i][0] << endl;
			}
		}

		if (structed_CMD[i][0].find("ccs") != string::npos)
		{
			cout << i << " " << "ccs" << endl;
			int ind = stoi(structed_CMD[i][1]);
			ind--;
			vector <string> row = vector <string>(ruleMatr[0].size(), "R");
			if (!(ruleMatr.size() == 0 || ruleMatr[0].size() == 0))
			{
				if (ind <= ruleMatr.size())
					ruleMatr.insert(ruleMatr.begin() + ind, row);
				else
					ruleMatr.push_back(row);
			}
			else {
				cout << "выполнить команду невозможно, команда -" << structed_CMD[i][0] << endl;
			}
		}

		if (structed_CMD[i][0].find("cdo") != string::npos)
		{
			cout << i << " " << "cdo" << endl;
			int ind = stoi(structed_CMD[i][1]);
			ind--;

			if (!(ruleMatr.size() == 0 || ruleMatr[0].size() <= ind))
			{
				for (int d = 0; d < ruleMatr.size(); d++)
				{
					ruleMatr[d].erase(ruleMatr[d].begin() + ind);
				}
			}
			else {
				cout << "выполнить команду невозможно, команда -" << structed_CMD[i][0] << endl;
			}
		}

		if (structed_CMD[i][0].find("cds") != string::npos)
		{
			cout << i << " " << "cds" << endl;
			int ind = stoi(structed_CMD[i][1]);
			ind--;
			if (!(ruleMatr.size() == 0 || ruleMatr.size() <= ind))
			{
				ruleMatr.erase(ruleMatr.begin() + ind);
			}
			else {
				cout << "выполнить команду невозможно, команда -" << structed_CMD[i][0] << endl;
			}
		}

		if (structed_CMD[i][0].find("cer") != string::npos)
		{
			cout << i << " " << "cer" << endl;
			int ind_S = stoi(structed_CMD[i][1]);
			int ind_O = stoi(structed_CMD[i][2]);
			ind_S--; ind_O--;

			if (!(ruleMatr.size() == 0 || ruleMatr.size() <= ind_S || ruleMatr[0].size() <= ind_O))
			{
				string input2 = structed_CMD[i][0];
				regex re2("cer = ");
				sregex_token_iterator first{ input2.begin(), input2.end(), re2, -1 }, last;//the '-1' is what makes the regex split (-1 := what was not matched)
				vector<string> tokens{ first, last };

				string temp = "";
				for (int z = 0; z < tokens.size(); z++)
					temp += tokens[z];

				regex re("'");
				sregex_token_iterator first2{ temp.begin(), temp.end(), re, -1 }, last2;
				vector<string> tokens2{ first2, last2 };
				string res = "";
				for (int z = 0; z < tokens2.size(); z++)
					res += tokens2[z];


				for (int inputInd = 0; inputInd < res.size(); inputInd++)
				{
					bool f = true;
					for (int nowInd = 0; nowInd < ruleMatr[ind_S][ind_O].size(); nowInd++)
					{
						if (res[inputInd] == ruleMatr[ind_S][ind_O][nowInd])
						{
							f = false;
							break;
						}
					}
					if (f)
					{
						ruleMatr[ind_S][ind_O] += res[inputInd];
					}
				}


			}
		}

		if (structed_CMD[i][0].find("cdr") != string::npos)
		{
			cout << i << " " << "cdr" << endl;
			int ind_S = stoi(structed_CMD[i][1]);
			int ind_O = stoi(structed_CMD[i][2]);
			ind_S--; ind_O--;
			if (!(ruleMatr.size() == 0 || ruleMatr.size() <= ind_S || ruleMatr[0].size() <= ind_O))
			{

				string input2 = structed_CMD[i][0];
				regex re2("cdr = ");
				sregex_token_iterator first{ input2.begin(), input2.end(), re2, -1 }, last;//the '-1' is what makes the regex split (-1 := what was not matched)
				vector<string> tokens{ first, last };

				string temp = "";
				for (int z = 0; z < tokens.size(); z++)
					temp += tokens[z];

				regex re("'");
				sregex_token_iterator first2{ temp.begin(), temp.end(), re, -1 }, last2;
				vector<string> tokens2{ first2, last2 };
				string res = "";
				for (int z = 0; z < tokens2.size(); z++)
					res += tokens2[z];

				for (int inputInd = 0; inputInd < res.size(); inputInd++)
				{
					for (int nowInd = 0; nowInd < ruleMatr[ind_S][ind_O].size(); nowInd++)
					{
						if (res[inputInd] == ruleMatr[ind_S][ind_O][nowInd])
						{
							ruleMatr[ind_S][ind_O] = ruleMatr[ind_S][ind_O].substr(0, nowInd) + ruleMatr[ind_S][ind_O].substr(nowInd + 1, ruleMatr[ind_S][ind_O].size() - nowInd - 1);
							break;
						}
					}
				}

			}
		}
	}




	ofstream output_rzlt;
	output_rzlt.open("rzlt.txt", fstream::in | fstream::out | fstream::trunc);
	if (output_rzlt.is_open())
	{

		for (int i = 0; i < ruleMatr.size(); i++) // строки
		{
			for (int j = 0; j < ruleMatr[i].size(); j++) // столбцы
				output_rzlt << ruleMatr[i][j] << ' ';
			output_rzlt << endl;
		}
	}
	output_rzlt.close();


}

void main()
{
	setlocale(LC_ALL, "RUS");
	srand(time(0));
	int n; // число субъектов(программ обработчиков) n
	int m; // число объектов(содержат информацию) m 
	int k; // число генераций команд
	n = rand() % 10 + 1;
	m = rand() % 10 + 1;
	k = rand() % 10 + 5;

	vector < vector <string> > RW(n, vector <string>(m));
	vector <int> LS(n); // уровни допуска субъектов s
	vector <int> LO(m); // уровень секретности объектов o

	cout << "генерация rw" << endl;
	for (int i = 0; i < n; i++)
		for (int j = 0; j < m; j++)
			RW[i][j] = ruleSetGen();

	ofstream output_environ;
	output_environ.open("environ.txt", fstream::in | fstream::out | fstream::trunc);
	if (output_environ.is_open())
	{
		output_environ << n << endl;
		output_environ << m << endl;
		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < m; j++)
			{
				output_environ << RW[i][j] << " ";
			}

			output_environ << endl;
		}
	}
	output_environ.close();

	writeGen(k, n, m);
	interpreter();


}