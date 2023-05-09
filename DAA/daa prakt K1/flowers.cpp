
#include <bits/stdc++.h>
#define f first
#define s second
#define sz(a) ((long long)a.size())
#define all(a) a.begin(), a.end()
#define pb push_back
#define eb emplace_back

using namespace std;
using ll = long long;

template<class T, class T2> inline bool chkmax(T &x, const T2 & y) { return x < y ? x = y, 1 : 0; }
template<class T, class T2> inline bool chkmin(T &x, const T2 & y) { return x > y ? x = y, 1 : 0; }
template<class T, class T2> inline istream &operator>>(istream &is,       pair<T, T2> &p)             { is>>p.f>>p.s;        return is; }
template<class T, class T2> inline ostream &operator<<(ostream &os, const pair<T, T2> &p)             { os<<p.f<<' '<<p.s;   return os; }
template<class T>           inline istream &operator>>(istream &is,       vector<T> &v)               { for (auto &id : v)   is>>id;       return is; }
template<class T>           inline ostream &operator<<(ostream &os, const vector<T> &v)               { for (auto id : v)    os<<id<<' ';  return os; }
template<class T, class T2> inline ostream &operator<<(ostream &os, const map<T, T2> &m)              { for (auto id : m)    os<<id<<'\n'; return os; }
template<class T, class T2> inline ostream &operator<<(ostream &os, const unordered_map<T, T2> &um)   { for (auto id : um)   os<<id<<'\n'; return os; }
template<class T>           inline ostream &operator<<(ostream &os, const list<T> &l)                 { for (auto id : l)    os<<id<<' ';  return os; }
template<class T>           inline ostream &operator<<(ostream &os, const set<T> &s)                  { for (auto id : s)    os<<id<<' ';  return os; }
template<class T>           inline ostream &operator<<(ostream &os, const unordered_set<T> &us)       { for (auto id : us)   os<<id<<' ';  return os; }
const long long INF = 1e18;
const bool hasTests = 0;

const long long MAXN = 5e5 + 100;

long long n, k;
vector<long long> prices;
long long sum = 0;

void input() {
    cin>>k>>n;
    prices.resize(k);
    cin>>prices;
}

void output() {
    cout<<sum<<endl;
}

void solve() {
    sort(all(prices), greater<long long>());

    long long mul = 1;
    for (long long i=0; i<k; i++) {
        for (long long j=0; j<n && i+j<k; j++) {
            sum = (long long) sum + prices[i+j] * mul;
        }
        i += n - 1;
        mul++;
    }
}

void start() {
    long long t = 1;
    if (hasTests) {
        cin>>t;
    }

    for (long long i=1; i<=t; i++) {
        input();
        solve();
        //cout<<"Case #"<<i<<": ";
        output();
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    start();
    return 0;
}

/*
9 3
2 3 4 2 3 4 2 3 4

4*3 + 3*3*2 + 2*3*3
12 + 18 + 18
*/



