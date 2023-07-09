#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>


#include "IDauthenticator.h"
#include "new_linkedlist.c"

#define maxinputLen 100
#define maxinputleaves 100
#define COUNT 10
#define MAX_STRINGS 100
#define MAX_LENGTH 100
#define NUM_CANDIDATES 4
#define MAX_LINE_LENGTH 100
#define MAX_STRING_COUNT 100


//GLOBAL VARIABLES
struct Block *Pune_block = NULL;

//STRUCT DEFINITIONS
struct Node *newNode(int data)
{
    struct Node *node = (struct Node *)malloc(sizeof(struct Node));

    node->data = data;
    node->left = node->right = NULL;
    return (node);
};

//++++++++ MERKLE TREE CREATION +++++++++//

// Function to insert nodes in tree in level order
struct Node *insertLevelOrder(unsigned int arr[], int i, int n)
{
    struct Node *root = NULL;
    // Base case for recursion
    if (i < n)
    {
        root = newNode(arr[i]);

        // insert left child
        root->left = insertLevelOrder(arr,
                                      2 * i + 1, n);

        // insert right child
        root->right = insertLevelOrder(arr,
                                       2 * i + 2, n);
    }
    return root;
};

void print2DUtil(struct Node *root, int space)
{
    // Base case
    if (root == NULL)
        return;

    // Increase distance between levels
    space += COUNT;

    // Process right child first
    print2DUtil(root->right, space);

    // Print current node after space
    // count
    printf("\n");
    for (int i = COUNT; i < space; i++)
        printf(" ");
    printf("%d\n", root->data);

    // Process left child
    print2DUtil(root->left, space);
}

// Wrapper over print2DUtil()
void print2D(struct Node *root)
{
    // Pass initial space count as 0
    print2DUtil(root, 0);
}

//++++++++++++++++++++++++++CSV FUNCTIONS++++++++++++++++++++++//

void move_string_to_csv(const char *input)
{
    // Open the CSV file in append mode
    FILE *file = fopen("input.csv", "a");

    if (file != NULL)
    {
        // Write the string to the file
        fprintf(file, "\n%s", input);

        // Close the file
        fclose(file);

        printf("String successfully added to input.csv\n");
    }
    else
    {
        printf("Failed to open the file input.csv\n");
    }
}


int moveCSVContents(const char *sourcePath, const char *destinationPath)
{
    FILE *sourceFile = fopen(sourcePath, "r");
    FILE *destinationFile = fopen(destinationPath, "w");

    if (sourceFile == NULL || destinationFile == NULL)
    {
        printf("Error opening file.\n");
        return 1;
    }

    char line[MAX_LINE_LENGTH];

    // Read from source file and write to destination file
    while (fgets(line, MAX_LINE_LENGTH, sourceFile) != NULL)
    {
        fputs(line, destinationFile);
    }

    // Close the files
    fclose(sourceFile);
    fclose(destinationFile);

    //printf("Content moved successfully.\n");

    return 0;
}



int readCSVFile(const char *filePath, char **stringArray)
{
    FILE *file = fopen(filePath, "r");
    if (file == NULL)
    {
        printf("Error opening file.\n");
        return 1;
    }

    char line[MAX_LINE_LENGTH];
    int stringCount = 0;

    while (fgets(line, MAX_LINE_LENGTH, file) != NULL)
    {
        // Remove newline character from the end of the line
        line[strcspn(line, "\n")] = '\0';
        if (strcmp(line, "") != 0)
        {
            // Allocate memory for the string and copy it to the string array
            stringArray[stringCount] = malloc(sizeof(char) * (strlen(line) + 1));
            strcpy(stringArray[stringCount], line);

            stringCount++;
        }

        if (stringCount >= MAX_STRING_COUNT)
        {
            printf("Exceeded maximum string count.\n");
            break;
        }
    }

    // Close the file
    fclose(file);

    return stringCount;
}

void storeLevelOrder(struct Node *root)
{
    if (root == NULL)
        return;

    FILE *file = fopen("CURRENTHASHES.csv", "w");
    if (file == NULL)
    {
        printf("Failed to open file.");
        return;
    }

    // Create a queue to hold nodes at the current level
    struct Node **queue = (struct Node **)malloc(sizeof(struct Node *));
    int front = 0, rear = 0;
    queue[rear] = root;
    rear++;

    while (front < rear)
    {
        int levelNodes = rear - front;

        while (levelNodes > 0)
        {
            struct Node *node = queue[front];
            fprintf(file, "%d\n", node->data);

            // Enqueue left child
            if (node->left != NULL)
            {
                queue = (struct Node **)realloc(queue, (rear + 1) * sizeof(struct Node *));
                queue[rear] = node->left;
                rear++;
            }

            // Enqueue right child
            if (node->right != NULL)
            {
                queue = (struct Node **)realloc(queue, (rear + 1) * sizeof(struct Node *));
                queue[rear] = node->right;
                rear++;
            }

            front++;
            levelNodes--;
        }
    }

    fclose(file);
}

// Function to write vote counts to a CSV file
void writeVoteCountsToCSV(const char *filename, int voteCounts[])
{
    FILE *file = fopen(filename, "w");
    if (file == NULL)
    {
        printf("Error: Unable to open the file for writing.\n");
        return;
    }

    for (int i = 0; i < 4; i++)
    {
        fprintf(file, "%d\n", voteCounts[i]);
    }

    fclose(file);
}

// Function to read vote counts from a CSV file to an array
void readVoteCountsFromCSV(const char *filename, int voteCounts[])
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        // If the file doesn't exist or is empty, initialize the vote counts to 0
        for (int i = 0; i < 4; i++)
        {
            voteCounts[i] = 0;
        }
        return;
    }

    for (int i = 0; i < 4; i++)
    {
        fscanf(file, "%d", &voteCounts[i]);
    }

    fclose(file);
}

//++++++++++++++++++HASHING FUNCTION++++++++++++++++++++++//
//hash helper function//
char *getCurrentTime()
{
    time_t currentTime;
    time(&currentTime);
    return ctime(&currentTime);
}

//hash helper function//
int numberlength(int num)
{
    int len = 0, n;
    n = num;
    while (n != 0)
    {
        len++;
        n /= 10;
    }
    return len;
}
//hash helper function//
void tostring(char str[], unsigned int num)
{
    int i, rem, len = 0;
    unsigned int n = num;
    while (n != 0)
    {
        len++;
        n /= 10;
    }
    for (i = 0; i < len; i++)
    {
        rem = num % 10;
        num = num / 10;
        str[len - (i + 1)] = rem + '0';
    }
    str[len] = '\0';
}

//individual hash//
unsigned int hash(char *str)
{
    unsigned int hash = 5381;
    int c;
    while ((c = *str++))
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
}

//dual hash of two leaves OR leaf and time//
unsigned int hash_function(unsigned int input, unsigned int prev_hash)
{
// converting nos to strings
    int prevhashlength = numberlength(prev_hash);
    char prevhashstring[prevhashlength];
    int inputlength = numberlength(input);
    
    char inputstring[inputlength];
    tostring(prevhashstring, prev_hash);
    tostring(inputstring, input);
// mallocing concat of strigs
    unsigned long n = (strlen(inputstring) + strlen(prevhashstring));
    char *cat = (char *)malloc((n + 1) * sizeof(char));
    strcat(cat, prevhashstring);
    strcat(cat, inputstring);
//hashing cat
    unsigned int Hash = hash(cat);
    return Hash;
}

//++++++++++++++++++2D ARRAY FUNCTIONS++++++++++++++++++++++//

int HowmanyTrans(char Data[maxinputleaves][maxinputLen])
{
    int c = -1;
    do
    {
        c++;
    } while (strcmp(Data[c], ""));
    return c;
}
//CONVERTER TO 1D
void transferStrings(char *strings[maxinputleaves], char Data[maxinputleaves][maxinputLen])
{
    for (int i = 0; i < maxinputleaves; i++)
    {
        if (strings[i] != NULL)
        {
            strncpy(Data[i], strings[i], maxinputLen - 1);
            Data[i][maxinputLen - 1] = '\0'; // Ensure null-termination
        }
    }
}


//++++++++++++++++++BLOCKCHAIN FUNCTIONS++++++++++++++++++++++//


void assign_root(int R, struct Node *root) {
    struct Block *currentBlock = NULL;
    currentBlock = Pune_block; // Start at the head of the linked list

    // Traverse to the specified node number
    for (int i = 1; i <= R && currentBlock != NULL; i++) {
        currentBlock = currentBlock->next;
    }

    // Assign the region pointer of the node to the passed root
    if (currentBlock != NULL) {
        currentBlock->region = root;
    }

     if(currentBlock->previous!=NULL)
     {
         currentBlock->hash = customHash(currentBlock->previous->hash, root->data);
     }
     else{
        currentBlock->hash = customHash(currentBlock->hash, root->data);
     }

     

     //Store the block's hash value in a CSV file
    FILE *csvFile = fopen("BlockHashes.csv", "a"); // Open the CSV file in append mode

    if (csvFile != NULL) {
        fprintf(csvFile, "%s,%d\n", currentBlock->region_name, currentBlock->hash);
        fclose(csvFile); // Close the CSV file
    } else {
        printf("Failed to open the CSV file for writing.\n");
    }
    return;
}

//+++++++++++++++VOTE STRING ARRAY TO MERKLE MAIN FUNCTION++++++++++++++++++++++//
void processStrings(char *strings[], int count, int R)
{
//    printf("Processing %d strings:\n", count);
//    for (int i = 0; i < count; i++)
//    {
//        printf("String %d: %s\n", i + 1, strings[i]);
//    }
    char Data[maxinputleaves][maxinputLen];
    memset(Data, 0, sizeof(Data)); // Initialize Data array to all zeros

    // I MADE THE FUNCTIONS FOR A 2D ARRAY BUT TOOK A 1D INPUT SO I HAD TO DO THIS -- strings is 1D and Data is 2D
    transferStrings(strings, Data);

    int leaves = HowmanyTrans(Data);
    // leaf nodes not necessarily on same level
    int totalnodes = (2 * leaves) - 1;
    // level starting at 1 at root

    unsigned int FinalArray[totalnodes];
    int leafindex = 0;
    int level = 0;
    for (int i = 1; (pow(2, i)) <= totalnodes; i++)
    {
        level = i;
    }

    if ((totalnodes) == pow(2, level))
    {
    }
    else
    {
        level++;
    }

    // fill in leaves right to left
    // printf("totalnodes %d\n", totalnodes);
    // printf("level %d\n",level);
    // printf("leaves %d\n",leaves);
    for (int i = (totalnodes - leaves); i <= (totalnodes - 1); i++)
    {
        FinalArray[i] = atoi(Data[leafindex]); // THIS IS ME REMOVING HASHING CUZ WE'RE HASHING WITH TIME ANYWAYS
        // FinalArray[i] = Data[leafindex];
        leafindex++;
    }
    for (int i = (totalnodes - leaves - 1); i >= 0; i--)
    {
        int right = 2 * i + 1;
        int left = 2 * i + 2;
        if (totalnodes <= right)
        {
            FinalArray[i] = FinalArray[right];
        }
        else
        {
            FinalArray[i] = hash_function(FinalArray[left], FinalArray[right]);
        }
    }

    // for (int i = 0; i < totalnodes - 1; i++)
    // {
    //     printf("%d\n", FinalArray[i]);
    // }
    struct Node *root = insertLevelOrder(FinalArray, 0, totalnodes);
    assign_root(R, root);

    // point to diff roots
    print2D(root);
    storeLevelOrder(root);
    printf("\nProcessing complete.\n");
}



















//+++++++++++++++ UNUSED FUNCTIONS++++++++++++++++++++++//

//unsigned int *getLeafAncestors(const unsigned int *levelOrder, int size, int leafIndex, int *ancestorCount)
//{
//    int maxAncestors = leafIndex / 2;
//    unsigned int *ancestors = (unsigned int *)malloc(maxAncestors * sizeof(unsigned int));
//
//    int index = leafIndex;
//    int count = 0;
//
//    while (index > 0)
//    {
//        index = (index - 1) / 2;
//        ancestors[count] = index;
//        count++;
//
//        if (count >= maxAncestors)
//        {
//            break;
//        }
//    }
//
//    *ancestorCount = count;
//    return ancestors;
//}

//void assign_root(int R, struct Node *root) {
//    struct Block *currentBlock = NULL;
//    currentBlock = Pune_block; // Start at the head of the linked list
//
//    // Traverse to the specified node number
//    for (int i = 1; i < R && currentBlock != NULL; i++) {
//        currentBlock = currentBlock->next;
//    }
//
//    // Assign the region pointer of the node to the passed root
//    if (currentBlock != NULL) {
//        currentBlock->region = root;
//    }
//
//     if(currentBlock->previous!=NULL)
//     {
//         currentBlock->hash = customHash(currentBlock->previous->hash, root->data);
//     }
//
//     //Store the block's hash value in a CSV file
//    FILE *csvFile = fopen("BlockHashes.csv", "a"); // Open the CSV file in append mode
//
//    if (csvFile != NULL) {
//        fprintf(csvFile, "%s,%d\n", currentBlock->next->region_name, currentBlock->next->hash);
//        fclose(csvFile); // Close the CSV file
//    } else {
//        printf("Failed to open the CSV file for writing.\n");
//    }
//    return;
//}
//
