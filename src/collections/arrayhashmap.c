#include "arrayhashmap.h"

ArrayHashMapEntry* ArrayHashMapEntry__new(ArrayHashMap* map, void* key, void* value)
{
    ArrayHashMapEntry* entry = (ArrayHashMapEntry*) malloc(sizeof(ArrayHashMapEntry));
    PANIC_IF_NULL(entry, "Unable to create ArrayHashMapEntry");
    
    entry->map = map;
    entry->key = key;
    entry->value = value;
    entry->next = NULL;
    entry->index = 0;

    return entry;
}

// Entry elements are owned by the map - use the map to free them.
void ArrayHashMapEntry__del(ArrayHashMapEntry* entry)
{
    del_c(entry->map->key_class, entry->key);
    if (entry->value != NULL) // We allow null values - only delete them if it's non-null
    {
        del_c(entry->map->value_class, entry->value);
    }
    free(entry);
}

String* ArrayHashMapEntry__format(ArrayHashMapEntry* entry)
{
    String* s = new(String, "(");
    str_append_string(s, format_c(entry->map->key_class, entry->key));
    str_append_slice(s, ": ");
    str_append_string(s, format_c(entry->map->value_class, entry->value));
    str_append_char(s, ')');
    return s;
}

ArrayHashMap* ArrayHashMap__new(uint32_t initial_size, Class* key_class, Class* value_class)
{
    initial_size = next_highest_power_of_two(initial_size);

    ArrayHashMap* map = (ArrayHashMap*) malloc(sizeof(ArrayHashMap));
    ArrayHashMapEntry** array = (ArrayHashMapEntry**) malloc(sizeof(ArrayHashMapEntry*) * initial_size);

    PANIC_IF_NULL(map, "Unable to create ArrayHashMap with initial_size %d", initial_size);
    PANIC_IF_NULL(array, "Unable to create ArrayHashMap backing array with initial_size %d", initial_size);

    for (uint32_t i = 0; i < initial_size; i++)
    {
        array[i] = NULL;
    }

    map->array = array;
    map->size = initial_size;
    map->key_class = key_class;
    map->value_class = value_class;
    map->length = 0;

    return map;
}

void ArrayHashMap__del(ArrayHashMap* map)
{
    // Deletion is one delayed as we are iterating through the map using this entry
    ArrayHashMapEntry* prev = NULL;
    iter(ArrayHashMap, map, index, entry)
    {
        if (prev != NULL)
        {
            del(ArrayHashMapEntry, prev);
        }
        prev = entry;
    }
    if (prev != NULL)
    {
        del(ArrayHashMapEntry, prev);
    }
    free(map->array);
    free(map);
}

String* ArrayHashMap__format(ArrayHashMap* map)
{
    String* s = new(String, "ArrayHashMap{");
    if (map->length == 0)
    {
        str_append_char(s, '}');
        return s;
    }
    else
    {
        iter(ArrayHashMap, map, index, entry)
        {
            str_append_string(s, format(ArrayHashMapEntry, entry));
            str_append_slice(s, ", ");
        }
    }
    str_pop(s, 2); // Pop the last ', '
    str_append_char(s, '}');
    return s;
}


bool ahm_put(ArrayHashMap* map, void* key, void* value)
{
    uint32_t index = hash_c(map->key_class, key);
    index &= (map->size - 1);

    ArrayHashMapEntry* entry = map->array[index];
    if (entry == NULL)
    {
        // No entry at this key, we can insert the key value pair directly.
        entry = new(ArrayHashMapEntry, map, key, value);
        entry->index = index;
        map->array[index] = entry;
        map->length++;
    }
    else
    {
        // Already a value here - treat it as a linked list and traverse until the end, looking for equality matches
        ArrayHashMapEntry* parent = entry; // Moved
        ArrayHashMapEntry* prev = NULL;
        do
        {
            // parent is non-null, parent->next may be null
            if (equals_c(map->key_class, parent->key, key))
            {
                // parent->key == key, so this element needs to be modified, and the previous key-value pair dropped. Length does not change.
                del_c(map->key_class, parent->key);
                del_c(map->value_class, parent->value);

                parent->key = key;
                parent->value = value;
                return true;
            }
            // Otherwise, advance in the search
            prev = parent;
            parent = parent->next;
        } while (parent != NULL);

        // No matching entry found, so append this entry to the end of the chain
        entry = new(ArrayHashMapEntry, map, key, value);
        entry->index = index;

        // Insert by linking to the previous
        prev->next = entry;
        map->length++;
    }
    // No previous entry
    return false;
}

bool ahm_key_in(ArrayHashMap* map, void* key)
{
    return ahm_get(map, key) != NULL;
}

void* ahm_get(ArrayHashMap* map, void* key)
{
    uint32_t index = hash_c(map->key_class, key);
    index &= (map->size - 1);
    ArrayHashMapEntry* entry = map->array[index];
    
    if (entry == NULL) 
    {
        return NULL; // No value at this key
    }
    else
    {
        ArrayHashMapEntry* parent = entry; // Moved
        do
        {
            // parent is non-null, parent->next may be null
            if (equals_c(map->key_class, parent->key, key))
            {
                // Found a matching element
                return parent->value;
            }
            parent = parent->next;
        } while (parent != NULL);
        
        // No matching value, return NULL
        return NULL;
    }
}

void ahm_clear(ArrayHashMap* map)
{
    // Deletion is one delayed as we are iterating through the map using this entry
    // Copied from ArrayHashMap__del() except without the final free's
    for (uint32_t i = 0; i < map->size; i++)
    {
        ArrayHashMapEntry* prev = NULL;
        ArrayHashMapEntry* curr = map->array[i];
        while (curr != NULL)
        {
            if (prev != NULL)
            {
                del(ArrayHashMapEntry, prev);
            }

            prev = curr;
            curr = curr->next;
        }

        if (prev != NULL)
        {
            del(ArrayHashMapEntry, prev);
        }

        map->array[i] = NULL;
    }
    map->length = 0;
}