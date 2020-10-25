#include "list.h"
#include <stdlib.h>

static Node nodes[LIST_MAX_NUM_NODES];
static List lists[LIST_MAX_NUM_HEADS];
static Node listsnodes[LIST_MAX_NUM_HEADS];

static List queueNode;
static List queueList;
static List usedList;
static bool isListIntialized = false;

void queueNode_create();
void queueNode_queue(Node * node);
Node * queueNode_dequeue();
void queueList_create();
void queueList_queue(List * list);
List * queueList_dequeue();
void usedList_queue(Node * node);
Node * usedList_dequeue();

// Make queue of empty nodes
void queueNode_create() {
    nodes[0].prev = NULL;
    queueNode.head = &nodes[0];

    for (int i = 1; i < LIST_MAX_NUM_NODES; i++) {
        nodes[i-1].next = &nodes[i];
        nodes[i].prev = &nodes[i-1];
    }

    nodes[LIST_MAX_NUM_NODES-1].next = NULL;
    queueNode.tail = &nodes[LIST_MAX_NUM_NODES-1];
    queueNode.count = LIST_MAX_NUM_NODES;
}

// Queue node back to queueNode
void queueNode_queue(Node * node) {
    node->data = NULL;

    if (queueNode.count == 0) {
        queueNode.head = node;
    } else {
        queueNode.tail->next = node;
    }
    queueNode.tail = node;
    queueNode.count++;
}

// Dequeue node from queueNode
Node * queueNode_dequeue() {
    if (queueNode.count == 0) {
        return NULL;
    }

    Node * node = queueNode.head;
    
    if (queueNode.count == 1) {
        queueNode.head = NULL;
        queueNode.tail = NULL;
    } else {
        queueNode.head = queueNode.head->next;
        queueNode.head->prev = NULL;
    }
    queueNode.count--;

    return node;
}

// Make queue of unused lists
void queueList_create() {
    listsnodes[0].prev = NULL;
    listsnodes[0].data = &lists[0];
    queueList.head = &listsnodes[0];

    for (int i = 1; i < LIST_MAX_NUM_HEADS; i++) {
        listsnodes[i-1].next = &listsnodes[i];
        listsnodes[i].data = &lists[i];
        listsnodes[i].prev = &listsnodes[i-1];
    }
    listsnodes[LIST_MAX_NUM_HEADS-1].next = NULL;
    queueList.tail = &listsnodes[LIST_MAX_NUM_HEADS-1];
    queueList.count = LIST_MAX_NUM_HEADS;
}

// Queue list to queueList
void queueList_queue(List * list) {
    Node * node = usedList_dequeue();
    
    if (queueList.count == 0) {
        queueList.head = node;
    } else {
        queueList.tail->next = node;
    }
    queueList.tail = node;
    queueList.count++;
}

// Dequeue list from queueList
List * queueList_dequeue() {
    if (queueList.count == 0) {
        return NULL;
    }

    Node * listnode = queueList.head;
    List * list = listnode->data;
    
    if (queueList.count == 1) {
        queueList.head = NULL;
        queueList.tail = NULL;
    } else {
        queueList.head = queueList.head->next;
        queueList.head->prev = NULL;
    }
    queueList.count--;
    usedList_queue(listnode);

    return list;
}

// Queue to nodes that held lists to usedList
void usedList_queue(Node * node) {
    if (usedList.count == 0) {
        usedList.head = node;
    } else {
        usedList.tail->next = node;
    }
    usedList.tail = node;
    usedList.count++;
}

// Dequeue from nodes that held lists to usedList
Node * usedList_dequeue() {
    if (usedList.count == 0) {
        return NULL;
    }

    Node * node = usedList.head;

    if (usedList.count == 1) {
        usedList.head = NULL;
        usedList.tail = NULL;
    } else {
        usedList.head = usedList.head->next;
        usedList.head->prev = NULL;
    }

    usedList.count--;
    return node;
}


// Makes a new, empty list, and returns its reference on success. 
// Returns a NULL pointer on failure.
List* List_create() {
    if (!isListIntialized) {
        queueNode_create();
        queueList_create();
        isListIntialized = true;
    }

    List * list = queueList_dequeue();

    if (!list) {
        return NULL;
    }

    list->head = NULL;
    list->tail = NULL;
    list->count = 0;
    list->currentNode = NULL;
    list->currentState = LIST_OOB_START;

    return list;
}

// Returns the number of items in pList.
int List_count(List* pList) {
    return pList->count;
}

// Returns a pointer to the first item in pList and makes the first item the current item.
// Returns NULL and sets current item to NULL if list is empty.
void* List_first(List* pList) {
    if (pList->count == 0) {
        pList->currentNode = NULL;
        return NULL;
    }

    pList->currentNode = pList->head;
    pList->currentState = 2;
    return pList->head->data;
}

// Returns a pointer to the last item in pList and makes the last item the current item.
// Returns NULL and sets current item to NULL if list is empty.
void* List_last(List* pList) {
    if (pList->count == 0) {
        pList->currentNode = NULL;
        return NULL;
    }

    pList->currentNode = pList->tail;
    pList->currentState = 2;
    return pList->tail->data;
}

// Advances pList's current item by one, and returns a pointer to the new current item.
// If this operation advances the current item beyond the end of the pList, a NULL pointer 
// is returned and the current item is set to be beyond end of pList.
void* List_next(List* pList) {

    // current already beyond tail of pList
    if (pList->currentState == LIST_OOB_END) {
        return NULL;
    }

    // current at tail of pList
    if (pList->tail == pList->currentNode) {
        pList->currentNode = NULL;
        pList->currentState = LIST_OOB_END;
        return NULL;
    }

    // current before head of pList
    if (pList->currentState == LIST_OOB_START) {
        pList->currentNode = pList->head;
        pList->currentState = 2;
    } else {
        pList->currentNode = pList->currentNode->next;
    }

    return pList->currentNode->data;
}

// Backs up pList's current item by one, and returns a pointer to the new current item. 
// If this operation backs up the current item beyond the start of the pList, a NULL pointer 
// is returned and the current item is set to be before the start of pList.
void* List_prev(List* pList) {

    // current already before head of pList
    if (pList->currentState == LIST_OOB_START) {
        return NULL;
    }

    // current at head of pList
    if (pList->head == pList->currentNode) {
        pList->currentNode = NULL;
        pList->currentState = LIST_OOB_START;
        return NULL;
    }

    // current beyond tail of pList
    if (pList->currentState == LIST_OOB_END) {
        pList->currentNode = pList->tail;
        pList->currentState = 2;
    } else {
        pList->currentNode = pList->currentNode->prev;
    }

    return pList->currentNode->data;

}

// Returns a pointer to the current item in pList.
void* List_curr(List* pList) {
    if (!pList->currentNode) {
        return NULL;
    }
    return pList->currentNode->data;
}

// Adds the new item to pList directly after the current item, and makes item the current item. 
// If the current pointer is before the start of the pList, the item is added at the start. If 
// the current pointer is beyond the end of the pList, the item is added at the end. 
// Returns 0 on success, -1 on failure.
int List_add(List* pList, void* pItem) {

    if (pList->currentState == LIST_OOB_END || pList->tail == pList->currentNode) {
        return List_append(pList, pItem);
    } else if (pList->currentState == LIST_OOB_START || pList->count == 0) {
        return List_prepend(pList, pItem);
    }

    Node * node = queueNode_dequeue();
    if (!node) {
        return LIST_FAIL;
    }

    node->data = pItem;
    node->prev = pList->currentNode;
    node->next = pList->currentNode->next;

    pList->currentNode->next->prev = node;
    pList->currentNode->next = node;
    pList->currentNode = node;

    pList->currentState = 2;
    pList->count++;

    return LIST_SUCCESS;
}

// Adds item to pList directly before the current item, and makes the new item the current one. 
// If the current pointer is before the start of the pList, the item is added at the start. 
// If the current pointer is beyond the end of the pList, the item is added at the end. 
// Returns 0 on success, -1 on failure.
int List_insert(List* pList, void* pItem) {

    if (pList->currentState == LIST_OOB_START || pList->head == pList->currentNode) {
        return List_prepend(pList, pItem);
    } else if (pList->currentState == LIST_OOB_END || pList->count == 0) {
        return List_append(pList, pItem);
    }

    Node * node = queueNode_dequeue();
    if (!node) {
        return LIST_FAIL;
    }

    node->data = pItem;
    node->prev = pList->currentNode->prev;
    node->next = pList->currentNode;

    pList->currentNode->prev->next = node;
    pList->currentNode->prev = node;
    pList->currentNode = node;

    pList->currentState = 2;
    pList->count++;

    return LIST_SUCCESS;
}

// Adds item to the end of pList, and makes the new item the current one. 
// Returns 0 on success, -1 on failure.
int List_append(List* pList, void* pItem) {
    Node * node = queueNode_dequeue();
    if (!node) {
        return LIST_FAIL;
    }
    
    node->next = NULL;
    node->data = pItem;

    if (pList->count == 0) {
        node->prev = NULL;
        pList->head = node;
    } else {
        node->prev = pList->tail;
        pList->tail->next = node;
    }

    pList->tail = node;
    pList->currentNode = node;
    pList->currentState = 2;
    pList->count++;
    
    return LIST_SUCCESS;
}

// Adds item to the front of pList, and makes the new item the current one. 
// Returns 0 on success, -1 on failure.
int List_prepend(List* pList, void* pItem) {
    Node * node = queueNode_dequeue();
    if (!node) {
        return LIST_FAIL;
    }

    node->prev = NULL;
    node->data = pItem;

    if (pList->count == 0) {
        node->next = NULL;
        pList->tail = node;
    } else {
        node->next = pList->head; 
        pList->head->prev = node;
    }

    pList->head = node;
    pList->currentNode = node;
    pList->currentState = 2;
    pList->count++;

    return LIST_SUCCESS;
}

// Return current item and take it out of pList. Make the next item the current one.
// If the current pointer is before the start of the pList, or beyond the end of the pList,
// then do not change the pList and return NULL.
void* List_remove(List* pList) {
    if (pList->count == 0) {
        return NULL;
    }

    if (pList->currentState == LIST_OOB_START || pList->currentState == LIST_OOB_END) {
        return NULL;
    }

    Node * removeNode = pList->currentNode;
    void * data = removeNode->data;

    if (pList->count == 1) {
        pList->head = NULL;
        pList->tail = NULL;
        pList->currentNode = NULL;

    } else {

        if (pList->tail == pList->currentNode) {
            pList->tail = pList->tail->prev;
            pList->tail->next = NULL;
            pList->currentNode = NULL;
            pList->currentState = LIST_OOB_END;

        } else if (pList->head == pList->currentNode) {
            pList->head = pList->head->next;
            pList->head->prev = NULL;
            pList->currentNode = pList->head;

        } else {
            pList->currentNode->prev->next = pList->currentNode->next;
            pList->currentNode->next->prev = pList->currentNode->prev;
            pList->currentNode = pList->currentNode->next;
        }   
    }

    queueNode_queue(removeNode);
    pList->count--;

    return data;
}

// Adds pList2 to the end of pList1. The current pointer is set to the current pointer of pList1. 
// pList2 no longer exists after the operation; its head is available
// for future operations.
void List_concat(List* pList1, List* pList2) {
    if (pList2->count != 0) {
        if (pList1->count == 0) {
            pList1->head = pList2->head;
        } else {
            pList1->tail->next = pList2->head;
            pList2->head->prev = pList1->tail;
        }

        pList1->count += pList2->count;
        pList1->tail = pList2->tail;
    }

    queueList_queue(pList2);
}

// Delete pList. pItemFreeFn is a pointer to a routine that frees an item. 
// It should be invoked (within List_free) as: (*pItemFreeFn)(itemToBeFreedFromNode);
// pList and all its nodes no longer exists after the operation; its head and nodes are 
// available for future operations.
void List_free(List* pList, FREE_FN pItemFreeFn) {
    void * data;
    Node * node;

    pList->currentNode = pList->head;

    for (int i = 0; i < pList->count; i++) {
        node = pList->currentNode;
        data = node->data;
        (*pItemFreeFn) (data);
        pList->currentNode = pList->currentNode->next;
        queueNode_queue(node);
    }

    queueList_queue(pList);
}

// Return last item and take it out of pList. Make the new last item the current one.
// Return NULL if pList is initially empty.
void* List_trim(List* pList) {

    if (pList->count == 0) {
        return NULL;
    }

    Node * removeNode = pList->tail;
    void * data = removeNode->data;

    if (pList->count == 1) {
        pList->head = NULL;
        pList->tail = NULL;
        pList->currentNode = NULL;
    } else {
        pList->tail = pList->tail->prev;
        pList->currentNode = pList->tail;
        pList->tail->next = NULL;
    }
    
    queueNode_queue(removeNode);
    pList->count--;

    return data;
}

// Search pList, starting at the current item, until the end is reached or a match is found. 
// In this context, a match is determined by the comparator parameter. This parameter is a
// pointer to a routine that takes as its first argument an item pointer, and as its second 
// argument pComparisonArg. Comparator returns 0 if the item and comparisonArg don't match, 
// or 1 if they do. Exactly what constitutes a match is up to the implementor of comparator. 
// 
// If a match is found, the current pointer is left at the matched item and the pointer to 
// that item is returned. If no match is found, the current pointer is left beyond the end of 
// the list and a NULL pointer is returned.
// 
// If the current pointer is before the start of the pList, then start searching from
// the first node in the list (if any).
void* List_search(List* pList, COMPARATOR_FN pComparator, void* pComparisonArg) {

    if (pList->currentState == LIST_OOB_END || pList->count == 0) {
        return NULL;
    }

    if (pList->currentState == LIST_OOB_START) {
        pList->currentNode = pList->head;
        pList->currentState = 2;
    }

    void * data;

    while (pList->currentNode) {
        data = pList->currentNode->data;

        bool isItemMatched = (*pComparator)(data, pComparisonArg);
        if (isItemMatched) {
            return data;
        }

        pList->currentNode = pList->currentNode->next;
    }

    pList->currentState = LIST_OOB_END;
    return NULL;
}