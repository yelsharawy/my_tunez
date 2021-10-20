#include <stdlib.h>
#include <time.h>
#include <stdio.h>

#include "songs.h"

#define SECTION(x) printf("\e[32m=== "x" ===\e[0m\n")
#define EXPECT(x) printf("\e[36mExpected:\e[0m\n"x"\n\n")

void standard_tests() {
	struct song_node *list = 0;
	SECTION("Add two songs to front");
	list = insert_front(list, "test", "ok");
	list = insert_front(list, "test", "good");
	print_node(list); printf("\n");
	EXPECT("test: good");
	print_list(list);
	EXPECT("test: good | test: ok");
	SECTION("Inserting songs in order");
	list = insert_ordered(list, new_song("no idea", "Paris St. Germain"));
	list = insert_ordered(list, new_song("Not a real song", "party pooper"));
	list = insert_ordered(list, new_song("Not a real song", "party life"));
	list = insert_ordered(list, new_song("noz coz", "part of my tests"));
	list = insert_ordered(list, new_song("me", "this code"));
	print_list(list);
	EXPECT("me: this code | no idea: Paris St. Germain | Not a real song: party life"
			" | Not a real song: party pooper | noz coz: part of my tests | test: good | test: ok");
	printf("\n");
	SECTION("Freeing list");
	list = free_list(list);
	print_list(list);
	EXPECT("");
	printf("\n");
}

void random_test() {
	static const int num_trials = 1000;
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
	printf("\n");
	EXPECT("250 250 250 250 (approx.)");
	free_list(list);
}

void find_remove_tests() {
	SECTION("Find by artist");
	struct song_node *list = 0;
	list = insert_front(list, "C", "C song");
	list = insert_front(list, "B", "B's other song");
	list = insert_front(list, "B", "B song");
	list = insert_front(list, "A", "A song");
	println_node(find_artist(list, "A"));
	EXPECT("A: A song");
	println_node(find_artist(list, "B"));
	EXPECT("B: B song");
	println_node(find_artist(list, "C"));
	EXPECT("C: C song");
	println_node(find_artist(list, "D"));
	EXPECT("(nil)");
	
	SECTION("Find by artist and name");
	println_node(find_song(list, "A", "A song"));
	EXPECT("A: A song");
	println_node(find_song(list, "B", "B song"));
	EXPECT("B: B song");
	println_node(find_song(list, "B", "B's other song"));
	EXPECT("B: B's other song");
	println_node(find_song(list, "C", "C song"));
	EXPECT("C: C song");
	println_node(find_song(list, "C", "C's secret song"));
	EXPECT("(nil)");
	
	SECTION("Remove by artist and name");
	list = remove_song(list, "A", "A song");
	print_list(list);
	EXPECT("B: B song | B: B's other song | C: C song");
	list = remove_song(list, "D", "D song");
	print_list(list);
	EXPECT("B: B song | B: B's other song | C: C song");
	list = remove_song(list, "B", "B's other song");
	print_list(list);
	EXPECT("B: B song | C: C song");
	list = remove_song(list, "C", "C song");
	print_list(list);
	EXPECT("B: B song");
	
	free_list(list);
}

int main() {
	srand(time(NULL));
	standard_tests();
	find_remove_tests();
	random_test();
}
