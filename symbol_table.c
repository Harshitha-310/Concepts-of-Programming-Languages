#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TABLE_SIZE 97  

// Structure for symbol table entry
typedef struct Entry {
    char *key;
    int value;
    struct Entry *next;
} Entry;

// Structure for symbol table (scope)
typedef struct SymbolTable {
    Entry *table[TABLE_SIZE];
    struct SymbolTable *parent;
} SymbolTable;

// Hash function (DJB2 - efficient string hashing)
unsigned int hash(const char *key) {
    unsigned long hash = 5381;
    while (*key)
        hash = ((hash << 5) + hash) + *key++;
    return hash % TABLE_SIZE;
}


SymbolTable *create_table(SymbolTable *parent) {
    SymbolTable *new_table = (SymbolTable *)calloc(1, sizeof(SymbolTable));
    if (!new_table) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    new_table->parent = parent;
    return new_table;
}


void insert(SymbolTable *table, const char *key, int value) {
    unsigned int index = hash(key);
    Entry *entry = table->table[index];

    // Checking if key already exists and update it
    while (entry) {
        if (strcmp(entry->key, key) == 0) {
            entry->value = value;
            return;
        }
        entry = entry->next;
    }

    // Creating new entry if key doesn't exist
    Entry *new_entry = (Entry *)malloc(sizeof(Entry));
    if (!new_entry) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    new_entry->key = strdup(key);
    new_entry->value = value;
    new_entry->next = table->table[index];
    table->table[index] = new_entry;
}


int lookup(SymbolTable *table, const char *key, int *found) {
    while (table) {
        unsigned int index = hash(key);
        for (Entry *entry = table->table[index]; entry; entry = entry->next) {
            if (strcmp(entry->key, key) == 0) {
                *found = 1;
                return entry->value;
            }
        }
        table = table->parent;
    }
    *found = 0;
    return 0;
}


void free_table(SymbolTable *table) {
    if (!table) return;
    for (int i = 0; i < TABLE_SIZE; i++) {
        Entry *entry = table->table[i];
        while (entry) {
            Entry *temp = entry;
            entry = entry->next;
            free(temp->key);
            free(temp);
        }
    }
    free(table);
}

int main() {
    SymbolTable *current_scope = create_table(NULL);
    
    
    current_scope = create_table(current_scope); 
    insert(current_scope, "a", 1);
    insert(current_scope, "b", 2);
    
    current_scope = create_table(current_scope); 
    insert(current_scope, "a", 3);
    insert(current_scope, "c", 4);
    
    int found;
    printf("%d\n", lookup(current_scope, "b", &found)); 
    
    current_scope = create_table(current_scope); 
    insert(current_scope, "c", 5);
    printf("%d\n", lookup(current_scope, "b", &found)); 
    printf("%d\n", lookup(current_scope, "a", &found)); 
    printf("%d\n", lookup(current_scope, "c", &found)); 
    
    current_scope = current_scope->parent; 
    printf("%d\n", lookup(current_scope, "a", &found)); 
    printf("%d\n", lookup(current_scope, "c", &found)); 
    
    current_scope = current_scope->parent; 
    printf("%d\n", lookup(current_scope, "a", &found)); 
    
    current_scope = current_scope->parent; 
    
    // Freeing all the remaining scopes to prevent memory leaks
    while (current_scope) {
        SymbolTable *parent = current_scope->parent;
        free_table(current_scope);
        current_scope = parent;
    }
    
    return 0;
}
