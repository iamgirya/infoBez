#include "iostream"
#include "fstream"
#include "vector"
#include <math.h>
#include <tuple>
#include <stdio.h>
#include <string>
#include <algorithm>
#include <queue>
using namespace std;
typedef long long ll;

#include <iostream>
#include <regex>


int minCost = 0;
vector<vector<pair<int, int>>> shemeOfSignalOfMinCost;

std::vector<std::string> split(const string& input, const string& regex) {
    // passing -1 as the submatch index parameter performs splitting
    std::regex re(regex);
    std::sregex_token_iterator
        first{ input.begin(), input.end(), re, -1 },
        last;
    return { first, last };
}

// алгоритм нахождения максимального кратчайшего пути и схемы распространения сигнала
int diikstra(vector<vector<int>>& matr, vector<vector<pair<int,int>>>&  shemeOfSignal) {
    int SIZE = matr.size();
    vector<int> d(SIZE); // минимальное расстояние
    vector<int> v(SIZE); // посещенные вершины
    int temp, minindex, min;
    int begin_index = 0;
    // Вывод матрицы связей
    // 
    //Инициализация вершин и расстояний
    for (int i = 0; i < SIZE; i++)
    {
        d[i] = 10000;
        v[i] = 1;
    }
    d[begin_index] = 0;
    // Шаг алгоритма
    do {
        minindex = 10000;
        min = 10000;
        for (int i = 0; i < SIZE; i++)
        { // Если вершину ещё не обошли и вес меньше min
            if ((v[i] == 1) && (d[i] < min))
            { // Переприсваиваем значения
                min = d[i];
                minindex = i;
            }
        }
        // Добавляем найденный минимальный вес
        // к текущему весу вершины
        // и сравниваем с текущим минимальным весом вершины
        if (minindex != 10000)
        {
            for (int i = 0; i < SIZE; i++)
            {
                if (matr[minindex][i] > 0)
                {
                    temp = min + matr[minindex][i];
                    if (temp < d[i])
                    {
                        d[i] = temp;
                    }
                }
            }
            v[minindex] = 0;
        }
    } while (minindex < 10000);
    // Вывод максимального из кратчайших расстояний до вершин
    int minD = d[0];
    for (int i = 0; i < SIZE; i++) {
        if (minD < d[i]) {
            minD = d[i];
        }
    }
    
    // рассмотрим каждую минуту
    for (int i = 1; i <= minD; i++) {
        shemeOfSignal.push_back(vector<pair<int, int>>());
        for (int j = 0; j < SIZE; j++) {
            // находим вершины, что были активированны в эту минуту
            if (d[j] == i) {
                // находим родителя, который активировал найденную вершину
                for (int k = 0; k < SIZE; k++) {
                    if (matr[k][j] + d[k] == d[j]) {
                        shemeOfSignal[i - 1].push_back(make_pair(k, j));
                        break;
                    }
                }
            }
        }
    }

    return minD;
}

void swap(int* a, int i, int j)
{
    int s = a[i];
    a[i] = a[j];
    a[j] = s;
}

// функция для получения размещений без повторений
bool NextSet(int* a, int n, int m)
{
    int j;
    do  // повторяем пока не будет найдено следующее размещение
    {
        j = n - 1;
        while (j != -1 && a[j] >= a[j + 1]) j--;
        if (j == -1)
            return false; // больше размещений нет
        int k = n - 1;
        while (a[j] >= a[k]) k--;
        swap(a, j, k);
        int l = j + 1, r = n - 1; // сортируем оставшуюся часть последовательности
        while (l < r)
            swap(a, l++, r--);
    } while (j > m - 1);
    return true;
}

void chooseWeight(vector<vector<int>>& matr, vector<int>& countOfChilds, int i) {
    // если мы разместили все веса, то у нас есть готовый граф
    if (i == countOfChilds.size()) {
        vector<vector<pair<int, int>>> shemeOfSignal;
        //  Алгоритмом Диикстры находим максимальное из кратчайших растояние до вершин и схему прохода по графу 
        int minCostOfThisGraph = diikstra(matr, shemeOfSignal);
        // запоминаем наименьшее
        if (minCostOfThisGraph < minCost) {
            minCost = minCostOfThisGraph;
            shemeOfSignalOfMinCost = shemeOfSignal;
        }
        return;
    }

    // если ещё не для всех вершин расположили их веса, распологаем дальше
    int n,*a;
    n = countOfChilds[i];
    a = new int[n];
    for (int i = 0; i < n; i++)
        a[i] = i + 1;

    // получаем в цикле размещения без повторений чисел от 1 до n и заменяем это размещение на веса вершин до тех пор,
    // пока не переберём все размещения
    do {
        int k = 0;
        for (int j = 0; j < matr.size(); j++) {
            if (matr[i][j] != -10000) {
                matr[i][j] = a[k]; 
                k++;
            }
        }
        // разместив веса одной вершины, переходим к следующей
        chooseWeight(matr, countOfChilds, i + 1);
    } while (NextSet(a, n, n));
    
}

int main()
{
    ifstream input("input.txt");

    vector<vector<int>> matr;
    vector<int> countOfChild;

    // вводим данные, создаём матрицу смежности
    int n; input >> n;
    minCost = n;
    string tmp1;
    getline(input, tmp1);
    matr.resize(n, vector<int>(n, -10000));
    countOfChild.resize(n, 0);
    for (int i = 0; i < n; i++) {
        string tmp;
        getline(input, tmp);
        vector<string> childs = split(tmp, " ");
        countOfChild[i] = childs.size() - 1;
        for (int j = 1; j < childs.size(); j++) {
            matr[i][stoi(childs[j])-1] = 0;
        }
    }

    // в задаче представлено не дерево, а транспортная сеть, однако для простоты буду использовать термины деревьев
    // можем уйти от понятия времени к весам рёбер. То есть для каждого родителя с n детьми мы должны выбрать размещение без повторений
    // чисел от 1 до n, которое будет олицетворять задержку во времени, которую он имеет, чтобы передать сигнал каждому ребёнку
    // Теперь переберём всё множество таких графов и для каждого найдём алгоритмом Диикстры максимальный кратчайши путь
    // это число одновременно является минимальной наибольшей задержкой во времени до каждого из детей или же минимальным временем, необходимым
    // для оповещения каждого сотрудника. Вместе этим с помощью алгоритма Диикстры составляем схему обхода
    chooseWeight(matr, countOfChild, 0);

    ofstream output("output.txt");
    output << minCost << endl;
    for (int i = 0; i < minCost; i++) {
        for (int j = 0; j < shemeOfSignalOfMinCost[i].size(); j++) {
            output << shemeOfSignalOfMinCost[i][j].first+1 << ' ' << shemeOfSignalOfMinCost[i][j].second + 1 << ' ';
        }
        output << endl;
    }
}