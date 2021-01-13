// Template
// Header for PrimitiveArrayList(type)
// @param type : The type of the array list

// Local definitions
// Undef'd at the end of this template
#define PrimitiveArrayList_t CONCAT(ArrayList_, type)

struct CONCAT3(ArrayList_, type, __struct)
{
    type* values; // Backing array
    uint32_t size; // Length of backing array
    uint32_t length; // Size of filled elements
};

typedef struct CONCAT3(ArrayList_, type, __struct) * PrimitiveArrayList_t;

// Class
declare_class(PrimitiveArrayList_t);
declare_constructor(PrimitiveArrayList_t, uint32_t initial_size);

// Iterator
typedef struct
{
    uint32_t index;
    type value;
} Iterator(PrimitiveArrayList_t);

// Static Methods
PrimitiveArrayList_t CONCAT3(al_, type, _from_array)(type array[], uint32_t size);

// Instance Methods
type CONCAT3(al_, type, _get)(PrimitiveArrayList_t list, uint32_t index);

void CONCAT3(al_, type, _set)(PrimitiveArrayList_t list, uint32_t index, type value);
void CONCAT3(al_, type, _append)(PrimitiveArrayList_t list, type value);

void CONCAT3(al_, type, _clear)(PrimitiveArrayList_t list);

#undef type
#undef PrimitiveArrayList_t
