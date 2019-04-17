// my macros
#define DEBUG 1
#define log(x) if(DEBUG) printf(x)
#define dbg if(DEBUG)

#include <stdlib.h> // malloc
#include <stdio.h>
#include <string.h> // strdup, strcmp
#include <ctype.h> // toupper

#define QUIT			1
#define FORWARD_PRINT	2
#define BACKWARD_PRINT	3
#define DELETE			4

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
    struct node	*llink;
    struct node	*rlink;
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
void printListR( LIST *pList);
static int _insert( LIST *pList, NODE *pPre, tTOKEN *dataInPtr);
static void _delete( LIST *pList, NODE *pPre, NODE *pLoc, tTOKEN **dataOutPtr);
static int _search( LIST *pList, NODE **pPre, NODE **pLoc, char *pArgu);
tTOKEN *createToken( char *str);
tTOKEN *destroyToken( tTOKEN *pToken);
// My additional functions
NODE *createNode(void);
NODE *destroyNode(NODE *pNode);

/* gets user's input
*/
int get_action()
{
    char ch;
    scanf( "%c", &ch);
    ch = toupper( ch);
    switch( ch)
    {
        case 'Q':
            return QUIT;
        case 'F':
            return FORWARD_PRINT;
        case 'B':
            return BACKWARD_PRINT;
        case 'D':
            return DELETE;
    }
    return 0; // undefined action
}

////////////////////////////////////////////////////////////////////////////////
int main( int argc, char **argv)
{
    LIST *list;
    char str[1024];
    tTOKEN *pToken;
    int ret;
    FILE *fp;

    if (argc != 2)
    {
        fprintf( stderr, "usage: %s FILE\n", argv[0]);
        return 1;
    }

    fp = fopen( argv[1], "rt");
    if (!fp)
    {
        fprintf( stderr, "Error: cannot open file [%s]\n", argv[1]);
        return 2;
    }

    // creates a null list
    list = createList();
    if (!list)
    {
    printf( "Cannot create list\n");
    return 100;
    }

    log("entering while loop\n");

    while(fscanf( fp, "%s", str) == 1)
    {
        pToken = createToken( str);
        log("create token\n");

        // insert function call
        ret = addNode( list, pToken);
        log("add node\n");

        if (ret == 1) // duplicated
        destroyToken( pToken);
    }

    fclose( fp);

    fprintf( stdout, "Select Q)uit, F)orward print, B)ackward print, D)elete: ");

    while (1)
    {
        int action = get_action();

        switch( action)
        {
            case QUIT:
                destroyList( list);
                return 0;

            case FORWARD_PRINT:
                printList( list);
                break;

            case BACKWARD_PRINT:
                //printListR( list);
                break;

            case DELETE:
                fprintf( stdout, "Input a string to delete: ");
                fscanf( stdin, "%s", str);
                int ret = removeNode( list, str, &pToken);
                if (ret)
                {
                fprintf( stdout, "%s deleted\n", pToken->token);
                destroyToken( pToken);
                }
                else fprintf( stdout, "%s not found\n", str);
                break;
        }

        if (action) fprintf( stdout, "Select Q)uit, F)orward print, B)ackward print, D)elete: ");
    }
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
    ptr->pos = NULL;
    ptr->head = NULL;
    ptr->rear = NULL;
    return ptr;
}

/* Deletes all data in list and recycles memory
   return	NULL head pointer
   */
LIST *destroyList( LIST *pList){
    NODE *delp;
    while(pList->head != NULL){
        delp = pList->head;
        pList->head = pList->head->rlink;
        destroyNode(delp);
    }
    pList->pos = NULL;
    pList->head = NULL;
    pList->rear = NULL;
    pList->count = 0;
    free(pList);
    return NULL;
}


/* returns number of nodes in list
*/
int listCount( LIST *pList){
    return pList->count;
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

/* prints data from list (forward)
*/
void printList( LIST *pList){
    NODE* curr = pList->head;
    while(curr){
        printf("%s\t%d\n", curr->dataPtr->token, curr->dataPtr->freq);
        printf("\n");
        curr = curr->rlink;
    }
}

/* prints data from list (backward)
*/
void printListR( LIST *pList){
    NODE *curr = pList->rear;
    while(curr){
        printf("%s\t%d\n", curr->dataPtr->token, curr->dataPtr->freq);
        printf("\n");
        curr = curr->llink;
    }
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

NODE *destroyNode(NODE * pNode){
    destroyToken(pNode->dataPtr);
    pNode->llink = NULL;
    pNode->rlink = NULL;
    free(pNode);
    return NULL;
}

NODE *createNode(void){
    NODE *ptr = malloc(sizeof *ptr);
    if(!ptr){
        return NULL;
    }
    ptr->dataPtr = NULL;
    ptr->llink = NULL;
    ptr->rlink = NULL;
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
        *pLoc = (*pLoc)->rlink;
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
    NODE *pNew = createNode(); // check memory overflow
    if(pNew == NULL){
        return 0;
    }

    pNew->dataPtr = dataInPtr;

    if(pPre == NULL){ // pNew must be the new head
        pNew->rlink = pList->head; // NULL
        pList->head = pNew;
    }else{
        pNew->rlink = pPre->rlink;
        pPre->rlink = pNew;
    }
    pList->count++;
    return 1;
    log("_insert end\n");
}

/* internal delete function
	deletes data from a list and saves the (deleted) data to dataOut
*/
static void _delete( LIST *pList, NODE *pPre, NODE *pLoc, tTOKEN **dataOutPtr){
    *dataOutPtr = pLoc->dataPtr;
    if(pPre == NULL){
        pList->head = pLoc->rlink;
    }else{
        pPre->rlink = pLoc->rlink;
    }
    destroyNode(pLoc);
    pList->count--;
}

