import csv
import sys


def main():
    # Print out usage
    if len(sys.argv) != 3:
        sys.exit("Usage: python dna.py data.csv sequence.txt")

    database_filename = sys.argv[1]
    sequence_filename = sys.argv[2]

    database = read_db_csv_to_dict(database_filename)

    STRs = get_STRs_list(database_filename)
    sequence_dict = read_seq_to_dict(sequence_filename, STRs)

    key = hash_STRs_dict(sequence_dict)
    match = database.get(key)
    if match is None:
        print('No match')
    else:
        print(match)

    print(STRs)
    print(sequence_dict)

    return


def hash_STRs_dict(sequence_dict):
    """
    Turn list of dict {sequence:count} into string. The output of such string is ordered count of each STR
    e.g. {'AATG': 1, 'TCTAG': 2: 'GATA': 1} => '121'
    """
    return ''.join([str(sequence_dict[s]) for s in sequence_dict])


def get_STRs_list(filename):
    """
    Fetch the first line of database csv file and turn the rest of columns into list of STRs
    e.g. name,AATG,TCTAG,GATA => ['AATG','TCTAG','GATA']
    """
    with open(filename) as file:
        reader = csv.reader(file)
        return next(reader)[1:]


def read_db_csv_to_dict(filename):
    """
    Read database csv file and turn data into a dict which key is hashed string
    e.g. {'name':'Alan', 'AATG': 1, 'TCTAG': 2: 'GATA': 1} => {'121': 'Alan'}
    """
    res = {}
    with open(filename) as file:
        reader = csv.DictReader(file)

        for row in reader:
            name = row['name']
            row.pop('name', None)
            res[hash_STRs_dict(row)] = name

    return res


def read_seq_to_dict(filename, STRs):
    """
    Read sequence file text into dict of STRs & counts
    e.g. AAGTAAGATATCTAGATCTAGAA => {'AATG': 1, 'TCTAG': 2: 'GATA': 1}
    """
    res = {}
    with open(filename) as file:
        reader = file.read()

        for s in STRs:
            i = 1
            temp = s
            # If the repeated string exists, then add up the string and check once more
            # AGATC => exists then AGATCAGATC => exists then AGATCAGATCAGATC => not exist
            # In this case, AGATC has largest repeats of 2
            while reader.find(temp) >= 0:
                i = i + 1
                # generate consecutive STRs
                # e.g. "AGATC" * 2 = "AGATCAGATC"
                temp = s * i

            # Remove duplicated first time check
            res[s] = i - 1
    return res

if __name__ == '__main__':
    main()
