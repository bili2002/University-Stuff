#include < iostream >
#include < set >

using namespace std;

const int MAXN = 1e6;

int n;
int a[MAXN];
int b[MAXN];
int cntBM[MAXN];
int commonBM[MAXN];
long long ans;

void input() {
    cin>>n;
    for (int i=0; i<n; i++) {
        cin>>a[i];
    }
}

void makeBitmasks() {
    for (int i=0; i<n; i++) {
        set<int> s;
        int temp = a[i];
        while (temp > 0) {
            s.insert(temp % 10);
            temp /= 10;
        }

        int curr = 0, pw = 1;
        while (curr < 10) {
            if (s.find(curr) != s.end()) {
                b[i] += pw;
            }
            curr++;
            pw *= 2;
        }

        cntBM[b[i]]++;
    }
}

void countDigits(int curr, int bm) {
    for (int i=0; i<(1 << 10); i++) {
        for (int j=0; j<(1 << 10); j++) {
            if ((i & j) == i) {
                commonBM[i] += cntBM[j];
            }
        }
    }
}

int countSameDigit(int numBM, int curr, int currBM, int sign) {
    if (curr == 10) {
        ans += commonBM[currBM] * sign;
        return;
    }

    if (numBM & (1 << curr) == 0) {
        countSameDigit(numBM, curr + 1, currBM, sign);
        return;
    }

    int sum = countSameDigit(numBM, curr + 1, currBM, sign);
    currBM += 1 << curr;
    sum += countSameDigit(numBM, curr + 1, currBM, -sign);

    return sum;
}


void findAns() {
    for (int i=0; i<(1 << 10); i++) {
        ans += cntBM[i] * countSameDigit(i, 0, 0, 1);
    }
}

int main() {
    input();
    findAns();
    cout<<ans<<endl;
}