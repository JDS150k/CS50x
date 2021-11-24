#include <ctype.h>
#include <cs50.h>
#include <stdio.h>
#include <string.h>


string encrypt(string key, string plain);


int main(int argc, string argv[])

{
    //________________________________________________________// TO DO: add more conditions to filter out duplicate chars

    if ((argc == 2) && (strlen(argv[1]) == 26))
    {
        string input = argv[1];

        for (int i = 0; i < 26; i++)
        {
            if ((input[i] < 65) || (input[i] > 90 && input[i] < 97) || (input[i] > 122))
            {
                printf("invalid input - includes non alphabetical character at position %i\n", i);
                return 1;
            }
            for (int j = i + 1; j < 26; j++)
            {
                if (input[i] == input[j])
                {
                    printf("invalid input - duplicate characters at position %i and %i", i, j);
                    return 1;
                }
            }
        }

        char key[52];
        int length = 26;

        //___________________________________________________// Routine to transpose toupper of input array onto first half of key array
        for (int i = 0; i < 26; i++)
        {
            key[i] = toupper(input[i]);
        }


        //___________________________________________________// Routine to transpose tolower of input array onto second half of key array
        for (int i = 0; input[i] != '\0'; i++, length++)
        {
            key[length] = tolower(input[i]);
        }
        key[length] = '\0';


        const string plain = get_string("plaintext: ");
        string cipher = plain;

        length = strlen(plain);
        for (int i = 0; i < length; i++)
        {
            if (plain[i] > 96 && plain[i] < 123)
            {
                cipher[i] = key[(plain[i] - 71)];
            }
            else if (plain[i] > 64 && plain[i] < 91)
            {
                cipher[i] = key[(plain[i] - 65)];
            }
        }
        printf("ciphertext: %s\n", cipher);
        return 0;
    }
    else
    {
        printf("invalid input - did not recieve 26 characters");
        return 1;
    }
}