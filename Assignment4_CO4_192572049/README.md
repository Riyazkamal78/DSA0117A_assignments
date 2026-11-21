# Assignment 4 – CO4

**Name:** Riyaz Kamal  
**Register number:** 192572049  

Each task is self-contained. Change to its folder before running the commands. The demonstration data are built into the programs and listed in each `input.txt`. PNGs show the captured results in a compiler-console layout. The corresponding TXT files preserve the full console logs.

## [Task 1 – Payroll](Task01)

The base-pointer payroll loop prints 76000.00, 35500.00 and 16000.00.

```bash
g++ -std=c++17 -Wall -Wextra payroll.cpp -o payroll
./payroll
```

## [Task 2 – University hierarchy](Task02)

The virtual diamond shares one Person; composition has two. Both prediction runs compile. The extra implicit conversion test intentionally fails.

```bash
g++ -std=c++17 -Wall -Wextra hierarchy.cpp -o hierarchy
./hierarchy
g++ -std=c++17 -Wall -Wextra virtual_prediction.cpp -o virtual_prediction
./virtual_prediction
g++ -std=c++17 -Wall -Wextra nonvirtual_prediction.cpp -o nonvirtual_prediction
./nonvirtual_prediction
g++ -std=c++17 -Wall -Wextra ambiguous_conversion.cpp -o ambiguous_conversion
```

## [Task 3 – Construction and member classes](Task03)

Virtual and nonvirtual construction traces show 152 and 208 bytes here. The Student TODO prints the worksheet result and the reordered variant intentionally emits -Wreorder warnings.

```bash
g++ -std=c++17 -Wall -Wextra construction.cpp -o construction
./construction
g++ -std=c++17 -Wall -Wextra construction_nonvirtual.cpp -o construction_nonvirtual
./construction_nonvirtual
g++ -std=c++17 -Wall -Wextra car_prediction.cpp -o car_prediction
./car_prediction
g++ -std=c++17 -Wall -Wextra student_todo.cpp -o student_todo
./student_todo
g++ -std=c++17 -Wall -Wextra student_reorder.cpp -o student_reorder
./student_reorder
```

## [Task 4 – Base pointers and forensics](Task04)

The prediction prints 3.5 4 3.5. ASan diagnoses a new-delete-type-mismatch. The continuation run uses an explicit counter and observes one outstanding history array. The corrected example completes its ASan run.

```bash
g++ -std=c++17 -Wall -Wextra binding_prediction.cpp -o binding_prediction
./binding_prediction
g++ -std=c++17 -Wall -Wextra -g -O0 -fno-omit-frame-pointer -fsanitize=address -no-pie broken.cpp -o broken
ASAN_OPTIONS=detect_leaks=0 ./broken
g++ -std=c++17 -Wall -Wextra -g -O0 -fno-omit-frame-pointer -fsanitize=address -no-pie broken.cpp -o broken
ASAN_OPTIONS=new_delete_type_mismatch=0:detect_leaks=0 ./broken
g++ -std=c++17 -Wall -Wextra -g -O0 -fno-omit-frame-pointer -fsanitize=address -no-pie repaired.cpp -o repaired
ASAN_OPTIONS=detect_leaks=0 ./repaired
```

## [Task 5 – Cart access and this](Task05)

The worksheet result is 324.00, the by-value experiment produces 240.00, and the largest active cart is 427.50.

```bash
g++ -std=c++17 -Wall -Wextra cart_todo.cpp -o cart_todo
./cart_todo
g++ -std=c++17 -Wall -Wextra cart_by_value.cpp -o cart_by_value
./cart_by_value
g++ -std=c++17 -Wall -Wextra cart_search.cpp -o cart_search
./cart_search
```

## [Task 6 – Rental fleet](Task06)

All vehicle types rent and return; the duplicate rent is rejected. C01 incurs a 150.00 late fee and the electric bus uses the same fleet interface.

```bash
g++ -std=c++17 -Wall -Wextra fleet.cpp -o fleet
./fleet
```

## Diagnostic notes

- The intentionally ambiguous conversion in Task 2 exits during compilation. The Student initializer-order variant in Task 3 compiles with warnings.
- The first broken-account run terminates after an AddressSanitizer type-mismatch report. The second suppresses that specific diagnostic so the observed destructor sequence and allocation counter can finish. Both modes run the same intentionally undefined deletion.
- LeakSanitizer is disabled in the commands because it could not run in the execution environment. The continuation uses the allocation counter, and the repaired program was checked with AddressSanitizer.
- Predictions were recorded before running the examples. `␠` in the notes marks a trailing space.
