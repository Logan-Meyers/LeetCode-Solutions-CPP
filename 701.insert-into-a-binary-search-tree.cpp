/*
 * @lc app=leetcode id=701 lang=cpp
 *
 * [701] Insert into a Binary Search Tree
 */

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};

// @lc code=start
/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode() : val(0), left(nullptr), right(nullptr) {}
 *     TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
 *     TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
 * };
 */
#include <cstdlib>

class Solution {
public:
    // O(h) time, where h is height. h is log n if balanced, or n if skewed
    TreeNode* insertIntoBST(TreeNode* root, int val) {
        // base case: empty tree, malloc node and return root
        if (root == nullptr) {
            root = new TreeNode(val);
        }

        // go left
        else if (val < root->val) {
            root->left = insertIntoBST(root->left, val);
        }

        // go right
        else if (val > root->val) {
            root->right = insertIntoBST(root->right, val);
        }

        // return root
        return root;
    }
};
// @lc code=end