#include "siatkonator_hash.h"
#include <stdio.h>

int sh_function(sh_hash hash, int a, int b){
  return (a*100000 + b) % hash.size;
}

int sh_contains(sh_hash hash, int id_a, int id_b){
  int hash_address = sh_function(hash, id_a, id_b);
  sh_node *node = NULL;
  if (hash.values[hash_address] == (sh_node *) NULL)
    return 0;
  node = hash.values[hash_address];
  while (node != NULL){
    if (node->id_a == id_a && node->id_b == id_b)
      return 1;
    node = node->next;
  }
  return 0;
}

void sh_remove(sh_hash hash, int id_a, int id_b){
  int hash_address = sh_function(hash, id_a, id_b);
  sh_node *node = NULL;
  sh_node *prev = NULL;
  node = hash.values[hash_address];
  if (node->id_a == id_a && node->id_b == id_b){
    free(node);
    hash.values[hash_address] = NULL;
    return;
  }
  while (node != NULL){
    if (node->id_a == id_a && node->id_b == id_b){
      if (prev == NULL){
        hash.values[hash_address] = NULL;
      } else {
        prev->next = node->next;
      }
      free(node);
      return;
    }
    prev = node;
    node = node->next;
  }
}

void sh_insert(sh_hash hash, int id_a, int id_b){
  int hash_address = sh_function(hash, id_a, id_b);
  sh_node *node = malloc(sizeof(sh_node));
  sh_node *prev = NULL;
  node->id_a = id_a;
  node->id_b = id_b;
  prev = hash.values[hash_address];
  node->next = prev;
  hash.values[hash_address] = node;
}

void sh_try(sh_hash hash, int id_a, int id_b){
  int swap_id;

  // id_a should always be the larger one
  if (id_a < id_b){
    swap_id = id_a;
    id_a = id_b;
    id_b = swap_id;
  }

  if (sh_contains(hash, id_a, id_b)){
    sh_remove(hash, id_a, id_b);
    siatkonator_log(DEBUG, "HASH REMOVE: %d %d\n", id_a, id_b);
  } else {
    sh_insert(hash, id_a, id_b);
    siatkonator_log(DEBUG, "HASH INSERT: %d %d\n", id_a, id_b);
  }

}

void hash_init(sh_hash *hash, int size){

  sh_node **element;

  while(!is_prime(size)){
    ++size;
  }

  hash->size = size;
  hash->values = malloc(size * sizeof(sh_node*));
  assert(hash->values != NULL);

  // initialize the array of pointers with nulls
  for(element = hash->values; element < hash->values + size; element++){
    *element  = NULL;
  }
}

int is_prime(int number){
  int div;

  if (number < 5){
    return 0;
  }

  for(div=2; div <= number/2; div++){
    if (number % div == 0)
      return 0;
  }
  return 1;
}

