// An array backed hash set
// Has O(1) contains checks (ahs_in)
// Implemented using an ArrayHashMap with NULL values

#include "../lib.h"
#include "map.h"

#ifndef COLLECTIONS_SET_H
#define COLLECTIONS_SET_H

// A set is just a map in disguise
typedef Map Set;
typedef Iterator(Map) Iterator(Set);

// Like HashMap, HashSet is not a full class but still supports new(), del() and format()
// Since most are copies of the hash map implementation, they are simply redirected through macros

#define Set__new(initial_size, key_class) new(Map, initial_size, key_class, class(Void))
#define Set__del(set) del(Map, set)

String Set__format(Set set);

// Iterator

#define Set__iterator__start(map) Map__iterator__start(map)
#define Set__iterator__test(it, map) Map__iterator__test(it, map)
#define Set__iterator__next(it, map) Map__iterator__next(it, map)


// Public Instance Methods - these all borrow the map
// These are variants on the HashMap variants

#define set_put(set, key) map_put(set, key, NULL)
#define set_contains(set, key) map_contains_key(set, key)
#define set_clear(set) map_clear(set)

#endif