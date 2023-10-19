
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

struct edge {
    int a, b;
    int c;
};
vector<edge> e;
int n, m;

int mx;
vector<int> srt;

void input() {
    cin>>n>>m;
    e.resize(m);
    for (int i=0; i<m; i++) {
        cin>>e[i].a>>e[i].b>>e[i].c;
        e[i].a--;
        e[i].b--;
    }
}

void output() {
    cout<<mx<<'\n';
    for (int i=0; i<n; i++) {
        cout<<srt[i]+1<<' ';
    }
    cout<<'\n';
}

vector<int> par;
vector<bool> used;
vector<vector<int>> con;

void rec(int curr) {
    used[curr] = true;
    srt.push_back(curr);
    for (auto ch : con[curr]) {
        par[ch]--;
        if (par[ch] == 0) {
            rec(ch);
        }
    }
}

bool isOk(int temp) {
    con.clear();
    par.clear();
    used.clear();
    srt.clear();

    con.resize(n);
    par.resize(n, 0);
    used.resize(n, 0);

    for (auto ed : e) {
        if (temp >= ed.c) {
            con[ed.b].push_back(ed.a);
            par[ed.a]++;
        }
    }

    for (int i=0; i<n; i++) {
        if (par[i] == 0 && !used[i]) {
            rec(i);
        }
    }

    for (int i=0; i<n; i++) {
        if (!used[i]) {
            return false;
        }
    }

    return true;
}

void solve() {
    int l = 1, r = 1e6;
    while (l < r) {
        int mid = (l+r)/2;
        if (isOk(mid)) {
            l = mid + 1;
        } else {
            r = mid;
        }
    }

    if (l > 1e5) {
        mx = -1;
        isOk(1e5);
    } else {
        mx = l-1;
        isOk(mx);
    }
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



