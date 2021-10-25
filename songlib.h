#ifndef SONGLIB_H
#define SONGLIB_H

#include "songs.h"

struct song_node ** new_lib(); // TESTED

void add_song(struct song_node **lib, char *artist, char *name); // TESTED

void clear_lib(struct song_node **lib); // TESTED (indirectly)

void free_lib(struct song_node **lib); // TESTED

struct song_node * lib_find_artist(struct song_node **lib, char *artist); // TESTED (indirectly)

struct song_node * lib_find_song(struct song_node **lib, char *artist, char *song); // TESTED

void lib_remove_song(struct song_node **lib, char *artist, char *song);

void print_letter(struct song_node **lib, char c); // TESTED

void print_artist(struct song_node **lib, char *artist); // TESTED

void print_all(struct song_node **lib); // TESTED

struct song_node * lib_random(struct song_node **lib); // TESTED

void lib_shuffle(struct song_node **lib, int n); // TESTED

#endif
