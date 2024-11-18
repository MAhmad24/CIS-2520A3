#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "a3q1_header.h"


Variable variables[10];
int varCount = 0; 

// Helper function to check if a character is an operator
int isOperator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/';
}

// Helper function to get the precedence of an operator
int precedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    return 0;
}

// Helper function to create a new node
Node* createNode(char *data) {
    Node *newNode = (Node*)malloc(sizeof(Node));
    strcpy(newNode->data, data);
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

// Function to parse the expression using the Shunting Yard algorithm
Node* parseExpression(char *expr) {
    int len = strlen(expr);
    Node *outputStack[100]; // Stack to hold nodes for the output
    char operatorStack[100]; // Stack to hold operators
    int outputIndex = 0, operatorIndex = 0; // Indices for the stacks
    char buffer[10]; // Buffer to hold multi-character tokens (numbers/variables)
    int bufferIndex = 0; // Index for the buffer

    for (int i = 0; i < len; i++) {
        if (isdigit(expr[i]) || expr[i] == '.' || isalpha(expr[i])) {
            // If the character is part of a number or variable, add it to the buffer
            buffer[bufferIndex++] = expr[i];
        } else {
            // If the buffer has accumulated characters, create a node and push it to the output stack
            if (bufferIndex > 0) {
                buffer[bufferIndex] = '\0';
                outputStack[outputIndex++] = createNode(buffer);
                bufferIndex = 0;
            }
            if (isOperator(expr[i])) {
                // If the character is an operator, pop operators from the operator stack to the output stack
                // until an operator with lower precedence is found or the stack is empty
                while (operatorIndex > 0 && precedence(operatorStack[operatorIndex - 1]) >= precedence(expr[i])) {
                    char op[2] = {operatorStack[--operatorIndex], '\0'};
                    Node *operatorNode = createNode(op);
                    operatorNode->right = outputStack[--outputIndex];
                    operatorNode->left = outputStack[--outputIndex];
                    outputStack[outputIndex++] = operatorNode;
                }
                // Push the current operator to the operator stack
                operatorStack[operatorIndex++] = expr[i];
            } else if (expr[i] == '(') {
                // If the character is '(', push it to the operator stack
                operatorStack[operatorIndex++] = expr[i];
            } else if (expr[i] == ')') {
                // If the character is ')', pop operators from the operator stack to the output stack
                // until '(' is found
                while (operatorIndex > 0 && operatorStack[operatorIndex - 1] != '(') {
                    char op[2] = {operatorStack[--operatorIndex], '\0'};
                    Node *operatorNode = createNode(op);
                    operatorNode->right = outputStack[--outputIndex];
                    operatorNode->left = outputStack[--outputIndex];
                    outputStack[outputIndex++] = operatorNode;
                }
                operatorIndex--; // Pop the '(' from the stack
            }
        }
    }

    // If the buffer has accumulated characters, create a node and push it to the output stack
    if (bufferIndex > 0) {
        buffer[bufferIndex] = '\0';
        outputStack[outputIndex++] = createNode(buffer);
    }

    // Pop all remaining operators from the operator stack to the output stack
    while (operatorIndex > 0) {
        char op[2] = {operatorStack[--operatorIndex], '\0'};
        Node *operatorNode = createNode(op);
        operatorNode->right = outputStack[--outputIndex];
        operatorNode->left = outputStack[--outputIndex];
        outputStack[outputIndex++] = operatorNode;
    }

    // The root of the expression tree is the last node in the output stack
    return outputStack[0];
}

// The preOrder function prints tree nodes in preorder traversal.
void preorder(Node *root) {
    if (root != NULL) {
        // Print the current node's data
        printf("%s ", root->data);
        // Recursively traverse the left subtree
        preorder(root->left);
        // Recursively traverse the right subtree
        preorder(root->right);
    }
}

// The inOrder function prints tree nodes in inorder traversal fully parenthesized.
void inorder(Node *root) {
    if (root != NULL) {
        // If the node has children, print an opening parenthesis
        if (root->left != NULL || root->right != NULL) {
            printf("(");
        }
        // Recursively traverse the left subtree
        inorder(root->left);
        // Print the current node's data
        printf("%s", root->data);
        // Recursively traverse the right subtree
        inorder(root->right);
        // If the node has children, print a closing parenthesis
        if (root->left != NULL || root->right != NULL) {
            printf(")");
        }
    }
}

// The postOrder function prints tree nodes in postorder traversal.
void postorder(Node *root) {
    if (root != NULL) {
        // Recursively traverse the left subtree
        postorder(root->left);
        // Recursively traverse the right subtree
        postorder(root->right);
        // Print the current node's data
        printf("%s ", root->data);
    }
}

// The promptVariables function prompts the user to assign values to each variable found in the expression tree. The values should be stored in the Variables struct.
void promptVariables(Node *root) {
    if (root != NULL) {
        // If the current node is a variable, prompt the user for its value
        if (isalpha(root->data[0])) {
            printf("Enter value for %s: ", root->data);
            float value;
            scanf("%f", &value);
            // Store the variable name and value in the variables array
            strcpy(variables[varCount].varName, root->data);
            variables[varCount].value = value;
            varCount++;
        }
        // Recursively prompt for variables in the left subtree
        promptVariables(root->left);
        // Recursively prompt for variables in the right subtree
        promptVariables(root->right);
    }
}

// The calculate function calculates the expression and returns its result. Division by 0 and/or other error scenarios should be checked.
float calculate(Node *root) {
    if (root == NULL) return 0;

    // If the current node is an operator, calculate the values of the left and right subtrees
    if (isOperator(root->data[0])) {
        float leftValue = calculate(root->left);
        float rightValue = calculate(root->right);
        // Perform the operation based on the operator
        switch (root->data[0]) {
            case '+': return leftValue + rightValue;
            case '-': return leftValue - rightValue;
            case '*': return leftValue * rightValue;
            case '/':
                // Check for division by zero
                if (rightValue == 0) {
                    printf("Error: Division by zero\n");
                    exit(1);
                }
                return leftValue / rightValue;
        }
    // If the current node is a variable, return its value
    } else if (isalpha(root->data[0])) {
        for (int i = 0; i < varCount; i++) {
            if (strcmp(variables[i].varName, root->data) == 0) {
                return variables[i].value;
            }
        }
        printf("Error: Variable %s not found\n", root->data);
        exit(1);
    // If the current node is a number, return its value
    } else {
        return atof(root->data);
    }

    return 0;
}
