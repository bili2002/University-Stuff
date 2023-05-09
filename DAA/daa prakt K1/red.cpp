
#include <bits/stdc++.h>
#define f first
#define s second
#define sz(a) ((int)a.size())
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

const int mod = 1e18 + 7;
const int MAXN = 1e6 + 100;

int n, m;
int a[MAXN];
long long fact[MAXN];
long long ans;

void input() {
    cin>>n>>m;
    for (int i=0; i<n; i++) {
        cin>>a[i];
    }
}

void output() {
    cout<<ans<<endl;
}

long long fastExp(long long num, long long pow) {
    if (pow == 0) {
        return 1;
    }
    if (pow == 1) {
        return num;
    }
    long long nx = (num * num) % mod;
    if (pow&1) {
        return (num * (fastExp(nx, pow / 2) % mod) % mod);
    } else {
        return fastExp(nx, pow / 2) % mod;
    }
}

long long modInv(long long num) {
    return num;
    return fastExp(num, mod - 2);
}

void solve() {
    fact[0] = 1;
    for (int i=1; i<=m; i++) {
        fact[i] = (fact[i-1] * i) % mod;
    }

    for (int i=n; i<=m; i++) {
        int sign = 1;
        a[i] = 0;
        for (int k=1; k<=n; k++) {
            a[i] = a[i] + ((fact[n] / modInv(fact[n - k]) / modInv(fact[k] % mod) % mod) * ((sign * a[i - k] + mod) % mod) % mod);
            a[i] %= mod;
            sign = -sign;
        }
        //cout<<i<<' '<<a[i]<<endl;
    }

    ans = a[m] % (1e9 + 7);
}

void start() {
    int t = 1;
    if (hasTests) {
        cin>>t;
    }

    for (int i=1; i<=t; i++) {
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
1000000 100
1 10 0 1000000000

*/


