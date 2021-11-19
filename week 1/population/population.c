#include <cs50.h>
#include <stdio.h>



int main(void)

{
    
    
    // Prompt for & print a starting population that is > 9
    int start_pop;
    do
    {
        start_pop = get_int("Start size?: \n");
        if (start_pop < 9)
        {
            printf("Please enter a starting population size of 9 or more.\n");
        }
    }
    while (start_pop < 9);
    
    
    // Prompt for & print an ending population that is > the starting population
    int end_pop;
    do
    {
        end_pop = get_int("End size?: \n");
        if (end_pop < start_pop)
        {
            printf("Please enter an ending population size greater than or equal to the starting population.\n");
        }

    }
    while (end_pop < start_pop);


    // Calculate number of years until we reach threshold
    int n = 0;
    if (start_pop < end_pop)
    {
        do
        {  
            start_pop = start_pop + (start_pop / 3) - (start_pop / 4);
            n++;
        }
        while (start_pop < end_pop);    
    }
    
    
    // Print number of years
    printf("Years: %i\n", n);
}