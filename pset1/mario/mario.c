#include <cs50.h>
#include <stdio.h>

int get_height(void)
{

    if (height > 8 || height < 1)
    {
        return get_height();
    }

    return height;
}

int main(void)
{
    // Get pyramid height
    int height = get_height();
    // Set row length (2 times of height and space)
    int row = (height + 1) * 2;

    // Height
    for (int i = 0; i < height; i++)
    {
        // Row
        for (int j = 0; j < row; j++)
        {
            if (j < height - i - 1 || j == height + 0 || j == height + 1)
            {
                printf(" ");
            }
            else if (j <= height + i + 2)
            {
                printf("#");
            }
        }

        // Breakline
        printf("\n");
    }
}
