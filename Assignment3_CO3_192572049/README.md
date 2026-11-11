# Assignment 3 CO3

Name: Riyaz Kamal  
Register number: 192572049  
Compiler: g++ (Debian 14.2.0-19) 14.2.0  
Language: C++17

## Evidence and submission

The task folders contain C++ source files, input notes, predictions saved before execution, actual stdout, compiler logs, sanitizer diagnostics and compiler-style PNG views. These images present local executions; they are not online compiler screenshots. Benchmarks use three trials, with the raw repetitions retained and medians in timing_summary.json. Diagrams are exact memory-layout illustrations, not measured pointer addresses.

This preparation uses the existing repository and a new Assignment3_CO3_192572049 folder as requested. The PDF asks for a standalone DSA01-192572049-Assignment3 repository and progressively recorded student work; commits in this session do not demonstrate work across separate days or individual student execution. Review and independently run the programs before submission.

## Compile and run

Run each .cpp separately because each has its own main. From the assignment folder, a normal example is:

    g++ -std=c++17 -Wall -Wextra -O2 Task01/books.cpp -o program
    ./program < Task01/input.txt

All input values are embedded in main; input.txt states that no stdin is needed. commands.txt and each .compiler.txt contain the exact workspace build commands and exit status. Change only the source/output paths as needed on your machine.

## Deliberate diagnostics and variants

- Task01/no_default.cpp intentionally does not compile. Task01/reorder.cpp deliberately triggers -Wreorder.
- Task01/explicit_demo.cpp compiles normally, but intentionally fails with -DEXPLICIT=explicit.
- Task02/shallow.cpp and complete_no_copy.cpp are intentionally broken demonstrations. With AddressSanitizer they report heap-use-after-free. They are not production solutions.
- Task02/complete.cpp is the completed supplied program. prescriptions.cpp instruments safe deep copying, self assignment, argument passing and copy timings; unique_ptr owns its raw allocated string array.
- Task03/leak.cpp omits delete[] with -DLEAK and releases the same buffer without that macro. Its outstanding byte line is derived from the code path, not a leak profiler.
- Task05/benchmark.cpp is run at both -O0 and -O2, three times each. It benchmarks scalar Time increments and integer playlist access as a controlled surrogate.
- Task06/text.cpp is run at -O2, with -O0 -fno-elide-constructors, and under AddressSanitizer. The first measures 100000 independent short concatenations; it does not repeatedly grow one string.

## Sanitizer reproduction and environment limitation

Memory-access checks were built with -O1 -g -fsanitize=address -fno-omit-frame-pointer where shown, and run with ASAN_OPTIONS=detect_leaks=0:halt_on_error=1. Corrected Prescription, Auditorium and Text programs exited zero without invalid-access diagnostics. Broken shallow variants failed as expected. LSAN_OPTIONS=exitcode=23 was used, so deliberate sanitizer failures may exit 23.

A separate leak detector attempt used ASAN_OPTIONS=detect_leaks=1:halt_on_error=1. LeakSanitizer could not operate under the execution environment's ptrace and returned a fatal error, retained in Task03/leak_leak_detection_attempt.output.txt. This is not evidence that a leak was detected or that the fixed program is leak-free. To finish that check on a supported local machine:

    g++ -std=c++17 -Wall -Wextra -O0 -g -fsanitize=address -DLEAK Task03/leak.cpp -o leak
    ASAN_OPTIONS=detect_leaks=1 ./leak
    g++ -std=c++17 -Wall -Wextra -O0 -g -fsanitize=address Task03/leak.cpp -o fixed
    ASAN_OPTIONS=detect_leaks=1 ./fixed

## Results

- Task01: All constructor forms initialize the expected state. Negative copies are rejected; explicit, no-default and reorder diagnostics match their intended cases.
- Task02: Deep copies are independent; pass by value makes one copy, const reference zero. Exact completion output matches. Both shallow variants trigger sanitizer use-after-free.
- Task03: Scope and heap lifetime order matches the prediction. Zero rows is safe. Requested seat-map payloads are 280, 20800 and 2008000 bytes; allocator overhead is excluded. Leak scanning is unavailable as documented.
- Task04: Carry and negative results are correct. One million integer additions are exact; the double test accumulates approximately 1.72e-7 rupees of error.
- Task05: Prefix/postfix predictions, rollovers and invalid indices pass. Timing checksums match at both optimization levels.
- Task06: Self assignment, empty and long messages pass. a+b+c copies zero times with the optimized build and twice with optional elision disabled. Timed checksums match std::string.
- Task07: Series impedance 100+j100 and parallel 80-j40 match hand calculation and std::complex within 1e-9; zero division throws.
