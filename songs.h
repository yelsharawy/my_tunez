#ifndef SONGS_H
#define SONGS_H

#define NAME_LEN 99

struct song_node {
	char artist[NAME_LEN+1];
	char name[NAME_LEN+1];
	struct song_node *next;
};

struct song_node * insert_front(struct song_node *head, char *artist, char *name); // TESTED

#define new_song(artist, name) insert_front(0, artist, name)
// QUESTION: Is it okay to use macros in cases like this? It should be faster,
// but it has possibly unwanted implications (e.g. you cannot create a pointer to this "function").
// Same goes for `println_node`, `random_song`, and `strhash` (in songs.c)

void print_node(struct song_node *song); // TESTED

#define println_node(song) do { print_node(song); printf("\n"); } while(0)

void print_list(struct song_node *head); // TESTED

struct song_node * free_list(struct song_node *head); // TESTED

struct song_node * insert_ordered(struct song_node *head, struct song_node *to_insert); // TESTED

int songcmp(struct song_node *a, struct song_node *b); // TESTED (indirectly)

struct song_node * find_artist(struct song_node *head, char *artist); // TESTED

struct song_node * find_song(struct song_node *head, char *artist, char *name); // TESTED

struct song_node * remove_song(struct song_node *head, char *artist, char *name); // TESTED

// struct song_node * random_song(struct song_node *head); // TESTED

struct random_result {
	struct song_node *result;
	int i;
};

// I changed the design to this to allow for uniformly random song choosing in `songlib`
struct random_result random_song_from(struct song_node *head, struct random_result prev);

#define random_song(head) (random_song_from(head, (struct random_result){.result=0, .i=0}).result)

#endif
