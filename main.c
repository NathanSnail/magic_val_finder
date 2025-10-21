#include <assert.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#define EXPR_LEN 20
#define ABS(x) ((x) > 0 ? (x) : -(x))
#define LEN(x) (sizeof(x) / sizeof(x[0]))
// #define SANITY_CHECKS
#define PRECOMPUTE_PROD

typedef enum {
	NUMERIC,
	DIV,
	MUL,
	ADD,
	SUB,
	POW,
	SIN,
	SQRT,
	LOG,
} TermTag;

typedef struct {
	TermTag tag;
	float value;
} Term;

static inline void check(Term expr[EXPR_LEN], unsigned len, float goal) {
	float stack[EXPR_LEN] = {0.0};
	int stack_ptr = -1; // the current highest point
	for (unsigned i = 0; i < len; ++i) {
		Term t = expr[i];
		switch (t.tag) {
		case NUMERIC:
			stack[stack_ptr + 1] = t.value;
			++stack_ptr;
			break;
		case DIV:
#ifdef SANITY_CHECKS
			if (stack_ptr < 1)
				return;
#endif
			stack[stack_ptr - 1] /= stack[stack_ptr];
			--stack_ptr;
			break;
		case MUL:
#ifdef SANITY_CHECKS
			if (stack_ptr < 1)
				return;
#endif
			stack[stack_ptr - 1] *= stack[stack_ptr];
			--stack_ptr;
			break;
		case ADD:
#ifdef SANITY_CHECKS
			if (stack_ptr < 1)
				return;
#endif
			stack[stack_ptr - 1] += stack[stack_ptr];
			--stack_ptr;
			break;
		case SUB:
#ifdef SANITY_CHECKS
			if (stack_ptr < 1)
				return;
#endif
			stack[stack_ptr - 1] -= stack[stack_ptr];
			--stack_ptr;
			break;
		case POW:
#ifdef SANITY_CHECKS
			if (stack_ptr < 1)
				return;
#endif
			stack[stack_ptr - 1] =
			    powf(stack[stack_ptr - 1], stack[stack_ptr]);
			--stack_ptr;
			break;
		case SIN:
#ifdef SANITY_CHECKS
			if (stack_ptr < 0)
				return;
#endif
			stack[stack_ptr] = sinf(stack[stack_ptr]);
			break;
		case SQRT:
#ifdef SANITY_CHECKS
			if (stack_ptr < 0)
				return;
#endif
			stack[stack_ptr] = sqrtf(stack[stack_ptr]);
			break;
		case LOG:
#ifdef SANITY_CHECKS
			if (stack_ptr < 0)
				return;
#endif
			stack[stack_ptr] = logf(stack[stack_ptr]);
			break;
		}
	}
	if (stack_ptr != 0)
		return;
	float ans = *stack;
	if (ABS(ans - goal) < 0.5) {
		// if (ABS(ans / goal - 1) < 0.00001) {
		for (unsigned i = 0; i < len; ++i) {
			if (expr[i].tag == NUMERIC) {
				printf("%.4f, ", expr[i].value);
			} else {
				char *tags[] = {
				    [NUMERIC] = "?", [DIV] = "/",	  [MUL] = "*",
				    [ADD] = "+",	   [SUB] = "-",	  [POW] = "^",
				    [SIN] = "sin",   [SQRT] = "sqrt", [LOG] = "ln"};
				printf("%s, ", tags[expr[i].tag]);
			}
		}
		printf("= %f (%f, %f%%)\n", ans, ans - goal, ans / goal * 100);
	}
}

int main(int argv, char **argc) {
	const uint8_t num_nums = 1;
	const uint8_t num_unary = 3;
	const Term opts[] = {
	    // numeric
	    {.tag = NUMERIC, .value = 1},
	    {.tag = NUMERIC, .value = 2},
	    {.tag = NUMERIC, .value = 3},
	    {.tag = NUMERIC, .value = 4},
	    //{.tag = NUMERIC, .value = 3.14159265358979323846264338327950},
	    //{.tag = NUMERIC, .value = 2.71828182845904523536028747135266},

	    // unary
	    {.tag = SIN},
	    {.tag = SQRT},
	    {.tag = LOG},

	    // binary
	    {.tag = DIV},
	    {.tag = MUL},
	    {.tag = ADD},
	    {.tag = SUB},
	    {.tag = POW},
	};
	char shift[] = {

	    // 1,
	    // 1,
	    1, 1, 1, 1, 0, 0, 0, -1, -1, -1, -1, -1};
	assert(LEN(opts) == LEN(shift));

	Term expr[EXPR_LEN];
	float goal = atof(argc[1]);

#ifdef PRECOMPUTE_PROD
	// clang-format off
	const uint64_t prods[EXPR_LEN] = {
		1,
		LEN(opts),
		LEN(opts) * LEN(opts),
		LEN(opts) * LEN(opts) * LEN(opts),
		LEN(opts) * LEN(opts) * LEN(opts) * LEN(opts),
		LEN(opts) * LEN(opts) * LEN(opts) * LEN(opts) * LEN(opts),
		LEN(opts) * LEN(opts) * LEN(opts) * LEN(opts) * LEN(opts) * LEN(opts),
		LEN(opts) * LEN(opts) * LEN(opts) * LEN(opts) * LEN(opts) * LEN(opts) * LEN(opts),
		LEN(opts) * LEN(opts) * LEN(opts) * LEN(opts) * LEN(opts) * LEN(opts) * LEN(opts) * LEN(opts),
		LEN(opts) * LEN(opts) * LEN(opts) * LEN(opts) * LEN(opts) * LEN(opts) * LEN(opts) * LEN(opts) * LEN(opts),
		LEN(opts) * LEN(opts) * LEN(opts) * LEN(opts) * LEN(opts) * LEN(opts) * LEN(opts) * LEN(opts) * LEN(opts) * LEN(opts),
		LEN(opts) * LEN(opts) * LEN(opts) * LEN(opts) * LEN(opts) * LEN(opts) * LEN(opts) * LEN(opts) * LEN(opts) * LEN(opts) * LEN(opts),
		LEN(opts) * LEN(opts) * LEN(opts) * LEN(opts) * LEN(opts) * LEN(opts) * LEN(opts) * LEN(opts) * LEN(opts) * LEN(opts) * LEN(opts) * LEN(opts),
		LEN(opts) * LEN(opts) * LEN(opts) * LEN(opts) * LEN(opts) * LEN(opts) * LEN(opts) * LEN(opts) * LEN(opts) * LEN(opts) * LEN(opts) * LEN(opts) * LEN(opts),
		LEN(opts) * LEN(opts) * LEN(opts) * LEN(opts) * LEN(opts) * LEN(opts) * LEN(opts) * LEN(opts) * LEN(opts) * LEN(opts) * LEN(opts) * LEN(opts) * LEN(opts) * LEN(opts),
		LEN(opts) * LEN(opts) * LEN(opts) * LEN(opts) * LEN(opts) * LEN(opts) * LEN(opts) * LEN(opts) * LEN(opts) * LEN(opts) * LEN(opts) * LEN(opts) * LEN(opts) * LEN(opts) * LEN(opts),
		LEN(opts) * LEN(opts) * LEN(opts) * LEN(opts) * LEN(opts) * LEN(opts) * LEN(opts) * LEN(opts) * LEN(opts) * LEN(opts) * LEN(opts) * LEN(opts) * LEN(opts) * LEN(opts) * LEN(opts) * LEN(opts),
		LEN(opts) * LEN(opts) * LEN(opts) * LEN(opts) * LEN(opts) * LEN(opts) * LEN(opts) * LEN(opts) * LEN(opts) * LEN(opts) * LEN(opts) * LEN(opts) * LEN(opts) * LEN(opts) * LEN(opts) * LEN(opts) * LEN(opts),
		LEN(opts) * LEN(opts) * LEN(opts) * LEN(opts) * LEN(opts) * LEN(opts) * LEN(opts) * LEN(opts) * LEN(opts) * LEN(opts) * LEN(opts) * LEN(opts) * LEN(opts) * LEN(opts) * LEN(opts) * LEN(opts) * LEN(opts) * LEN(opts),
		LEN(opts) * LEN(opts) * LEN(opts) * LEN(opts) * LEN(opts) * LEN(opts) * LEN(opts) * LEN(opts) * LEN(opts) * LEN(opts) * LEN(opts) * LEN(opts) * LEN(opts) * LEN(opts) * LEN(opts) * LEN(opts) * LEN(opts) * LEN(opts) * LEN(opts),
	};
	// clang-format on
#else
	unsigned long long prods[EXPR_LEN] = {};
	for (int i = 0; i < EXPR_LEN; ++i) {
		prods[i] = 1;
		for (int j = 0; j < i; j++) {
			prods[i] *= LEN(opts);
		}
	}
#endif
#pragma unroll
	for (uint32_t len = 1; len < EXPR_LEN; ++len) {
		printf("%d\n", len);
		for (uint64_t i = 0; i < prods[len]; ++i) {
			char depth = 0;
			// if we choose something too late for the last opt then we
			// will be invalid, as thats an operator
			if (i % LEN(opts) >= num_nums)
				continue;
			// similar to above, cant do a binary op second
			if ((i % (LEN(opts) * LEN(opts))) / LEN(opts) >=
			    num_unary + num_nums)
				continue;
#pragma unroll
			for (uint32_t j = 0; j < len; ++j) {
				uint32_t index = (i % prods[j + 1]) / prods[j];
				depth += shift[index];
				if (depth < 0)
					goto skip;
				expr[j] = opts[index];
			}
			if (depth != 1)
				continue;
			check(expr, len, goal);
		skip:
		}
	}
}
