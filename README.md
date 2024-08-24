# graveler
C++ Code for [ShoddyCast's Graveler Challenge](https://www.youtube.com/watch?v=M8C8dHQE2Ro)

Simulates 231 rolls of a D4 die N times, where N is one billion by default.
The default can be overriden via the command-line.

This application uses the C++ `subtract_with_carry_engine` PRNG for speed, sampling with `uniform_int_distribution` to emulate a fair D4.

## Binaries
Binaries are provided for 64-bit Linux and Windows under [Releases](https://github.com/LMBernardo/graveler/releases).

## Building
### Linux
Run `g++ -O3 ./graveler.cpp -o graveler`

### Windows
Run `g++.exe -O3 ./graveler.cpp -o graveler.exe`

## Running
Build as above, then run either `./graveler` or `./graveler.exe`.

The program defaults to a billion runs, but also accepts a single command-line argument to override the default.

E.g., run `./graveler 1000` to perform a thousand runs
