#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <mutex>
#include <iterator>
#include <thread>
#include <filesystem>

namespace fs = std::filesystem;

size_t batch_size = 10000;
size_t N = 0;
std::mutex mut;

void my_sort(const char* file) {
    std::vector<uint64_t> vec(batch_size);
    FILE *fp;
    if ((fp = fopen(file, "rb")) == NULL) {
        std::cout << "Cannot open file." << std::endl;
        return;
    }
    while(!feof(fp)) {

        {
            std::lock_guard<std::mutex> lock(mut);
            fread(vec.data(), sizeof(uint64_t), batch_size, fp);
        }

        std::sort(begin(vec), end(vec));
        {
            std::lock_guard<std::mutex> lock(mut);
            std::ofstream out("tmp/" + std::to_string(N) + ".bin", std::ios::binary );
            for (auto j : vec) {
                if (j != 0)
                    out << j << " ";
                else
                    continue;
            }
            ++N;
        }
    }
    fclose(fp);
}

void my_merge(std::string inOne, std::string inTwo, std::string output) {
    std::ifstream inputOne(inOne, std::ios::binary );
    std::ifstream inputTwo(inTwo, std::ios::binary );
    std::ofstream mergeFile(output, std::ios::binary );
    std::merge(std::istream_iterator<uint64_t>(inputOne), std::istream_iterator<uint64_t>(),
               std::istream_iterator<uint64_t>(inputTwo), std::istream_iterator<uint64_t>(),
               std::ostream_iterator<uint64_t>(mergeFile, " "));
   return;
}

void mergeFiles(size_t start, size_t end) {
    std::string next = std::to_string(start + 1);
    std::string curr = std::to_string(start);
    std::rename( ("tmp/" + curr + ".bin").c_str(), ("tmp/a_" + curr + ".bin").c_str() );
    while (start < end) {
        my_merge("tmp/a_" + curr + ".bin", "tmp/" + next + ".bin",
                "tmp/a_" + next + ".bin");

        start++;
        next = std::to_string(start + 1);
        curr = std::to_string(start);
    }
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
    std::thread t1(my_sort, file);
    std::thread t2(my_sort, file);

    t1.join();
    t2.join();

    if (N > 0) {

        std::thread t3(mergeFiles, 0, N / 2);
        std::thread t4(mergeFiles, N / 2 + 1, N);

        t3.join();
        t4.join();
    }

    if (N == 0)
        std::rename( ("tmp/" + std::to_string(N) + ".txt").c_str(), outputFile);
    else {
        my_merge("tmp/a_" + std::to_string(N / 2) + ".bin",
                "tmp/a_" + std::to_string(N) + ".bin", outputFile);
    }
    fs::remove_all("tmp");
    return 0;
}
