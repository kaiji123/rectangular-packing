#include <stdio.h>
#include <stdlib.h>

// Define structures for binary tree nodes and rectangular blocks
typedef struct TreeNode {
    char type; // 'V' for vertical cut, 'H' for horizontal cut
    int width; // width of the room
    int height; // height of the room
    struct TreeNode* left;
    struct TreeNode* right;
} TreeNode;

typedef struct RectBlock {
    int label;
    int width;
    int height;
} RectBlock;

// Function prototypes
TreeNode* buildBinaryTree(FILE* inputFile);
void postOrderTraversal(TreeNode* root, FILE* outputFile);
void computeDimensions(TreeNode* root, FILE* dimOutputFile);
void freeTree(TreeNode* root);

int main(int argc, char* argv[]) {
    // Check command-line arguments
    if (argc != 5) {
        printf("Usage: %s <input_file> <output_file1> <output_file2> <output_file3>\n", argv[0]);
        return EXIT_FAILURE;
    }

    // Open input file
    FILE* inputFile = fopen(argv[1], "r");
    if (inputFile == NULL) {
        printf("Error: Unable to open input file %s\n", argv[1]);
        return EXIT_FAILURE;
    }

    // Build binary tree from input file
    TreeNode* root = buildBinaryTree(inputFile);
    fclose(inputFile);

    if (root == NULL) {
        printf("Error: Failed to build binary tree\n");
        return EXIT_FAILURE;
    }

    // Open output files
    FILE* outputFile1 = fopen(argv[2], "w");
    FILE* outputFile2 = fopen(argv[3], "w");
    FILE* outputFile3 = fopen(argv[4], "w");
    if (outputFile1 == NULL || outputFile2 == NULL || outputFile3 == NULL) {
        printf("Error: Unable to open output files\n");
        freeTree(root);
        return EXIT_FAILURE;
    }

    // Perform post-order traversal and write to output file1
    postOrderTraversal(root, outputFile1);

    // Compute dimensions and block coordinates, write to output files 2 and 3
    computeDimensions(root, outputFile2);
    // Assume the coordinate calculation function is named computeCoordinates()
    // computeCoordinates(root, outputFile3);

    // Close output files and free memory
    fclose(outputFile1);
    fclose(outputFile2);
    fclose(outputFile3);
    freeTree(root);

    return EXIT_SUCCESS;
}

TreeNode* buildBinaryTree(FILE* inputFile) {
    char buffer[10];
    if (fgets(buffer, sizeof(buffer), inputFile) == NULL) {
        return NULL;
    }

    TreeNode* node = (TreeNode*)malloc(sizeof(TreeNode));
    if (node == NULL) {
        return NULL;
    }

    node->type = buffer[0];
    if (buffer[0] == 'H' || buffer[0] == 'V') {
        node->left = buildBinaryTree(inputFile);
        node->right = buildBinaryTree(inputFile);
    } else {
        sscanf(buffer, "%d(%d,%d)", &node->label, &node->width, &node->height);
        node->left = NULL;
        node->right = NULL;
    }

    return node;
}

void postOrderTraversal(TreeNode* root, FILE* outputFile) {
    if (root == NULL) {
        return;
    }

    postOrderTraversal(root->left, outputFile);
    postOrderTraversal(root->right, outputFile);

    if (root->type != 'H' && root->type != 'V') {
        fprintf(outputFile, "%d(%d,%d)\n", root->label, root->width, root->height);
    } else {
        fprintf(outputFile, "%c\n", root->type);
    }
}

void computeDimensions(TreeNode* root, FILE* dimOutputFile) {
    if (root == NULL) {
        return;
    }

    computeDimensions(root->left, dimOutputFile);
    computeDimensions(root->right, dimOutputFile);

    if (root->type == 'V') {
        root->width = root->left->width + root->right->width;
        root->height = (root->left->height > root->right->height) ? root->left->height : root->right->height;
    } else if (root->type == 'H') {
        root->width = (root->left->width > root->right->width) ? root->left->width : root->right->width;
        root->height = root->left->height + root->right->height;
    }

    fprintf(dimOutputFile, "%c(%d,%d)\n", root->type, root->width, root->height);
}

void freeTree(TreeNode* root) {
    if (root == NULL) {
        return;
    }

    freeTree(root->left);
    freeTree(root->right);
    free(root);
}
