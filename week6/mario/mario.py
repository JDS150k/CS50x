from cs50 import get_int

height = 0
while height < 1 or height > 8:
    height = get_int("Height: ")            # Get valid user input

for i in range(height, 0, -1):              # Iterate rows
    space_counter = i - 1
    for j in range(space_counter, 0, -1):   # Iterate over spaces
        print(" ", end="")
    for k in range(i, height + 1, 1):       # Iterate over 1st pyramid hashes
        print("#", end="")
    print("  ", end="")                     # Insert double space gap
    hash_counter_right = i
    for l in range(i, height + 1, 1):       # Iterate over 2nd pyramid hashes
        print("#", end="")
    print("")                               # End row, new line