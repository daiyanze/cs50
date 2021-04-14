from cs50 import get_string

# get input string
input_str = get_string('Text: ')

# word count
# split into list and count items
word_count = len(input_str.split())

# letter count
# count letter by checking if is alphabetic
letter_count = sum(1 for i in input_str if i.isalpha())

# sentence count
# if char is . ? !
sentence_count = sum(1 for i in input_str if i == '.' or i == '!' or i == '?')

# Coleman-Liau index
index = 0.0588 * (letter_count / word_count * 100) - 0.296 * (sentence_count / word_count * 100) - 15.8

if index < 1:
    print('Before Grade 1')
elif index >= 16:
    print('Grade 16+')
else:
    print(f'Grade {int(index + 0.5)}')