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


def read_csv_data_to_list(csv_list, re_list):
    data_list = []
    lcnt = 0
    re_cnt = 0
    while lcnt < len(csv_list):
        if re.match(file_name + re_list[re_cnt] + '_\d', csv_list[lcnt], flags=0) is not None:
            print file_name + re_list[re_cnt]

            slist = read_movement_data(ddir + mdir + csv_list[lcnt])
            tmp_list = [slist, re_cnt]
            data_list.append(tmp_list)

        else:
            if re_cnt < (len(re_list) - 1):
                re_cnt += 1
                lcnt -= 1
            else:
                re_cnt = 0

        lcnt += 1
    return data_list


def length_of_movement(dlist):
    jump = int(round(len(dlist)/10))
    cnt = 0
    length = 0
    while cnt < len(dlist)-jump:
        length += m.hypot(dlist[cnt+jump][0] - dlist[cnt][0], dlist[cnt+jump][1] - dlist[cnt][1])
        cnt += jump
    if jump > 1:
        length += m.hypot(dlist[len(dlist)-1][0] - dlist[cnt][0], dlist[len(dlist)-1][1] - dlist[cnt][1])
    return length


def plt_distance_from_ref_circle(data_list):

    samples = 300
    theta = np.linspace(-np.pi, np.pi, samples)
    x = 36+30*np.sin(theta)
    y = 38+30*np.cos(theta)

    plt.figure()
    for dlist in data_list:
        jump = int(round(len(dlist[0])/samples))
        cnt = 0
        jcnt = 0
        data = []
        while cnt < samples:
            data.append(m.hypot(dlist[0][jcnt][0] - x[cnt], dlist[0][jcnt][1] - y[cnt]))
            cnt += 1
            jcnt += jump
        plt.plot(data, label=label[dlist[1]], color=color_list[dlist[1]], marker=marker_list[dlist[1]], markevery=20, alpha=0.5)

    # remove duplicate labels
    handles, labels = plt.gca().get_legend_handles_labels()
    by_label = OrderedDict(zip(labels, handles))
    plt.legend(by_label.values(), by_label.keys(), loc='upper right')
    plt.show()