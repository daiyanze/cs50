from cs50 import get_int

# input height
height = 0
while height < 1 or height > 8:
    height = get_int("Height: ")

# print out pyramid
for i in range(height):
    for j in range(height + 2 + i + 1):
        if j < height - i - 1 or j == height + 0 or j == height + 1:
            print(' ', end='')
        else:
            print('#', end='')
    print()
