/*
 * Title: subchain.h
 *
 * Description:
 * Creation of Subchain object to search for a malicious code inside a
 * transmission using the Z-function algorithm. The class only compares the
 * strings that it receives; reading files and displaying results are handled
 * by the main program.
 *
 * Implementation for the subject- Analysis and Design of Advanced
 * Algorithms
 *
 * Author: Alexis Yaocalli Berthou Haas - A01713458 & Rodrigo Alejandro Hurtado Cortes - A01713854
 * Date: Septiembre 3, 2026
 */

#ifndef SUBCHAIN_H
#define SUBCHAIN_H

#include <algorithm>
#include <string>
#include <vector>

using namespace std;

/*
In order to analyze complexity:
T = length of a transmission file (characters, without line breaks).
M = length of a malicious code file (characters, without line breaks).
n = M + T + 1 = length of the chain mcode + '$' + transmission.
*/

class Subchain {
public:
	/**
	 * zFunction()
	 * Computes the Z-array of a string. zArray[i] is the length of the longest
	 * substring starting at i that is also a prefix of text. Uses a Z-box
	 * [boxLeft, boxRight] to reuse previously computed values instead of
	 * comparing from scratch.
	 *
	 * Complexity:
	 *  Time: O(n) (each character is compared at most twice: once extending
	 *        a Z-box and once when it is reused)
	 *  Space: O(n)
	 *
	 * Params:
	 * text is the string to analyze (in this class, mcode + '$' + transmission).
	 * Returns:
	 * a vector of integers with the Z-array of text. zArray[0] is left as 0.
	 */
	vector<int> zFunction(const string &text) const {
		int length = static_cast<int>(text.length());
		vector<int> zArray(length, 0);
		int boxLeft = 0;
		int boxRight = 0;

		for (int i = 1; i < length; i++) {
			// Reuse previous calculations inside the current Z-box
			if (i <= boxRight) {
				int mirrorIndex = i - boxLeft;
				zArray[i] = min(boxRight - i + 1, zArray[mirrorIndex]);
			}

			// Compare characters while the prefix continues matching
			while (i + zArray[i] < length && text[zArray[i]] == text[i + zArray[i]]) {
				zArray[i]++;
			}

			// Update the Z-box when the current match reaches farther
			if (i + zArray[i] - 1 > boxRight) {
				boxLeft = i;
				boxRight = i + zArray[i] - 1;
			}
		}
		return zArray;
	}

	/**
	 * search()
	 * Finds all the occurrences of a malicious code inside a transmission.
	 * Builds the chain mcode + '$' + transmission and stores every position
	 * where the Z-value equals the length of the mcode.
	 *
	 * Complexity:
	 *  Time: O(T + M) (building the chain, zFunction and the final scan are
	 *        all linear in n)
	 *  Space: O(T + M + k) (the chain, the Z-array and the k positions found)
	 *
	 * Params:
	 * transmission is the content of a transmission file without line breaks.
	 * mcode is the content of a malicious code file without line breaks.
	 * Returns:
	 * a vector with the zero-based positions of every occurrence in
	 * transmission. The vector is empty if mcode is empty or does not appear.
	 */
	vector<int> search(const string &transmission, const string &mcode) const {
		vector<int> positions;
		if (transmission.empty() || mcode.empty()) {
			return positions;
		}

		// '$' separates the pattern from the text because it is not a valid input character
		string chain = mcode + '$' + transmission;
		vector<int> zIndex = zFunction(chain);
		int mcodeLength = static_cast<int>(mcode.length());
		int chainLength = static_cast<int>(chain.length());

		// Only the part of the chain that belongs to the transmission is checked
		for (int i = mcodeLength + 1; i < chainLength; i++) {
			if (zIndex[i] == mcodeLength) {
				positions.push_back(i - mcodeLength - 1);
			}
		}
		return positions;
	}
};

#endif