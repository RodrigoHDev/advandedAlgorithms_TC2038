/*
 * Title: palindrome.h
 *
 * Description:
 * Finds the longest palindromic substring of a string in linear time using
 * Manacher's algorithm. The input is expanded with a separator character
 * between every character so odd and even length palindromes are handled
 * the same way. The result is the start and end index (inclusive) of the
 * palindrome in the original string.
 *
 * Implementation for the subject- Analysis and Design of Advanced
 * Algorithms
 *
 * Author: Alexis Yaocalli Berthou Haas - A01713458 & Rodrigo Alejandro Hurtado Cortes - A01713854
 * Date: Septiembre 21, 2026
 */

#ifndef PALINDROME_H
#define PALINDROME_H

#include <algorithm>
#include <string>
#include <tuple>
#include <vector>

using namespace std;

class Palindrome {
public:
	/*
	 * palindrome()
      * Finds the longest palindrome inside a string.
	 * Time complexity: O(n), where n is the length of transmission.
	 *
	 * Parameters:
	 *   transmission - original string to analyze (not modified).
	 *
	 * Returns:
	 *   tuple<int,int> with the start and end index (inclusive) of the
	 *   longest palindrome in transmission. If several palindromes share the
	 *   maximum length, the last one is returned. For an empty string it
	 *   returns (0, -1).
	 */
	tuple<int,int> palindrome(const string &transmission) const {
		string chain = expandedChain(transmission);
		vector<int> palindromeIndex = searchPalindrome(chain);

		int maxPalindromePosition = -1;
		int maxPalindromeValue = -1;
		for (int i = 0; i < (int) palindromeIndex.size(); i++) {
			// >= keeps the last palindrome when there is a tie in length
			if (palindromeIndex[i] >= maxPalindromeValue) {
				maxPalindromePosition = i;
				maxPalindromeValue = palindromeIndex[i];
			}
		}

		// In the expanded chain the radius equals the palindrome length in
		// the original string, and the center maps to (center - radius) / 2
		int start = (maxPalindromePosition - maxPalindromeValue) / 2;
		tuple<int,int> response = {start, start + maxPalindromeValue - 1};
		return response;
	}

	/*
     * expandedChain()
	 * Builds the expanded version of a string by placing a '|' separator
	 * before, between and after its characters (e.g. "aba" -> "|a|b|a|").
	 *
	 * Parameters:
	 *   transmission - original string to expand (not modified).
	 *
	 * Returns:
	 *   Expanded string of length 2n + 1, where n is the length of
	 *   transmission.
	 */
	string expandedChain(const string &transmission) const {
		string chain = "|";
		for (char c : transmission) {
			chain += c;
			chain += '|';
		}
		return chain;
	}

	/*
	 * searchPalindrome()
      * Computes, for every position of the expanded chain, the radius of the
	 * longest palindrome centered there (Manacher's algorithm).
	 * Time complexity: O(n), where n is the length of chain.
	 *
	 * Parameters:
	 *   chain - expanded string produced by expandedChain (not modified).
	 *
	 * Returns:
	 *   Vector with the same size as chain where position i holds the radius
	 *   of the palindrome centered at i. In the expanded chain this radius is
	 *   also the length of that palindrome in the original string.
	 */
	vector<int> searchPalindrome(const string &chain) const {
		int chainLength = chain.length();
		vector<int> palindrome(chainLength, 0);
		int center = 0;
		int right = 0;
		for (int i = 1; i < chainLength; i++) {
			int iMirror = center - (i - center);
			// Reuse the mirror's radius, limited by the right edge of the
			// rightmost palindrome found so far
			if (right > i) {
				palindrome[i] = min(right - i, palindrome[iMirror]);
			}

			// Expand around i while both sides are inside the chain and match
			while (i + 1 + palindrome[i] < chainLength &&
                         i - 1 - palindrome[i] >= 0 &&
                         chain[i + 1 + palindrome[i]] == chain[i - 1 - palindrome[i]]) {
				palindrome[i]++;
			}

			// Track the palindrome that reaches farthest to the right
			if (i + palindrome[i] > right) {
				center = i;
				right = i + palindrome[i];
			}
		}
		return palindrome;
	}
};

#endif