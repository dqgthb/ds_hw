#define DEBUG 0
#define log(x) if(DEBUG) printf(x)
#define dbg if(DEBUG)
#include <stdlib.h> // malloc
#include <stdio.h>
#include <string.h> // strdup, strcmp

// User structure type definition
typedef struct
{
	char	*token;
	int		freq;
} tTOKEN;

////////////////////////////////////////////////////////////////////////////////
// LIST type definition
typedef struct node
{
	tTOKEN		*dataPtr;
	struct node	*link;
} NODE;

typedef struct
{
	int		count;
	NODE	*pos;
	NODE	*head;
	NODE	*rear;
} LIST;

////////////////////////////////////////////////////////////////////////////////
// Prototype declarations

LIST *createList( void);
LIST *destroyList( LIST *pList);
int addNode( LIST *pList, tTOKEN *dataInPtr);
int removeNode( LIST *pList, char *keyPtr, tTOKEN **dataOut);
int searchList( LIST *pList, char *pArgu, tTOKEN **pDataOut);
int listCount( LIST *pList);
int emptyList( LIST *pList);
//int fullList( LIST *pList);
void printList( LIST *pList);
static int _insert( LIST *pList, NODE *pPre, tTOKEN *dataInPtr);
static void _delete( LIST *pList, NODE *pPre, NODE *pLoc, tTOKEN **dataOutPtr);
static int _search( LIST *pList, NODE **pPre, NODE **pLoc, char *pArgu);
tTOKEN *createToken( char *str);
tTOKEN *destroyToken( tTOKEN *pToken);
// What I created in addition
NODE *createNode(void);
NODE *destroyNode(NODE * node);



////////////////////////////////////////////////////////////////////////////////
int main( void)
{
	LIST *list;
	char str[1024];
	tTOKEN *pToken;
	int ret;

	// creates a null list
	list = createList();
    log("list created!\n");

	if (!list)
	{
		printf( "Cannot create list\n");
		return 100;
	}

	while(scanf( "%s", str) == 1)
	{
        dbg printf("str is : %s\n", str);

		pToken = createToken(str);
        log("after create return main\n");

		// insert function call
		ret = addNode( list, pToken);

		if (ret == 1) // duplicated
			destroyToken( pToken);
	}
	// print function call
	printList( list);
	destroyList( list);

	return 0;
}

/* Allocates dynamic memory for a list head node and returns its address to caller
	return	head node pointer
			NULL if overflow
*/
LIST *createList( void){
    LIST *ptr = malloc(sizeof *ptr);
    if(!ptr){
        return NULL;
    }
    ptr->count = 0;
    ptr->head = NULL;
    return ptr;
}

/* Allocates dynamic memory for a token structure, initialize fields(token, freq) and returns its address to caller
	return	token structure pointer
			NULL if overflow
*/
tTOKEN *createToken( char *str){
    tTOKEN *ptr = malloc(sizeof *ptr);
    if(!ptr){
        return NULL;
    }
    dbg printf("token created with %s\n", str);
    ptr->freq=1;
    ptr->token=strdup(str);
    return ptr;
}

/* Inserts data into list
	return	-1 if overflow
			0 if successful
			1 if duplicated key
*/
int addNode( LIST *pList, tTOKEN *dataInPtr){
    NODE *pPre=NULL;
    NODE *pLoc=NULL;
    int found = _search(pList, &pPre, &pLoc, dataInPtr->token);
    dbg printf("_search end. pPre: %p, pLoc: %p\n", pPre, pLoc);
    dbg printf("found: %d\n", found);
    if(found){ // dupe key
        pLoc->dataPtr->freq++;
        destroyToken(dataInPtr);
        return 1;
    }

    // pPre contains info about location
    int res = _insert(pList, pPre, dataInPtr);
    if(res == 0){ // overflow
        return -1;
    }else{ // successful
        return 0;
    }
}

/* internal search function
	searches list and passes back address of node
	containing target and its logical predecessor
	return	1 found
			0 not found
*/

static int _search( LIST *pList, NODE **pPre, NODE **pLoc, char *pArgu){
    *pPre = NULL;
    *pLoc = pList->head;
    while(
            (*pLoc != NULL) &&
            (strcmp((*pLoc)->dataPtr->token, pArgu) < 0)
            ){ // proceed until token is not less than argu.
        *pPre = *pLoc;
        *pLoc = (*pLoc)->link;
    }

    // pPre < token < pLoc
    // *pPre == NULL if the node with a new token must be the new head.

    int found = 0;
    if((*pLoc != NULL) &&
            (strcmp((*pLoc)->dataPtr->token, pArgu) == 0)
            ){ // token is the same as the ARgu.
            found = 1;
    }
    log("_search returning...");
    return found;
}

/* internal insert function
	inserts data into a new node
	return	1 if successful
			0 if memory overflow
*/
static int _insert( LIST *pList, NODE *pPre, tTOKEN *dataInPtr){
    NODE *pNew = malloc(sizeof *pNew);
    // check memory overflow
    if(pNew == NULL){
        return 0;
    }

    pNew->dataPtr = dataInPtr;

    if(pPre == NULL){ // pNew must be the new head
        pNew->link = pList->head; // NULL
        pList->head = pNew;
    }else{
        pNew->link = pPre->link;
        pPre->link = pNew;
    }
    pList->count++;
    return 1;
    log("_insert end\n");
}

/* prints data from list
*/
void printList( LIST *pList){
    NODE* curr = pList->head;
    while(curr){
        printf("%s %d\n",curr->dataPtr->token, curr->dataPtr->freq);
        printf("\n");
        curr = curr->link;
    }
}

/* returns	1 empty
			0 list has data
*/
int emptyList( LIST *pList){
    if(pList->count == 0){
        return 1;
    }else{
        return 0;
    }
}

/* returns number of nodes in list
*/
int listCount( LIST *pList){
    return pList->count;
}

/* Deletes all data in token structure and recycles memory
	return	NULL head pointer
*/
tTOKEN *destroyToken( tTOKEN *pToken){
    free(pToken->token);
    pToken->token = NULL;
    pToken->freq = 0;
    free(pToken);
    return NULL;
}

/* interface to search function
	Argu	key being sought
	dataOut	contains found data
	return	1 successful
			0 not found
*/
int searchList( LIST *pList, char *pArgu, tTOKEN **pDataOut){
    NODE *pPre = NULL;
    NODE *pLoc = NULL;
    int found = _search(pList, &pPre, &pLoc, pArgu);
    if(found){
        *pDataOut = pLoc->dataPtr;
    }else{
        *pDataOut = NULL;
    }
    return found;
}

/* Removes data from list
	return	0 not found
			1 deleted
*/
int removeNode( LIST *pList, char *keyPtr, tTOKEN **dataOut){
    NODE *pPre = NULL;
    NODE *pLoc = NULL;
    int found = _search(pList, &pPre, &pLoc, keyPtr);
    if(found){
        _delete(pList, pPre, pLoc, dataOut);
        return 1;
    }
    return 0;
}

/* internal delete function
	deletes data from a list and saves the (deleted) data to dataOut
*/
static void _delete( LIST *pList, NODE *pPre, NODE *pLoc, tTOKEN **dataOutPtr){
    *dataOutPtr = pLoc->dataPtr;
    if(pPre == NULL){
        pList->head = pLoc->link;
    }else{
        pPre->link = pLoc->link;
    }
    destroyNode(pLoc);
    pList->count--;
}

/* Deletes all data in list and recycles memory
	return	NULL head pointer
*/
LIST *destroyList( LIST *pList){
    NODE *delp;
    while(pList->head != NULL){
        delp = pList->head;
        pList->head = pList->head->link;
        destroyNode(delp);
    }
    pList->pos = NULL;
    pList->count = 0;
    free(pList);
    return NULL;
}

NODE *destroyNode(NODE * node){
    destroyToken(node->dataPtr);
    node->link = NULL;
    free(node);
    return NULL;
}
NODE *createNode(void){
    NODE *ptr = malloc(sizeof *ptr);
    if(!ptr){
        fprintf(stderr, "FATAL: malloc fail(createNode).");
        abort();
    }
    ptr->dataPtr = NULL;
    ptr->link = NULL;
    return ptr;
}

