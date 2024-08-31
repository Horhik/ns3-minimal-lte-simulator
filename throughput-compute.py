#!/usr/bin/env python3

#!/usr/bin/env python
# coding: utf-8

import pandas as pd

# Reading DownLink RLS Stats

dl = pd.read_csv('DlRlcStats.txt', sep='\s+')
for i in range(len(dl.columns) -1, 0, -1):
    last = dl.columns[i]
    prev = dl.columns[i - 1]
    dl[last] = dl[prev]
dl = dl.drop(columns="%")


# Reading UP Link RLS Stats


ul = pd.read_csv('DlRlcStats.txt', sep='\s+')
for i in range(len(ul.columns) -1, 0, -1):
    last = ul.columns[i]
    prev = ul.columns[i - 1]
    ul[last] = ul[prev]
ul = ul.drop(columns="%")



# Computing Throughput for Down Link
dl['TimeInterval'] = dl['end'] - dl['start']
dl['Throughput_DL'] = (dl['TxBytes'] * 8) / (dl['TimeInterval'] * 1e6)  # Mbps

# Computing Throughput for UP Link
ul['TimeInterval'] = ul['end'] - ul['start']
ul['Throughput_UL'] = (ul['TxBytes'] * 8) / (ul['TimeInterval'] * 1e6)  # Mbps



# Grouping by IMSI and computing mean
dl_throughput_per_user = dl.groupby('IMSI')['Throughput_DL'].mean().reset_index()
ul_throughput_per_user = ul.groupby('IMSI')['Throughput_UL'].mean().reset_index()


# Printing results
print("\n\nFinal result! \n\n")
print("DL Throughput per user (Mbps):")
print(dl_throughput_per_user)

print("\nUL Throughput per user (Mbps):")
print(ul_throughput_per_user)
