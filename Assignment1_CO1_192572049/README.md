# Assignment 1 CO1 Metro Smart Card Fare System

Name: Riyaz Kamal  
Register number: 192572049  
Repository requested: https://github.com/Riyazkamal78/DSA0117A_assignments  
Compiler used: g++ (Debian 14.2.0-19) 14.2.0  
Standard: C++17

## Contents and execution

This is a new self-contained folder. It does not depend on or replace existing repository files.
Run commands from this folder. For each N from 01 through 12:

```bash
cd Task01
g++ -std=c++17 -Wall -Wextra main.cpp -o main
./main
cd ..
```

Replace Task01 with the required task. Task08 reads its supplied input:

```bash
cd Task08
g++ -std=c++17 -Wall -Wextra main.cpp -o main
./main < input.txt
cd ..
```

Task11 must run from its folder so it can read fares.txt. Other programs use deterministic data
in source and do not read standard input. Their input.txt explains this.
Auxiliary runnable files use the same command with the file stem substituted:
Task04/explicit.cpp, Task05/peak_table.cpp and Task06/boundaries.cpp.
Task04/narrowing.cpp intentionally fails to compile to demonstrate brace narrowing protection.
Task04/main.cpp intentionally emits -Wsign-compare; other runnable files compiled without
-Wall/-Wextra warnings. Common/metro.hpp is shared by the programs.

## Assumptions

Money is signed long long integer paise; reserve is Rs 50 after every accepted trip.
Student concession is 10%, senior concession 20% (rates unspecified in the brief).
Concessions apply after time adjustment, rounded half up to the nearest paisa per stage.
Peak periods are [08:00,10:00) and [17:00,20:00); integer hours represent these boundaries.
Weekend discount replaces peak surcharge. A monthly pass is valid for its YYYYMM month and
only when every traversed zone is among its two chosen zones. Zones are radially ordered;
travel between nonadjacent zones crosses intervening zones. The Rs 50 reserve also applies
when a pass makes the fare zero. Pass purchase pricing is outside the assignment.

## Results

| Task | Result |
|---|---|
|01|Student outer peak quote Rs 32.40; balance Rs 67.60|
|02|Central fare and reserve constants printed|
|03|Measured type sizes, binary precision difference and int range limit|
|04|Prediction matched; signed comparison warning explained; explicit version run|
|05|Prediction matched; all 24 lookup hours agree with expressions|
|06|Given expected output matched; 10:00 and 20:00 off peak|
|07|Procedural and encapsulated deductions agree; public data can break reserve|
|08|Recovered from abc, 25 and 8x; accepted 10; fare Rs 20.00|
|09|24 checks passed; five-trip total 9880 paise|
|10|Measured all three scales; int insufficient for network total|
|11|Configuration loaded and validated at startup|
|12|11 revised design checks passed|

## Evidence

Each task contains source, source image panels, actual output text, output images and compile logs.
Images use a compiler-style layout rendered from actual local g++ runs. They are not screenshots
from a third-party online compiler. Raw text is included for independent verification.
Task04 and Task05 predictions were recorded before executing the programs during preparation;
they do not attest to the student's own prior prediction or execution. Timings vary between runs.

## Submission conditions to check

The PDF specifies a repository named DSA01-192572049-Assignment1. The supplied repository has
a different name. Confirm faculty acceptance of the supplied repository name.
The PDF also requires progressive work over time and at least one commit per task. No fabricated
or backdated development history is included. Uploading this folder in one commit will not satisfy
that condition. Review, execute, understand and submit the work according to the faculty's rules.
The report and source are prepared; GitHub upload requires an authenticated session.
