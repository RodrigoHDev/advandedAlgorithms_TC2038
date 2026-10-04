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

<br>
<div align=center>
<img width="700" height="auto" alt="SituacionProblema1 drawio" src="https://github.com/user-attachments/assets/6fd81457-8841-4873-9c70-f4ea4d8fb31a" />
</div>
<br>

## Testing strategy

The program was tested using different provided versions of mcode and transmission files. In total, 9 mcode files and 6 transmission files were used across multiple test runs, combining them in different pairings to exercise Subchain, Palindrome and Substring against a variety of scenarios rather than a single fixed input.

| Test No. | Transmission files | Mcode files | Outcome 1st Part | Outcome 2nd Part | Outcome 3rd Part |
|---|---|---|---|---|---|
| 1 | transmission1.txt transmission2.txt | mcode1.txt mcode2.txt mcode3.txt | <img width="585" height="605" alt="Screenshot 2026-09-23 224601" src="https://github.com/user-attachments/assets/b66ca876-ec71-46bf-9159-092df3070446" /> | <img width="592" height="247" alt="Screenshot 2026-09-23 224614" src="https://github.com/user-attachments/assets/0aef1d6a-27ba-4bc3-a9de-36f572205da1" /> | <img width="590" height="83" alt="Screenshot 2026-09-23 224625" src="https://github.com/user-attachments/assets/da0c73fd-3f95-4724-abc1-9a26f76e34be" /> |
| 2 | transmission11.txt transmission12.txt | mcode11.txt mcode12.txt mcode13.txt | <img width="592" height="508" alt="Screenshot 2026-09-23 224730" src="https://github.com/user-attachments/assets/d71685c6-a84d-42c1-a6fa-ffbe6d18282b" /> | <img width="593" height="245" alt="Screenshot 2026-09-23 224741" src="https://github.com/user-attachments/assets/069af415-f31e-42d8-8b54-1e9d461c897b" /> | <img width="588" height="81" alt="Screenshot 2026-09-23 224751" src="https://github.com/user-attachments/assets/3532bd55-90c3-476e-bb68-1de304c9a1e2" /> |
| 3 | transmission21.txt transmission22.txt | mcode21.txt mcode22.txt mcode23.txt | <img width="587" height="496" alt="Screenshot 2026-09-23 224833" src="https://github.com/user-attachments/assets/eb816132-8ec0-4de8-b315-b5c2c802fe10" /> | <img width="591" height="247" alt="Screenshot 2026-09-23 224844" src="https://github.com/user-attachments/assets/eb0c8e6b-9adc-4733-9178-57e528568c1d" /> | <img width="593" height="80" alt="Screenshot 2026-09-23 224853" src="https://github.com/user-attachments/assets/4e3adbe2-4014-437f-b089-ca91b0f84aa7" /> |
| 4 | transmission31.txt transmission32.txt | mcode31.txt mcode32.txt mcode33.txt | <img width="587" height="497" alt="Screenshot 2026-09-23 224943" src="https://github.com/user-attachments/assets/fb35be68-28f7-4f71-a7f5-fcaedecf564a" /> | <img width="590" height="213" alt="Screenshot 2026-09-23 224950" src="https://github.com/user-attachments/assets/ce7ad102-1b5d-4ef4-81b9-f731a9da9829" /> | <img width="592" height="85" alt="Screenshot 2026-09-23 225000" src="https://github.com/user-attachments/assets/8947c68a-40b5-477c-937b-bd8bd0db58bb" /> |
| 5 | transmission41.txt transmission42.txt | mcode41.txt mcode42.txt mcode43.txt | <img width="587" height="500" alt="Screenshot 2026-09-23 225040" src="https://github.com/user-attachments/assets/2b3dd125-a41e-44b5-a7ea-c41c50467c49" /> | <img width="590" height="247" alt="Screenshot 2026-09-23 225050" src="https://github.com/user-attachments/assets/e11050a6-2325-402f-ac43-3024e57fe8bc" /> | <img width="592" height="76" alt="Screenshot 2026-09-23 225058" src="https://github.com/user-attachments/assets/b276764d-9547-4b09-909b-c14ac70d0bb9" /> |
| 6 | transmission51.txt transmission52.txt | mcode51.txt mcode52.txt mcode53.txt | <img width="588" height="500" alt="Screenshot 2026-09-23 225133" src="https://github.com/user-attachments/assets/a1cd848c-1152-431e-afc1-264216e2dcf4" /> | <img width="592" height="247" alt="Screenshot 2026-09-23 225144" src="https://github.com/user-attachments/assets/3acfb3e1-87bb-4e3b-ab1a-ba077a178697" /> | <img width="596" height="83" alt="Screenshot 2026-09-23 225153" src="https://github.com/user-attachments/assets/52a2de43-0467-48fd-812b-bf59ff9dbc91" /> |
| 7 | transmission61.txt transmission62.txt | mcode61.txt mcode62.txt mcode63.txt | <img width="591" height="500" alt="Screenshot 2026-09-23 225228" src="https://github.com/user-attachments/assets/b506e583-4f8f-40e5-a2d0-881a5bcef46d" /> | <img width="588" height="250" alt="Screenshot 2026-09-23 225238" src="https://github.com/user-attachments/assets/27a09cdf-102c-4813-9e12-89884d1eddcf" /> | <img width="588" height="82" alt="Screenshot 2026-09-23 225247" src="https://github.com/user-attachments/assets/c846c1be-bc50-41d4-b153-a6db80e674e1" /> |
| 8 | transmission1.txt transmissionempty.txt | mcodeempty.txt mcode2.txt mcode3.txt | <img width="592" height="496" alt="Screenshot 2026-09-23 225330" src="https://github.com/user-attachments/assets/19116e72-4499-40ca-b20a-bc4aed347dd4" /> | <img width="595" height="227" alt="Screenshot 2026-09-23 225337" src="https://github.com/user-attachments/assets/dd4610f6-a037-4106-9770-a6c06bbb9404" /> | <img width="592" height="82" alt="Screenshot 2026-09-23 225346" src="https://github.com/user-attachments/assets/f8e6e191-f223-4c0e-950b-5f4c4274c2e3" /> |


Beyond the provided cases, additional modified test files were built specifically to stress edge cases that are not guaranteed to appear in the standard test set:

- Empty transmission: verifies that Subchain::search() and Palindrome::palindrome() handle a zero-length transmission without crashing or reading out of bounds, and that they report the expected "not found" / "no palindrome" behavior instead of a false positive.
- Empty mcode: verifies that Subchain::search() treats an empty pattern as a non-match (rather than matching at every position, which would be the naive behavior of some substring-search implementations) and returns an empty result.
- mcode longer than the transmission: verifies that Subchain::search() correctly returns no occurrences when M > T, since a pattern longer than the text it is being searched in can never be fully contained.

For every test run, in addition to comparing the program's output against the expected true/false and position values, the reported palindrome positions were manually checked at the reported start/end indices and confirming by inspection that it reads the same forwards and backwards. This manual verification step was particularly important for Palindrome, since an off-by-one error in the index mapping from the expanded (Manacher) representation back to the original string would still produce a palindrome, just not necessarily the longest one — a class of bug that a purely automated true/false check would not catch.

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

## Test Cases



## General conclusion

The current document exposes the logic, decisions and alteratives to the program developed within the context of detecting malicious code inside data transmissions between devices, based on the algorithms learned in the subject *Advanced Algorithms Analysis and Design*. The reflected three classes and a main file are the result of identifying the requirements to solve (very clearly depicted in the problem descrciption) and based on the acquired knowledge and consulting notes and presentations, implementing a total of three algorithms for string manipulation: `Z-Algorithm`, `Manacher's Algorithm`, and application of `Dynamic Programming`.

However, the conclusion we present here are not based on the difficulty of the implementation, but about the **importance of choosing the right algorithm** not only based on the outcome you require or the complexity in the worst case scenario, but finding a balance by understanding the dimensions of its usage, the required implementation or preparation that it may or may not require, and the data it will process, as these three factors make the difference between overworking and delivering just the right result for the ocassion.

Although we are aware that the implemented algotithms are not the best of their class inside string manipulation, we selected these by finding the balance between the relatively limited and small data to process, as well as an easy implementation that does not consume excessive memory for storage or overcomplicates the process; finally delivering the correct outputs on time and place.

## References
 
Fuentes Valdéz, R. (n.d.). *M2_02 Strings: Z y Manacher* [Class slides]. Tecnológico de Monterrey, Campus Querétaro, TC2038 Análisis y diseño de Algoritmos A.
 
Fuentes Valdéz, R. (n.d.). *M2_05 Strings: Subcadenas común y LCS* [Class slides]. Tecnológico de Monterrey, Campus Querétaro, TC2038 Análisis y diseño de Algoritmos A.
 
GeeksforGeeks. (n.d.). *Z algorithm (linear time pattern searching algorithm)*. GeeksforGeeks. https://www.geeksforgeeks.org/dsa/z-algorithm-linear-time-pattern-searching-algorithm/
 
GeeksforGeeks. (n.d.). *Manacher's algorithm – linear time longest palindromic substring – Part 1*. GeeksforGeeks. https://www.geeksforgeeks.org/dsa/manachers-algorithm-linear-time-longest-palindromic-substring-part-1/
 
