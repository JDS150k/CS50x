import sys
import cs50
import csv

# Count the longest run of consecutive repeats of the STR in the seq


def count_consec_rep(STR, seq):
    if seq.find(STR) >= 0:
        counter = top_count = 1
        seq = seq[seq.find(STR)+len(STR):len(seq)]
        while seq.find(STR) >= 0:
            if seq.find(STR) == 0:
                counter += 1
                if counter > top_count:
                    top_count = counter
            else:
                counter = 1
            seq = seq[seq.find(STR)+len(STR):len(seq)]
        return top_count


if len(sys.argv) != 3:                              # Check usage
    print("Usage: dna.py database sequence")
    sys.exit()
else:                                               # Read inputs into memory
    people = []
    with open(sys.argv[1]) as database:
        reader = csv.DictReader(database)
        for row in reader:
            people.append(row)
    unk = {"name": "unknown", }               # make unknown dictionary
    seq = ""
    with open(sys.argv[2]) as sequence:
        seq = sequence.read()
    for key in people[0]:
        if not key == "name":
            value = count_consec_rep(key, seq)
            unk.update({key: str(value)})  # updates unknown dictionary with new key:value pair
    unk.pop("name")
    for p in people:
        current_name = p["name"]
        p.pop("name")
        if p == unk:
            print(current_name)
            sys.exit()
    print("No match")
    sys.exit()