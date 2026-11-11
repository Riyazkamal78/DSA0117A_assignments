# Assignment 2 CO2

Name: Riyaz Kamal  
Register number: 192572049  

## Execution evidence

These programs were executed locally with g++; PNG files show the actual captured results in a compiler-style presentation. They are not captures from an online compiler. Predictions were recorded before the first execution. The local reference example is compiled to record its warning; its dangling reference is never used. ambiguous.cpp is an intentional compile-failure diagnostic.

The PDF requests a standalone repository named DSA01-192572049-Assignment2. This submission instead uses the existing repository and a new Assignment2_CO2_192572049 folder as requested. Task commits were made during this preparation session; they do not represent work on separate days. Faculty approval of these submission differences may be needed. No faculty collaborator was added.

## Build and run

From this assignment folder, for each source use:

    g++ -std=c++17 -Wall -Wextra -O2 Task01/concessions.cpp -o program
    ./program < Task01/input.txt

Replace the source path with any program listed below. Each program has its own main and is compiled separately. input.txt records that deterministic test data are embedded in the program; no stdin values are required. For Task06/benchmark.cpp run both -O0 and -O2. Run benchmark executables three times; run2 and run3 transcripts and timing_summary.txt retain repetitions. Commands in *.compiler.txt record the exact workspace compile commands used. The -o path can be changed for your machine. PNGs are the output evidence, *.output.txt the raw stdout, and *.compiler.txt the compiler diagnostics.

## Task results

### Task 01 Call by Value Reference and Pointer

- Task01/complete.cpp: Executed successfully.
- Task01/concessions.cpp: Executed successfully.
- Task01/prediction.cpp: Executed successfully.

### Task 02 Return by Reference and Its Dangers

- Task02/dangling_warning.cpp: Warning captured; unsafe function not called.
- Task02/marks.cpp: Executed successfully.

### Task 03 Default Arguments and Function Overloading

- Task03/ambiguous.cpp: Expected ambiguous-call compilation error.
- Task03/courier.cpp: Executed successfully.

### Task 04 Savings Account with a Transaction Array

- Task04/savings.cpp: Executed successfully.

### Task 05 Static Members and Arrays of Objects

- Task05/complete.cpp: Executed successfully.
- Task05/function_twice.cpp: Executed successfully.
- Task05/prediction.cpp: Executed successfully.

### Task 06 Friend Functions and Inline Functions

- Task06/benchmark.cpp: Executed successfully.
- Task06/complete.cpp: Executed successfully.
- Task06/correctness.cpp: Executed successfully.

### Task 07 Diagnose Redesign and Validate

- Task07/redesign.cpp: Executed successfully.

Q7: all retained sequences matched. At k=1000 element writes fell from 199001000 to 200000, a 99.8995% reduction. Timings are environment dependent. See timing_summary.txt and the report for explanations and measured comparisons.
