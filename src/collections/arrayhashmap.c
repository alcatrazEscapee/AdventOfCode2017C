#include "arrayhashmap.h"

ArrayHashMap* ArrayHashMap__new(uint32_t initial_size, Class* key_class, Class* value_class)
{
    initial_size = next_highest_power_of_two(initial_size);

    ArrayHashMap* map = malloc(sizeof(ArrayHashMap));
    void** key_array = malloc(sizeof(void*) * initial_size);
    void** value_array = malloc(sizeof(void*) * initial_size);

    PANIC_IF_NULL(map, "Unable to create ArrayHashMap with initial_size %d", initial_size);
    PANIC_IF_NULL(key_array, "Unable to create ArrayHashMap with initial_size %d", initial_size);
    PANIC_IF_NULL(value_array, "Unable to create ArrayHashMap with initial_size %d", initial_size);

    for (uint32_t i = 0; i < initial_size; i++)
    {
        key_array[i] = NULL;
        value_array[i] = NULL;
    }

    map->keys = key_array;
    map->values = value_array;
    map->key_class = key_class;
    map->value_class = value_class;
    map->size = initial_size;
    map->length = 0;

    return map;
}

void ArrayHashMap__del(ArrayHashMap* map)
{
    for iter(ArrayHashMap, it, map)
    {
        del_c(map->key_class, it->key);
        del_c(map->value_class, it->value);
    }
    free(map->keys);
    free(map->values);
    free(map);
}

String* ArrayHashMap__format(ArrayHashMap* map)
{
    String* s = new(String, "ArrayHashMap<");
    str_append_slice(s, map->key_class->name);
    str_append_slice(s, ", ");
    str_append_slice(s, map->value_class->name);
    str_append_slice(s, ">{");
    if (map->length == 0)
    {
        str_append_char(s, '}');
        return s;
    }
    else
    {
        for iter(ArrayHashMap, it, map)
        {
            str_append_string(s, format_c(map->key_class, it->key));
            str_append_slice(s, ": ");
            str_append_string(s, format_c(map->value_class, it->value));
            str_append_slice(s, ", ");
        }
    }
    str_pop(s, 2); // Pop the last ', '
    str_append_char(s, '}');
    return s;
}


// Iterator

Iterator(ArrayHashMap)* ArrayHashMap__iterator__new(ArrayHashMap* map)
{
    Iterator(ArrayHashMap)* it = malloc(sizeof(Iterator(ArrayHashMap)));
    PANIC_IF_NULL(it, "Unable to create Iterator<ArrayHashMap>");

    it->index = 0;
    it->key = NULL;
    it->value = NULL;

    return it;
}

void ArrayHashMap__iterator__del(Iterator(ArrayHashMap)* it)
{
    free(it);
}

bool ArrayHashMap__iterator__test(Iterator(ArrayHashMap)* it, ArrayHashMap* map)
{
    for (;it->index < map->size; it->index++) // Don't iterate off the end of the map
    {
        void* key = map->keys[it->index]; // Stop at valid key locations
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

bool ahm_put(ArrayHashMap* map, void* key, void* value)
{
    uint32_t mask = (map->size - 1);

    // In order to gaurentee proper map function, the map must have always at least one empty entry
    // This means we need to re-hash the map when inserting at specific sizes
    // For now, just panic if this invariant would be violated
    PANIC_IF((map->length + 1) == map->size, "ArrayHashMap needs to be rehashed! It is currently %d / %d", map->length, map->size);

    uint32_t index = hash_c(map->key_class, key) & mask;
    void* current_key = map->keys[index];

    while (current_key != NULL)
    {
        // Test current key
        if (equals_c(map->key_class, current_key, key))
        {
            // Key match. Replace the value at this index and delete the queried key (the map key is kept intact)
            // Probing is unaffected
            del_c(map->key_class, key);
            del_c(map->value_class, map->values[index]);
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

bool ahm_key_in(ArrayHashMap* map, void* key)
{
    return ahm_get(map, key) != NULL;
}

void* ahm_get(ArrayHashMap* map, void* key)
{
    PANIC_IF_NULL(key, "ahm_get(ArrayHashMap*, void*) requires a non-null key");

    uint32_t mask = (map->size - 1);
    uint32_t index = hash_c(map->key_class, key) & mask;
    void* current_key = map->keys[index];
    
    while (current_key != NULL) // The map must always have at least one empty spot - so this is gaurentee'd to terminate
    {
        // Test current key
        if (equals_c(map->key_class, current_key, key))
        {
            return map->values[index]; // Key match
        }
        index = (index + 1) & mask;
        current_key = map->keys[index];
    }
    return NULL; // No match
}

void ahm_clear(ArrayHashMap* map)
{
    for iter(ArrayHashMap, it, map)
    {
        del_c(map->key_class, it->key);
        del_c(map->value_class, it->value);
    }
    map->length = 0;
}