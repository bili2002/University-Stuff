
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

int n, m, k;

vector<pair<int, int>> con[MAXN];
vector<bool> used;
vector<int> pth;
int ans;

void input() {
    cin>>n>>m>>k;
    used.resize(n, 0);
    pth.resize(n, -1);
    for (int i=0; i<m; i++) {
        int a, b, c, d;
        cin>>a>>b>>c>>d;
        con[a-1].push_back({b-1, c-d});
        con[b-1].push_back({a-1, c-d});
    }
}

void output() {
    cout<<ans<<endl;
}

void solve() {
    priority_queue<pair<int, int>> pq;
    pq.push({0, 0});

    while (!pq.empty()) {
        auto curr = pq.top();
        pq.pop();

        if (used[curr.s]) {
            continue;
        }
        used[curr.s] = true;
        if (curr.s == n-1) {
            break;
        }

        for (auto ch : con[curr.s]) {
            if (!used[ch.f] && (-curr.f-ch.s < pth[ch.f] || pth[ch.f] == -1)) {
                pth[ch.f] = -curr.f+-ch.s;
                pq.push({-pth[ch.f], ch.f});
            }
        }
    }

    /*for (int i=0; i<n; i++) {
        cout<<i<<' '<<pth[i]<<endl;
    }*/

    ans = k - pth[n-1];
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



