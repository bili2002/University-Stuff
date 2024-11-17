// C++ program for solving 0/1 Knapsack Problem using
// recursion

#include <iostream>
#include <vector>
#include <map>
using namespace std;

map<pair<long long, long long>, long long> mem;

long long knapsackRecursive(vector<long long>& weight,
                      vector<long long>& value, long long W, long long n)
{
    if (mem.find({W, n}) != mem.end()) {
        return mem[{W, n}];
    }

    if (n == 0 || W == 0)
        return 0;

    if (weight[n - 1] > W)
        return knapsackRecursive(weight, value, W, n - 1);

    return mem[{W, n}] = max(value[n - 1]
                   + knapsackRecursive(weight, value,
                                       W - weight[n - 1],
                                       n - 1),
               knapsackRecursive(weight, value, W, n - 1));
}

int main()
{
    long long capacity, num_items;
    cin >> capacity >> num_items;

    vector<long long> weight(num_items);
    vector<long long> value(num_items);

    for (long long i = 0; i < num_items; ++i) {
        cin >> weight[i] >> value[i];
    }

    cout << "Maximum value = "
         << knapsackRecursive(weight, value, capacity,
                              num_items)
         << endl;
    return 0;
}