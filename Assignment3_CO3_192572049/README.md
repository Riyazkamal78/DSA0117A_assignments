# Assignment 3 CO3

Name: Riyaz Kamal  
Register number: 192572049  

# Results

- Task01: All constructor forms initialize the expected state. Negative copies are rejected; explicit, no-default and reorder diagnostics match their intended cases.
- Task02: Deep copies are independent; pass by value makes one copy, const reference zero. Exact completion output matches. Both shallow variants trigger sanitizer use-after-free.
- Task03: Scope and heap lifetime order matches the prediction. Zero rows is safe. Requested seat-map payloads are 280, 20800 and 2008000 bytes; allocator overhead is excluded. Leak scanning is unavailable as documented.
- Task04: Carry and negative results are correct. One million integer additions are exact; the double test accumulates approximately 1.72e-7 rupees of error.
- Task05: Prefix/postfix predictions, rollovers and invalid indices pass. Timing checksums match at both optimization levels.
- Task06: Self assignment, empty and long messages pass. a+b+c copies zero times with the optimized build and twice with optional elision disabled. Timed checksums match std::string.
- Task07: Series impedance 100+j100 and parallel 80-j40 match hand calculation and std::complex within 1e-9; zero division throws.
