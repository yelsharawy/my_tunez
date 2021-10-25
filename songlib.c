#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "songlib.h"

struct song_node ** new_lib() {
	return calloc(27, sizeof(struct song_node *));
}

int chrhash(char chr) {
	unsigned int result = chr - 'A';
	// maps a..z to A..Z (also affects all others past 'z', but it doesn't matter)
	if (result >= 32) result -= 32;
	// maps all past Z to 26 (`result` is unsigned, so it will underflow rather than be 0)
	// technically this relies on undefined behavior? but it should be reliable
	if (result > 26) result = 26;
	return result;
}

//int strhash(char *str) {
//	return chrhash(*str);
//}
#define strhash(str) chrhash(*(str))

int store_index(struct song_node *node) {
	return strhash(node->artist);
}

void add_song(struct song_node **lib, char *artist, char *name) {
	// OTHER VERSION (effectively does the same thing)
	//struct song_node *node = new_node(artist, name);
	//int ind = store_index(node);
	//lib[ind] = insert_ordered(lib[ind], node);
	
	int ind = strhash(artist);
	lib[ind] = insert_ordered(lib[ind], new_song(artist, name));
}

void clear_lib(struct song_node **lib) {
	int i;
	for (i = 0; i < 27; i++) lib[i] = free_list(lib[i]);
}

void free_lib(struct song_node **lib) {
	int i;
	// doesn't need to reassign, lib will be freed
	for (i = 0; i < 27; i++) free_list(lib[i]);
	free(lib);
}

struct song_node * lib_find_artist(struct song_node **lib, char *artist) {
	return find_artist(lib[strhash(artist)], artist);
}

struct song_node * lib_find_song(struct song_node **lib, char *artist, char *song) {
	return find_song(lib[strhash(artist)], artist, song);
}

void lib_remove_song(struct song_node **lib, char *artist, char *song) {
	int ind = strhash(artist);
	lib[ind] = remove_song(lib[ind], artist, song);
}

void print_letter(struct song_node **lib, char c) {
	int ind = chrhash(c);
	printf("%c - ", (ind+1)%27+'@');
	print_list(lib[ind]);
}

void print_artist(struct song_node **lib, char *artist) {
	struct song_node *head = lib_find_artist(lib, artist);
	if (head) do {
		print_node(head);
	} while ((head = head->next) && !strncasecmp(head->artist, artist, NAME_LEN) && printf(" | "));
	printf("\n");
}

void print_all(struct song_node **lib) {
	int i;
	for (i = 0; i < 27; i++) {
		if (lib[i]) {
			printf("%c - ", (i+1)%27+'@');
			print_list(lib[i]);
		}
	}
}

// completely uniformly random!
struct song_node * lib_random(struct song_node **lib) {
	struct random_result result = (struct random_result){.result=0, .i=0};
	int i;
	for (i = 0; i < 27; i++) {
		result = random_song_from(lib[i], result);
	}
	return result.result;
}

void lib_shuffle(struct song_node **lib, int n) {
	if (n) do {
		print_node(lib_random(lib));
	} while (--n && printf(" | "));
	printf("\n");
}