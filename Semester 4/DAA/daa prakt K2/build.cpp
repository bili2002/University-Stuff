
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
template<class T>           inline ostream &operator<<(ostream &os, const vector<T> &v)               { for (auto id : v)    os<<id<<'\n';  return os; }
template<class T, class T2> inline ostream &operator<<(ostream &os, const map<T, T2> &m)              { for (auto id : m)    os<<id<<'\n'; return os; }
template<class T, class T2> inline ostream &operator<<(ostream &os, const unordered_map<T, T2> &um)   { for (auto id : um)   os<<id<<'\n'; return os; }
template<class T>           inline ostream &operator<<(ostream &os, const list<T> &l)                 { for (auto id : l)    os<<id<<' ';  return os; }
template<class T>           inline ostream &operator<<(ostream &os, const set<T> &s)                  { for (auto id : s)    os<<id<<' ';  return os; }
template<class T>           inline ostream &operator<<(ostream &os, const unordered_set<T> &us)       { for (auto id : us)   os<<id<<' ';  return os; }
const long long INF = 1e18;
const bool hasTests = 0;

const int MAXN = 1e5 + 100;

int n, m;
vector<int> pr;
struct edge {
    int fr, to;
    int price;
};

struct cmp {
    bool operator()(const edge& l, const edge& r) {
        return l.price < r.price;
    }
};
vector<edge> e;

long long sum = 0;
vector<pair<int, int>> path;

void input() {
    cin>>n>>m;
    pr.resize(n);
    e.resize(m);

    cin>>pr;

    for (int i=0; i<m; i++) {
        int a, b, c;
        cin>>a>>b>>c;
        a--, b--;
        if (a > b) {
            swap(a, b);
        }
        int p = ((c+1)/2) * pr[a] + (c/2) * pr[b];
        e[i] = {a, b, p};
    }
}

void output() {
    cout<<sum<<'\n';
    cout<<path<<'\n';
}

int par[MAXN];

int findPar(int p) {
    if (p == par[p]) {
        return p;
    }
    return par[p] = findPar(par[p]);
}

bool unite(int u, int v) {
    u = findPar(u);
    v = findPar(v);

    if (u != v) {
        par[u] = v;
        return true;
    } else {
        return false;
    }
}



void solve() {
    sort(all(e), cmp());

    for (int i=0; i<n; i++) {
        par[i] = i;
    }

    for (auto ed : e) {
        if (unite(ed.fr, ed.to)) {
            sum = (long long) sum + ed.price;
            path.pb({ed.fr+1, ed.to+1});
        }
    }

    sort(all(path));
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


