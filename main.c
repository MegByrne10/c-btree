#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

typedef struct Node {
  int value;
  struct Node* left;
  struct Node* right;
} Node;

int max(int a, int b);
bool node_exists(Node* root, int value);
int find_min(Node* root);
int find_min_rec(Node* root);
int find_max(Node* root);
int find_height(Node* root);
void print_inorder(Node* root);
void print_tree_rec(Node* root, int numtabs);
void print_tree(Node* root);
// void print_tabs(int numtabs);
void free_tree(Node* root);

Node* create_node(int value) {
  Node* new_node = malloc(sizeof(Node));
  if (new_node != NULL) {
    new_node->left = NULL;
    new_node->right = NULL;
    new_node->value = value;
  }
  return new_node;
}

Node* insert_node(Node* node, int value) {
  // Return new node if at the end of a branch
  if (node == NULL) return create_node(value);

  // Traverse the tree to find the right place to insert the node
  if (value < node->value) {
    node->left = insert_node(node->left, value);
  } else {
    node->right = insert_node(node->right, value);
  }
  return node;
}

Node* find_node(Node* root, int value) {
  if (root == NULL) return root;
  if (value == root->value) return root;
  if (value < root->value) {
    return find_node(root->left, value);
  } else {
    return find_node(root->right, value);
  }
}

Node* delete_node(Node* root, int value) {
  if (root == NULL) return root;
  // Find the node to remove
  if (value < root->value) {
    root->left = delete_node(root->left, value);
  } else if (value > root->value) {
    root->right = delete_node(root->right, value);
  } else {
    // has one or no children
    if (root->left == NULL) {
      Node* temp = root->right;
      free(root);
      return temp;
    } else if (root->right == NULL) {
      Node* temp = root->left;
      free(root);
      return temp;
    } else {
      // has two children
      Node* temp = find_node(root, find_min_rec(root->right));
      root->value = temp->value;
      root->right = delete_node(root->right, temp->value);
    }
  }

  return root;
}

int main() {
  int min, max, height;
  Node *root = NULL;
  root = insert_node(root, 8);
  root = insert_node(root, 3);
  root = insert_node(root, 1);
  root = insert_node(root, 6);
  root = insert_node(root, 17);
  root = insert_node(root, 7);
  root = insert_node(root, 10);
  root = insert_node(root, 14);
  root = insert_node(root, 4);

  printf("Traverse and Print Tree in order:\n");
  print_inorder(root);

  printf("\nInsert 5 and reprint:\n");
  root = insert_node(root, 5);
  print_inorder(root);

  printf("\nFind the Min Iteratively: ");
  min = find_min(root);
  printf("%d\n", min);

  printf("\nFind the Min Recursively: ");
  min = find_min_rec(root);
  printf("%d\n", min);

  printf("\nFind Max Iteratively: ");
  max = find_max(root);
  printf("%d\n", max);

  printf("\nFind the Tree Height: ");
  height = find_height(root);
  printf("%d\n", height);

  printf("\nFind 5: ");
  printf("%d\n", node_exists(root, 5));
  
  printf("\nFind 9: ");
  printf("%d\n", node_exists(root, 9));

  printf("\nFind 14: ");
  printf("%d\n", node_exists(root, 14));

  printf("\nFind 19: ");
  printf("%d\n", node_exists(root, 19));

  printf("\nDelete 10:");
  root = delete_node(root, 10);
  print_inorder(root);

  printf("\nPrint the Tree in a really gross way:\n");
  print_tree(root);

  Node *other_root = NULL;
  printf("\nFind the Tree Height with NULL root: ");
  height = find_height(other_root);
  printf("%d\n", height);
  other_root = insert_node(other_root, 8);
  printf("\nFind the Tree Height with 1 insert to NULL: ");
  height = find_height(other_root);
  printf("%d\n", height);

  free_tree(root);
  free_tree(other_root);
  return 0;
}

bool node_exists(Node* root, int value) {
  if (root == NULL) return false;
  if (value == root->value) return true;
  if (value < root->value) {
    return node_exists(root->left, value);
  } else {
    return node_exists(root->right, value);
  }
}

int find_min(Node* root) {
  Node* current = root;
  if (root == NULL) {
    printf("Tree is empty");
    return -1;
  }

  while (current && current->left != NULL) {
    current = current->left;
  }
  return current->value;
}

int find_min_rec(Node* root) {
  if (root == NULL) {
    printf("Tree is empty");
    return -1;
  } else if (root->left == NULL ) {
    return root->value;
  }
  return find_min_rec(root->left);
}

int find_max(Node* root) {
  Node* current = root;
  if (root == NULL) {
    printf("Tree is empty");
    return -1;
  }

  while (current && current->right != NULL) {
    current = current->right;
  }
  return current->value;
}

int find_height(Node* root) {
  // counting edges not nodes, for edges return -1 when root == NULL (because there's nothing), 0 when only root present (0 sub-tree levels)
  if (root == NULL) {
    return -1;
  }
  return max(find_height(root->left), find_height(root->right)) + 1;
}

void print_inorder(Node* root) {
  if (root == NULL) return; 
  print_inorder(root->left);
  printf("%d -> ", root->value);
  print_inorder(root->right);
}

void print_tree_rec(Node* root, int space) {
  if (root == NULL) return;

  // Increase distance between levels
  space += 10;

  // Process right child first
  print_tree_rec(root->right, space);

  // Print current node after space count
  printf("\n");
  for (int i = 10; i < space; i++)
      printf(" ");
  printf("%d\n", root->value);

  // Process left child
  print_tree_rec(root->left, space);
}

void print_tree(Node* root) {
  print_tree_rec(root, 0);
}

// void print_tabs(int numtabs) {
//   for (int i=0; i < numtabs; i++) {
//     printf("\t");
//   }
// }

void free_tree(Node* root) {
  // Deallocates memory for every node in the tree
  Node* temp = root;
  if (temp == NULL) return;
  free_tree(temp->left);
  free_tree(temp->right);
  if (temp->left == NULL && temp->right == NULL) {
    free(temp);
    return;
  }
}

int max(int a, int b) {
  if (a >= b) {
    return a;
  } else if (a < b) {
    return b;
  } else {
    printf("Something went wrong finding the max number.");
    return 1;
  }
}