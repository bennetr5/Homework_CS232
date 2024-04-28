#include "indexPage.h"
#include "crawler.h"

int main(int argc, char* argv[]) {
    if (argc < 4) {
        printf("Invalid Format ./prog argv[1] argv[2] argv[3]\n");
        return 1;
    }

    printf("Indexing...\n");

    struct listNode* pListStart = malloc(sizeof(struct listNode));
    if (pListStart == NULL) {
        fprintf(stderr, "ERROR: could not allocate memory\n");
        return 1;
    }
    pListStart->next = NULL;

    char* filePath = argv[1];
    long seed;
    if (argc >= 4) {
        seed = atol(argv[3]);
    }
    else {
        seed = time(NULL);
    }

    srand(seed);

    char startURL[MAX_ADDR_LENGTH];
    char destURL[MAX_ADDR_LENGTH];
    int maxHops, numHops;
    int MAX_N = atoi(argv[2]);
    int n = 0;

    char pageStr[BUFFER + 1];
    int strSize = 0;

    FILE* fp = fopen(filePath, "r");
    if (fp == NULL) {
        printf("Error opening file\n");
        free(pListStart); // Free allocated memory before exiting
        return 1;
    }

    int res = fscanf(fp, "%s %d", startURL, &maxHops);
    if (res != 2) {
        printf("Error reading file\n");
        fclose(fp);
        free(pListStart); // Free allocated memory before exiting
        return 1;
    }

    strncpy(pListStart->addr, startURL, MAX_ADDR_LENGTH);

    while (res == 2) {
        for (numHops = 1; numHops <= maxHops; numHops++) {
            int linkRes = getLink(startURL, destURL, MAX_ADDR_LENGTH);
            if (!linkRes) {
                break;
            }

            if (contains(pListStart, destURL)) {
                numHops--;
            }
            else {
                insertBack(pListStart, destURL);
                strncpy(startURL, destURL, MAX_ADDR_LENGTH);
            }
        }
        res = fscanf(fp, "%s %d", startURL, &maxHops);
    }

    fclose(fp); // Close file after reading

    while (pListStart != NULL) {
        printf("%s\n", pListStart->addr);
        strSize = getText(pListStart->addr, pageStr, BUFFER);
        struct trieNode* pageTrie = indexPage(pageStr, strSize);
        struct listNode* temp = pListStart->next; // Store the next pointer before freeing current node
        freeTrieMemory(pageTrie);
        free(pListStart);
        pListStart = temp; // Move to the next node
    }

    destroyList(pListStart); // Ensure list is properly deallocated

    return 0;
}
