#include <iostream>
#include <random>
#include <chrono>
#include <functional>

using namespace std::chrono;

typedef struct Duration {
    hours        hour{0};
    minutes      minute{0};
    seconds      second{0};
    milliseconds milli{0};
    microseconds micro{0};
    nanoseconds  nano{0};
} Duration;

Duration nanosToDuration(const nanoseconds& duration){
    Duration res = {
        .hour   = duration_cast<hours>(duration),
        .minute = duration_cast<minutes>(duration - res.hour),
        .second = duration_cast<seconds>(duration - res.hour - res.minute),
        .milli  = duration_cast<milliseconds>(duration - res.hour - res.minute - res.second),
        .micro  = duration_cast<microseconds>(duration - res.hour - res.minute - res.second - res.milli),
        .nano   = duration_cast<nanoseconds>(duration - res.hour - res.minute - res.second - res.milli - res.micro)
    };
    return res;
}

template <typename ReturnType,
          typename = std::enable_if<!std::is_void_v<ReturnType>>>
struct TimedFunctionResult {
    ReturnType res;
    Duration duration;
};

template <typename ReturnType,
          typename TimedFunction,
          typename ...Args,
          typename = std::enable_if<!std::is_void_v<ReturnType>>>
TimedFunctionResult<ReturnType> timeFn(TimedFunction&& fn, Args&&... args){
    const auto before = high_resolution_clock::now();
    ReturnType fnRes = std::forward<TimedFunction>(fn)(std::forward<Args>(args)...);
    const auto after = high_resolution_clock::now();

    return TimedFunctionResult<ReturnType>{
        .res = fnRes,
        .duration = nanosToDuration(after - before)
    };
}

typedef struct RollResults {
    uint_fast32_t maxOnes;
    uint_fast32_t totalRuns;
} RollResults;

template <typename Distribution, typename Engine>
RollResults do_real_rolls(const uint_fast32_t numberOfRuns,
                                Distribution& dist,
                                Engine&       engine)
{
    uint_fast32_t currentRun = 0;
    uint_fast32_t currentOnes = 0;
    uint_fast32_t maxOnes = 0;
    while (maxOnes < 177 && currentRun < numberOfRuns){
        for (int i = 0; i < 231; ++i){
            if (dist(engine) == 1) ++currentOnes;
        }
        maxOnes = std::max(currentOnes, maxOnes);
        currentOnes = 0;
        ++currentRun;
    };
    return RollResults{.maxOnes   = maxOnes,
                       .totalRuns = currentRun};
}

int main(int argc, char *argv[]){
    const uint_fast32_t numberOfRuns =
            (argc > 1) ? std::stoi(argv[1]) : 1000000000;

    // Setup prng
    std::random_device seed;
    std::subtract_with_carry_engine<uint_fast32_t, 24, 10, 24> engine(seed());
    std::uniform_int_distribution<> d4(1, 4);
    
    // Roll and count ones
    std::cout << "\nDoing "<< numberOfRuns << " runs with 231 dice rolls per run...\n";
    // const auto timed = timeFn<RollResults>(do_real_rolls<decltype(d4)&, decltype(engine)&>, numberOfRuns, d4, engine);
    const auto fn = [](auto numberOfRuns, auto d4, auto engine){ return do_real_rolls(numberOfRuns, d4, engine); };
    const auto timed = timeFn<RollResults>(fn, numberOfRuns, d4, engine);
    const auto res = timed.res;
    const auto duration = timed.duration;

    std::cout << "\nDone. Took "
              << duration.hour.count() << " hours, "
              << duration.minute.count() << " minutes, "
              << duration.second.count() << "."
              << duration.milli.count() << " seconds.\n\n";

    std::cout << "Number Of Runs: " << res.totalRuns << "\n";
    std::cout << "Number Of Rolls: " << (res.totalRuns*231) << "\n";
    std::cout << "Highest Ones Rolled: " << res.maxOnes << "\n\n";

    return 0;
}