# Situation Problem 1 — Analysis and Design of Advanced Algorithms

**Team 01**
- Rodrigo Alejandro Hurtado Cortes — A01713854
- Alexis Yaocalli Berthou Haas — A01713458

---

## What needs to be done?

The program must read 5 fixed-name text files (never requested from the user), made up exclusively of hexadecimal characters (`0-9`, `A-F`) and line breaks:

- `transmission1.txt`, `transmission2.txt`: represent data transmissions between two devices.
- `mcode1.txt`, `mcode2.txt`, `mcode3.txt`: represent fragments of malicious code that might be hidden inside a transmission.

Based on these files, the program must solve three independent tasks:

1. **Subchain search (Part 1):** determine whether each `mcodeY.txt` is contained inside each `transmissionX.txt`, and if so, report the position (or positions) where each match starts.
2. **Mirrored code / palindromes (Part 2):** since malicious code is always "mirrored" (a character-level palindrome), find the longest palindrome contained in each transmission file and report its start and end position.
3. **Similarity between transmissions (Part 3):** find the longest common substring between `transmission1.txt` and `transmission2.txt`, reporting the start and end position at which it appears within the first file.

The program takes no user input: the 5 files must exist in the execution path with their exact fixed names. The output must strictly follow the requested format (`true`/`false` + position, or pairs of integers depending on the part).

## Basic example of how it works

Assuming a `transmission1.txt` with content `1A2B3C4D3C2B1A` and an `mcode1.txt` with content `3C4D3C`, running the program would produce something like:

```
P A R T 1: S U B C H A I N   T E S T

T R A N S M I S S I O N 1

mcode 1
(true) Initial position: 5 Final position: 10
```

Then, if the longest palindrome inside that same transmission is `2B3C4D3C2B` (positions 3 to 12), Part 2 would report:

```
P A R T  2:  P A L I N D R O M E   T E S T

T R A N S M I S S I O N 1

Longest Polindrom at start: 3 end: 12
```

And finally, comparing `transmission1.txt` against `transmission2.txt`, Part 3 would report the position inside `transmission1.txt` where the longest common substring between both transmissions starts and ends.

## What has been developed

### In general terms

A C++ program was implemented, split into a `main.cpp` file and three helper classes (`Subchain`, `Palindrome`, `Substring`), each encapsulating the algorithm for one of the three parts of the problem. `main.cpp` is only responsible for:

1. Reading the 5 fixed files through the helper function `getFile()`, discarding line breaks so each file is treated as a single continuous string.
2. Invoking the corresponding algorithm for each part over the transmissions and malicious code fragments.
3. Displaying the results in the format required by the specification.

This separation lets each class own only the algorithmic logic (it receives strings and returns positions), while file reading and output formatting stay isolated inside `main.cpp`.

### Symbology

The complexity analysis below (and the comments in the source files) uses the following notation consistently:

| Symbol | Meaning |
|---|---|
| `t` | Number of transmission files (2 in this problem: `transmission1.txt`, `transmission2.txt`). |
| `m` | Number of malicious code files (3 in this problem: `mcode1.txt`, `mcode2.txt`, `mcode3.txt`). |
| `T` | Length, in characters, of a transmission file (line breaks excluded). |
| `M` | Length, in characters, of a malicious code file (line breaks excluded). |
| `T1` | Length of `transmission1.txt`, used specifically in Part 3 (longest common substring). |
| `T2` | Length of `transmission2.txt`, used specifically in Part 3 (longest common substring). |
| `n` | Length of the specific string an algorithm operates on; its meaning depends on context: in the Z-algorithm it is the length of the concatenated string `mcode + '$' + transmission` (so `n = M + T + 1`), and in Manacher's algorithm it is the length of the expanded string (`n = 2·length + 1`, where `length` is the length of the original transmission). |
| `k` | Number of matches (positions) found — e.g., the number of times an `mcode` occurs inside a `transmission`. |

These symbols are reused throughout the "What has been developed" section so that every complexity expression can be traced back to a concrete, measurable quantity from the input files.

### Part 1 — Subchain search: Z-Algorithm (`Subchain` class)

To determine whether an `mcode` is contained inside a `transmission`, and at which position(s), the **Z-algorithm** was chosen. The idea is to concatenate `mcode + '$' + transmission` (using `$` as a separator, since it is not a valid character within the problem's alphabet) and compute the Z-array of that combined string. Each position `i` of the Z-array holds the length of the common prefix between the full string and the suffix starting at `i`; when that value matches the length of the `mcode`, a full occurrence has been found in the transmission.

- **Complexity of `zFunction()`:** O(n), where n is the length of the concatenated string, since each character is compared at most twice thanks to the reuse of the Z-box.
- **Complexity of `search()`:** O(T + M), where T is the length of the transmission and M the length of the mcode (building the string, computing the Z-array and the final scan are all linear).
- **Overall complexity of the program for this part:** O(t · m · (T + M)), since a search is run for every combination of transmission and mcode.
- **Space:** O(T + M + k), where k is the number of positions found.

### Part 2 — Mirrored code: Manacher's Algorithm (`Palindrome` class)

To find the longest palindrome inside each transmission, **Manacher's algorithm** was implemented. The original string is transformed into an expanded version by inserting a separator (`|`) before, between and after every character, so that odd- and even-length palindromes are handled uniformly. Then, for every position of the expanded string, the radius of the longest palindrome centered there is computed, reusing previous calculations by tracking a center and a right boundary (avoiding re-comparing characters that were already verified).

- **Complexity of `searchPalindrome()` (core of Manacher's algorithm):** O(n), where n is the length of the expanded string (2·length+1).
- **Complexity of `palindrome()`:** O(n), since expanding the string, running Manacher's algorithm and finding the maximum are all linear operations.
- **Overall complexity of the program for this part:** O(t · T), one run per transmission.
- **Space:** O(n), for the expanded string and the radius array.

### Part 3 — Similarity between transmissions: Dynamic Programming (`Substring` class)

To find the longest common substring between the two transmissions, the classic **dynamic programming** approach was implemented over a matrix `M[i][j]`, where each cell stores the length of the common substring ending at `transmission1[i-1]` and `transmission2[j-1]`. If the characters match, the cell is built from the upper-left diagonal (`M[i-1][j-1] + 1`); otherwise it resets to 0. The maximum length observed and the position where it ends are tracked, in order to reconstruct the start and end position in `transmission1` at the end.

- **Time complexity:** O(T1 · T2), where T1 and T2 are the lengths of the two transmissions, since a full matrix of that size is filled.
- **Space complexity:** O(T1 · T2), for the full stored matrix.

## Additional options

### Subchain (subchain search)

- **Brute force:** compare the mcode against every position of the transmission. Worst-case complexity O(T · M). It is simpler to implement, but considerably less efficient than the Z-algorithm when there are many repeated characters in a reduced alphabet (hexadecimal), as is the case in this problem.
- **KMP (Knuth-Morris-Pratt):** builds a failure table over the pattern and achieves O(T + M), the same asymptotic complexity as the Z-algorithm. It would be an equally efficient alternative; choosing between the two is mostly a matter of implementation preference, since KMP requires explicitly handling the failure table while the Z-algorithm solves the problem with a single concatenation and one array.
- **Rabin-Karp:** uses polynomial hashing to compare the pattern against windows of the transmission. On average it is O(T + M), but in the worst case (many hash collisions) it can degrade to O(T · M). It would be reasonable for a single-pattern search but is less predictable than Z or KMP due to the collision risk.
- **Aho-Corasick automaton:** since there are multiple patterns (3 mcodes) being matched against the same transmissions, this algorithm would allow building a single automaton with all 3 mcodes and scanning each transmission only once, achieving O(M_total + T + k) per transmission instead of repeating the search mcode by mcode. It would be more efficient overall if the number of mcodes grew, at the cost of a more complex implementation.

### Palindrome (mirrored code)

- **Brute force (expand around each center):** for each of the 2n-1 possible centers, expand outward while characters match. Worst-case complexity O(n²) (for example, a string of a single repeated character). It is much simpler to code, but noticeably less efficient than Manacher's algorithm for large transmissions.
- **Dynamic programming:** build a boolean table `isPalindrome[i][j]` indicating whether the substring between i and j is a palindrome, filling it by increasing length. Also O(n²) in time and space. It is easier to reason about and extend (for example, to count all palindromes), but inefficient in memory compared to Manacher's algorithm, which only needs O(n) extra space.
- **Eertree (palindromic tree):** a structure that maintains all distinct palindromes of a string in O(n) total time. It would be useful if additional information were needed (such as counting distinct palindromes), but for the specific goal of finding only the longest one, Manacher's algorithm is more direct and has lower memory overhead.

### Substring (similarity between transmissions)

- **Brute force:** compare every possible pair of substrings between both transmissions. Worst-case complexity around O(T1² · T2²), clearly impractical for large files.
- **Suffix Array + LCP (Longest Common Prefix):** concatenate both transmissions with a unique separator, build the suffix array (O(n log n) with efficient algorithms) and the LCP array (O(n) with Kasai's algorithm), then scan the LCP array looking for the maximum value between suffixes coming from different transmissions. Overall complexity O(n log n), which is asymptotically better than the O(T1 · T2) dynamic programming solution when transmissions are very large, although its implementation is considerably more complex.
- **Generalized Suffix Automaton / Suffix Tree:** allows solving the problem in O(T1 + T2), making it the asymptotically most efficient option of all. The cost is a significantly more elaborate implementation (building a generalized suffix automaton and traversing it to find the "deepest" state reachable from both strings). For the expected size of the transmissions in this problem, the implemented O(T1 · T2) dynamic programming solution is simpler to verify and debug, at the cost of scaling worse if the test files were to become very large.

## General conclusion

In the imaginary context of the problem —detecting "mirrored" malicious code hidden inside data transmissions between devices— the developed solution prioritizes algorithms with linear (or near-linear) complexity in each individual stage: the Z-algorithm resolves the search for each mcode in O(T + M) time, and Manacher's algorithm finds the longest palindrome of each transmission in O(T). This is especially relevant given that a real intrusion-detection system would need to analyze potentially very large transmissions close to real time, so avoiding quadratic algorithms in these two stages represents a significant advantage.

The only stage that retains quadratic complexity, O(T1 · T2), is the similarity comparison between the two full transmissions (Part 3). This is acceptable given that this comparison is performed only once between exactly two files (unlike the other parts, which repeat for every transmission-mcode combination), and the simplicity of the dynamic programming approach makes it easier to verify and maintain. However, as detailed in the additional options section, if the volume of transmissions to compare grew — for example, if a real-world scenario required comparing many transmissions against each other to detect repeated suspicious behavior patterns — it would be worth migrating this stage toward a solution based on suffix arrays or suffix automata, trading implementation simplicity for scalability.

Overall, the design balances the asymptotic efficiency needed for a real-time threat-detection scenario with code clarity and maintainability, leaving the door open for further optimizations (such as Aho-Corasick for multiple patterns, or suffix structures for large-scale comparisons) should the data volume of the problem grow beyond what was considered in this situation problem.
