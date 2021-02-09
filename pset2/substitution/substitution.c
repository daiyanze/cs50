#include <cs50.h>
#include <stdio.h>
#include <string.h>

// The upper-case alphabet characters calulous result based on its ASCII number.
// This will be used for checking key validity
// 65 ^ 66 ^ 67 ^ ... ^ 90 = 27
const int ALPHABET_XOR = 27;

int main(int argc, string argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }

    // Check key size
    string key = argv[1];
    int key_len = strlen(key);
    if (key_len != 26)
    {
        printf("Key must contain 26 characters.\n");
        return 1;
    }

    // Check if each key character valid
    int dup = 0;
    for (int i = 0; i < key_len; i++)
    {
        char k = key[i];
        if (k >= 65 && k <= 90)
        {
            dup ^= key[i];
        }
        // to upper case
        else if (k >= 97 && k <= 122)
        {
            dup ^= key[i] - 32;
        }
        else
        {
            printf("Invalid character in key: %c.\n", key[i]);
            return 1;
        }
    }

    // Check if duplicated
    if (dup != ALPHABET_XOR)
    {
        printf("Duplicated character in key.\n");
        return 1;
    }


    string plaintext = get_string("plaintext: ");
    int plaintext_len = strlen(plaintext);
    char ciphertext[plaintext_len + 1];
    char c;
    for (int i = 0; i < plaintext_len; i++)
    {
        if (plaintext[i] >= 65 && plaintext[i] <= 90)
        {
            c = key[plaintext[i] - 65];
            if (c > 90)
            {
                c -= 32;
            }
        }
        else if (plaintext[i] >= 97 && plaintext[i] <= 122)
        {
            c = key[plaintext[i] - 97];
            if (c < 97)
            {
                c += 32;
            }
        }
        else
        {
            c = plaintext[i];
        }
        ciphertext[i] = c;
    }
    ciphertext[plaintext_len] = '\0';

    printf("ciphertext: %s\n", ciphertext);
    return 0;
}