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
    struct list_head *q_head = malloc(sizeof(struct list_head));
    if (!q_head)  // If allocate failed
        return NULL;
    INIT_LIST_HEAD(q_head);
    return q_head;
}

/* Free all storage used by queue */
void q_free(struct list_head *l)
{
    if (!l)
        return;
    element_t *c, *n;
    list_for_each_entry_safe (c, n, l, list) {
        list_del(&c->list);
        q_release_element(c);
    }
    free(l);  // q_new functiopn has malloced it
}
/* Insert an element at head of queue */
bool q_insert_head(struct list_head *head, char *s)
{
    if (!head)
        return false;
    element_t *new_element = malloc(sizeof(element_t));  // new head element
    if (!new_element)                                    // If allocate failed
        return false;
    size_t len = strlen(s) + 1;  // plus 1 for `\0`
    new_element->value = malloc(len * sizeof(char));
    if (!new_element->value) {  // If allocate failed
        free(new_element);
        return false;
    }
    memcpy(new_element->value, s, len);
    list_add(&new_element->list, head);
    return true;
}

/* Insert an element at tail of queue */
bool q_insert_tail(struct list_head *head, char *s)
{
    if (!head)
        return false;
    element_t *new_element = malloc(sizeof(element_t));  // new tail element
    if (!new_element)                                    // If allocate failed
        return false;
    size_t len = strlen(s) + 1;  // plus 1 for `\0`
    new_element->value = malloc(len * sizeof(char));
    if (!new_element->value) {  // If allocate failed
        free(new_element);
        return false;
    }
    memcpy(new_element->value, s, len);
    list_add_tail(&new_element->list, head);
    return true;
}

/* Remove an element from head of queue */
element_t *q_remove_head(struct list_head *head, char *sp, size_t bufsize)
{
    if (!head || list_empty(head))
        return NULL;
    element_t *rm_element = container_of(head->next, element_t, list);
    if (sp) {
        strncpy(sp, rm_element->value, bufsize - 1);
        sp[bufsize - 1] = '\0';
    }
    list_del(head->next);
    return rm_element;
}
/* Remove an element from tail of queue */
element_t *q_remove_tail(struct list_head *head, char *sp, size_t bufsize)
{
    if (!head || list_empty(head))
        return NULL;
    element_t *rm_element = list_last_entry(head, element_t, list);
    if (sp) {
        strncpy(sp, rm_element->value, bufsize - 1);
        sp[bufsize - 1] = '\0';
    }
    list_del(head->prev);
    return rm_element;
}

/* Return number of elements in queue */
int q_size(struct list_head *head)
{
    if (!head || list_empty(head))
        return 0;
    int cnt = 0;
    struct list_head *n;
    list_for_each (n, head)
        cnt++;

    return cnt;
}

/* Delete the middle node in queue */
bool q_delete_mid(struct list_head *head)
{
    if (!head || list_empty(head))
        return false;
    struct list_head *n = head->next;
    for (struct list_head *p = head->prev; n->next != p && n != p;) {
        n = n->next;
        p = p->prev;
    }
    list_del(n);
    q_release_element(list_entry(n, element_t, list));
    return true;
}

/* Delete all nodes that have duplicate string */
bool q_delete_dup(struct list_head *head)
{
    if (!head || list_empty(head))
        return false;
    element_t *c, *n;  // current and next element
    bool is_dup = false;
    list_for_each_entry_safe (c, n, head,
                              list) {  // current node (iterator) is allowed to
                                       // be removed from the list.
        if (c->list.next != head &&
            strcmp(c->value, n->value) == 0)  // duplicate string detection
        {
            list_del(&c->list);  // delete node
            q_release_element(c);
            is_dup = true;
        } else if (is_dup) {
            list_del(&c->list);
            q_release_element(c);
            is_dup = false;
        }
    }

    return true;
}

/* Swap every two adjacent nodes */
void q_swap(struct list_head *head)
{
    if (!head || list_empty(head))
        return;
    struct list_head *n = head->next;
    while (n != head && n->next != head) {
        struct list_head *t = n;
        list_move(n, t->next);
        n = n->next;
    }
}

/* Reverse elements in queue */
void q_reverse(struct list_head *head)
{
    if (!head || list_empty(head))
        return;
    struct list_head *n, *s, *t;
    list_for_each_safe (n, s, head) {
        t = n->next;
        n->next = n->prev;
        n->prev = t;
    }
    t = head->next;
    head->next = head->prev;
    head->prev = t;
}

/* Reverse the nodes of the list k at a time */
void q_reverseK(struct list_head *head, int k)
{
    // https://leetcode.com/problems/reverse-nodes-in-k-group/
}

/* Sort elements of queue in ascending order */
void q_sort(struct list_head *head) {}

/* Remove every node which has a node with a strictly greater value anywhere to
 * the right side of it */
int q_descend(struct list_head *head)
{
    struct list_head *c = head->prev, *n = c->prev;

    while (n != head) {
        if (strcmp(list_entry(n, element_t, list)->value,
                   list_entry(c, element_t, list)->value) < 0) {
            list_del(n);
        } else {
            c = n;
        }
        n = n->prev;
    }
    return q_size(head);
}

/* Merge all the queues into one sorted queue, which is in ascending order */
int q_merge(struct list_head *head)
{
    // https://leetcode.com/problems/merge-k-sorted-lists/
    return 0;
}
