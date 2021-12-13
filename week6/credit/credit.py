from cs50 import get_int
import sys

# Get valid number from user
accepted = False
input = get_int("Please type your card number: ")
if input < 4000000000000 or input > 5599999999999999:
    print("INVALID")
    sys.exit(0)
else:
    accepted = True

# calculate the value for each digit (d#) variable.
d = []
for i in range(0, 18):
    d.append(0)
m = []
for i in range(0, 18):
    m.append(0)


if accepted == True:
    for counter in range(1, 17, 1):
        n = pow(10, counter)
        working_number = input % n
        m[counter] = working_number
        d[counter] = ((m[counter] - m[counter - 1]) * 10) / n

    # Calculate the First Checksum Value
    checksum1 = 0
    for counter_2 in range(2, 18, 2):
        current_digit = d[counter_2]
        units = (current_digit * 2) % 10
        tens = (((current_digit * 2) % 100) - units) / 10
        checksum1 = checksum1 + units + tens

    # Compare 1st checksum value with the 2nd checksum value
    checksum2 = d[1] + d[3] + d[5] + d[7] + d[9] + d[11] + d[13] + d[15]
    if not (checksum1 + checksum2) % 10 == 0:
        print("INVALID")
        sys.exit(0)

    # Identify the card type
    if d[16] == 0:
        if d[15] == d[14] == 0:
            if d[13] == 4:
                print("VISA")
                sys.exit(0)
        elif d[15] == 3:
            if d[14] == 4 or d[14] == 7:
                print("AMEX")
                sys.exit(0)
    elif d[16] == 4:
        print("VISA")
    elif d[16] == 5:
        if d[15] > 0 and d[15] < 6:
            print("MASTERCARD")
            sys.exit(0)
    else:
        print("INVALID")
        sys.exit(0)