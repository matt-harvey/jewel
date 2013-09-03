#!/usr/bin/env python3

import json
filepath = "scratch.log.json"
lines = []
with open(filepath, 'r') as f:
    for line in f:
        lines.append(line)
json_string = "".join(lines)
print(json_string)
log_object = json.loads(json_string)
print(log_object)

