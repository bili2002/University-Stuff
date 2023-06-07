
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

const long long MAXN = 1000;


long long n, m;
long long k;
bool field[MAXN][MAXN];
bool used[MAXN][MAXN];
long long ans;

void input() {
    cin>>n>>m>>k;
    for (long long i=0; i<n; i++) {
        for (long long j=0; j<m; j++) {
            char temp;
            cin>>temp;

            used[i][j] = false;
            if (temp == '.') {
                field[i][j] = true;
            } else {
                field[i][j] = false;
            }
        }
    }
}

void output() {
    cout<<ans<<endl;
}

struct QU{
    long long x, y;
    long long val;
};

vector<pair<long long, long long>> dir = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};

void solve() {
    queue<QU> qu;
    for (long long i=0; i<k; i++) {
        long long a, b;
        cin>>a>>b;
        qu.push({a-1, b-1, 0});
        used[a-1][b-1] = true;
    }

    while (!qu.empty()) {
        auto curr = qu.front();
        qu.pop();
        for (auto d : dir) {
            long long x = curr.x + d.f;
            long long y = curr.y + d.s;

            if (x < 0 || x >= n || y < 0 || y >= m)
                continue;

            if (field[x][y] && !used[x][y]) {
                used[x][y] = true;
                qu.push({x, y, curr.val+1});
                ans = (long long)ans + curr.val + 1;
            }
        }
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
1000000 100
1 10 0 1000000000

*/



