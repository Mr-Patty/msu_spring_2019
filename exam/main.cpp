#include <fstream>
#include <iostream>
#include <iterator>
#include <unordered_map>
#include <set>
#include <algorithm>
#include <functional>

using namespace std;
using Comparator = std::function<bool(std::pair<std::string, int>, std::pair<std::string, int>)>;

int main(int argc, char* argv[]) {

    if (argc != 2 ) {
        std::cout << "Invalid number of parameters" << std::endl;
        return 0;
    }

    auto file = argv[1];

    ifstream input(file);
    unordered_map<string, int> tf;
    string word;

    while (input >> word) {
        auto it = tf.find(word);
        if (it != tf.end()) {
            it->second += 1;
        } else {
            it->second = 1;
        }
    }

    Comparator comp =
			[](std::pair<std::string, int> elem1 ,std::pair<std::string, int> elem2)
			{
				return elem1.second <= elem2.second;
			};

    std::set<std::pair<std::string, int>, Comparator> words(tf.begin(), tf.end(), comp);

    int i = 0;
    for (auto it = words.begin(); it != words.end() && i < 5; ++it) {
      cout << (*it).first << " " << (*it).second << endl;
      i++;
    }
    cout << '\n';

    i = 0;
    for (auto it = words.rbegin(); it != words.rend() && i < 5; ++it) {
      cout << (*it).first << " " << (*it).second << endl;
      i++;
    }

    return 0;
}
