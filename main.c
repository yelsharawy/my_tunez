#include <stdlib.h>
#include <time.h>
#include <stdio.h>

#include "songs.h"
#include "songlib.h"

#define SECTION(x) printf("\e[32m=== "x" ===\e[0m\n")
#define EXPECT(f, x) do {\
	printf("\e[1m"#f"\e[0m\n");\
	f;\
	printf("\e[36mExpected:\e[0m\n"x"\n\n");\
} while(0)

void standard_tests() {
	struct song_node *list = 0;
	SECTION("Add two songs to front");
	list = insert_front(list, "test", "ok");
	list = insert_front(list, "test", "good");
	EXPECT(println_node(list), "test: good");
	EXPECT(print_list(list),"test: good | test: ok");
	SECTION("Inserting songs in order");
	list = insert_ordered(list, new_song("no idea", "Paris St. Germain"));
	list = insert_ordered(list, new_song("Not a real song", "party pooper"));
	list = insert_ordered(list, new_song("Not a real song", "party life"));
	list = insert_ordered(list, new_song("noz coz", "part of my tests"));
	list = insert_ordered(list, new_song("me", "this code"));
	EXPECT(print_list(list),"me: this code | no idea: Paris St. Germain | Not a real song: party life"
			" | Not a real song: party pooper | noz coz: part of my tests | test: good | test: ok");
	printf("\n");
	SECTION("Freeing list");
	list = free_list(list);
	EXPECT(print_list(list),"");
	printf("\n");
}

void random_test() {
	static const int num_trials = 10000;
	SECTION("Testing random_node");
	struct song_node *list = 0;
	list = insert_front(list, "Yusuf", "D");
	list = insert_front(list, "Yusuf", "C");
	list = insert_front(list, "Yusuf", "B");
	list = insert_front(list, "Yusuf", "A");
	int counts[4];
	int i;
	for (i = 0; i < 4; i++) counts[i] = 0;
	
	int t;
	for (t = 0; t < num_trials; t++) {
		struct song_node *chosen = random_song(list);
		counts[*(chosen->name) - 'A']++;
	}
	
	for (i = 0; i < 4; i++) {
		printf("%d ", counts[i]);
	}
	EXPECT(,"2500 2500 2500 2500 (approx.)");
	free_list(list);
}

void find_remove_tests() {
	SECTION("Find by artist");
	struct song_node *list = 0;
	list = insert_front(list, "C", "C song");
	list = insert_front(list, "B", "B's other song");
	list = insert_front(list, "B", "B song");
	list = insert_front(list, "A", "A song");
	
	print_list(list);
	
	EXPECT(println_node(find_artist(list, "A")), "A: A song");
	EXPECT(println_node(find_artist(list, "B")), "B: B song");
	EXPECT(println_node(find_artist(list, "C")), "C: C song");
	EXPECT(println_node(find_artist(list, "D")), "(nil)");
	
	SECTION("Find by artist and name");
	EXPECT(println_node(find_song(list, "A", "A song")), "A: A song");
	EXPECT(println_node(find_song(list, "B", "B song")), "B: B song");
	EXPECT(println_node(find_song(list, "B", "B's other song")), "B: B's other song");
	EXPECT(println_node(find_song(list, "C", "C song")), "C: C song");
	EXPECT(println_node(find_song(list, "C", "C's secret song")), "(nil)");
	
	SECTION("Remove by artist and name");
	EXPECT(print_list(list = remove_song(list, "A", "A song")), "B: B song | B: B's other song | C: C song");
	EXPECT(print_list(list = remove_song(list, "D", "D song")), "B: B song | B: B's other song | C: C song");
	EXPECT(print_list(list = remove_song(list, "B", "B's other song")), "B: B song | C: C song");
	EXPECT(print_list(list = remove_song(list, "C", "C song")), "B: B song");
	
	free_list(list);
}

void linkedlisttests() {
	standard_tests();
	find_remove_tests();
	random_test();
}

void libtests() {
	struct song_node ** mylib = new_lib();
	SECTION("Adding songs to lib");
	add_song(mylib, "A artist", "A song");
	add_song(mylib, "A artist", "B song");
	add_song(mylib, "C artist", "C song");
	add_song(mylib, "A artist", "C song");
	add_song(mylib, "B artist", "B song");
	add_song(mylib, "C artist", "B song");
	add_song(mylib, "B artist", "A song");
	add_song(mylib, "C artist", "A song");
	add_song(mylib, "B artist", "C song");
	add_song(mylib, "Car driver", "beep beep");
	add_song(mylib, "Car driver", "traffic jam");
	add_song(mylib, "Candy", "sweet tooth");
	add_song(mylib, "Candy", "sugar rush");
	add_song(mylib, "#1 artist", "the best song");
	SECTION("Printing from lib");
	EXPECT(print_all(mylib),
		"A - A artist: A song | A artist: B song | A artist: C song\n"
		"B - B artist: A song | B artist: B song | B artist: C song\n"
		"C - C artist: A song | C artist: B song | C artist: C song | "
		"Candy: sugar rush | Candy: sweet tooth | Car driver: beep beep | Car driver: traffic jam\n"
		"@ - #1 artist: the best song"
	);
	EXPECT(print_letter(mylib, 'c'),
		"C - C artist: A song | C artist: B song | C artist: C song | "
		"Candy: sugar rush | Candy: sweet tooth | Car driver: beep beep | Car driver: traffic jam"
	);
	SECTION("Finding songs");
	EXPECT(println_node(lib_find_song(mylib, "A artist", "C song")), "A artist: C song");
	EXPECT(println_node(lib_find_song(mylib, "B artist", "B song")), "B artist: B song");
	EXPECT(println_node(lib_find_song(mylib, "C artist", "A song")), "C artist: A song");
	EXPECT(println_node(lib_find_song(mylib, "C artist", "no song")), "(nil)");
	EXPECT(println_node(lib_find_song(mylib, "no artist", "A song")), "(nil)");
	SECTION("Finding songs from artist");
	EXPECT(print_list(lib_find_artist(mylib, "B artist")),
		"B artist: A song | B artist: B song | B artist: C song"
	);
	EXPECT(print_list(lib_find_artist(mylib, "Candy")),
		"Candy: sugar rush | Candy: sweet tooth | Car driver: beep beep | Car driver: traffic jam"
	);
	SECTION("Printing songs from artist");
	EXPECT(print_artist(mylib, "C artist"), "C artist: A song | C artist: B song | C artist: C song");
	EXPECT(print_artist(mylib, "cAr DrIver"), "Car driver: beep beep | Car driver: traffic jam");
	EXPECT(print_artist(mylib, "CaNdY"), "Candy: sugar rush | Candy: sweet tooth");
	EXPECT(print_artist(mylib, "Bad artist"), "");
	SECTION("Removing songs");
	EXPECT(
		{ lib_remove_song(mylib, "A artist", "B song"); print_letter(mylib, 'a'); },
		"A - A artist: A song | A artist: C song"
	);
	EXPECT(
		{ lib_remove_song(mylib, "A artist", "C song"); print_letter(mylib, 'a'); },
		"A - A artist: A song"
	);
	EXPECT(
		{ lib_remove_song(mylib, "B artist", "A song"); print_letter(mylib, 'b'); },
		"A - B artist: B song | B artist: C song"
	);
	SECTION("Freeing lib");
	free_lib(mylib);
	
	SECTION("Lib random test");
	static const int num_trials = 10000;
	mylib = new_lib();
	add_song(mylib, "A", "1");
	add_song(mylib, "A", "2");
	add_song(mylib, "A", "3");
	add_song(mylib, "A", "4");
	add_song(mylib, "B", "1");
	add_song(mylib, "B", "2");
	add_song(mylib, "B", "3");
	add_song(mylib, "C", "1");
	add_song(mylib, "C", "2");
	add_song(mylib, "D", "1");
	print_all(mylib);
	
	int counts[4];
	int i;
	for (i = 0; i < 4; i++) counts[i] = 0;
	
	int t;
	for (t = 0; t < num_trials; t++) {
		struct song_node *chosen = lib_random(mylib);
		counts[*(chosen->artist) - 'A']++;
	}
	
	for (i = 0; i < 4; i++) {
		printf("%d ", counts[i]);
	}
	EXPECT(,"4000 3000 2000 1000 (approx.)");
	lib_shuffle(mylib, 5);
	free_lib(mylib);
}

int main() {
	srand(time(NULL));
	linkedlisttests();
	libtests();
}
