
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

const int MAXN = 1e5 + 100;
const long long MAXM = 2<<15;

int n, m, k;
long long dp[2][MAXM];
long long ans;

void input() {
    cin>>n>>k>>m;
}

void output() {
    cout<<ans<<endl;
}

void solve() {
    const long long mod = 1 << m;
    for (int i=1; i<=26; i++) {
        dp[1][(i * 33) % mod] = 1;
    }

    for (int i=2; i<=n; i++) {
        for (int hs=0; hs < mod; hs++) {
            long long h = (hs * 33) % mod;
            dp[i&1][h] = 0;
            for (int j=1; j<=26; j++) {
                int real = hs ^ j;
                dp[i&1][h] += dp[(i+1)&1][real];
            }
            //cout<<dp[2][hs]<<endl;
        }

    }
    ans = dp[n&1][(k * 33) % mod];
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
    cout<<(1<<20)<<endl;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    start();
    return 0;
}

/*
3 2594 12

2 416 12



*/


