#include "Functions.h"

int main()

{
// +++++++++++++++intializing blockchain++++++++++++//
    struct Node *region_root_Pune = (struct Node *)malloc(sizeof(struct Node));
    region_root_Pune->data = -6436;
    struct Node *region_root_Mumbai = (struct Node *)malloc(sizeof(struct Node));
    region_root_Mumbai->data = 3584;
    struct Node *region_root_Nashik = (struct Node *)malloc(sizeof(struct Node));
    region_root_Nashik->data = 2846;
    struct Node *region_root_Nagpur = (struct Node *)malloc(sizeof(struct Node));
    region_root_Nagpur->data = 7849;

    // making a Pune block
    Pune_block = createBlock("Pune", region_root_Pune);

    // inserting the Mumbai block
    struct Block *Mumbai_block = insertBlock(Pune_block, "Mumbai", region_root_Mumbai);

    // inserting the Nashik block
    struct Block *Nashik_block = insertBlock(Pune_block, "Nashik", region_root_Nashik);

    // inserting the Nagpur block
    struct Block *Nagpur_block = insertBlock(Pune_block, "Nagpur", region_root_Nagpur);

    printLinkedList(Pune_block);

     // +++++++++++++++INITIALIZATIONS++++++++++++//
//for past input reading
    const char *filePath = "input.csv";
    char *strings[maxinputleaves] = {0};
    int count = 0;
// for input loop
    char loop;
//for votecount per candidate
    const char *votefile = "VoteCounts.csv";
    int voteCounts[4];
// Read vote counts from the CSV file
    readVoteCountsFromCSV(votefile, voteCounts);
    
    // +++++++++++++++REGION SELECT++++++++++++//
    char REGION[MAX_LENGTH];
    FILE *myfile = NULL;
    int R;
    
   
    printf("\nADMIN ONLY SCREEN\n++++++REGION IDS++++++\nPUNE - fiui57eofn\nMUMBAI - fnidd2764n\nNASHIK - eitnir4783\nNAGPUR - h9348jremh\n");
    printf("Enter your region code: "); scanf("%s", REGION);
    
   
    
    // +++++++++++++++ID DATABASE SELECT++++++++++++//
    if (areSame(REGION, "fiui57eofn"))
    {
        printf("PUNE REGION\n");
        myfile = fopen("ValidIDs.csv", "rb+");

        if (myfile == NULL)
        {
            printf("Trouble parsing through IDs. \nRetry again in some time....\n");
            exit(0); // or return 1
        }
        R = 0;
    }
    else if (areSame(REGION, "fnidd2764n"))
    {
        printf("MUMBAI REGION\n");
        myfile = fopen("ValidIDsMumbai.csv", "rb+");

        if (myfile == NULL)
        {
            printf("Trouble parsing through IDs. \nRetry again in some time....\n");
            exit(0); // or return 1
        }
        R = 1;
    }
    else if (areSame(REGION, "eitnir4783"))
    {
        printf("NASHIK REGION\n");
        myfile = fopen("ValidIDsNashik.csv", "rb+");

        if (myfile == NULL)
        {
            printf("Trouble parsing through IDs. \nRetry again in some time....\n");
            exit(0); // or return 1
        }
        R = 2;
    }
    else if (areSame(REGION, "h9348jremh"))
    {
        printf("NAGPUR REGION\n");
        myfile = fopen("ValidIDsNagpur.csv", "rb+");

        if (myfile == NULL)
        {
            printf("Trouble parsing through IDs. \nRetry again in some time....\n");
            exit(0); // or return 1
        }
        R = 3;
    }
    ///

    // +++++++++++++++PAST INPUT TRANS DATABASE SELECT++++++++++++//
    const char *sourcePath;
    const char *destinationPath;
    int result;

    destinationPath = "input.csv";
    // HERE IS MY INPUT FILE LOADER
    if (areSame(REGION, "fiui57eofn"))
    {
        sourcePath = "MERKLECSV/PUNEmerkle.csv";
    }
    if (areSame(REGION, "fnidd2764n"))
    {
        sourcePath = "MERKLECSV/MUMBAImerkle.csv";
    }
    if (areSame(REGION, "eitnir4783"))
    {
        sourcePath = "MERKLECSV/NASHIKmerkle.csv";
    }
    if (areSame(REGION, "h9348jremh"))
    {
        sourcePath = "MERKLECSV/NAGPURmerkle.csv";
    }
    
    
    // here we move contents from CITY to INPUT
    result = moveCSVContents(sourcePath, destinationPath);
    // here we move contents from INPUT to STRINGS
    count = readCSVFile(filePath, strings);
    
    int stringCount = readCSVFile(filePath, strings);
    if (stringCount <= 0)
    {
        printf("You're the first voter in the region!\n");
    }
    if (result != 0)
    {
        printf("An error occurred while moving the contents from city to input database.\n");
    }
    
    
    // +++++++++++++++INPUT DO WHILE LOOP++++++++++++//

    int flag;// for valid id
    do
    {
       
        char input[MAX_LENGTH];
        
        printf("\nVOTER SCREEN\nEnter your voter ID: ");
        scanf("%s", input);

        flag = IDauthenticator(myfile, input);
       
        if (flag == 1)
        {
            printf("Your ID is valid!\n");
        }
        else if (flag == -1)
        {
            printf("You have already voted!\n");
            flag = 0;
        }
        else
        {
            printf("Your ID is not valid...\n");
        }

        if (flag == 1)
            // +++++++++++++++OPTIONS WHEN VALIDATED++++++++++++//
        {
            printf("\n1. Candidate 1\n2. Candidate 2\n3. Candidate 3\n4. Candidate 4\n");
            printf("Enter your option: ");
            int option;
            scanf("%d", &option);
            if (option == 1 || option == 2 || option == 3 || option == 4)
            {
                // token system
                modifycsv(indexNumber-1 , R);
                
                if (option >= 1 && option <= 4)
                {
                    voteCounts[option - 1]++;
                    
                    // Write the updated vote counts to the CSV file
                    writeVoteCountsToCSV(votefile, voteCounts);
                }
            }

                else
                {
                    printf("Invalid option. Program terminating...\n");
                    break;
                }
            
            }

            input[strcspn(input, "\n")] = '\0'; // Remove newline character

            // +++++++++++++++TIMEHASHING++++++++++++//
            
            unsigned int inputHash = hash(input);
            unsigned int timeHash;
                char *currentTime;
            
            currentTime = getCurrentTime();
            printf("Current time: %s", currentTime);
            
            timeHash = hash(currentTime);
            unsigned int combinedHash = hash_function(inputHash, timeHash);
            
            // Array to store the combined hash as a string
            char combinedHashString[65];
            sprintf(combinedHashString, "%u", combinedHash);
            
            //put at end of strings
            strings[count] = malloc(sizeof(char) * (strlen(combinedHashString) + 1));
            strcpy(strings[count], combinedHashString);
            //put at end of input file
               move_string_to_csv(combinedHashString);

            count++;
            
            printf("Do you want to add another vote? (y/n): ");
            scanf(" %c", &loop);
            getchar(); // Consume the newline character

            processStrings(strings, count, R); // HERE IS WHERE THE ACTUAL MERKLE TREE GETS MADE

            
            // HERE U NEED TO MOVE PAST MERKLE TO OLDHASH CSV(storing occurs in processing)
            
            sourcePath = "CURRENTHASHES.csv";
            destinationPath = "OLDHASHES.csv";

            result = moveCSVContents(sourcePath, destinationPath);
            if (result != 0)
            {
                printf("An error occurred while moving the contents.\n");
            }

     
    } while (loop == 'y' || loop == 'Y');

    sourcePath = "input.csv";
    // HERE IS MY INPUT FILE TO CITY TRANSFER
    if (areSame(REGION, "fiui57eofn"))
    {
        destinationPath = "MERKLECSV/PUNEmerkle.csv";
    }
    if (areSame(REGION, "fnidd2764n"))
    {
        destinationPath = "MERKLECSV/MUMBAImerkle.csv";
    }
    if (areSame(REGION, "eitnir4783"))
    {
        destinationPath = "MERKLECSV/NASHIKmerkle.csv";
    }
    if (areSame(REGION, "h9348jremh"))
    {
        destinationPath = "MERKLECSV/NAGPURmerkle.csv";
    }
        
    result = moveCSVContents(sourcePath, destinationPath);

    if (result != 0)
    {
        printf("An error occurred while moving the contents from input to city database.\n");
    }
}

