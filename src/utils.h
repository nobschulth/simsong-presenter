#ifndef H_UTILS
#define H_UTILS
#include <stddef.h>

/*Wrapper for malloc that just fails the programm if malloc fails as most malloc
 * failures are irecoverable
*/
void* safe_malloc(size_t size);
void* safe_realloc(void* ptr, size_t size);

#endif // !H_UTILS
