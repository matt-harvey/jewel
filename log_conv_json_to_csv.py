#!/usr/bin/env python3

import sys
infile = sys.argv[1]
outfile = sys.argv[2]

# Create a JSON object
lines = []
with open(infile, 'r') as f:
    for line in f:
        lines.append(line)
json_string = "".join(lines)
import json
log_object = json.loads(json_string)

# Get all the fields and make headers from these,
# putting them into an OrderedDict for fast lookup
# the position of each field
from collections import OrderedDict
headers = OrderedDict()
for message in log_object:
    for item in message.items():
        key = item[0]
        if not key in headers:
            headers[key] = len(headers)

# Create a list of lists representing what will be contents
# of our CSV file
rows = [list(headers.keys())]
for message in log_object:
    row = [""] * len(headers)
    for item in message.items():
        key = item[0]
        value = item[1]
        row[headers[key]] = value
    rows.append(row)

# Write to CSV file

with open(outfile, 'w') as csv_file:
    import csv
    csv_writer = csv.writer(csv_file)
    for row in rows:
        csv_writer.writerow(row)


    
