#include <iostream>

int fastExponent(int base, int power, int mod) {
    if (power == 1) 
        return base;

    if (power & 1) 
        return (long long)fastExponent((long long)base * base % mod, power / 2, mod) * base % mod;
    else 
        return fastExponent((long long)base * base % mod, power / 2, mod);
}

inline bool checkComposite(int n, int base, int odd, int power2) {
    int x = fastExponent(base, odd, n);

    if (x == 1 || x == n - 1)
        return false;

    for (int r = 1; r < power2; r++) {
        x = (long long)x * x % n;
        if (x == n - 1)
            return false;
    }

    return true;
};

bool MillerRabin(int n, int iter=4) { 
    if (n < 4)
        return n == 2 || n == 3;

    int power2 = 0;
    int odd = n - 1;
    while (odd % 2 == 0) {
        odd /= 2;
        power2++;
    }

    for (int i = 0; i < iter; i++) {
        int base = 2 + rand() % (n - 3);
        if (checkComposite(n, base, odd, power2))
            return false;
    }
    return true;
}

int main() {
    int n;
    int iter;

    std::cin >> n >> iter;
    std::cout << (MillerRabin(n, iter) ? "probably prime" : "composite") << std::endl;
    return 0;
}