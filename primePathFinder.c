/* 
 * File:   primePathFinder.c
 * Author: Rahmanidashti
 *
 * Created on March 14, 2018, 1:52 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

/* Global Variables and Structures*/
int V, E;
int* nodePtrs;
int* nodeAdjs;

/* Path structure */
struct path {
    int node;
    struct path *next;
};

/* Head and Tail for Paths List */
struct path *pathHead = NULL;
struct path *pathTail = NULL;

/* Path List structure */
struct path_list {
    struct path *path_head;
    struct path *path_tail;
    struct path_list *next;
};

/* Head and Tail for Path List Linked Lists */
struct path_list *path_list_head = NULL;
struct path_list *path_list_tail = NULL;

struct simplePath {
    struct path *sp_pathHead;
    struct path *sp_pathTail;
    int pathSize;
    struct simplePath *next;
};

/* Head and Tail for Simple Path List Linked Lists */
struct simplePath *simplePathHead = NULL;
struct simplePath *simplePathTail = NULL;

struct primePath {
    struct path *pp_pathHead;
    struct path *pp_pathTail;
    struct primePath *next;
};

/* Head and Tail for Prime Path List Linked Lists */
struct primePath *primePathHead = NULL;
struct primePath *primePathTail = NULL;

/* Function Prototypes */
void readFile();
void printArray();
void simplePathFinder();
void newPathElement(int node);
void newPathListElement(struct path* pathHead, struct path* pathTail);
void newSimplePathListElement(struct path* pathHead, struct path* pathTail, int nodeCount);
void newPrimePathListElement(struct path* pathHead, struct path* pathTail);
int visitChecker(struct path* path_head, int newNode);
void printList();
void printSimplePath();
void printPrimePath();
struct simplePath* SortedMerge(struct simplePath* a, struct simplePath* b);
void FrontBackSplit(struct simplePath* source, struct simplePath** frontRef, struct simplePath** backRef);
void MergeSort(struct simplePath** headRef);
int findList(struct path* first, struct path* second);
void primePath();
void simpleListFreer();
void primeListFreer();
void pathListFreer();
void emptyPath();

int main() {

    printf("Read File Started! \n");

    readFile();

    // printArray();

    printf("\nRead File Finished!");

    printf("\nSimple Path Finder Started!");

    simplePathFinder();

    // free all of the variables    
    printf("\nSimple Path Finder Finished!");
    // printSimplePath();
    printf("\nSimple Path List Sorter Started!");
    MergeSort(&simplePathHead);
    printf("\nSimple Path List Sorter Finished!");
    // printSimplePath();
    pathListFreer();
    printf("\nPrime Path Finder Started!");
    primePath();
    printf("\nPrime Path Finder Finished!");
    printPrimePath();    

    return (EXIT_SUCCESS);
}

void readFile() {
    FILE *fp;
    char* graphFile = "dataset/sample.gr";
    char line[128]; /* or other suitable maximum line size */
    char splitedWord[10][10];
    int i, j, ctr;
    int vertexIndex, preVertexIndex, ptrsToAdjsPointer = 0;

    fp = fopen(graphFile, "r");

    if (fp == NULL) {
        printf("Could not open file %s", graphFile);
        exit(1);
    }

    while (fgets(line, sizeof line, fp) != NULL) { /* read a line */
        if (line[0] == 'a') {
            j = 0;
            ctr = 0;
            for (i = 0; i <= (strlen(line)); i++) {
                // if space or NULL found, assign NULL into splitedWord[ctr]
                if (line[i] == ' ' || line[i] == '\0') {
                    splitedWord[ctr][j] = '\0';
                    ctr++; //for next word
                    j = 0; //for next word, init index to 0
                } else {
                    splitedWord[ctr][j] = line[i];
                    j++;
                }
            }
            /* Make Ptrs and Adjs matrices*/
            /* The first one is 1 and I managed it from index 1 in array*/
            vertexIndex = atoi(splitedWord[1]);
            if (vertexIndex != preVertexIndex) {
                if (vertexIndex != (preVertexIndex + 1)) {
                    int i;
                    for (i = preVertexIndex + 1; i < vertexIndex; i++) {
                        *(nodePtrs + i) = ptrsToAdjsPointer;
                    }
                }
                *(nodePtrs + vertexIndex) = ptrsToAdjsPointer;
                *(nodeAdjs + ptrsToAdjsPointer) = atoi(splitedWord[2]);
            } else {
                *(nodeAdjs + ptrsToAdjsPointer) = atoi(splitedWord[2]);
            }
            ptrsToAdjsPointer = ptrsToAdjsPointer + 1; /* Next place in nodeAdjs */
            preVertexIndex = vertexIndex;

        } else if (line[0] == 'p') {
            j = 0;
            ctr = 0;
            for (i = 0; i <= (strlen(line)); i++) {
                // if space or NULL found, assign NULL into splitedWord[ctr]
                if (line[i] == ' ' || line[i] == '\0') {
                    splitedWord[ctr][j] = '\0';
                    ctr++; //for next word
                    j = 0; //for next word, init index to 0
                } else {
                    splitedWord[ctr][j] = line[i];
                    j++;
                }
            }

            /* convert string to integer using atoi (stdlib.io) */
            V = atoi(splitedWord[2]);
            E = atoi(splitedWord[3]);

            printf("The number of Vertices are (%d) and Edges are (%d).\n", V, E);

            /* Create Ptrs and Adjs array*/
            nodePtrs = calloc((V + 2), sizeof (int));
            *(nodePtrs) = 0;
            nodeAdjs = calloc(E, sizeof (int));
        }
    }

    if (vertexIndex <= V) {
        int i;
        for (i = vertexIndex + 1; i <= V + 1; i++) {
            *(nodePtrs + i) = E;
        }
    }
    fclose(fp);
}

void printArray() {
    int i;
    for (i = 0; i <= V + 1; i++) {
        printf("*(nodePtrs + %d) = %d\n", i, *(nodePtrs + i));
    }

    for (i = 0; i < E; i++) {
        printf("*(nodeAdjs + %d) = %d\n", i, *(nodeAdjs + i));
    }
}

void newPathElement(int node) {

    struct path *rv = malloc(sizeof (struct path));
    rv->node = node;

    if (pathHead == NULL) {
        pathHead = rv;
        pathHead -> next = NULL;
        pathTail = pathHead;
    } else {
        pathTail -> next = rv;
        pathTail = rv;
        pathTail -> next = NULL;
    }
}

void newPathListElement(struct path *pathHead, struct path *pathTail) {

    struct path_list *rv = malloc(sizeof (struct path_list));
    rv -> path_head = pathHead;
    rv -> path_tail = pathTail;

    if (path_list_head == NULL) {
        path_list_head = rv;
        path_list_head -> next = NULL;
        path_list_tail = path_list_head;
    } else {
        path_list_tail -> next = rv;
        path_list_tail = rv;
        path_list_tail -> next = NULL;
    }
}

void newSimplePathListElement(struct path *pathHead, struct path *pathTail, int nodeCount) {

    struct simplePath *rv = malloc(sizeof (struct simplePath));
    rv -> sp_pathHead = pathHead;
    rv -> sp_pathTail = pathTail;
    rv -> pathSize = nodeCount;

    if (simplePathHead == NULL) {
        simplePathHead = rv;
        simplePathHead -> next = NULL;
        simplePathTail = simplePathHead;
    } else {
        simplePathTail -> next = rv;
        simplePathTail = rv;
        simplePathTail -> next = NULL;
    }
}

void newPrimePathListElement(struct path *pathHead, struct path *pathTail) {

    struct primePath *rv = malloc(sizeof (struct primePath));
    rv -> pp_pathHead = pathHead;
    rv -> pp_pathTail = pathTail;

    if (primePathHead == NULL) {
        primePathHead = rv;
        primePathHead -> next = NULL;
        primePathTail = primePathHead;
    } else {
        primePathTail -> next = rv;
        primePathTail = rv;
        primePathTail -> next = NULL;
    }
}

int visitChecker(struct path *path_head, int newNode) {
    struct path *current = path_head; // Initialize current
    while (current != NULL) {
        if (current->node == newNode) {
            return 1;
        }
        current = current->next;
    }
    return 0;
}

void copyPath(struct path *head, int newNode) {

    struct path *temp;
    // array of newHead and newTail
    struct path *newHead = NULL;
    struct path *newTail = NULL;

    while (head != NULL) {
        temp = (struct path *) malloc(sizeof (struct path));
        if (newHead == NULL) {
            newHead = temp;
            newHead -> node = head -> node;
            newHead -> next = NULL;
            newTail = newHead;
        } else {
            newTail -> next = temp;
            newTail = temp;
            newTail -> node = head -> node;
            newTail -> next = NULL;
        }
        head = head -> next;
    }

    temp = (struct path *) malloc(sizeof (struct path));

    if (temp == NULL) {
        printf("%d", errno);
    }

    newTail -> next = temp;
    newTail = temp;
    newTail -> node = newNode;
    newTail -> next = NULL;
    
    newPathListElement(newHead, newTail);
}

void copySimplePathWithNode(struct path *head, int newNode) {
    struct path *temp;
    // array of newHead and newTail
    struct path *newHead = NULL;
    struct path *newTail = NULL;
    int nodeCounter = 0;

    while (head != NULL) {
        temp = (struct path *) malloc(sizeof (struct path));
        nodeCounter = nodeCounter + 1;
        if (newHead == NULL) {
            newHead = temp;
            newHead -> node = head -> node;
            newHead -> next = NULL;
            newTail = newHead;
        } else {
            newTail -> next = temp;
            newTail = temp;
            newTail -> node = head -> node;
            newTail -> next = NULL;
        }
        head = head -> next;
    }
    temp = (struct path *) malloc(sizeof (struct path));
    nodeCounter = nodeCounter + 1;
    newTail -> next = temp;
    newTail = temp;
    newTail -> node = newNode;
    newTail -> next = NULL;

    newSimplePathListElement(newHead, newTail, nodeCounter);
}

void copySimplePathWithoutNode(struct path *head) {

    struct path *temp;
    // array of newHead and newTail
    struct path *newHead = NULL;
    struct path *newTail = NULL;
    int nodeCounter = 0;

    while (head != NULL) {
        temp = (struct path *) malloc(sizeof (struct path));
        nodeCounter = nodeCounter + 1;
        if (newHead == NULL) {
            newHead = temp;
            newHead -> node = head -> node;
            newHead -> next = NULL;
            newTail = newHead;
        } else {
            newTail -> next = temp;
            newTail = temp;
            newTail -> node = head -> node;
            newTail -> next = NULL;
        }
        head = head -> next;
    }
    newSimplePathListElement(newHead, newTail, nodeCounter);
}

void printList() {
    struct path_list *ptr = path_list_head;
    struct path *ptrPath;
    printf("\n[ ");
    //start from the beginning
    while (ptr != NULL) {
        ptrPath = ptr->path_head;
        while (ptrPath) {
            printf("(%d)-> ", ptrPath -> node);
            ptrPath = ptrPath -> next;
        }
        ptr = ptr -> next;
        printf("|");
    }
    printf(" ]");
}

void emptyPath() {
    struct path *temp;
    //start from the beginning
    while (pathHead != NULL) {
        temp = pathHead;
        pathHead = pathHead->next;
        free(temp);
    }
}

void printSimplePath() {
    struct simplePath *ptr = simplePathHead;
    struct path *ptrPath;
    printf("\nSimple Paths:\n[ ");
    //start from the beginning
    while (ptr != NULL) {
        ptrPath = ptr->sp_pathHead;
        printf("|%d| ", ptr->pathSize);
        while (ptrPath) {
            printf("(%d)->", ptrPath -> node);
            ptrPath = ptrPath -> next;
        }
        ptr = ptr -> next;
        printf(" - ");
    }
    printf(" ]");
}

void printPrimePath() {
    struct primePath *ptr = primePathHead;
    struct path *ptrPath;
    printf("\nPrime Paths: [ ");
    //start from the beginning
    while (ptr != NULL) {
        ptrPath = ptr->pp_pathHead;
        while (ptrPath) {
            printf("(%d)-> ", ptrPath -> node);
            ptrPath = ptrPath -> next;
        }
        ptr = ptr -> next;
        printf(" || ");
    }
    printf(" ]");
}

/* sorts the linked list by changing next pointers (not data) */
void MergeSort(struct simplePath** headRef) {
    struct simplePath* head = *headRef;
    struct simplePath* a;
    struct simplePath* b;

    /* Base case -- length 0 or 1 */
    if ((head == NULL) || (head->next == NULL)) {
        return;
    }

    /* Split head into 'a' and 'b' sublists */
    FrontBackSplit(head, &a, &b);

    /* Recursively sort the sublists */
    MergeSort(&a);
    MergeSort(&b);

    /* answer = merge the two sorted lists together */
    *headRef = SortedMerge(a, b);
}

struct simplePath* SortedMerge(struct simplePath* a, struct simplePath* b) {
    struct simplePath* result = NULL;

    /* Base cases */
    if (a == NULL)
        return (b);
    else if (b == NULL)
        return (a);

    /* Pick either a or b, and recur */
    if (a->pathSize >= b->pathSize) {
        result = a;
        result->next = SortedMerge(a->next, b);
    } else {
        result = b;
        result->next = SortedMerge(a, b->next);
    }
    return (result);
}

/* UTILITY FUNCTIONS */

/* Split the nodes of the given list into front and back halves,
    and return the two lists using the reference parameters.
    If the length is odd, the extra node should go in the front list.
    Uses the fast/slow pointer strategy. */
void FrontBackSplit(struct simplePath* source, struct simplePath** frontRef, struct simplePath** backRef) {
    struct simplePath* fast;
    struct simplePath* slow;
    slow = source;
    fast = source->next;

    /* Advance 'fast' two nodes, and advance 'slow' one node */
    while (fast != NULL) {
        fast = fast->next;
        if (fast != NULL) {
            slow = slow->next;
            fast = fast->next;
        }
    }
    /* 'slow' is before the midpoint in the list, so split it in two at that point. */
    *frontRef = source;
    *backRef = slow->next;
    slow->next = NULL;
}

void simplePathFinder() {
    /* As Pi list, each edge consider as a member of Pi*/
    int i, j, k;
    int firstPathNode, lastPathNode;

    for (i = 1; i <= V; i++) {
        printf("i= %d", i);
        for (j = *(nodePtrs + i); j <= (*(nodePtrs + i + 1) - 1); j++) {
            // printf("%d -> %d\n",i, *(nodeAdjs + j));

            /* Create the Path linked list */
            newPathElement(i);
            newPathElement(*(nodeAdjs + j));

            /* Is Pi a Cycle? */
            firstPathNode = pathHead -> node;
            lastPathNode = pathTail -> node;

            if (firstPathNode != lastPathNode) {
                /* Pi is not a Cycle */
                // check for there is any new node to visit?
                if (*(nodePtrs + lastPathNode) < *(nodePtrs + lastPathNode + 1)) {

                    // add new edge to explore
                    newPathListElement(pathHead, pathTail);

                    while (path_list_head != NULL) {
                        int firstNode = path_list_head -> path_head -> node;
                        int lastNode = path_list_head -> path_tail -> node;
                        for (k = *(nodePtrs + lastNode); k <= *(nodePtrs + lastNode + 1) - 1; k++) {
                            int nextNode = *(nodeAdjs + k);
                            // Check for Cycle and Visit a vertex in Pi
                            // If the above is false then add to Path List
                            // for cycle
                            if (firstNode != nextNode) {
                                // check for visit                                
                                if (!(visitChecker(path_list_head->path_head, nextNode))) {
                                    //check for: there is a node to extend                                    
                                    if (*(nodePtrs + nextNode) < *(nodePtrs + nextNode + 1)) {
                                        // now we will create a copy of this path with next node to add Path List
                                        // make a different copy address of path                                                                               
                                        copyPath(path_list_head->path_head, nextNode);
                                        // newPathListElement(newHead, newTail);                                        
                                    } else {
                                        // there is no new node to extend and will added to the simple path list                                        
                                        // add node to the Simple Path List                                        
                                        copySimplePathWithNode(path_list_head->path_head, nextNode);
                                    }
                                } else {
                                    // there is a visited node when you added new node at the end of path                                    
                                    // add node to the Simple Path List
                                    copySimplePathWithoutNode(path_list_head->path_head);
                                }
                            } else {
                                // there is a cycle when you added new node at the end of path
                                // add node to the Simple Path List                             
                                copySimplePathWithNode(path_list_head->path_head, nextNode);
                            }
                        } // End of 'for' loop
                        // after for loop the path_list head pointer will point to the next
                        struct path_list *temp;
                        temp = path_list_head;
                        path_list_head = path_list_head -> next;
                        free(temp);
                    } // End of the While
                    // You Must delete the Path to create new one!
                    // Print this list
                    emptyPath();
                } else {
                    // there is no any vertex to continue and visit
                    // will be added to the Simple Path List
                    // add node to the Simple Path List
                    copySimplePathWithoutNode(pathHead);
                    emptyPath();
                }
            } else {
                copySimplePathWithoutNode(pathHead);
                emptyPath();
            }
        }
    } // End of first For loop for Vertex    
}

void primePath() {

    struct path *sp_ph;
    struct path *sp_pt;
    struct path *pp_ph;

    sp_ph = simplePathHead -> sp_pathHead;
    sp_pt = simplePathHead -> sp_pathTail;

    // First and biggest simple path added to the prime path list
    newPrimePathListElement(sp_ph, sp_pt);

    // next simple path
    simplePathHead = simplePathHead -> next;

    // Save prime path head list to restart the head of list
    struct primePath *pph = primePathHead;
    int flag = 0;

    // pick simple path one by one
    while (simplePathHead != NULL) {

        sp_ph = simplePathHead->sp_pathHead;
        sp_pt = simplePathHead->sp_pathTail;

        // check for the sub-path of paths in Prime Path list        
        while (primePathHead != NULL) {
            pp_ph = primePathHead -> pp_pathHead;

            flag = findList(sp_ph, pp_ph);

            if (flag == 1) {
                break;
            }

            primePathHead = primePathHead -> next;
        } // End of while for Prime Path

        primePathHead = pph;

        if (flag == 0) {
            newPrimePathListElement(sp_ph, sp_pt);
        }

        flag = 0;

        simplePathHead = simplePathHead -> next;

    }
}

// Returns true if first list is present in second list
int findList(struct path *first, struct path *second) {
    struct path *ptr1 = first;
    struct path *ptr2 = second;

    // If both linked lists are empty, return true
    if (first == NULL && second == NULL)
        return 1;

    // Else If one is empty and other is not return
    // false
    if (first == NULL || (first != NULL && second == NULL))
        return 0;

    // Traverse the second list by picking nodes
    // one by one
    while (second != NULL) {
        // Initialize ptr2 with current node of second
        ptr2 = second;

        // Start matching first list with second list
        while (ptr1 != NULL) {
            // If second list becomes empty and first
            // not then return false
            if (ptr2 == NULL)
                return 0;

                // If data part is same, go to next
                // of both lists
            else if (ptr1->node == ptr2->node) {
                ptr1 = ptr1->next;
                ptr2 = ptr2->next;
            }// If not equal then  break the loop
            else
                break;
        }

        // Return true if first list gets traversed
        // completely that means it is matched.
        if (ptr1 == NULL)
            return 1;

        // Initialize ptr1 with first again
        ptr1 = first;

        // And go to next node of second list
        second = second->next;
    }
    return 0;
}

void simpleListFreer() {
    struct simplePath *temp;
    //start from the beginning
    while (simplePathHead != NULL) {
        temp = simplePathHead;
        simplePathHead = simplePathHead->next;
        free(temp);
    }
}

void primeListFreer() {
    struct primePath *temp;
    //start from the beginning
    while (primePathHead != NULL) {
        temp = primePathHead;
        primePathHead = primePathHead->next;
        free(temp);
    }
}

void pathListFreer() {
    
    struct path *temp;
    struct path *pathTemp;
    
    //start from the beginning
    while (path_list_head != NULL) {
        
        temp = path_list_head -> path_head;
        
        while (temp != NULL) {
            
            pathTemp = temp;
            
            temp = temp -> next;
            
            free(pathTemp);            
        }
        
        path_list_head = path_list_head->next;
        
        free(temp);
    }
}
