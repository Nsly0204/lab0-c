#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"

/* Notice: sometimes, Cppcheck would find the potential NULL pointer bugs,
 * but some of them cannot occur. You can suppress them by adding the
 * following line.
 *   cppcheck-suppress nullPointer
 */

/* Create an empty queue */
struct list_head *q_new()
{
    struct list_head *head = malloc(sizeof(struct list_head));
    if (!head) {
        return NULL;
        // https://hackmd.io/@sysprog/linux-macro-containerof
    }
    INIT_LIST_HEAD(head);
    return head;
}

/* Free all storage used by queue */
void q_free(struct list_head *head)
{
    if (!head)
        return;

    element_t *entry = NULL, *safe = NULL;
    list_for_each_entry_safe (entry, safe, head, list) {
        q_release_element(entry);
    }
    free(head);
    return;
    // https://hackmd.io/@sysprog/linux-macro-containerof#Linux-核心原始程式碼巨集-container_of
    // www.catb.org/esr/structure-packing/
}

/* Insert an element at head of queue */
bool q_insert_head(struct list_head *head, char *s)
{
    if (!head || !s)
        return false;

    element_t *ele = malloc(sizeof(element_t));
    if (!ele)
        return false;

    char *val = malloc((strlen(s) + 1) * sizeof(char));
    if (!val) {
        free(ele);
        return false;
    }
    // https://hackmd.io/@sysprog/HJmB5x5Hn

    memcpy(val, s, strlen(s) + 1);
    ele->value = val;

    list_add(&ele->list, head);  // notice the priority of operators
    // https://stackoverflow.com/questions/62943547/how-does-the-list-add-function-for-linux-kernel-linked-lists-work

    return true;
}

/* Insert an element at tail of queue */
bool q_insert_tail(struct list_head *head, char *s)
{
    /*
    static inline void list_add_tail(struct list_head *node, struct list_head
    *head) { struct list_head *prev = head->prev;

    prev->next = node;
    node->next = head;
    node->prev = prev;
    head->prev = node;
    }
    */
    if (!head || !s)
        return false;

    element_t *ele = malloc(sizeof(element_t));
    if (!ele)
        return false;

    char *val = malloc((strlen(s) + 1) * sizeof(char));
    if (!val) {
        free(ele);
        return false;
    }

    memcpy(val, s, strlen(s) + 1);
    ele->value = val;
    list_add_tail(&ele->list, head);

    return true;
}

/* Remove an element from head of queue */
element_t *q_remove_head(struct list_head *head, char *sp, size_t bufsize)
{
    if (!head || list_empty(head))
        return NULL;
    element_t *tmp = list_first_entry(head, element_t, list);
    // compare list_del_init and list_del
    list_del_init(&tmp->list);
    // comparison of strlcpy/snprintf/memcpy
    // https://blog.csdn.net/xx326664162/article/details/72781528
    // https://hackmd.io/@sysprog/HJmB5x5Hn
    if (sp)
        snprintf(sp, bufsize, "%s", tmp->value);
    return tmp;
}

/* Remove an element from tail of queue */
element_t *q_remove_tail(struct list_head *head, char *sp, size_t bufsize)
{
    if (!head || list_empty(head))
        return NULL;
    element_t *tmp = list_last_entry(head, element_t, list);
    list_del_init(&tmp->list);
    if (sp)
        snprintf(sp, bufsize, "%s", tmp->value);
    return tmp;
}

/* Return number of elements in queue */
int q_size(struct list_head *head)
{
    if (!head)
        return 0;
    int len = 0;
    struct list_head *li;

    /*
    #define list_for_each(node, head) \
    for (node = (head)->next; node != (head); node = node->next)
    */
    list_for_each (li, head)
        len++;

    return len;
}

/* Delete the middle node in queue */
bool q_delete_mid(struct list_head *head)
{
    // https://leetcode.com/problems/delete-the-middle-node-of-a-linked-list/
    if (!head || list_empty(head))
        return false;
    struct list_head *right, *left;
    right = head->next;
    left = head->prev;

    while (right != left && left->prev != right) {
        right = right->next;
        left = left->prev;
    }

    list_del(left);
    q_release_element(list_entry(left, element_t, list));

    return true;
}

/* Delete all nodes that have duplicate string */
bool q_delete_dup(struct list_head *head)
{
    // https://leetcode.com/problems/remove-duplicates-from-sorted-list-ii/
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

    return true;
}

/* Swap every two adjacent nodes */
void q_swap(struct list_head *head)
{
    // https://leetcode.com/problems/swap-nodes-in-pairs/
    if (!head || list_empty(head))
        return;
    struct list_head *node, *safe;
    for (node = head->next, safe = node->next; node != head && safe != head;
         node = node->next,
        safe = node->next)  // equivalently shift by two node
    {
        // using list_for_each_safe() need extra if(second == head) break;
        node->prev->next = safe;
        safe->next->prev = node;

        node->next = safe->next;
        safe->prev = node->prev;

        node->prev = safe;
        safe->next = node;
    }
}

/* Reverse elements in queue */
void q_reverse(struct list_head *head)
{
    if (!head || list_empty(head))
        return;

    struct list_head *node, *safe;

    list_for_each_safe (node, safe, head) {
        list_move(node, head);
    }
}

/* Reverse the nodes of the list k at a time */
void q_reverseK(struct list_head *head, int k)
{
    // https://leetcode.com/problems/reverse-nodes-in-k-group/

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
}

/* Sort elements of queue in ascending/descending order */
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

/* replace func copy from list.h */
static inline void list_replace(struct list_head *old, struct list_head *new)
{
    new->next = old->next;
    new->next->prev = new;
    new->prev = old->prev;
    new->prev->next = new;
}

/* swap func copy from list.h */
static inline void list_swap(struct list_head *entry1, struct list_head *entry2)
{
    struct list_head *pos = entry2->prev;

    list_del(entry2);
    list_replace(entry1, entry2);
    if (pos == entry1)
        pos = entry2;
    list_add(entry1, pos);
}

/* Fisher-Yates Shuffle */
void q_shuffle(struct list_head *head)
{
    if (!head || list_empty(head) || list_is_singular(head))
        return;
    int cnt = q_size(head);
    struct list_head *entry = head->prev;
    struct list_head *safe = entry->prev;
    for (; cnt > 0; entry = safe, safe = safe->prev) {
        int idx = rand() % (cnt--);
        struct list_head *tmp = head;
        for (; idx > 0; tmp = tmp->next, idx--) {
        }
        if (tmp == entry)  // skip swap
            continue;
        list_swap(tmp, entry);
    }
}