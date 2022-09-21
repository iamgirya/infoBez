#include "vector"
#include <math.h>
#include <tuple>
#include <stdio.h>
#include <algorithm>
#include <queue>
#include <string>
#include <iostream>
#include <fstream>

using namespace std;
typedef long long ll;

int main()
{
    ifstream input("input.txt");
    string line;
    getline(input, line);
    string newLine;
    int countOfLetter = 1;
    for (int i = 1; i < line.size(); i++) {
        if (line[i] == line[i - 1]) {
            countOfLetter++;
        }
        else if (countOfLetter > 1) {
            newLine += to_string(countOfLetter)+line[i-1];
            countOfLetter = 1;
        }
        else {
            newLine += line[i - 1];
            countOfLetter = 1;
        }
    }
    if (countOfLetter > 1) {
        newLine += to_string(countOfLetter) + line[line.size() - 1];
        countOfLetter = 1;
    }
    else {
        newLine += line[line.size()-1];
        countOfLetter = 1;
    }
    ofstream output("output.txt");
    output << newLine;
}
