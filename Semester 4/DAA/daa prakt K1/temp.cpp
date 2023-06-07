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
int n;
vector<vector<int>> con;
vector<pair<int, int>> coor;
vector<int> realPar;

map<pair<int, int>, int> revCoor;
int children[MAXN];
int biggest[MAXN];
int par[MAXN];
int subAns[MAXN];

void input() {
    cin>>n;
    con.resize(n);
    realPar.resize(n, -1);
    for (int i=0; i<n-1; i++) {
        int a, b;
        cin>>a>>b;
        con[a-1].push_back(b-1);
        realPar[b-1] = a-1;
    }
    coor.resize(n);
    cin>>coor;
}


int findPar(int a) {
    if (a == par[a]) {
        return a;
    }
    return par[a] = findPar(par[a]);
}

bool unite(int a, int b) {
    int pa = findPar(a);
    int pb = findPar(b);
    if (pa == pb) {
        return false;
    }

    par[pa] = pb;
    return true;
}

int dfsPre(int curr) {
    if (con[curr].empty()) {
        return children[curr] = 1;
    }

    biggest[curr] = con[curr][0];

    int cnt = 1;
    for (int to : con[curr]) {
        int temp = dfsPre(to);
        cnt += temp;

        if (temp > children[biggest[curr]]) {
            biggest[curr] = to;
        }
    }

    return children[curr] = cnt;
}

vector<pair<int, int>> dir = {{0,1}, {0,-1}, {1,0}, {-1, 0}};
void combine(set<pair<int, int>>& big, int& compBig, set<pair<int, int>>& small, int& compSmall) {
    for (auto curr : small) {
        big.insert(curr);
    }
}

void rec(set<pair<int, int>>& in, set<pair<int, int>>& used, pair<int, int> curr) {
    used.insert(curr);

    if (in.find({curr.f+1, curr.s}) != in.end() && used.find({curr.f+1, curr.s}) == used.end()) {
        rec(in, used, {curr.f+1, curr.s});
    }

    if (in.find({curr.f-1, curr.s}) != in.end() && used.find({curr.f-1, curr.s}) == used.end()) {
        rec(in, used, {curr.f-1, curr.s});
    }

    if (in.find({curr.f, curr.s+1}) != in.end() && used.find({curr.f, curr.s+1}) == used.end()) {
        rec(in, used, {curr.f, curr.s+1});
    }

    if (in.find({curr.f, curr.s-1}) != in.end() && used.find({curr.f, curr.s-1}) == used.end()) {
        rec(in, used, {curr.f, curr.s-1});
    }
}

int findComp(set<pair<int, int>>& in) {
    set<pair<int, int>> used;

    int cnt = 0;
    for (auto curr : in) {
        if (used.find(curr) != used.end())
            continue;
        cnt++;
        rec(in, used, curr);
    }

    return cnt;
}

void dfs(int curr, set<pair<int, int>>& in) {
    in.insert(coor[curr]);

    for (int to : con[curr]) {
        dfs(to, in);
    }

}

int solveFor(int curr) {
    set<pair<int, int>> a;
    dfs(curr, a);

    return findComp(a);
}

void output() {
    int q;
    cin>>q;
    for (int i=0; i<n; i++) {
        int a;
        cin>>a;

        cout<<solveFor(a-1)<<'\n';
    }
}

void start() {
    int t = 1;
    if (hasTests) {
        cin>>t;
    }

    for (int i=1; i<=t; i++) {
        input();
        for (int i=0; i<n; i++) {
            revCoor[coor[i]] = i;
        }
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
10
2 6
6 9
9 10
6 7
2 4
4 8
8 1
8 3
8 5

0 0
0 1
1 1
2 1
2 0
2 -1
2 -2
1 -2
0 -2
0 -1

10
10
9
8
7
6
5
4
3
2
1

*/




