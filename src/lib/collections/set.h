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
Set Set__new(uint32_t initial_size, Class key_class);
void Set__del(Set set);
String Set__format(Set set);

// Iterator

#define Set__iterator__start(set) Map__iterator__start(set)
#define Set__iterator__test(it, set) Map__iterator__test(it, set)
#define Set__iterator__next(it, set) Map__iterator__next(it, set)


// Public Instance Methods - these all borrow the map
// These are variants on the HashMap variants

#define set_put(set, key) map_put(set, key, NULL)
#define set_contains(set, key) map_contains_key(set, key)
#define set_clear(set) map_clear(set)

#endif