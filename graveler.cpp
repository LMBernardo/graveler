#include <iostream>
#include <random>
#include <chrono>

using namespace std::chrono;

int main(int argc, char *argv[]){
    uint32_t numberOfRuns = 1000000000;
    if (argc > 1){
        numberOfRuns = std::stoi(argv[1]);
    }

    // Setup pnrg
    std::random_device seed;
    std::subtract_with_carry_engine<uint_fast32_t, 24, 10, 24> generator(seed());
    std::uniform_int_distribution<> d4(1, 4);
    
    // Roll and count ones
    std::cout << "\nDoing 231 dice rolls " << numberOfRuns << " times...\n";

    const auto before = high_resolution_clock::now();
    uint_fast32_t maxOnes = 0;
    uint_fast32_t currentRun = 0;
    uint_fast32_t ones = 0;
    while (maxOnes < 177 && currentRun < numberOfRuns){
        for (int i = 0; i < 231; ++i) if (d4(generator) == 1) ++ones;
        maxOnes = std::max(ones, maxOnes);
        ones = 0;
        ++currentRun;
    };
    const auto after = high_resolution_clock::now();

    // Output clock time of run
    const auto duration = after - before;
    const auto numHours = duration_cast<hours>(duration);
    const auto numMinutes = duration_cast<minutes>(duration - numHours);
    const auto numSeconds = duration_cast<seconds>(duration - numHours - numMinutes);
    const auto numMillis = duration_cast<milliseconds>(duration - numHours - numMinutes - numSeconds); 

    std::cout << "\nDone. Took "
              << numHours.count() << " hours, "
              << numMinutes.count() << " minutes, "
              << numSeconds.count() << "."
              << numMillis.count() << " seconds.\n\n";

    std::cout << "Number of rolls: " << numberOfRuns << "\n";
    std::cout << "Highest Ones Rolled: " << maxOnes << "\n\n";

    return 0;
}