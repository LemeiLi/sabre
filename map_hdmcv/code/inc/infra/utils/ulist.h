#ifndef __ULIST_H__
#define __ULIST_H__

/**
 * Simple doubly linked list implementation.
 *
 * Some of the internal functions ("__xxx") are useful when
 * manipulating whole lists rather than single entries, as
 * sometimes we already know the next/prev entries and we can
 * generate better code by using them directly rather than
 * using the generic single-entry routines.
 */

struct list_head {
	struct list_head *next, *prev;
};

#define LIST_HEAD_INIT(name) { &(name), &(name) }

#define LIST_HEAD(name) \
	struct list_head name = LIST_HEAD_INIT(name)

#define INIT_LIST_HEAD(ptr) do { \
	(ptr)->next = (ptr); (ptr)->prev = (ptr); \
} while (0)

/*
 * @brief Insert a new entry between two known consecutive entries. 
 *
 * @note This is only for internal list manipulation where we know
 * the prev/next entries already!
 */
static inline void __list_add(struct list_head *new,
			      struct list_head *prev,
			      struct list_head *next)
{
	next->prev = new;
	new->next = next;
	new->prev = prev;
	prev->next = new;
}

/**
 * @brief list_add - add a new entry
 * @param[in] new: new entry to be added
 * @param[in] head: list head to add it after
 *
 * @note Insert a new entry after the specified head.
 * This is good for implementing stacks.
 */
static inline void list_add(struct list_head *new, struct list_head *head)
{
	__list_add(new, head, head->next);
}

/**
 * @brief list_add_tail - add a new entry
 * @param[in] new: new entry to be added
 * @param[in] head: list head to add it before
 *
 * @note Insert a new entry before the specified head.
 * This is useful for implementing queues.
 */
static inline void list_add_tail(struct list_head *new, struct list_head *head)
{
	__list_add(new, head->prev, head);
}

/*
 * @brief Delete a list entry by making the prev/next entries
 * point to each other.
 *
 * @note This is only for internal list manipulation where we know
 * the prev/next entries already!
 */
static inline void __list_del(struct list_head *prev, struct list_head *next)
{
	if (next)
		next->prev = prev;
	if (prev)
		prev->next = next;
}

/**
 * @brief list_del - deletes entry from list.
 * @param[in] entry: the element to delete from the list.
 * @note list_empty on entry does not return true after this, the entry is in an undefined state.
 */
static inline void list_del(struct list_head *entry)
{
	__list_del(entry->prev, entry->next);
	entry->next = (void *) 0;
	entry->prev = (void *) 0;
}

/**
 * @brief list_del_init - deletes entry from list and reinitialize it.
 * @param[in] entry: the element to delete from the list.
 */
static inline void list_del_init(struct list_head *entry)
{
	__list_del(entry->prev, entry->next);
	INIT_LIST_HEAD(entry); 
}

/**
 * @brief list_move - delete from one list and add as another's head
 * @param[in] list: the entry to move
 * @param[in] head: the head that will precede our entry
 */
static inline void list_move(struct list_head *list, struct list_head *head)
{
        __list_del(list->prev, list->next);
        list_add(list, head);
}

/**
 * @brief list_move_tail - delete from one list and add as another's tail
 * @param[in] list: the entry to move
 * @param[in] head: the head that will follow our entry
 */
static inline void list_move_tail(struct list_head *list,
				  struct list_head *head)
{
        __list_del(list->prev, list->next);
        list_add_tail(list, head);
}

/**
 * @brief list_empty - tests whether a list is empty
 * @param[in] head: the list to test.
 */
static inline int list_empty(struct list_head *head)
{
	return head->next == head;
}

static inline void __list_splice(struct list_head *list,
				 struct list_head *head)
{
	struct list_head *first = list->next;
	struct list_head *last = list->prev;
	struct list_head *at = head->next;

	first->prev = head;
	head->next = first;

	last->next = at;
	at->prev = last;
}

/**
 * @brief list_splice - join two lists
 * @param[in] list: the new list to add.
 * @param[in] head: the place to add it in the first list.
 */
static inline void list_splice(struct list_head *list, struct list_head *head)
{
	if (!list_empty(list))
		__list_splice(list, head);
}

/**
 * @brief list_splice_init - join two lists and reinitialise the emptied list.
 * @param[in] list the new list to add.
 * @param[in] head the place to add it in the first list.
 *
 * @note The list at @p list is reinitialised
 */
static inline void list_splice_init(struct list_head *list,
				    struct list_head *head)
{
	if (!list_empty(list)) {
		__list_splice(list, head);
		INIT_LIST_HEAD(list);
	}
}

/**
 * @brief list_entry - get the struct for this entry
 * @param[in] ptr:	the &struct list_head pointer.
 * @param[in] type:	the type of the struct this is embedded in.
 * @param[in] member:	the name of the list_struct within the struct.
 */
#define list_entry(ptr, type, member) \
	((type *)((char *)(ptr)-(unsigned long)(&((type *)0)->member)))

/**
 * @brief list_for_each	-	iterate over a list
 * @param[in] pos:	the &struct list_head to use as a loop counter.
 * @param[in] head:	the head for your list.
 */
#define list_for_each(pos, head) \
	for (pos = (head)->next; pos != (head); \
        	pos = pos->next)
/**
 * @brief list_for_each_prev	-	iterate over a list backwards
 * @param[in] pos:	the &struct list_head to use as a loop counter.
 * @param[in] head:	the head for your list.
 */
#define list_for_each_prev(pos, head) \
	for (pos = (head)->prev; pos != (head); \
        	pos = pos->prev)
        	
/**
 * @brief list_for_each_safe	-	iterate over a list safe against removal of list entry
 * @param[in] pos:	the &struct list_head to use as a loop counter.
 * @param[in] n:		another &struct list_head to use as temporary storage
 * @param[in] head:	the head for your list.
 */
#define list_for_each_safe(pos, n, head) \
	for (pos = (head)->next, n = pos->next; pos != (head); \
		pos = n, n = pos->next)

/**
 * @brief list_for_each_entry	-	iterate over list of given type
 * @param[in] pos:	the type * to use as a loop counter.
 * @param[in] head:	the head for your list.
 * @param[in] member:	the name of the list_struct within the struct.
 */
#define list_for_each_entry(pos, head, member)				\
	for (pos = list_entry((head)->next, typeof(*pos), member);	\
	     &pos->member != (head); 					\
	     pos = list_entry(pos->member.next, typeof(*pos), member))

/**
 * @brief list_for_each_entry_safe - iterate over list of given type safe against removal of list entry
 * @param[in] pos:	  the type * to use as a loop counter.
 * @param[in] n:      another type * to use as temporary storage
 * @param[in] head:	  the head for your list.
 * @param[in] member: the name of the list_struct within the struct.
 */
#define list_for_each_entry_safe(pos, n, head, member)			\
	for (pos = list_entry((head)->next, typeof(*pos), member),	\
		n = list_entry(pos->member.next, typeof(*pos), member);	\
	     &pos->member != (head); 					\
	     pos = n, n = list_entry(n->member.next, typeof(*n), member))

/**
 * @brief list_for_each_entry_continue - iterate over list of given type 
 * continuing after existing point
 * @param[in] pos:     the type * to use as a loop counter.
 * @param[in] head:    the head for your list.
 * @param[in] member:  the name of the list_struct within the struct.
 */
#define list_for_each_entry_continue(pos, head, member)			\
	for (pos = list_entry(pos->member.next, typeof(*pos), member);	\
	     &pos->member != (head);					\
	     pos = list_entry(pos->member.next, typeof(*pos), member))


#endif

