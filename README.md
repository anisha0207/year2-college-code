## CSE109 - Systems Software - Spring 2025

# Recitation 5 - Benchmarking a Linked List in C++**

**Objective:**

The goal of this assignment is to analyze the performance of a linked list in C++ using a custom benchmarking tool. Students will compare their previously implemented singly linked list with the built-in `std::list` to measure the efficiency of various operations, such as insertion, deletion, and lookup.

---

### **Benchmarking Setup**

Modify your existing benchmarking code to include a comparison with `std::list`. Specifically, you need to:

1. **Include the Built-in List**
   - Ensure that your benchmarking file includes `#include <list>`.
   - Declare a `std::list<int>` alongside your custom linked list.

2. **Modify Insertion Benchmarking**
   - Compare the insertion time of your custom linked list with `std::list`.
   - Use `push_back()` for `std::list` and your own insertion method for the custom list.

3. **Modify Lookup Benchmarking**
   - Use `std::find(builtInList.begin(), builtInList.end(), val);` for `std::list`.
   - Compare it with your linked list's lookup function.

4. **Modify Deletion Benchmarking**
   - Use `builtInList.remove(val);` for `std::list`.
   - Compare it with your linked list's delete function.

5. **Measure Memory Usage**
   - Ensure that memory usage is measured before and after insertions.
   
6. **Update Output Results**
   - Clearly display insertion, lookup, and deletion times for both implementations.
   - Include memory usage statistics.

---

### **Running and Analyzing Results**
1. Compile the program:
   ```sh
   g++ -std=c++17 -o benchmark benchmark.cpp LinkedList.cpp
   ```
2. Run the benchmark multiple times with different values of `N` (e.g., `N = 10^3, 10^4, 10^5, 10^6`).
   ```sh
   ./benchmark
   ```
3. Observe and record the time taken for insertion, lookup, and deletion as `N` increases.
   - Note how the lookup time grows as `N` increases.
   - Compare the growth rate between the custom linked list and `std::list`.
   - Evaluate memory usage before and after insertions.

---

### **Deliverables**

- A table and assoicated chart recording benchmark results for different values of `N`, showing trends in insertion times.

---

### **Questions to Answer:**

1. How does the insertion time of your custom linked list compare to `std::list`? Why do you think this is the case?
2. What trend do you observe in lookup times as `N` increases? How does this compare between your linked list and `std::list`?
3. How does deletion performance compare between your implementation and `std::list`?
4. What impact does increasing `N` have on memory usage? Which implementation uses more memory, and why?
5. If you were to optimize your linked list, what changes would you consider making to improve performance?