#include "common.h"
#include "stdlib.h"
#include <assert.h>

#ifndef SIATKONATOR_HASH
#define SIATKONATOR_HASH

typedef struct sh_node {
  int id_a;
  int id_b;
  struct sh_node *next;
} sh_node;

typedef struct {
  sh_node **values;
  int size;
} sh_hash;


#endif

int sh_function(sh_hash hash, int a, int b);
int sh_contains(sh_hash hash, int id_a, int id_b);
void sh_remove(sh_hash hash, int id_a, int id_b);
void sh_insert(sh_hash hash, int id_a, int id_b);
void sh_try(sh_hash hash, int id_a, int id_b);
void hash_init(sh_hash *hash, int size);
int is_prime(int number);
sh_node *sh_as_list(sh_hash hash, int *size);
void sh_free_list(sh_node* node);
void triangulatio_to_hash(sh_hash hash, triangulateio *source);
