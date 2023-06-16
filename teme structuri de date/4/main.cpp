#include <iostream>
#include <fstream>
#include <stack>
#include <cmath>
#include <vector>

struct TreeNode {
    std::string value;
    TreeNode* left;
    TreeNode* right;
};

bool isOperator(std::string value) {
    if (value == "+") return true;
    if (value == "-") return true;
    if (value == "*") return true;
    if (value == "/") return true;
    return false;
}

double performOperation(std::string value, double leftOperand, double rightOperand) {
    if (value == "+")
        return leftOperand + rightOperand;
    else if (value == "-")
        return leftOperand - rightOperand;
    else if (value == "*")
        return leftOperand * rightOperand;
    else if (value == "/")
        return leftOperand / rightOperand;
    else
        return 0.0; // Operator invalid
}

TreeNode* buildExpressionTree(std::vector<std::string>& tokens) {
    std::stack<TreeNode*> nodeStack;

    for (int i = tokens.size() - 1; i >= 0; i--) {
        std::string value = tokens[i];

        TreeNode* newNode = new TreeNode;
        newNode->value = value;
        newNode->left = newNode->right = nullptr;

        if (!isOperator(value)) {
            nodeStack.push(newNode);
        } else {
            newNode->left = nodeStack.top();
            nodeStack.pop();
            newNode->right = nodeStack.top();
            nodeStack.pop();
            nodeStack.push(newNode);
        }
    }

    return nodeStack.top();
}

double evaluateExpressionTree(TreeNode* root) {
    if (!isOperator(root->value))
        return std::stod(root->value);

    double leftResult = evaluateExpressionTree(root->left);
    double rightResult = evaluateExpressionTree(root->right);

    return performOperation(root->value, leftResult, rightResult);
}

int main() {
    std::string filename = "expresie.txt";
    std::ifstream inputFile(filename);

    std::vector<std::string> tokens;
    std::string token;

    while (inputFile >> token) {
        tokens.push_back(token);
    }

    TreeNode* root = buildExpressionTree(tokens);

    std::cout << "Rezultatul expresiei: " << evaluateExpressionTree(root);

    return 0;
}
