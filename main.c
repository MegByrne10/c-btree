#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

typedef struct Node {
  int value;
  struct Node* left;
  struct Node* right;
} Node;

typedef struct Edge {
  char* str;
  struct Edge* prev;
} Edge;

int max(int a, int b);
bool node_exists(Node* root, int value);
int find_min(Node* root);
int find_min_rec(Node* root);
int find_max(Node* root);
int find_height(Node* root);
void print_inorder(Node* root);
void print_tree(Node* root, Edge* prev, bool is_left);
void plot_edges(Edge* edge);
void free_tree(Node* root);

Node* create_node(int value) {
  Node* new_node = malloc(sizeof(struct Node));
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
  if (value <= node->value) {
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

Edge* create_edge(Edge* prev) {
  Edge* new_edge = malloc(sizeof(Edge));
  if (new_edge != NULL) {
    new_edge->str = "";
    new_edge->prev = prev;
  }
  return new_edge;
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
  root = insert_node(root, 32);
  root = insert_node(root, 10);
  root = insert_node(root, 14);
  root = insert_node(root, 27);
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

  printf("\nPrint the Tree in a prettier way:\n\n");
  print_tree(root, NULL, 0);

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
  printf(" %d ->", root->value);
  print_inorder(root->right);
}

void plot_edges(Edge* edge) {
  if (edge == NULL) return;
  plot_edges(edge->prev);
  printf("%s", edge->str);
}

void print_tree(Node* root, Edge* prev, bool is_left) {
  if (root == NULL) return;

  char* prev_str = "    ";
  Edge* edge = create_edge(prev);
  edge->str = prev_str;

  print_tree(root->right, edge, 0);

  if (prev == NULL) {
    edge->str = "---";
  } else if (is_left) {
    edge->str = "`--";
    prev->str = prev_str;
  } else {
    edge->str = ".--";
    prev_str = "   |";
  }

  plot_edges(edge);
  printf("%d\n", root->value);

  if (prev != NULL) {
    prev->str = prev_str;
  }
  edge->str = "   |";
  print_tree(root->left, edge, 1);
  free(edge);
}

void free_tree(Node* root) {
  // Deallocates memory for every node in the tree
  if (root == NULL) return;
  free_tree(root->left);
  free_tree(root->right);
  free(root);
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
