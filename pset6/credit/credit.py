from cs50 import get_string

# get credit card number input
input = [int(digit) for digit in get_string('Number: ')]
digits = len(input)

# Odd digit sum
odd_sum = sum(
    (digit * 2 > 9)
    and (digit * 2 % 10 + int(digit * 2 / 10))
    or digit * 2
    for idx, digit in enumerate(input[::-1]) if idx % 2 != 0)

# Even digit sum
even_sum = sum(digit for idx, digit in enumerate(input[::-1]) if idx % 2 == 0)

# Check Brand
if (odd_sum + even_sum) % 10 == 0:
    brand_index = int(''.join(['%d' % i for i in input[0:2]]))
    # Visa
    if 40 <= brand_index <= 49 and 13 <= digits <= 16:
        print('VISA')
    # MasterCard
    elif 51 <= brand_index <= 55 and digits == 16:
        print('MASTERCARD')
    # American Express
    elif brand_index == 34 or brand_index == 37 and digits == 15:
        print('AMEX')
else:
    print('INVALID')