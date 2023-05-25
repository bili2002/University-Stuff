
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

const int MAXN = 1e7 + 100;

int n, m, q;
vector<pair<int, int>> cross, tree;

void input() {
    cin>>n>>m;
    cross.resize(n);
    tree.resize(m);
    cin>>cross>>tree;
}

void output() {
}

int binary(vector<int>& s, int num) {
    int l = 0, r = s.size();
    while (l < r) {
        int mid = (l + r) / 2;
        if (s[mid] > num) {
            r = mid;
        } else {
            l = mid + 1;
        }
    }

    return l;
}

int closest(vector<int>& s, int num) {
    int big = binary(s, num);

    if (big == s.size()) {
        return num - s.back();
    } else if (big == 0) {
        return s[big] - num;
    }
    return min(s[big] - num, num - s[big - 1]);
}

void solve() {
    vector<int> sX(n), sY(n);

    for (int i=0; i<n; i++) {
        sX[i] = cross[i].first;
        sY[i] = cross[i].second;
    }
    sort(all(sX));
    sort(all(sY));

    vector<int> widths(m);
    for (int i=0; i<m; i++) {
        int temp = min(closest(sX, tree[i].first), closest(sY, tree[i].second));
        widths[i] = temp*2;
    }

    sort(all(widths));
    //cout<<widths<<endl;

    int q, curr;
    cin>>q;
    while (q--) {
        cin>>curr;
        cout<<binary(widths, curr)<<' ';
    }
    cout<<endl;
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

*/



