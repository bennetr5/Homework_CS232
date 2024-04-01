/* File: indexPage.c */
/* Author: Britton Wolfe */
/* Date: September 3rd, 2010 */

/* This program indexes a web page, printing out the counts of words on that page */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_LEN 26
#define BUFFER 300000 

/* TODO: structure definitions */
struct trieNode {
  char letter;
  int count;
  int numOfChildren;
  struct trieNode* child[MAX_LEN];
};

/* NOTE: int return values can be used to indicate errors (typically non-zero)
   or success (typically zero return value) */

/* TODO: change this return type */
struct trieNode* indexPage(char* url, int strSize);

void addWordOccurrence(const char* word, struct trieNode* root, int wordLength);

void printTrieContents(struct trieNode* root, char* buffer, int depth, const char* url, int* urlPrinted);

int freeTrieMemory(struct trieNode *root);

int getText(const char* srcAddr, char* buffer, const int bufSize);

int contains(char ch, struct trieNode* root);

struct trieNode* createTrieNode();

int getIndex(char letter, struct trieNode* node);

int main(int argc, char* argv[]) {
  /* TODO: write the (simple) main function
  
  argv[1] will be the URL to index, if argc > 1 */
  if (argc < 2) {
    printf("Invalid Format, need URL");
  } 
  const char* url = argv[1];

  char pageStr[BUFFER + 1];
  int strSize = getText(url, pageStr, BUFFER);
  int urlPrinted = 0;

  
  struct trieNode *pageTrie = indexPage(pageStr, strSize);

  printTrieContents(pageTrie, pageStr, 0, url, &urlPrinted);
  freeTrieMemory(pageTrie);

  return 0;
}

/* TODO: define the functions corresponding to the above prototypes */

/* TODO: change this return type */
struct trieNode * indexPage(char* url, int strSize) {
    if(url == NULL || url[0] == '\0') {
        fprintf(stderr, "Error: Empty URL or NULL string provided.\n");
        return NULL;
    }

    struct trieNode *rootNode = createTrieNode();
    if(rootNode == NULL) {
        fprintf(stderr, "Error: Memory allocation failed for root node.\n");
        return NULL;
    }

    char word[BUFFER];
    int wordLen = 0;
    int i = 0;

    while (url[i] != '\0') {
        if (isalpha(url[i])) {
            if (wordLen < BUFFER - 1) {
                word[wordLen++] = tolower(url[i]);
            } else {
                word[BUFFER - 1] = '\0'; 
                addWordOccurrence(word, rootNode, wordLen);
                wordLen = 0; 
            }
        } else {
            if (wordLen > 0) {
                word[wordLen] = '\0';
                addWordOccurrence(word, rootNode, wordLen);
                wordLen = 0; 
            }
        }
        i++;
    }

    if (wordLen > 0) {
        word[wordLen] = '\0';
        addWordOccurrence(word, rootNode, wordLen);
    }

    return rootNode; 
}

void addWordOccurrence(const char* word, struct trieNode* root, int wordLength) {
    struct trieNode* tNode = root;
    int index = 0;
    for (int i = 0; i < wordLength; i++) {
        char lower = tolower(word[i]);
        if (!contains(lower, tNode)) {
            for (index = 0; index < tNode->numOfChildren; index++) {
                if (lower < tNode->child[index]->letter) {
                    break;
                }
            }

            for (int j = tNode->numOfChildren; j > index; j--) {
                tNode->child[j] = tNode->child[j - 1];
            }

            tNode->child[index] = createTrieNode();
            tNode->child[index]->letter = lower;
            tNode->numOfChildren++;
        }

        tNode = tNode->child[getIndex(lower, tNode)];
    }
    tNode->count++;
}


void printTrieContents(struct trieNode* root, char* buffer, int depth, const char* url, int* urlPrinted) {
    if (root == NULL) {
        return;
    }

    if (root->count > 0 && depth > 0) {
        buffer[depth] = '\0';
        if (!(*urlPrinted)) {
            printf("%s\n", url);
            *urlPrinted = 1;
        }
        
        printf("%s: %d\n", buffer, root->count);
    }

    for (int i = 0; i < root->numOfChildren; i++) {
        buffer[depth] = root->child[i]->letter;
        printTrieContents(root->child[i], buffer, depth + 1, url, urlPrinted);
    }
}

int freeTrieMemory(struct trieNode *root) {
    if (root == NULL) {
        return 0;
    }

    for (int i = 0; i < root->numOfChildren; i++) {
        freeTrieMemory(root->child[i]);
    }
    free(root);
    return 1;
}


struct trieNode* createTrieNode() { 
  struct trieNode* node = malloc(sizeof(struct trieNode));
  if (node == NULL) {
    printf("failed to create node\n");
    return NULL;
  }

  node->numOfChildren = 0;
  for (int i = 0; i < MAX_LEN; i++) {
    node->child[i] = NULL;
  }
  return node;
}

int contains(char ch, struct trieNode* root) {
  for (int i = 0; i < root->numOfChildren; i++) {
    if (root->child[i]->letter == ch) {
      return 1;
    }
  }
  return 0;
}

int getIndex(char letter, struct trieNode* node) {
    for (int i = 0; i < node->numOfChildren; i++) {
        if (node->child[i]->letter == letter) {
            return i;
        }
    }
    return -1;
}

/* You should not need to modify this function */
int getText(const char* srcAddr, char* buffer, const int bufSize) {
  FILE *pipe;
  int bytesRead;

  snprintf(buffer, bufSize, "curl -s \"%s\" | python3 getText.py", srcAddr);

  pipe = popen(buffer, "r");
  if(pipe == NULL){
    fprintf(stderr, "ERROR: could not open the pipe for command %s\n",
	    buffer);
    return 0;
  }

  bytesRead = fread(buffer, sizeof(char), bufSize-1, pipe);
  buffer[bytesRead] = '\0';

  pclose(pipe);

  return bytesRead;
}
