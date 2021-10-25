#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "songs.h"

#define PRINT_FREES

struct song_node * insert_front(struct song_node * head, char *artist, char *name) {
    struct song_node *result = malloc(sizeof(struct song_node));
    
    strncpy(result->artist, artist, NAME_LEN);
    result->artist[NAME_LEN] = 0;
    
    strncpy(result->name, name, NAME_LEN);
    result->name[NAME_LEN] = 0;
    
    result->next = head;
    
    return result;
}

void print_node(struct song_node *song) {
	if (song)
		printf("%s: %s", song->artist, song->name);
	else
		printf("(nil)");
}

void print_list(struct song_node *head) {
	if (head) do {
		print_node(head);
	} while ((head = head->next) && printf(" | "));
	printf("\n");
}

struct song_node * free_node(struct song_node *head) {
	struct song_node *next = head->next;
#ifdef PRINT_FREES
	printf("free_node - ");
	println_node(head);
#endif
	free(head);
	return next;
}

struct song_node * free_list(struct song_node *head) {
	if (head) 
		while (head = free_node(head));
	return head;
	// struct song_node *next;
	// if (head) do {
	// 	next = head->next;
	// 	//printf("freeing node: ");
	// 	//println_node(head);
	// 	free(head);
	// } while (head = next);
}

struct song_node * insert_ordered(struct song_node *head, struct song_node *to_insert) {
	if (!head) return to_insert;
	// if head > to_insert (as in, head should be *after* to_insert)
	if (songcmp(head, to_insert) > 0) {
		to_insert->next = head;
		return to_insert;
	} else {
		head->next = insert_ordered(head->next, to_insert);
		return head;
	}
}

int songcmp(struct song_node *a, struct song_node *b) {
	if (!b) return 1;
	int result;
	if (result = strncasecmp(a->artist, b->artist, NAME_LEN))
		return result;
	return strncasecmp(a->name, b->name, NAME_LEN);
}

struct song_node * find_artist(struct song_node *head, char *artist) {
	// while pointing to a song whose artist is not the one we're looking for:
	while (head && strncasecmp(head->artist, artist, NAME_LEN)) head = head->next;
	return head;
}

struct song_node * find_song(struct song_node *head, char *artist, char *name) {
	while (head && (strncasecmp(head->artist, artist, NAME_LEN)
				|| strncasecmp(head->name, name, NAME_LEN))) head = head->next;
	return head;
}

struct song_node * remove_song(struct song_node *head, char *artist, char *name) {
	if (!head) return 0;
	if (!strncasecmp(head->artist, artist, NAME_LEN)
		&& !strncasecmp(head->name, name, NAME_LEN)) {
			return free_node(head);
	} else {
		head->next = remove_song(head->next, artist, name);
	}
	return head;
}

struct random_result random_song_from(struct song_node *head, struct random_result prev) {
	for (; head; head = head->next) {
		if (!(rand() % ++prev.i)) prev.result = head;
	}
	return prev;
}

// struct song_node * random_song(struct song_node *head) {
//     struct song_node *result = 0;
//     int i;
//     for (i = 1; head; i++, head = head->next) {
//         if (!(rand() % i)) result = head;
//     }
//     return result;
// }
