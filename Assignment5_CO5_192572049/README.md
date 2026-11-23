# Assignment 5 CO5 Hospital Billing and Insurance Claims

**Name** Riyaz Kamal  
**Register number** 192572049  

## Build and run

Run these commands from this assignment folder using Bash. The ASan builds use -no-pie for this runtime. LeakSanitizer was unavailable in the execution environment, so leak detection is disabled in these commands. Address checking remains enabled.

```bash
mkdir -p bin
base=(-std=c++17 -Wall -Wextra)
san=(-g -fno-omit-frame-pointer -fsanitize=address -no-pie)
export ASAN_OPTIONS=detect_leaks=0:color=never

for source in A2/predict A2/nonvirtual A2/nonvirtual_larger A5/predict \
    B1/complete B1/validate B1/remove_destructor \
    B2/predict B2/complete B2/throw_copy B2/errors B4/extension; do
    name="${source/\//_}"
    g++ "${base[@]}" "${san[@]}" "$source.cpp" -o "bin/$name"
    "bin/$name"
done

g++ -std=c++17 -Wall -Wextra B1/remove_virtual.cpp -o bin/remove_virtual
g++ -std=c++17 -Wall -Wextra B1/early_binding.cpp -o bin/early_binding
./bin/early_binding

g++ -std=c++17 -Wall -Wextra -O2 B3/bench.cpp -o bin/bench
./bin/bench
g++ -std=c++17 -Wall -Wextra -O2 -g -fno-omit-frame-pointer \
    -fsanitize=address -no-pie B3/bench.cpp -o bin/bench_asan
./bin/bench_asan --small
```

`A2/nonvirtual_larger.cpp` and `B1/remove_destructor.cpp` are expected to exit with an AddressSanitizer diagnostic. `B1/remove_virtual.cpp` is intended to fail to compile. `A2/nonvirtual.cpp` exhibits undefined behavior and happened to exit normally on the measured build. The fixed cases in A2 and A5 require no runtime input. B1, B2 and B4 use the listed embedded inputs. B3 generates every input deterministically.

## Results by task

| Task | Result |
| --- | --- |
| [A1](A1) | Abstract bill hierarchy, per-type formulas, daily total pseudocode and switch comparison. |
| [A2](A2) | Late binding through a non-virtual wrapper, prediction matched, destructor experiment checked with ASan. |
| [A3](A3) | Abstract payer interface, three base policies and the corporate extension. |
| [A4](A4) | Error taxonomy and conditions for throwing or using noexcept. |
| [A5](A5) | Prediction matched and the two guards were destroyed in reverse scope order. |
| [B1](B1) | All 12 payer combinations passed, gross 70700, and dispatch and destruction controls behaved as expected. |
| [B2](B2) | Prediction and completion matched. Fault injection exercised specific and catch-all handlers and demonstrated recovery. |
| [B3](B3) | Ten bills agreed across designs. Logs include five-run median timings and representation sizes. |
| [B4](B4) | Telehealth and corporate coverage worked, all baseline checks passed again, and the address-sanitized recovery run exited successfully. |
