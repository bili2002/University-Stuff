
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

const long long MAXN = 1e7 + 100;

long long n;
vector<double> x;
vector<double> v;
double ans;

void input() {
    cin>>n;
    x.resize(n);
    v.resize(n);
    cin>>x>>v;
}

void output() {
    cout<<fixed<<setprecision(6)<<ans<<endl;
}

double calc(double point) {
    double mx = 0;
    for (int i=0; i<n; i++) {
        mx = max(mx, abs(x[i] - point) / v[i]);
    }

    return mx;
}


const double eps = 1e-7;
void solve() {
    double l = 0, r = 1e9;

    while (l + eps < r) {
        double mid1 = l + (r - l) / 3;
        double mid2 = r - (r - l) / 3;

        double ans1 = calc(mid1);
        double ans2 = calc(mid2);

        //cout<<l<<' '<<r<<endl;
        //cout<<mid1<<' '<<ans1<<' '<<mid2<<' '<<ans2<<endl;

        if (ans1 < ans2) {
            r = mid2;
        } else {
            l = mid1;
        }
    }

    ans = calc(l);
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




