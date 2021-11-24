#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

int main(void)

{
    int letters = 0;
    int words = 1;
    int sentences = 0;
    string str = get_string("Text: ");
    //iterates over string to quantify letters
    for (int i = 0; str[i] != 0; i++)
    {
        if (isalpha(str[i]))
        {
            letters++;
        }
    }
    //iterates over string to quantify words
    for (int i = 0; str[i] != 0; i++)
    {
        if (str[i] == 32)
        {
            words++;
        }
    }
    //iterates over string to quantify sentences
    for (int i = 0; str[i] != 0; i++)
    {
        if ((str[i] == 33) || (str[i] == 63) || (str[i] == 46))
        {
            sentences++;
        }
    }
    float l = (float)letters / (float)words * 100;
    float s = (float)sentences / (float)words * 100;
    float index = 0.0588 * l - 0.296 * s - 15.8;
    //  L is the average number of letters per 100 words in the text, and S is the average number of sentences per 100 words in the text.
    int grade = round(index);
    if (grade > 15)
    {
        printf("Grade 16+\n");
    }
    else if (grade < 1)
    {
        printf("Before Grade 1\n");
    }
    else
    {
        printf("Grade %i\n", grade);
    }
}
