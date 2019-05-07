#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <mutex>
#include <iterator>
#include <thread>
#include <filesystem>
#include <future>

namespace fs = std::filesystem;

size_t batch_size = 10000;
std::mutex mut;

void my_merge(std::string inOne, std::string inTwo, std::string output) {
    std::ifstream inputOne(inOne, std::ios::binary );
    std::ifstream inputTwo(inTwo, std::ios::binary );
    std::ofstream mergeFile(output, std::ios::binary );
    std::merge(std::istream_iterator<uint64_t>(inputOne),
               std::istream_iterator<uint64_t>(),
               std::istream_iterator<uint64_t>(inputTwo),
               std::istream_iterator<uint64_t>(),
               std::ostream_iterator<uint64_t>(mergeFile, " "));
   return;
}

void my_sort(const char* file, std::string prefix) {
    std::vector<uint64_t> vec(batch_size);
    FILE *fp;
    if ((fp = fopen(file, "rb")) == NULL) {
        std::cout << "Cannot open file." << std::endl;
        return;
    }

    int i = 0;
    while(!feof(fp)) {

        {
            std::lock_guard<std::mutex> lock(mut);
            fread(vec.data(), sizeof(uint64_t), batch_size, fp);
        }

        std::sort(begin(vec), end(vec));
        std::ofstream out("tmp/" + prefix + std::to_string(i) + ".bin",
                        std::ios::binary);
        for (auto j : vec) {
            if (j != 0)
                out << j << " ";
            else
                continue;
        }
        i++;
    }
    fclose(fp);

    int start = 0;
    int end = i;

    std::string next = prefix + std::to_string(start + 1);
    std::string curr = prefix + std::to_string(start);
    std::rename(("tmp/" + curr + ".bin").c_str(),
                ("tmp/a_" + curr + ".bin").c_str() );

    while (start < end - 1) {
        my_merge("tmp/a_" + curr + ".bin", "tmp/" + next + ".bin",
                "tmp/a_" + next + ".bin");

        start++;
        next = prefix + std::to_string(start + 1);
        curr = prefix + std::to_string(start);
    }
    std::rename(("tmp/a_" + curr + ".bin").c_str(),
                ("tmp/" + prefix + "output.bin").c_str() );
    return;
}

int main(int argc, char* argv[]) {
    if (argc != 3 ) {
        std::cout << "Invalid number of parameters" << std::endl;
        return 0;
    }

    fs::create_directories("tmp");
    auto file = argv[1];
    auto outputFile = argv[2];
    std::thread t1(my_sort, file, "one_");
    std::thread t2(my_sort, file, "two_");

    t1.join();
    t2.join();
    my_merge("tmp/one_output.bin", "tmp/two_output.bin", outputFile);

    fs::remove_all("tmp");
    return 0;
}
