#include <stdint.h>

typedef uint64_t BTreeKeyType;

struct BTreeNode;

void insert(struct BTreeNode **root, BTreeKeyType key);
struct BTreeNode* findBTreeNode(struct BTreeNode *root, BTreeKeyType key);
