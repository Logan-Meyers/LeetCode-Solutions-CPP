/*
 * @lc app=leetcode id=1 lang=cpp
 *
 * [1] Two Sum
 */

// @lc code=start
#include <unordered_map>
#include <utility>
#include <vector>

using std::vector;

class Solution {
public:
    vector<int> twoSum(vector<int>& nums, int target) {
        // key: remainder number
        // value: index of number that gives us that remainder
        std::unordered_map<int, int> remainders;

        int i = 0;
        for (const auto& element : nums) {
            // first get remainder of number
            int remainder = target - element;

            // see if we have a number in our map with our current number as their remainder
            const auto& found = remainders.find(element);
            if (found != remainders.end()) {
                return {found->second, i};
            }
            
            // else, populate map with our remainder, index pair
            else {
                remainders[remainder] = i;
            }

            i++;
        }

        return {-1, -1};
    }
};
// @lc code=end

