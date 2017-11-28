import csv
import os
import re
import math as m

# https://stackoverflow.com/questions/5967500/how-to-correctly-sort-a-string-with-a-number-inside
def atoi(text):
    return int(text) if text.isdigit() else text


def natural_keys(text):
    '''
    alist.sort(key=natural_keys) sorts in human order
    http://nedbatchelder.com/blog/200712/human_sorting.html
    (See Toothy's implementation in the comments)
    '''
    return [ atoi(c) for c in re.split('(\d+)', text) ]


def read_movement_data(dfile):
    dlist = []
    with open(dfile) as csvfile:
        reader = csv.DictReader(csvfile)
        for row in reader:
            dlist.append((float(row['x']), float(row['y'])))
    return dlist


def write_movement_data(fname, dlist):
    with open(fname, 'wb') as out:
        csv_out = csv.writer(out)
        csv_out.writerow(['x', 'y'])
        for row in dlist:
            csv_out.writerow(row)


def length_of_movement(dlist, step):
    jump = int(round(len(dlist)/step))
    cnt = 0
    length = 0
    while cnt < len(dlist)-jump:
        length += m.hypot(dlist[cnt+jump][0] - dlist[cnt][0], dlist[cnt+jump][1] - dlist[cnt][1])
        cnt += jump
    cnt -= jump
    length += m.hypot(dlist[len(dlist)-1][0] - dlist[cnt][0], dlist[len(dlist)-1][1] - dlist[cnt][1])
    return length
