#include "map.h"

#define LOAD_FACTOR 0.75

// Private Methods

static Result(pointer_t) map_get_internal(Map map, pointer_t key);
static void map_rehash(Map map);


Map Map__new(uint32_t initial_size, Class key_class, Class value_class)
{
    initial_size = next_highest_power_of_two(initial_size);
    Map map = class_malloc(Map);

    map->keys = safe_malloc(sizeof(pointer_t) * initial_size);
    map->values = safe_malloc(sizeof(pointer_t) * initial_size);
    map->key_class = key_class;
    map->value_class = value_class;
    map->size = initial_size;
    map->length = 0;

    for (uint32_t i = 0; i < initial_size; i++)
    {
        map->keys[i] = NULL;
        map->values[i] = NULL;
    }

    return map;
}

void Map__del(Map map)
{
    for iter(Map, it, map)
    {
        del_c(map->key_class, it.key);
        del_c(map->value_class, it.value);
    }
    free(map->keys);
    free(map->values);
    free(map);
}

String Map__format(Map map)
{
    String s = new(String, "Map<");
    str_append(s, map->key_class->name);
    str_append(s, ", ");
    str_append(s, map->value_class->name);
    str_append(s, ">{");
    if (map->length == 0)
    {
        str_append(s, "}");
        return s;
    }
    else
    {
        for iter(Map, it, map)
        {
            str_append(s, format_c(map->key_class, it.key));
            str_append(s, ": ");
            str_append(s, format_c(map->value_class, it.value));
            str_append(s, ", ");
        }
    }
    str_pop(s, 2); // Pop the last ', '
    str_append(s, "}");
    return s;
}


// Iterator

bool Map__iterator__test(Iterator(Map)* it, Map map)
{
    for (;it->index < map->size; it->index++) // Don't iterate off the end of the map
    {
        pointer_t key = map->keys[it->index]; // Stop at valid key locations
        if (key != NULL)
        {
            it->key = key;
            it->value = map->values[it->index];
            return true;
        }
    }
    return false;
}


// Instance Methods

bool map_put(Map map, pointer_t key, pointer_t value)
{
    panic_if_null(key, "Null Pointer: Map key must not be null");

    // In order to gaurentee proper map function, the map must have always at least one empty entry
    // This means we need to re-hash the map when inserting at specific sizes
    if (map->length + 1 >= (uint32_t) (LOAD_FACTOR * map->size))
    {
        map_rehash(map);
    }

    uint32_t mask = (map->size - 1);
    uint32_t index = hash_c(map->key_class, key) & mask;
    pointer_t current_key = map->keys[index];

    while (current_key != NULL)
    {
        // Test current key
        if (equals_c(map->key_class, current_key, key))
        {
            // Key match. Replace the value at this index.
            // Delete the original key, keep the queried key intact. Ownership is given to the map (but borrowed in return)
            del_c(map->key_class, current_key);
            del_c(map->value_class, map->values[index]);
            map->keys[index] = key;
            map->values[index] = value;
            return true; // Previous value found
        }
        index = (index + 1) & mask;
        current_key = map->keys[index];
    }
    // Found a null key position before a match, there was no matching element
    map->keys[index] = key;
    map->values[index] = value;
    map->length++;
    return false;
}

bool map_contains_key(Map map, pointer_t key)
{
    return is_ok(map_get_internal(map, key));
}

pointer_t map_get(Map map, pointer_t key)
{
    return unwrap_default(map_get_internal(map, key));
}

void map_clear(Map map)
{
    for iter(Map, it, map)
    {
        del_c(map->key_class, it.key);
        del_c(map->value_class, it.value);

        // Keys need to be nulled as they are checked against null for existance
        // Values only exist with a non-null key, so they don't need to be nulled.
        map->keys[it.index] = NULL;
    }
    map->length = 0;
}


// Private Methods

// Get, but returns the value as a result
// Allows NULL values, but identifies if the key was present in the map or not.
static Result(pointer_t) map_get_internal(Map map, pointer_t key)
{
    panic_if_null(key, "Null Pointer: Map key must not be null.");

    uint32_t mask = (map->size - 1);
    uint32_t index = hash_c(map->key_class, key) & mask;
    pointer_t current_key = map->keys[index];
    
    while (current_key != NULL) // The map must always have at least one empty spot - so this is gaurenteed to terminate
    {
        // Test current key
        if (equals_c(map->key_class, current_key, key))
        {
            return Ok(pointer_t, map->values[index]); // Key match
        }
        index = (index + 1) & mask;
        current_key = map->keys[index];
    }
    return Err(pointer_t); // No match
}

static void map_rehash(Map map)
{
    // Save a reference to the existing keys and values arrays
    pointer_t* old_keys = map->keys;
    pointer_t* old_values = map->values;
    uint32_t old_size = map->size;
    uint32_t new_size = old_size << 1;

    // Reallocate the new arrays
    map->keys = safe_malloc(sizeof(pointer_t) * new_size);
    map->values = safe_malloc(sizeof(pointer_t) * new_size);

    for (uint32_t i = 0; i < new_size; i++)
    {
        map->keys[i] = NULL;
        map->values[i] = NULL;
    }

    // Set the map to empty
    map->size = new_size;
    map->length = 0;

    // Insert all old values
    for (uint32_t index = 0; index < old_size; index++)
    {
        pointer_t old_key = old_keys[index];
        if (old_key != NULL)
        {
            map_put(map, old_key, old_values[index]);
        }
    }

    // Free now-unused old keys and values arrays
    free(old_keys);
    free(old_values);
}