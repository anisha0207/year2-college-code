# Hashset Analysis

## About
This code consists of hashset implmentation where it uses std::unorder_set from the standard library. The benchmark contains performance of how hashset is carried through load factors from insertioon, contains, and remove. Two charts are displayed to capture this performance.

## Steps
1. Data structures are implemented through Hashset implementation with the control std::unordered_set
2. Load Factors with thresholds 20%, 70%, 120%
3. The main operations used insert, contains, remove
4. Element of 1K, 10K, 100K, 1M
5. There are experiments run with overal total of 72.
All the results were stored in a CSV file (performance_results.csv) with columns: DataStructure, LoadFactor, Operation, Elements, and TimeMS.

## Charts
Two charts were created using Python with a knowledge of engineering 10 concepts. The charts included the following:
1. Operation Performance vs. Element Count (Line Chart):
X-axis: Number of elements (log scale).
Y-axis: Average execution time (ms).
Lines represent operations (Insert, Contains, Remove) for both data structures.
2. Load Factor Impact on Performance (Bar Chart):
X-axis: Load factor thresholds (20%, 70%, 120%).
Y-axis: Execution time (ms).
Bars represent operations grouped by data structure.

## Findings
1. Insert:
The custom HashSet performs well for small element counts but becomes slower than std::unordered_set as N increases.
Frequent rehashing in the custom implementation significantly impacts performance at low load factors (20%).
2. Contains:
Both data structures exhibit linear growth (O(N)) in lookup time as N increases.
The standard library's optimized hashing reduces collisions resulting in slightly faster lookups.
3. Remove:
Similar trends to insertion; however, deletion in the custom implementation incurs additional overhead due to manual pointer updates.

At low load factors (20%), rehashing in the custom HashSet results in significant performance degradation for all operations.
The default load factor threshold (70%) provides balanced performance across all operations.
At high load factors (120%), retrieval times increase due to larger bucket sizes and more collisions.

## Conclusion
This benchmarking tool allows for hash set implementation and the unordered set. Through frequent rehashing, the performance isn't allocated the best when doen manually. Although, when having std::unordered_set leverages good algorithms and memory management.
The findings highlights the importance of hash functions, load factors, and memory allocation to examine the hash set performance. 

## Files Implemented
src/lib.cpp: Source code for custom HashSet implementation.
tests/benchmarker.cpp: Benchmarking code that generates CSV results.
plot_charts.py: Python script for generating visualizations.
performance_results.csv: Benchmark results in CSV format.
Charts:
operation_performance.png: Line chart showing operation performance vs element count.
load_factor_impact.png: Bar chart showing load factor impact on performance.

## Sources
https://stackoverflow.com/questions/131803/unsigned-int-vs-size-t
https://stackoverflow.com/questions/25275516/should-you-pass-member-variables-within-member-functions (used size_t bucket_count; size_t element_count; unsigned int load_threshold; unsigned int load_factor; void rehash(size_t new_size);)

https://www.w3schools.com/cpp/cpp_structs.asp
https://www.reddit.com/r/cs50/comments/ggvgvm/pset5_djb2_hash_function/ (used for unsigned long prehash)

https://stackoverflow.com/questions/44097902/shared-libraries-c-makefile
https://www.cs.colby.edu/maxwell/courses/tutorials/maketutor/
https://gist.github.com/fbartolic/6b3c6ea3c84647b795f24a245e7959f5
https://www.softwaretestinghelp.com/cpp-makefile-tutorial/ (used for MakeFile)

https://www.geeksforgeeks.org/c-program-hashing-chaining/
https://www.geeksforgeeks.org/implementation-of-hash-table-in-c-using-separate-chaining/
(insert () )

https://medium.com/%40omerhalidcinar/building-your-own-hashmap-in-c-open-addressing-separate-chaining-implementations-ead22ca955c2 (remove () )

https://www.geeksforgeeks.org/setsize-c-stl/ (count() )

https://www.geeksforgeeks.org/load-factor-and-rehashing/ (load () )

https://www.geeksforgeeks.org/load-factor-and-rehashing/?utm_source=chatgpt.com updateLoadFactor()

https://stackoverflow.com/questions/20039265/clearing-a-hash-table-in-c clear()

Homework 2: main.c format is used for main.cpp
https://embeddedartistry.com/blog/2017/04/24/thoughts-on-header-file-extensions-h-vs-hpp/ (understanding .hpp and .h)
https://github.com/bbondy/hashset-cpp
https://www.tutorialspoint.com/how-to-process-strings-using-std-istringstream (iss)

Added 20 additional test cases using ChatGPT







Benchmarker.cpp sources:
<iostream> https://en.cppreference.com/w/cpp/header/iostream
<unordered_set>https://en.cppreference.com/w/cpp/container/unordered_set
<vector>https://en.cppreference.com/w/cpp/container/vector
<chrono>https://en.cppreference.com/w/cpp/header/chrono
<fstream>https://en.cppreference.com/w/cpp/header/fstream
<iomanip> https://en.cppreference.com/w/cpp/header/iomanip
<sys/resource.h> https://man7.org/linux/man-pages/man2/getrusage.2.html

Python File:
https://datacarpentry.github.io/python-ecology-lesson/02-starting-with-data.html
https://plotly.com/python/plot-data-from-csv/
https://stackoverflow.com/questions/70007610/how-do-i-make-a-graph-diagram-from-a-csv-file-in-python
Engineering 10 knowledge

generateCSV():
https://github.com/al-eax/CSVWriter
https://products.aspose.com/cells/cpp/create/csv/
https://cplusplus.com/forum/beginner/135603/
Lin.cpp sources:
https://github.com/bbondy/hashset-cpp
https://cppbyexample.com/hash_set.html
https://learn.microsoft.com/en-us/cpp/standard-library/hash-set-class?view=msvc-170 (hashset class)
https://blog.devgenius.io/how-to-implement-a-hashset-data-structure-in-c-using-a-hash-table-approach-9581e51da8df (unordered_set)
https://stackoverflow.com/questions/49101721/hash-set-implementation-using-dynamically-allocated-array-of-implementation-of-l (linkedlist)

Hashset.hpp sources:
https://grapeprogrammer.com/const_string_reference_cpp/ 
(changed some of the function parameters to const std :: string&; which influenced lib.c when calling onto the function)

Markdown File:
https://medium.com/@saumya.ranjan/how-to-write-a-readme-md-file-markdown-file-20cb7cbcd6f
