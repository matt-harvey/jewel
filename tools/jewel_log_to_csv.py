#!/usr/bin/env python3

###
# Copyright 2012-2013 Matthew Harvey
# 
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
###

# Usage:
# jewel_log_to_csv.py logfile

# Takes a logfile produced by jewel::Log as input and
# prints it in CSV form to stdout.

import csv
import sys
import collections


def check_argc(required_argc):
    """
    Bail with a message unless the number of command line arguments (including
    the name of this script) is equal to required_arc.
    """
    if len(sys.argv) != required_argc:
        print("Incorrect number of arguments passed to command line.")
        print("Required number of arguments:", required_argc - 1)
        exit(1)


def get_file_contents_as_string(filepath):
    """
    Read a file and returns its contents as one big string.
    """
    ret = []
    with open(filepath, 'r') as infile:
        for line in infile.readlines():
            ret.append(line)
    ret = "".join(ret) 
    return ret


def log_string_to_dict_list(log_string):
    """
    Take a single string representing the contents of a jewel::Log file,
    and return a list of OrderedDicts representing the logging events in the
    log file.
    """
    record_separator = "{RECORD}"
    field_separator = "{FIELD}"
    raw_records = log_string.split(record_separator)
    ret = []
    for raw_record in raw_records:
        raw_record = raw_record.strip()
        if len(raw_record) == 0:
            continue
        raw_record = raw_record.split(field_separator)
        record = collections.OrderedDict()
        for raw_cell in raw_record:
            raw_cell = raw_cell.strip()
            if len(raw_cell) == 0:
                continue
            assert raw_cell[0] == '['
            field_name = []
            field_contents = []
            in_contents = False
            for char in raw_cell:
                if char == ']':
                    in_contents = True
                    continue
                if char != '[' and not in_contents:
                    field_name.append(char)
                elif in_contents:
                    field_contents.append(char)
            field_name = "".join(field_name)
            field_contents = "".join(field_contents)
            record[field_name] = field_contents 
        ret.append(record)
    return ret


def make_table(log_dict_list):
    """
    Take a list of OrderedDicts representing a list of log
    entries made by jewel::Log, and return a list
    of lists, of which the first element comprises the names of
    the fields in the table, and each subsequent elements
    represents a record in the table.
    """
    def make_headers(dict_list):
        raw_ret = collections.OrderedDict()
        for dictionary in dict_list:
            for item in dictionary.items():
                raw_ret[item[0]] = None
        ret = collections.OrderedDict()
        for item in raw_ret.items():
            ret[item[0]] = len(ret)
        return ret

    def make_row(headers, dictionary):
        row = [""] * len(headers)
        for item in dictionary.items():
            pos = headers[item[0]]
            row[pos] = item[1]
        return row

    headers = make_headers(log_dict_list)
    ret = [[item[0] for item in headers.items()]]
    for dictionary in log_dict_list:
        ret.append(make_row(headers, dictionary))
    return ret


def write_csv(outfile, table):
    """
    Takes a list of lists, the first being the column headers of
    a table, and each subsequent element representing a row
    in the table. Outputs the contents in CSV form to outfile.
    Note outfile must be an open file stream. Caller is responsible
    for any opening and closing of the stream that may be required.
    """
    w = csv.writer(outfile)
    for row in table:
        w.writerow(row)
    return


def main():
    check_argc(2)
    log_filepath = sys.argv[1]
    raw_log = get_file_contents_as_string(log_filepath)
    log = log_string_to_dict_list(raw_log)
    table = make_table(log)
    write_csv(sys.stdout, table)
    return


main()
    
