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

/* Deletes all data in list and recycles memory
	return	NULL head pointer
*/
LIST *destroyList( LIST *pList);

int addNode( LIST *pList, tTOKEN *dataInPtr);

/* Removes data from list
	return	0 not found
			1 deleted
*/
int removeNode( LIST *pList, char *keyPtr, tTOKEN **dataOut);

/* interface to search function
	Argu	key being sought
	dataOut	contains found data
	return	1 successful
			0 not found
*/
int searchList( LIST *pList, char *pArgu, tTOKEN **pDataOut);

/* returns number of nodes in list
*/
int listCount( LIST *pList);

/* returns	1 empty
			0 list has data
*/
int emptyList( LIST *pList);

//int fullList( LIST *pList);

void printList( LIST *pList);
static int _insert( LIST *pList, NODE *pPre, tTOKEN *dataInPtr);

/* internal delete function
	deletes data from a list and saves the (deleted) data to dataOut
*/
static void _delete( LIST *pList, NODE *pPre, NODE *pLoc, tTOKEN **dataOutPtr);

static int _search( LIST *pList, NODE **pPre, NODE **pLoc, char *pArgu);

tTOKEN *createToken( char *str);

/* Deletes all data in token structure and recycles memory
	return	NULL head pointer
*/
tTOKEN *destroyToken( tTOKEN *pToken);

////////////////////////////////////////////////////////////////////////////////
int main( void)
{
	LIST *list;
	char str[1024];
	tTOKEN *pToken;
	int ret;

	// creates a null list
	list = createList();
	if (!list)
	{
		printf( "Cannot create list\n");
		return 100;
	}

	while(scanf( "%s", str) == 1)
	{
		pToken = createToken(str);

		// insert function call
		ret = addNode( list, pToken);

		//if (ret == 1) // duplicated
			//destroyToken( pToken);
	}
	// print function call
	//printList( list);

	//destroyList( list);

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
    ptr->freq=0;
    ptr->token=strdup(str); // must free after usage.
    return ptr;
}

/* Inserts data into list
	return	-1 if overflow
			0 if successful
			1 if duplicated key
*/
int addNode( LIST *pList, tTOKEN *dataInPtr){
    NODE *pPre, *pLoc;
    int found = _search(pList, &pPre, &pLoc, dataInPtr->token);
    if(found){ // dupe key
        return 1;
    }
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
    while(*pLoc != NULL && strcmp(
                (*pLoc)->dataPtr->token,
                pArgu
                ) < 0){ // proceed until token is not less than argu.

        *pPre = *pLoc;
        *pLoc = (*pLoc)->link;
    }

    int found;
    if(*pLoc == NULL){ // cannot be found.
        found = 0;
    }else if(strcmp(
            (*pLoc)->dataPtr->token,
            pArgu
            ) == 0){ // token is the same as the ARgu.
            found = 1;
    }else{ // token is larger than Argu.
        found = 0;
    }
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
        pNew->link = pList->head;
        pList->head = pNew;
    }else{
        pNew->link = pPre->link;
        pPre->link = pNew;
    }
    pList->count++;
    return 1;
}

/* prints data from list
*/
void printList( LIST *pList){
    NODE* curr = pList->head;
    while(curr){
        printf("%s\t%d\n",curr->dataPtr->);
        curr = curr->link;
    }
}

