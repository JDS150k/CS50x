from cs50 import get_string

letters = 0
words = 1
sentences = 0
str = get_string("Text: ")
len = len(str)

# iterates over string to quantify letters
for i in range(0, len):
    if str[i].isalpha():
        letters += 1

# iterates over string to quantify words
for i in range(0, len):
    if str[i] == " ":
        words += 1

# iterates over string to quantify sentences
for i in range(0, len):
    if (str[i] == ".") or (str[i] == "!") or (str[i] == "?"):
        sentences += 1

# L is the average number of letters per 100 words in the text, and S is the average number of sentences per 100 words in the text.

l = letters / words * 100.0000
s = sentences / words * 100.0000
index = 0.0588 * l - 0.296 * s - 15.8
grade = round(index)

if grade > 15:
    print("Grade 16+")
elif grade < 1:
    print("Before Grade 1")
else:
    print(f"Grade {grade}")