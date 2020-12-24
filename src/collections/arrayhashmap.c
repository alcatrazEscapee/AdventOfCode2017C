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
    del_c(entry->map->value_class, entry->value);

    free(entry);
}

ArrayHashMap* ArrayHashMap__new(uint32_t initial_size, Class* key_class, Class* value_class)
{
    initial_size = next_highest_power_of_two(initial_size);

    ArrayHashMap* map = (ArrayHashMap*) malloc(sizeof(ArrayHashMap));
    ArrayHashMapEntry** array = (ArrayHashMapEntry**) malloc(sizeof(ArrayHashMapEntry*) * initial_size);

    PANIC_IF_NULL(map, "Unable to create ArrayHashMap with initial_size %d", initial_size);
    PANIC_IF_NULL(array, "Unable to create ArrayHashMap backing array with initial_size %d", initial_size);

    map->array = array;
    map->size = initial_size;
    map->key_class = key_class;
    map->value_class = value_class;
    map->length = 0;

    return map;
}

void ArrayHashMap__del(ArrayHashMap* map)
{
    for (uint32_t i = 0; i < map->size; i++)
    {
        if (map->array[i] != NULL)
        {
            ArrayHashMapEntry* entry = map->array[i];
            ArrayHashMapEntry* prev = NULL;
            do
            {
                prev = entry;
                entry = entry->next;
                del(ArrayHashMapEntry, prev);
            } while (entry != NULL);
        }
    }
    free(map->array);
    free(map);
}

void* ahm_put(ArrayHashMap* map, void* key, void* value)
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
                // parent->key == key, so this element needs to be modified, and the previous value returned
                // The map only borrows all keys and values, so this does not transfer any ownership
                // All entries stay within the map
                void* old = parent->value;
                parent->value = value;
                return old;
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
    }
    // No previous entry
    return NULL;
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

void ahm_print(ArrayHashMap* map, void (*entry_writer)(ArrayHashMapEntry*))
{
    printf("ArrayHashMap: {\n");
    for (uint32_t i = 0; i < map->size; i++)
    {
        if (map->array[i] != NULL)
        {
            ArrayHashMapEntry* entry = map->array[i];
            printf("\t%4d\t", i);
            do
            {
                printf(" -> ");
                entry_writer(entry);
                entry = entry->next;
            } while (entry != NULL);
            printf("\n");
        }
    }
    printf("}\n");
}

void ahm_iter_next_entry(ArrayHashMap* map, ArrayHashMapEntry** entry)
{
    if ((*entry)->next == NULL)
    {
        // next entry in this linked list is null, so iterate indexes until we reach the next filled entry
        for (uint32_t i = (*entry)->index; i < map->size; i++)
        {
            ArrayHashMapEntry* next = map->array[i];
            if (next != NULL)
            {
                *entry = next;
                return;
            }
        }

        // Reached the end of the list, so assign NULL, and the iterator will terminate
        *entry = NULL;
    }
    else
    {
        // Iterate through the linked list
        *entry = (*entry)->next;
    }
}