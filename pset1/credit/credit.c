#include <cs50.h>
#include <stdio.h>

char *check_if_card_num_valid(long num)
{
    int prev_digit = 0;
    int cur_digit = 0;
    int digits = 0;
    int odds_sum = 0;
    int evens_sum = 0;
    int brand_index = 0;

    do
    {
        prev_digit = cur_digit;
        // The current digit
        cur_digit = num % 10;
        // Even digit
        if (digits % 2 == 0)
        {
            evens_sum += cur_digit;
        }
        // Odd digit
        else
        {
            int doubled = cur_digit * 2;
            // Get each bit of the number if the sum is bigger than 10
            // e.g. 12 => 1 + 2
            if (doubled > 9)
            {
                odds_sum += doubled % 10 + doubled / 10;
            }
            else
            {
                odds_sum += doubled;
            }
        }
        digits++;
    }
    while (num /= 10);

    if ((evens_sum + odds_sum) % 10 == 0)
    {
        // Calulate brand index
        brand_index = cur_digit * 10 + prev_digit;
        printf("%d\n", brand_index);
        // VISA
        if (brand_index >= 40
            && brand_index <= 49
            && digits >= 13
            && digits <= 16)
        {
            return "VISA";
        }
        // MasterCard
        else if (brand_index >= 51
                 && brand_index <= 55
                 && digits == 16)
        {
            return "MASTERCARD";
        }
        // American Express
        else if ((brand_index == 34 || brand_index == 37)
                 && digits == 15)
        {
            return "AMEX";
        }
    }

    return "INVALID";
}

int main(void)
{
    long input = get_long("Number: ");

    char *res = check_if_card_num_valid(input);
    printf("%s\n", res);
}
