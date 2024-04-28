#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#ifndef INDEXPAGE_H_
#define INDEXPAGE_H_
#define MAX_LEN 26
#define BUFFER 300000

struct trieNode {
  char letter;
  int count;
  int numOfChildren;
  struct trieNode* child[MAX_LEN];
};

struct trieNode* indexPage(char* url, int strSize);

void addWordOccurrence(const char* word, struct trieNode* root, int wordLength);

void printTrieContents(struct trieNode* root, char* buffer, int depth);

int freeTrieMemory(struct trieNode *root);

int getText(const char* srcAddr, char* buffer, const int bufSize);

int containsTrie(char ch, struct trieNode* root);

struct trieNode* createTrieNode();

int getIndex(char letter, struct trieNode* node);

#endif