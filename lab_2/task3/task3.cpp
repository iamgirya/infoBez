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

ll gcd_ext(ll a, ll b, ll& x, ll& y) {
    if (b == 0) {
        x = 1;
        y = 0;
        return a;
    }
    ll d = gcd_ext(b, a % b, x, y);
    x -= (a / b) * y;
    swap(x, y);
    return d;
}

int main()
{
    ifstream input("input.txt");
    ll n, m, d, c;
    ll x, y;
    input >> n >> m >> d >> c;
    vector<ll> cod;
    for (int i = 0; i < m; i++) {
        int tmp; input >> tmp;
        cod.push_back(tmp);
    }
    vector<ll> info;
    ofstream output("output.txt");
    for (int i = 0; i < m; i++) {
        ll e = gcd_ext(d, -n, x, y);
        if ((cod[i]-c) % e != 0) {
            output << "NO";
            return 0;
        }
        ll p = x * (cod[i] - c) / e;
        ll q = (-n) / e;
        ll ans = (p % q + q) % q;
        if (ans < 0) {
            ans += n;
        }
        info.push_back(ans);
        //y = ((cod[i]-c) - d * ans) / (-n);
    }
  
    output << "Yes" << endl;
    for (int i = 0; i < m; i++) {
        output << info[i] << ' ';
    }
    //y - целое
    //x*d - n*y = j-c
    
}
