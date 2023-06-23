#!/usr/bin/python3


# Adding the sys path is needed with the standard opensuse
# inst-sys. libstorage-ng-python3 must also be added e.g. via a dud.
import sys
sys.path += ["/usr/lib64/python3.8/site-packages"]


from storage import *


def find_disk(staging):

    if disks := Disk.get_all(staging):
        # TODO check for thing that cannot be deleted, e.g. multipath

        return disks[0]
    else:
        raise Exception("no disks found")


def create_partition(gpt, size):

    for slot in gpt.get_unused_partition_slots():

        if not slot.primary_possible:
            continue

        region = slot.region

        if region.to_bytes(region.get_length()) < size:
            continue

        region.set_length(int(size / region.get_block_size()))
        region = gpt.align(region)
        return gpt.create_partition(slot.name, region, PartitionType_PRIMARY)
    raise Exception("no suitable partition slot found")


class MyCommitCallbacks(CommitCallbacks):

    def __init__(self):
        super(MyCommitCallbacks, self).__init__()

    def message(self, message):
        print(f"message '{message}'")

    def error(self, message, what):
        print(f"error '{message}' '{what}'")
        return False


set_logger(get_logfile_logger())

environment = Environment(False)
environment.set_rootprefix("/mnt")

storage = Storage(environment)
storage.probe()                 # TODO error handling

staging = storage.get_staging()


disk = find_disk(staging)
disk.remove_descendants()

gpt = to_gpt(disk.create_partition_table(PtType_GPT))


partition1 = create_partition(gpt, 512 * MiB)
partition1.set_id(ID_ESP)
vfat = partition1.create_filesystem(FsType_VFAT)
vfat.create_mount_point("/boot/efi")


partition2 = create_partition(gpt, 1 * GiB)
partition2.set_id(ID_SWAP)
swap = partition2.create_filesystem(FsType_SWAP)
swap.create_mount_point("swap")


partition3 = create_partition(gpt, 8 * GiB)
partition3.set_id(ID_LINUX)
btrfs = to_btrfs(partition3.create_filesystem(FsType_BTRFS))
btrfs.create_mount_point("/")

top_level = btrfs.get_top_level_btrfs_subvolume()
at_subvolume = top_level.create_btrfs_subvolume("@")


# The following code does not rely on the snapper installation-helper. The procedure is untested.
# Also some code to make snapper work afterwards is missing.

class Subvolume():
    def __init__(self, path, nocow = False, mount = True, default = False, subvolumes = []):
        self.path = path
        self.nocow = nocow
        self.mount = mount
        self.default = default
        self.subvolumes = subvolumes


subvolumes = [ Subvolume(".snapshots", subvolumes = [ Subvolume(".snapshots/1/snapshot", mount = False, default = True) ]),
               Subvolume("root"), Subvolume("var", nocow = True), Subvolume("srv"), Subvolume("opt"), Subvolume("home"),
               Subvolume("usr/local"), Subvolume("boot/grub2/i386-pc"), Subvolume("boot/grub2/x86_64-efi") ]

def doit_subvolume(parent, subvolumes):
    for subvolume in subvolumes:
        tmp = parent.create_btrfs_subvolume(f"@/{subvolume.path}")
        tmp.set_nocow(subvolume.nocow)
        if subvolume.mount:
            tmp.create_mount_point(f"/{subvolume.path}")
        if subvolume.default:
            btrfs.set_default_btrfs_subvolume(tmp)

        doit_subvolume(tmp, subvolume.subvolumes)

doit_subvolume(at_subvolume, subvolumes)


commit_options = CommitOptions(True)
my_commit_callbacks = MyCommitCallbacks()

try:
    storage.calculate_actiongraph()
    storage.commit(commit_options, my_commit_callbacks)
except Exception as exception:
    print(exception.what())


# After package installation the call 'installation-helper --step
# <does-not-exist-yet>' is needed.
