// Program to implement Boyer Moore Algorithm

#include <stdio.h>
#include <string.h>

#define SIZE 256

// function to build bad character table
void badCharTable(char pattern[], int m, int badChar[])
{
    int i;

    for(i = 0; i < SIZE; i++)
        badChar[i] = -1;

    for(i = 0; i < m; i++)
        badChar[(int)pattern[i]] = i;
}

int main()
{
    char text[100], pattern[50];
    int badChar[SIZE];
    int n, m, s = 0;

    printf("Enter text: ");
    scanf("%s", text);

    printf("Enter pattern: ");
    scanf("%s", pattern);

    n = strlen(text);
    m = strlen(pattern);

    badCharTable(pattern, m, badChar);

    while(s <= (n - m))
    {
        int j = m - 1;

        while(j >= 0 && pattern[j] == text[s + j])
            j--;

        if(j < 0)
        {
            printf("Pattern found at position %d\n", s);

            if(s + m < n)
                s = s + m - badChar[text[s + m]];
            else
                s = s + 1;
        }
        else
        {
            int shift = j - badChar[text[s + j]];
            if(shift < 1)
                shift = 1;

            s = s + shift;
        }
    }

    return 0;
}