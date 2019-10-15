# cpp-interfaces
Results on my i7-4870HQ with Clang v. 10.0.1:

### Run 1
```
Managed 128 trials in 156248.858536ms

Average   263.32ms (sd 7.56ms) for test [V-Table Interface on Heap]
Average   261.36ms (sd 8.11ms) for test [Cpp Interface on Heap]
Average   251.91ms (sd 7.87ms) for test [V-Table Interface on Stack]
Average    87.10ms (sd 2.85ms) for test [Static Circle on Stack]
Average    86.90ms (sd 2.44ms) for test [Static Circle on Heap]
Average    86.88ms (sd 2.79ms) for test [Cpp Interface on Stack]
Average    86.88ms (sd 2.76ms) for test [Interface on Heap]
Average    86.79ms (sd 2.52ms) for test [Interface on Stack]
```

### Run 2
```
Managed 128 trials in 154688.150240ms

Average   260.55ms (sd 4.29ms) for test [V-Table Interface on Heap]
Average   258.26ms (sd 4.27ms) for test [Cpp Interface on Heap]
Average   249.46ms (sd 4.95ms) for test [V-Table Interface on Stack]
Average    86.28ms (sd 1.69ms) for test [Interface on Stack]
Average    86.16ms (sd 1.53ms) for test [Cpp Interface on Stack]
Average    86.13ms (sd 1.55ms) for test [Static Circle on Stack]
Average    86.08ms (sd 1.55ms) for test [Static Circle on Heap]
Average    86.08ms (sd 1.50ms) for test [Interface on Heap]
```
