#!/usr/bin/python3

from storage import *


environment = Environment(True)

storage = Storage(environment)

staging = storage.get_staging()

sda = Disk.create(staging, "/dev/sda")

gpt = sda.create_partition_table(PtType_GPT)

sda1 = gpt.create_partition("/dev/sda1", Region(0, 100, 512), PartitionType_PRIMARY)
sda2 = gpt.create_partition("/dev/sda2", Region(100, 100, 512), PartitionType_PRIMARY)

print(staging)


print("partitions on gpt:")
for partition in gpt.get_partitions():
    print(f"  {partition} {partition.get_number()}")
print()


print("descendants of sda:")
for device in sda.get_descendants(False):

    try:
        partition_table = to_partition_table(device)
        print(f"  {partition_table} is partition table")
    except DeviceHasWrongType:
        pass

    try:
        partition = to_partition(device)
        print(f"  {partition} {partition.get_number()} is partition")
    except DeviceHasWrongType:
        pass

print()

