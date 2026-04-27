// Program to implement Naive Algorithm.

#include <stdio.h>
#include <string.h>

int main()
{
    char text[100], pattern[50];
    int i, j, n, m, found;

    printf("Enter the main string: ");
    scanf("%s", text);

    printf("Enter the pattern: ");
    scanf("%s", pattern);

    n = strlen(text);
    m = strlen(pattern);

    for(i = 0; i <= n - m; i++)
    {
        found = 1;

        for(j = 0; j < m; j++)
        {
            if(text[i + j] != pattern[j])
            {
                found = 0;
                break;
            }
        }

        if(found == 1)
        {
            printf("Pattern found at position %d\n", i);
        }
    }

    return 0;
}