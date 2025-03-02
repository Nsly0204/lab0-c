#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"

/* Create an empty queue */
struct list_head *q_new()
{
    return NULL;
}

/* Free all storage used by queue */
void q_free(struct list_head *head) {}

/* Insert an element at head of queue */
bool q_insert_head(struct list_head *head, char *s)
{
    return true;
}

/* Insert an element at tail of queue */
bool q_insert_tail(struct list_head *head, char *s)
{
    return true;
}

/* Remove an element from head of queue */
element_t *q_remove_head(struct list_head *head, char *sp, size_t bufsize)
{
    return NULL;
}

/* Remove an element from tail of queue */
element_t *q_remove_tail(struct list_head *head, char *sp, size_t bufsize)
{
    return NULL;
}

/* Return number of elements in queue */
int q_size(struct list_head *head)
{
    return -1;
}

/* Delete the middle node in queue */
bool q_delete_mid(struct list_head *head)
{
    // https://leetcode.com/problems/delete-the-middle-node-of-a-linked-list/
    return true;
}

/* Delete all nodes that have duplicate string */
bool q_delete_dup(struct list_head *head)
{
    // https://leetcode.com/problems/remove-duplicates-from-sorted-list-ii/
<<<<<<< HEAD
=======
    if (!head || list_empty(head))
        return false;
    if (list_is_singular(head))
        return true;

    bool dup = false;
    element_t *curr = list_entry(head->next, element_t, list);
    element_t *next = list_entry(curr->list.next, element_t, list);
    list_for_each_entry_safe (curr, next, head, list) {
        // only curr is removable
        if (&next->list != head && strcmp(curr->value, next->value) == 0) {
            // does &next->list != head necessary?
            dup = true;
            list_del_init(&curr->list);  // remove node
            q_release_element(curr);     // release memory
        } else if (dup) {  // handling the last duplication in the list
            list_del_init(&curr->list);
            q_release_element(curr);  // it include free(curr) free(curr->value)
            dup = false;
        }
    }
    // why recursion
    // https://hackmd.io/@sysprog/c-recursion

>>>>>>> e417627 (Help commit)
    return true;
}

/* Swap every two adjacent nodes */
void q_swap(struct list_head *head)
{
    // https://leetcode.com/problems/swap-nodes-in-pairs/
}

/* Reverse elements in queue */
void q_reverse(struct list_head *head) {}

/* Reverse the nodes of the list k at a time */
void q_reverseK(struct list_head *head, int k)
{
    // https://leetcode.com/problems/reverse-nodes-in-k-group/
<<<<<<< HEAD
=======

    // https://hackmd.io/@vax-r/linux2024-homework1#2024q1-Homework1-lab0
    if ((!head || list_empty(head)) || k <= 1)
        return;
    int times = q_size(head);
    struct list_head *tail;

    LIST_HEAD(tmp);
    LIST_HEAD(new_head);

    for (int i = 0; i < times; i += k) {
        int j = 0;
        list_for_each (tail, head) {
            if (j == k)
                break;
            j++;
        }
        list_cut_position(&tmp, head, tail->prev);
        q_reverse(&tmp);
        list_splice_tail_init(&tmp, &new_head);
    }
    list_splice_init(&new_head, head);
>>>>>>> e417627 (Help commit)
}

bool cmp(const char *s1, const char *s2)
{
    return strcmp(s1, s2) > 0 ? true : false;
}

void mergeTwoLists(struct list_head *L1, struct list_head *L2, bool descend)
{
    if (!L1 || !L2)
        return;
    struct list_head head;
    INIT_LIST_HEAD(&head);
    while (!list_empty(L1) && !list_empty(L2)) {
        element_t *e1 = list_first_entry(L1, element_t, list);
        element_t *e2 = list_first_entry(L2, element_t, list);
        struct list_head *node = (descend ^ (strcmp(e1->value, e2->value) < 0))
                                     ? L1->next
                                     : L2->next;
        list_move_tail(node, &head);
    }
    list_splice_tail_init(list_empty(L1) ? L2 : L1, &head);
    list_splice(&head, L1);
}
/* Sort elements of queue in ascending/descending order */
void q_sort(struct list_head *head, bool descend)
{
    // https://hackmd.io/IKsnn85aRHGMrNcRP7BJ1Q?view#2024q1-Homework1-lab0
    if (!head || list_empty(head))
        return;

    if (list_is_singular(head))
        return;

    struct list_head *slow = head;
    struct list_head const *fast = NULL;

    for (fast = head->next; fast != head && fast->next != head;
         fast = fast->next->next)
        slow = slow->next;
    struct list_head left;
    list_cut_position(&left, head, slow);
    q_sort(&left, descend);
    q_sort(head, descend);
    mergeTwoLists(head, &left, descend);
}

/* Remove every node which has a node with a strictly less value anywhere to
 * the right side of it */
int q_ascend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    if (!head || list_empty(head))
        return 0;
    element_t *right = list_entry(head->prev, element_t, list);
    element_t *left = list_entry(right->list.prev, element_t, list);
    while (&left->list != head) {
        if (strcmp(right->value, left->value) <= 0) {
            // right less or equal then left >> delete left
            list_del(&left->list);
            q_release_element(left);
        } else {
            right = left;  // shift to next
        }
        left = list_entry(right->list.prev, element_t, list);
    }

    return q_size(head);
}

/* Remove every node which has a node with a strictly greater value anywhere to
 * the right side of it */
int q_descend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    if (!head || list_empty(head))
        return 0;
    element_t *right = list_entry(head->prev, element_t, list);
    element_t *left = list_entry(right->list.prev, element_t, list);
    while (&left->list != head) {
        if (strcmp(right->value, left->value) >= 0) {
            // right greater or equal then left >> delete left
            list_del(&left->list);
            q_release_element(left);
        } else {
            right = left;  // shift to next
        }
        left = list_entry(right->list.prev, element_t, list);
    }

    return q_size(head);
    // https://hackmd.io/@weihsinyeh/SJUVTnEn6
    // queue [3 2] -> [2]
}

/* Merge all the queues into one sorted queue, which is in ascending/descending
 * order */
int __merge(struct list_head *l1, struct list_head *l2)
{
    if (!l1 || !l2)
        return 0;
    LIST_HEAD(tmp_head);
    while (!list_empty(l1) && !list_empty(l2)) {
        element_t *ele_1 = list_first_entry(l1, element_t, list);
        element_t *ele_2 = list_first_entry(l2, element_t, list);
        element_t *ele_min =
            strcmp(ele_1->value, ele_2->value) < 0 ? ele_1 : ele_2;
        list_move_tail(&ele_min->list, &tmp_head);
    }
    list_splice_tail_init(l1, &tmp_head);
    list_splice_tail_init(l2, &tmp_head);
    list_splice(&tmp_head, l1);
    return q_size(l1);
}
int q_merge(struct list_head *head, bool descend)
{
    // https://leetcode.com/problems/merge-k-sorted-lists/
<<<<<<< HEAD
    return 0;
}
=======
    if (!head || list_empty(head))
        return 0;
    else if (list_is_singular(head))
        return q_size(list_first_entry(head, queue_contex_t, chain)->q);
    int size = q_size(head);
    int count = (size % 2) ? size / 2 + 1 : size / 2;
    int queue_size = 0;
    for (int i = 0; i < count; i++) {
        queue_contex_t *first = list_first_entry(head, queue_contex_t, chain);
        queue_contex_t *second =
            list_entry(first->chain.next, queue_contex_t, chain);
        while (!list_empty(first->q) && !list_empty(second->q)) {
            queue_size = __merge(first->q, second->q);
            list_move_tail(&second->chain, head);
            first = list_entry(first->chain.next, queue_contex_t, chain);
            second = list_entry(first->chain.next, queue_contex_t, chain);
        }
    }
    return queue_size;
}
>>>>>>> e417627 (Help commit)
