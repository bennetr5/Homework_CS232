#include "crawler.h"

/*
 * returns 1 if the list starting at pNode contains the address addr,
 *    and returns 0 otherwise
 */
int contains(const struct listNode *pNode, const char *addr){
  // TODO: complete this
  if (pNode == NULL) {
    return 0;
  }
  
  if (strcmp(pNode->addr, addr) == 0) {
    return 1;
  }

  return contains(pNode->next, addr);
}
/*
 * inserts the address addr as a new listNode at the end of
 *    the list
 */
void insertBack(struct listNode *pNode, const char *addr){
  // TODO: complete this
  if (pNode == NULL) {
    return;
  }
  if (pNode->next == NULL) {
    struct listNode *newNode = malloc(sizeof(struct listNode));
    if (newNode == NULL) {
      printf("Couldn't allocate memory.\n");
      return;
    }
    
    strncpy(newNode->addr, addr, MAX_ADDR_LENGTH - 1);
    newNode->addr[MAX_ADDR_LENGTH - 1] = '\0';
    
    newNode->next = NULL;
    pNode->next = newNode;
  }

  else {
    insertBack(pNode->next, addr);
  }
}


/*
 * prints the addresses from pNode to the end of the list,
 *   one on each line
 */
void printAddresses(const struct listNode *pNode){
  // TODO: complete this
  if (pNode->next == NULL) {
    printf("%s\n", pNode->addr);
    return;
  }
  
  printf("%s\n", pNode->addr);

  printAddresses(pNode->next);
}

/*
 * frees the memory associated with this node and all subsequent nodes
 */
void destroyList(struct listNode *pNode){
  // TODO: complete this
  if (pNode == NULL) {
    return;
  }

  destroyList(pNode->next);
  free(pNode);
}
  


int getLink(const char* srcAddr, char* link, const int maxLinkLength){
  const int bufSize = 1000;
  char buffer[bufSize];

  int numLinks = 0;

  FILE *pipe;

  snprintf(buffer, bufSize, "curl -s \"%s\" | python3 getLinks.py", srcAddr);

  pipe = popen(buffer, "r");
  if(pipe == NULL){
    fprintf(stderr, "ERROR: could not open the pipe for command %s\n", buffer);
    return 0;
  }

  fscanf(pipe, "%d\n", &numLinks);

  if(numLinks > 0){
    int linkNum;
    double r = (double)rand() / ((double)RAND_MAX + 1.0);

    for(linkNum=0; linkNum<numLinks; linkNum++){
      fgets(buffer, bufSize, pipe);
      
      if(r < (linkNum + 1.0) / numLinks){
		    break;
      }
    }

    /* copy the address from buffer to link */
    strncpy(link, buffer, maxLinkLength);
    link[maxLinkLength-1] = '\0';
    
    /* get rid of the newline */
    {
      char* pNewline = strchr(link, '\n');
      if(pNewline != NULL){
		    *pNewline = '\0';
      }
    }
  }

  pclose(pipe);

  if(numLinks > 0){
    return 1;
  }
  else{
    return 0;
  }
}