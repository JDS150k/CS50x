#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int height;
    do
    {
        height = get_int("Height: ");
    }
    while (height <1 || height >8 );


    for(int height; height > 0; height-- )
    {
        for(int space_counter_l = height - 1; space_counter_l > 0; space_counter_l-- )
        {
            printf(" ");
        }
        for(int hash_counter_l = height; hash_counter_l <= height; hash_counter_l++ )
        {
            printf("#");
        }
        printf("  ");
        for(int hash_counter_r = height; hash_counter_r <= height; hash_counter_r++ )
        {
            printf("#");
        }
        printf("\n");
    }

}