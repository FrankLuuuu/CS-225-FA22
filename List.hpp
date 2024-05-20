/**
 * @file list.cpp
 * Doubly Linked List (MP 3).
 */

template <class T>
List<T>::List() { 
  // @TODO: graded in MP3.1
    head_ = NULL;
    tail_ = NULL;
    length_ = 0;
}

// template <class T>
// List<T>::~List() { 
//   // @TODO: graded in MP3.1
//   _destroy();
// }

/**
 * Returns a ListIterator with a position at the beginning of
 * the List.
 */
template <typename T>
typename List<T>::ListIterator List<T>::begin() const {
  // @TODO: graded in MP3.1
  return List<T>::ListIterator(head_);
}

/**
 * Returns a ListIterator one past the end of the List.
 */
template <typename T>
typename List<T>::ListIterator List<T>::end() const {
  // @TODO: gra 5 in MP3.1
  return List<T>::ListIterator(NULL);
}


/**
 * Destroys all dynamically allocated memory associated with the current
 * List class.
 */
template <typename T>
void List<T>::_destroy() {
  /// @todo Graded in MP3.1
  ListNode* current = head_;

  while(current != NULL) {
    ListNode* next = current->next;
    delete current;
    current = next;
  }

  head_ = NULL;
  tail_ = NULL;
}

/**
 * Inserts a new node at the front of the List.
 * This function **SHOULD** create a new ListNode.
 *
 * @param ndata The data to be inserted.
 */
template <typename T>
void List<T>::insertFront(T const & ndata) {
  /// @todo Graded in MP3.1
  ListNode * newNode = new ListNode(ndata);

  if(head_ == NULL && tail_ == NULL) {
    head_ = newNode;
    tail_ = newNode;
    length_++;
    return;
  }

  head_->prev = newNode;
  newNode->next = head_;
  newNode->prev = NULL;

  head_ = newNode;
  length_++;
}

/**
 * Inserts a new node at the back of the List.
 * This function **SHOULD** create a new ListNode.
 *
 * @param ndata The data to be inserted.
 */
template <typename T>
void List<T>::insertBack(const T & ndata) {
  /// @todo Graded in MP3.1
  ListNode * newNode = new ListNode(ndata);

  if(head_ == NULL && tail_ == NULL) {
    head_ = newNode;
    tail_ = newNode;
    length_++;
    return;
  }

  tail_->next = newNode;
  newNode->next = NULL;
  newNode->prev = tail_;

  tail_ = newNode;
  length_++;
}

/**
 * Helper function to split a sequence of linked memory at the node
 * splitPoint steps **after** start. In other words, it should disconnect
 * the sequence of linked memory after the given number of nodes, and
 * return a pointer to the starting node of the new sequence of linked
 * memory.
 *
 * This function **SHOULD NOT** create **ANY** new List or ListNode objects!
 *
 * This function is also called by the public split() function located in
 * List-given.hpp
 *
 * @param start The node to start from.
 * @param splitPoint The number of steps to walk before splitting.
 * @return The starting node of the sequence that was split off.
 */
template <typename T>
typename List<T>::ListNode * List<T>::split(ListNode * start, int splitPoint) {
  /// @todo Graded in MP3.1
  if(start == NULL)
    return start;

  ListNode * curr = start;

  for (int i = 0; i < splitPoint && curr != NULL; i++) 
    curr = curr->next;

  if (curr != NULL) {
    if(curr->prev != NULL)
      curr->prev->next = NULL;
    curr->prev = NULL;
  }

  return curr;
}

/**
  * Modifies List using the rules for a TripleRotate.
  *
  * This function will to a wrapped rotation to the left on every three 
  * elements in the list starting for the first three elements. If the 
  * end of the list has a set of 1 or 2 elements, no rotation all be done 
  * on the last 1 or 2 elements.
  * 
  * You may NOT allocate ANY new ListNodes!
  */
template <typename T>
void List<T>::tripleRotate() {
  // @todo Graded in MP3.1

  // handling if there isn't enough nodes
  if(length_ < 3)
    return;

  // initialize variables
  int cycles = length_ / 3;
  ListNode * curr = head_;

  //assign head_ accordingly
  head_ = curr->next;

  // iterate each 3-node cycle
  for(int i = 0; i < cycles; i++) {
    bool end = length_ % 3 == 0 && i == cycles - 1;

    // initialize the three nodes
    ListNode* one = curr;
    ListNode* two = one->next;
    ListNode* three = two->next;

    // assign next/prev of surrounding nodes
    if(one->prev != NULL)
      one->prev->next = two;
    if(three->next != NULL)
      three->next->prev = one;

    // rearrange next and prev of the three nodes
    two->prev = one->prev;
    two->next = three;
    one->prev = three;
    one->next = three->next;
    three->prev = two;
    three->next = one;

    // move on to next three nodes
    curr = one->next;
    //assign tail_ accordingly
    if(end)
      tail_ = one;
  }
}


/**
 * Reverses the current List.
 */
template <typename T>
void List<T>::reverse() {
  reverse(head_, tail_);
}

/**
 * Helper function to reverse a sequence of linked memory inside a List,
 * starting at startPoint and ending at endPoint. You are responsible for
 * updating startPoint and endPoint to point to the new starting and ending
 * points of the rearranged sequence of linked memory in question.
 *
 * @param startPoint A pointer reference to the first node in the sequence
 *  to be reversed.
 * @param endPoint A pointer reference to the last node in the sequence to
 *  be reversed.
 */
template <typename T>
void List<T>::reverse(ListNode *& startPoint, ListNode *& endPoint) {
  /// @todo Graded in MP3.2

  // handling if paramenters are invalid
  if(startPoint == endPoint || startPoint == NULL || endPoint == NULL) 
    return;

  ListNode* curr = startPoint;
  ListNode* temp;
  ListNode* startPrevious = startPoint->prev;
  ListNode* endNext = endPoint->next;

  // switch next and prev pointers of each node
  while (curr != endNext) {
      temp = curr->prev;
      curr->prev = curr->next;
      curr->next = temp;
      curr = curr->prev;
  }

  // assign next and prev of node before startPoint
  if(startPrevious != NULL) {
    startPrevious->next = endPoint;
    endPoint->prev = startPrevious;
  } 
  else
    endPoint->prev = NULL;

  // assign prev and next of node after endPoint
  if(endNext !=NULL) {
    startPoint->next = endNext;
    endNext->prev = startPoint;
  } 
  else 
    startPoint->next = NULL;

  // switch start and end points
  temp = startPoint;
  startPoint = endPoint;
  endPoint = temp;

  // asssign new head_ and tail_ accordingly
  if (head_ == endPoint) 
    head_ = startPoint;
  if (tail_ == startPoint) 
    tail_ = endPoint;
}

/**
 * Reverses blocks of size n in the current List. You should use your
 * reverse( ListNode * &, ListNode * & ) helper function in this method!
 *
 * @param n The size of the blocks in the List to be reversed.
 */
template <typename T>
void List<T>::reverseNth(int n) {
  /// @todo Graded in MP3.2

  // handling if the list doesn't need to be changed
  if (n < 2)
    return;
    
  // handling if size of blocks is greater length of list
  if(n >= length_) {
    reverse(head_, tail_);
    return;
  }

  // iterate through the list
  ListNode* start = head_;
  while(start != NULL) {
    ListNode* end = start;

    // go through each block
    for (int i = 1; i < n; i++)
      if (start != tail_)
        start = start->next;
      // check if the block ends prematurely
      else {
        reverse(end, tail_);
        return;
      }

    // reverse this block
    reverse(end, start);
    // move on to next block
    start = start->next;
  }
}


/**
 * Merges the given sorted list into the current sorted list.
 *
 * @param otherList List to be merged into the current list.
 */
template <typename T>
void List<T>::mergeWith(List<T> & otherList) {
    // set up the current list
    head_ = merge(head_, otherList.head_);
    tail_ = head_;

    // make sure there is a node in the new list
    if (tail_ != NULL) {
        while (tail_->next != NULL)
            tail_ = tail_->next;
    }
    length_ = length_ + otherList.length_;

    // empty out the parameter list
    otherList.head_ = NULL;
    otherList.tail_ = NULL;
    otherList.length_ = 0;
}

/**
 * Helper function to merge two **sorted** and **independent** sequences of
 * linked memory. The result should be a single sequence that is itself
 * sorted.
 *
 * This function **SHOULD NOT** create **ANY** new List objects.
 *
 * @param first The starting node of the first sequence.
 * @param second The starting node of the second sequence.
 * @return The starting node of the resulting, sorted sequence.
 */
template <typename T>
typename List<T>::ListNode * List<T>::merge(ListNode * first, ListNode* second) {
  /// @todo Graded in MP3.2

  // handling if a list is empty
  if(first == NULL) 
    return second;
  if(second == NULL) 
    return first;
  
  // initialize variables to assign the head_
  bool firstHead = false;
  ListNode* firstNode = first;
  ListNode* secondNode = second;
  ListNode* returnNode;

  // store the first node and assign head_
  if(first->data < second->data) {
    firstHead = true;
    returnNode = firstNode;
    firstNode = firstNode->next;
  }
  else {
    returnNode = secondNode;
    secondNode = secondNode->next;
  }
  if(firstHead) 
    head_ = first;
  else 
    head_ = second;

  // initialize variables to sort
  bool firstNext = false;
  ListNode* curr;

  // iterate until loops are empty
  while(firstNode != NULL || secondNode != NULL) {
    // whether to use the list represented by first
    if (secondNode == NULL || (firstNode != NULL && firstNode->data < secondNode->data))
      firstNext = true;
      
    // store the smaller node to the return list and assign next and prev
    if(firstNext) {
      curr = firstNode;
      curr->prev = returnNode;
      returnNode->next = firstNode;

      firstNode = firstNode->next;
      firstNext = false;
    }
    else {
      curr = secondNode;
      curr->prev = returnNode;
      returnNode->next = secondNode;

      secondNode = secondNode->next;
    }
    returnNode = returnNode->next;
  }

  return head_;
}

/**
 * Sorts a chain of linked memory given a start node and a size.
 * This is the recursive helper for the Mergesort algorithm (i.e., this is
 * the divide-and-conquer step).
 *
 * Called by the public sort function in List-given.hpp
 *
 * @param start Starting point of the chain.
 * @param chainLength Size of the chain to be sorted.
 * @return A pointer to the beginning of the now sorted chain.
 */
template <typename T>
typename List<T>::ListNode* List<T>::mergesort(ListNode * start, int chainLength) {
  /// @todo Graded in MP3.2

  // handling if the list is less than 2
  if(chainLength <= 1)
    return start;

  // split the list in half and recursively sort each half
  ListNode* secondStart = split(start, chainLength / 2);
  ListNode* firstHalf = mergesort(start, chainLength / 2);
  ListNode* secondHalf = mergesort(secondStart, chainLength - chainLength / 2);

  // merge nodes starting from single nodes
  return merge(firstHalf, secondHalf);
}
