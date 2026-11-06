/*
 * @lc app=leetcode id=202 lang=cpp
 *
 * [202] Happy Number
 */

// Two examples by hand, both from the leetcode examples

/*
Input: n = 19
Output: true
Explanation:
1^2 + 9^2 = 82
8^2 + 2^2 = 68
6^2 + 8^2 = 100
1^2 + 0^2 + 0^2 = 1

Input: n = 2
Output: false
Explanation:
2^2 = 4
4^2 = 8
8^2 = 16
1^2 + 6^2 = 37
3^2 + 7^2 = 58
5^2 + 8^2 = 89
8^2 + 9^2 = 145
1^2 + 4^2 + 5^2 = 42
4^2 + 2^2 = 20
2^2 + 0^2 = REPEAT
*/

// @lc code=start
#include <unordered_set>
#include <vector>
#include <numeric>

class Solution {
public:
    bool isHappy(int n) {
        // map for seen results from adding squares of digits
        // key<int> = sum of digits
        // value<int> = seen (1) or not (0)
        std::unordered_set<int> seenSums;
        
        // main loop to see if n is a happy number
        while (true) {  // run until 1 or loop found, break inside loop only
            std::vector<int> digits = intToDigits(n);

            // sum square of digits to get result
            int result = std::accumulate(digits.begin(), digits.end(), 0, [](int total, int d) { return total + d * d; });

            // if result is 1, return true for happy number
            if (result == 1) {
                return true;
            }

            // if result in set, return false for not happy number
            if (!seenSums.insert(result).second) {
                return false;
            }

            // set n to the new result
            n = result;
        }
    }

private:
    std::vector<int> intToDigits(int n) {
        // create a vector to hold digits
        std::vector<int> digits = std::vector<int>();
        
        // pick out each digit from right to left and put in vector
        while (n > 0) {
            int d = n % 10;
            n /= 10;
            digits.push_back(d);
        }

        // return list of digits
        return digits;
    }
};
// @lc code=end

