#include <cs50.h>
#include <string.h>
#include <stdio.h>

int main(void)
{
    string input_text = get_string("Text: ");

    int word_count = 0;
    int letter_count = 0;
    int sentence_count = 0;
    int cur = '\0';
    for (int i = 0; input_text[i] != '\0'; i++)
    {
        cur = input_text[i];
        if ((cur >= 'a' && cur <= 'z') || (cur >= 'A' && cur <= 'Z'))
        {
            letter_count++;
        }
        else if (cur == ' ')
        {
            if (word_count == 0)
            {
                word_count++;
            }
            word_count++;
        }
        else if (cur == '.' || cur == '?' || cur == '!')
        {
            sentence_count++;
        }
    }

    // Letters per 100 words
    float L = letter_count * 1.0 / word_count * 100.0;
    // Sentences per 100 words
    float S = sentence_count * 1.0 / word_count * 100.0;
    // Coleman-Liau index
    float index = 0.0588 * L - 0.296 * S - 15.8;

    if (index < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (index >= 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %i\n", (int)(index + 0.5));
    }
}