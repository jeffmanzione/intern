#ifndef COM_GITHUB_JEFFMANZIONE_INTERN_INTERN_H_
#define COM_GITHUB_JEFFMANZIONE_INTERN_INTERN_H_

#include <stdint.h>
#include <stdlib.h>

#define DEFAULT_CHUNK_SIZE 32488
#define DEFAULT_HASHTABLE_SIZE 4091

typedef struct {
  char *block;
  StringChunk *next;
  uint32_t sz;
} StringChunk;

typedef struct {

  char *tail, *end;
  StringChunk *chunk, *last;
  Set strings;
} StringIntern;

StringChunk *StringChunk_create() {
  StringChunk *chunk = malloc(StringChunk);
  chunk->sz = DEFAULT_CHUNK_SIZE;
  chunk->block = malloc(sizeof(char *) * chunk->sz);
  chunk->next = NULL;
  return chunk;
}


#define DEFINE_INTERN(InternName, InternType)

#endif /* COM_GITHUB_JEFFMANZIONE_INTERN_INTERN_H_ */
