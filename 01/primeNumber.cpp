#include <iostream>
#include <vector>
#include <algorithm>
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

void check(vector<bool>& S, int x, int y) {
    int sum = 0;
    auto left = find(Data, Data + Size, x);
    auto right = find(Data, Data + Size, y);
    if (left == Data + Size || right == Data + Size) {
        cout << 0 << endl;
        return;
    }
    for (auto i = left; i < Data + Size; i++) {
        if (*i > y) break;
        if (S[*i]) sum++;
    }
    cout << sum << endl;
}

int main(int argc, char* argv[]) {
    if (argc % 2 == 0 || argc == 1)
        return -1;
    auto S = sieve();
    for (int i = 1; i < argc - 1; i += 2) {
        int v1 = std::atoi(argv[i]);
        int v2 = std::atoi(argv[i + 1]);
        check(S, v1, v2);
    }
    return 0;
}
