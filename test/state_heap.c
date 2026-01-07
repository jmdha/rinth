#include <utest.h/utest.h>

#include "state_heap.h"

UTEST(state_heap, empty) {
	state_heap* sh;

	sh = sh_new();

	ASSERT_EQ(0, sh_size(sh));

	sh_free(sh, true);
}

UTEST(state_heap, single) {
	state_heap* sh;
	state*      s;

	sh = sh_new();
	s  = state_new();

	sh_push(sh, state_new(), 0);

	ASSERT_EQ(1, sh_size(sh));

	s = sh_pop(sh);

	ASSERT_EQ(0, sh_size(sh));

	sh_free(sh, true);
	state_free(s);
}
