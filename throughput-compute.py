#!/usr/bin/env python3

#!/usr/bin/env python
# coding: utf-8

import pandas as pd

# Reading DownLink RLS Stats

def readRLCStats(filename):
    df = pd.read_csv(filename, sep='\s+')
    for i in range(len(df.columns) -1, 0, -1):
        last = df.columns[i]
        prev = df.columns[i - 1]
        df[last] = df[prev]

    df = df.drop(columns="%")
    return df

def compute_throughput(df):
    df['TimeInterval'] = df['end'] - df['start']
    df['Throughput'] = (df['TxBytes'] * 8) / (df['TimeInterval'] * 1e6)  # Mbps

    throughput_per_user = df.groupby('IMSI')['Throughput'].mean().reset_index()
    return throughput_per_user

dl = readRLCStats("DlRlcStats.txt")
ul = readRLCStats("UlRlcStats.txt")

print("\n\nFinal result! \n\n")

print("DL Throughput per user (Mbps):")
print(compute_throughput(dl), "\n")

print("UL Throughput per user (Mbps):")
print(compute_throughput(ul), "\n")
