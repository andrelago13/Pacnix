#include "i8254.h"

.global _freq
.data

_freq:
	.short 0

_trying:
	mov 4, %eax
	ret