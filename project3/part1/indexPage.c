#include "indexPage.h"

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
                printf("\t%s\n", word);
                addWordOccurrence(word, rootNode, wordLen);
                wordLen = 0; 
            }
        } else {
            if (wordLen > 0) {
                word[wordLen] = '\0';
                printf("\t%s\n", word);
                addWordOccurrence(word, rootNode, wordLen);
                wordLen = 0; 
            }
        }
        i++;
    }

    if (wordLen > 0) {
        word[wordLen] = '\0';
        printf("\t%s\n", word);
        addWordOccurrence(word, rootNode, wordLen);
    }

    return rootNode; 
}

void addWordOccurrence(const char* word, struct trieNode* root, int wordLength) {
    struct trieNode* tNode = root;
    int index = 0;
    for (int i = 0; i < wordLength; i++) {
        char lower = tolower(word[i]);
        if (!containsTrie(lower, tNode)) {
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


void printTrieContents(struct trieNode* root, char* buffer, int depth) {
    if (root == NULL) {
        return;
    }

    if (root->count > 0 && depth > 0) {
        buffer[depth] = '\0';  
        printf("%s: %d\n", buffer, root->count);
    }

    for (int i = 0; i < root->numOfChildren; i++) {
        buffer[depth] = root->child[i]->letter;
        printTrieContents(root->child[i], buffer, depth + 1);
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
  node->count = 0;
  for (int i = 0; i < MAX_LEN; i++) {
    node->child[i] = NULL;
  }
  return node;
}

int containsTrie(char ch, struct trieNode* root) {
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

#include "indexPage.h"

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