#include <cs50.h>
#include <stdio.h>
#include <math.h>

int main(void)
{
    bool accepted = false;
    long input, n, working_number, counter = 1, checksum1 = 0, checksum2 = 0, units = 0, tens = 0, subtotal = 0, current_digit = 0;
    long m16, m15, m14, m13, m12, m11, m10, m9, m8, m7, m6, m5, m4, m3, m2, m1,
        d16, d15, d14, d13, d12, d11, d10, d9, d8, d7, d6, d5, d4, d3, d2, d1;
    m16 = m15 = m14 = m13 = m12 = m11 = m10 = m9 = m8 = m7 = m6 = m5 = m4 = m3 = m2 = m1 = 0 ;
    d16 = d15 = d14 = d13 = d12 = d11 = d10 = d9 = d8 = d7 = d6 = d5 = d4 = d3 = d2 = d1 = 0 ;


    // Get valid number from user
    input = get_long("Please type your card number: \n");
    if (input - 4000000000000 < 0 || input - 5599999999999999 > 0)
    {
        printf("INVALID\n");
    }
    else
    {
        printf("Number accepted.\n");
        accepted = true;
    
    
        // calculate the value for each digit (d#) variable.
        for (counter = 1; counter > 0 && counter < 17; counter++)
        {
            n = pow(10, counter);
            working_number = input % n;
            if (counter == 1)
            {
                m1 = working_number;
                d1 = m1;
            }
            else if (counter == 2)
            {
                m2 = working_number;
                d2 = ((m2 - m1) * 10) / n;
            }
            else if (counter == 3)
            {
                m3 = working_number;
                d3 = ((m3 - m2) * 10) / n;
            }
            else if (counter == 4)
            {
                m4 = working_number;
                d4 = ((m4 - m3) * 10) / n;
            }
            else if (counter == 5)
            {
                m5 = working_number;
                d5 = ((m5 - m4) * 10) / n;
            }
            else if (counter == 6)
            {
                m6 = working_number;
                d6 = ((m6 - m5) * 10) / n;
            }
            else if (counter == 7)
            {
                m7 = working_number;
                d7 = ((m7 - m6) * 10) / n;
            }
            else if (counter == 8)
            {
                m8 = working_number;
                d8 = ((m8 - m7) * 10) / n;
            }
            else if (counter == 9)
            {
                m9 = working_number;
                d9 = ((m9 - m8) * 10) / n;
            }
            else if (counter == 10)
            {
                m10 = working_number;
                d10 = ((m10 - m9) * 10) / n;
            }
            else if (counter == 11)
            {
                m11 = working_number;
                d11 = ((m11 - m10) * 10) / n;
            }
            else if (counter == 12)
            {
                m12 = working_number; 
                d12 = ((m12 - m11) * 10) / n;
            }  
            else if (counter == 13)
            {
                m13 = working_number;
                d13 = ((m13 - m12) * 10) / n;
            }
            else if (counter == 14)
            {
                m14 = working_number;
                d14 = ((m14 - m13) * 10) / n;
            }
            else if (counter == 15)
            {
                m15 = working_number;
                d15 = ((m15 - m14) * 10) / n;
            }
            else
            {
                m16 = working_number;
                d16 = ((m16 - m15) * 10) / n;
            }
        }   
    
    
        // Calculate the First Checksum Value
    
        for (counter = 2; counter < 17; counter += 2)
        {   
            if (counter == 2)
            {
                current_digit = d2;
            }
            else if (counter == 4)
            {
                current_digit = d4;
            }
            else if (counter == 6)
            {
                current_digit = d6;
            }   
            else if (counter == 8)
            {
                current_digit = d8;
            }
            else if (counter == 10)
            {
                current_digit = d10;
            }   
            else if (counter == 12)
            {
                current_digit = d12;
            }   
            else if (counter == 14)
            {
                current_digit = d14;
            }
            else    
            {
                current_digit = d16;
            }   
            units = (current_digit * 2) % 10;
            tens = (((current_digit * 2) % 100) - units) / 10;
            subtotal = units + tens;
            checksum1 = checksum1 + subtotal;
        }
    
    
        // Compare 1st checksum value with the 2nd checksum value
        checksum2 = d1 + d3 + d5 + d7 + d9 + d11 + d13 + d15;
        if ((checksum1 + checksum2) % 10 == 0)
        {
            printf("Checksum passed.\n");

            //  Identify the card type
            if (d16 == 0)
            {
                if (d15 == 0)
                {
                    if (d14 == 0)
                    {
                        if (d13 == 0)
                        {
                            printf("INVALID\n");
                        }   
                        else if (d13 == 4)
                        {
                            printf("VISA\n");
                        }
                    }
                    else
                    {
                        printf("INVALID\n");
                    }
                }
                else if (d15 == 3)
                {
                    if (d14 == 4 || d14 == 7)
                    {
                        printf("AMEX\n");
                    }
                    else
                    {
                        printf("INVALID\n");
                    }
                }
            }
            else if (d16 == 4)
            {
                printf("VISA\n");
            }
            else if (d16 == 5)
            {
                if (d15 > 0 && d15 < 6)
                {
                    printf("MASTERCARD\n");
                }
                else
                {
                    printf("INVALID\n");
                }
            }
            else
            {
                printf("INVALID\n");
            }
        }
        else
        {
            printf("Checksum failed.\n");
            printf("INVALID\n");
        }
    }
}