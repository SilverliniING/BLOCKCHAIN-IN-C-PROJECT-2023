#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<math.h>
#define maxinputLen 100
#define maxinputleaves 100

#define COUNT 10

typedef struct Block {
    char region_name[256];
    signed int hash;
    struct Node* region;
    struct Block* next;
    struct Block* previous;
} Block;


struct Node
{
    signed int data;
    struct Node* left, * right;
};



signed int customHash(signed int num1, signed int num2);

//For linked list

/*explanation, Once every region has voted, the root will be passed to insertBlock. Here the block is cretaed first.
Then the block has a pointer towards the root, the region. The hash of the block will be the hash of the
previous block along with the hash of the root.
*/




Block* createBlock(const char* region, struct Node* region_root) {
    // here first create a region's root manually then region_block->root_region_created
    Block* block = (Block*)malloc(sizeof(Block));
    strcpy(block->region_name, region);
    block->hash = 2738596;
    block->region = region_root;
    block->next = NULL;
    block->previous = NULL;
    return block;
}

Block* insertBlock(Block* head, const char* region, struct Node * region_tree_root) {
    Block* block = createBlock(region, region_tree_root); 
    if (head == NULL) {
        // first block
        head = block;
    } else {
        Block* current = head;
        while (current->next != NULL) {
            current = current->next;
        }
        block->previous = current;
        block->hash = customHash(current->hash, region_tree_root->data);
        current->next = block;
    }
    return block;
}

void printLinkedList(Block* head) {
    Block* current = head;
    while (current != NULL) {
        printf("Region: %s\n", current->region_name);
        printf("Hash: %d\n", current->hash);
        printf("-----------\n");
        current = current->next;
    }
}


signed int customHash(signed int num1, signed int num2) {
    signed int hash = 0;
    unsigned char* p1 = (unsigned char*)&num1;
    unsigned char* p2 = (unsigned char*)&num2;

    // Process first integer
    while (*p1 != '\0') {
        hash += *p1++;
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }
    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);

    // Process second integer
    while (*p2 != '\0') {
        hash += *p2++;
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }
    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);

    return hash;
}
