#include <iostream>
#include <stdlib.h>
#include <vector>
#include <algorithm>
#include <math.h>
#include "numbers.dat"

using namespace std;
const int N = 100000;

vector<bool> sieve() {
    vector<bool> S (N + 1, 1);
    S[0] = S[1] = 0;
    for (int i = 2; i * i <= N; ++i)
        if (S[i])
            if (i * i <= N)
                for (int j = i * i; j <= N; j += i)
                    S[j] = 0;
    return S;
}



bool check_prime(int x, vector<bool>& S) {
    if (S[x]) return true;
    return false;
}

void check(vector<bool>& S, int x, int y) {
    int sum = 0;
    if (find(Data, Data + Size, x) == Data + Size || find(Data, Data + Size, y) == Data + Size) {
        cout << 0 << endl;
        return;
    }
    for (size_t i = 0; i < Size; i++) {
        if (Data[i] > y) break;
        if (Data[i] >= x)
            if (check_prime(Data[i], S)) sum++;
    }
    cout << sum << endl;
}

int main(int argc, char* argv[]) {
    vector<int> vec;
    if (argc % 2 == 0 || argc == 1)
        return -1;
    for (int i = 1; i < argc; ++i) {
        int v = std::atoi(argv[i]);
        vec.push_back(v);
    }
    auto S = sieve();
    for (size_t j = 0; j < vec.size() - 1; j += 2)
        check(S, vec[j], vec[j + 1]);
    return 0;
}