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

int main()
{
	ofstream output("output.txt");
    int n, * a;
    n = 8;
    a = new int[n];
    string starnNum = "22557777";
    for (int i = 0; i < n; i++)
        a[i] = int(starnNum[i]-48);
	
    int coumt = 0;
	do {
        for (int i = 0; i < n; i++)
            cout << a[i];
        output << endl;
        coumt++;
	} while (NextSet(a, n, n));
} 