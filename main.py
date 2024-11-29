import itertools
import math
import sys


def evaluate(expr, goal):
    stack = []
    for e in expr:
        if isinstance(e, float):
            stack.append(e)
        elif len(stack) < 1:
            return False
        elif e == "sin":
            stack[-1] = math.sin(stack[-1])
        elif e == "cos":
            stack[-1] = math.cos(stack[-1])
        elif e == "tan":
            stack[-1] = math.tan(stack[-1])
        elif e == "sqrt":
            if stack[-1] < 0:
                return False
            stack[-1] = math.sqrt(stack[-1])
        elif e == "log":
            if stack[-1] <= 0:
                return False
            stack[-1] = math.log2(stack[-1])
        elif len(stack) < 2:
            return False
        elif e == "/":
            if stack[-1] == 0:
                return False
            stack[-2] /= stack[-1]
            del stack[-1]
        elif e == "*":
            stack[-2] *= stack[-1]
            del stack[-1]
        elif e == "+":
            stack[-2] += stack[-1]
            del stack[-1]
        elif e == "-":
            stack[-2] -= stack[-1]
            del stack[-1]
        elif e == "^":
            if stack[-2] > 0:
                try:
                    stack[-2] = stack[-2] ** stack[-1]
                except OverflowError:
                    return False
            del stack[-1]
    if len(stack) != 1:
        return False
    if stack[0] == 0:
        stack[0] = 0.0000000001
    if stack[0] > 100000 or stack[0] < -100000:
        return False
    if abs(goal / stack[0] - 1) < 0.0001:
        return stack[0]


opts = [1.0, 2.0, 3.0, 4.0, math.pi, math.e, "/", "*", "+", "-", "^", "sin", "sqrt", "log"]
goal = float(sys.argv[1])
for i in range(1, 10):
    print(i)
    for e in itertools.product(opts, repeat=i):
        if ans := evaluate(e, goal):
            print(f"{e} = {ans} ({ans - goal}, {ans / goal * 100}%)")
