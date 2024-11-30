#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#define EXPR_LEN 10
#define ABS(x) ((x) > 0 ? (x) : -(x))
#define LEN(x) (sizeof(x) / sizeof(x[0]))

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
			if (stack_ptr < 1)
				return;
			stack[stack_ptr - 1] /= stack[stack_ptr];
			--stack_ptr;
			break;
		case MUL:
			if (stack_ptr < 1)
				return;
			stack[stack_ptr - 1] *= stack[stack_ptr];
			--stack_ptr;
			break;
		case ADD:
			if (stack_ptr < 1)
				return;
			stack[stack_ptr - 1] += stack[stack_ptr];
			--stack_ptr;
			break;
		case SUB:
			if (stack_ptr < 1)
				return;
			stack[stack_ptr - 1] -= stack[stack_ptr];
			--stack_ptr;
			break;
		case POW:
			if (stack_ptr < 1)
				return;
			stack[stack_ptr - 1] =
			    powf(stack[stack_ptr - 1], stack[stack_ptr]);
			--stack_ptr;
			break;
		case SIN:
			if (stack_ptr < 0)
				return;
			stack[stack_ptr] = sinf(stack[stack_ptr]);
			break;
		case SQRT:
			if (stack_ptr < 0)
				return;
			stack[stack_ptr] = sqrtf(stack[stack_ptr]);
			break;
		case LOG:
			if (stack_ptr < 0)
				return;
			stack[stack_ptr] = logf(stack[stack_ptr]);
			break;
		}
	}
	if (stack_ptr != 0)
		return;
	float ans = *stack;
	if (ABS(ans / goal - 1) < 0.00001) {
		for (unsigned i = 0; i < len; ++i) {
			if (expr[i].tag == NUMERIC) {
				printf("%.4f, ", expr[i].value);
			} else {
				char *tags[] = {"?", "/",   "*",	"+", "-",
						    "^", "sin", "sqrt", "ln"};
				printf("%s, ", tags[expr[i].tag]);
			}
		}
		printf("= %f (%f, %f%%)\n", ans, ans - goal, ans / goal * 100);
	}
}

int main(int argv, char **argc) {
	Term opts[] = {
	    {.tag = NUMERIC, .value = 1},
	    {.tag = NUMERIC, .value = 2},
	    {.tag = NUMERIC, .value = 3},
	    {.tag = NUMERIC, .value = 4},
	    {.tag = NUMERIC, .value = 3.14159265358979323846264338327950},
	    {.tag = NUMERIC, .value = 2.71828182845904523536028747135266},
	    {.tag = DIV},
	    {.tag = MUL},
	    {.tag = ADD},
	    {.tag = SUB},
	    {.tag = POW},
	    {.tag = SIN},
	    {.tag = SQRT},
	    {.tag = LOG},
	};
	Term t[] = {opts[3], opts[1], opts[6], opts[3],
			opts[0], opts[6], opts[4]};
	Term expr[EXPR_LEN];
	float goal = atof(argc[1]);

	unsigned long long prods[EXPR_LEN] = {};
	for (int i = 0; i < EXPR_LEN; ++i) {
		prods[i] = 1;
		for (int j = 0; j < i; j++) {
			prods[i] *= LEN(opts);
		}
	}
#pragma unroll
	for (unsigned len = 1; len < EXPR_LEN; ++len) {
		printf("%d\n", len);
		for (unsigned long long i = 0; i < prods[len]; ++i) {
#pragma unroll
			for (unsigned j = 0; j < len; ++j) {
				expr[j] = opts[(i % prods[j + 1]) / prods[j]];
			}
			check(expr, len, goal);
		}
	}
}
