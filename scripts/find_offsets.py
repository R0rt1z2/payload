import re
import mmap

from liblk.LkImage import LkImage
from argparse import ArgumentParser

patterns = {
    'app': ['42 48 70 b5 42 4d', 'b8 4b 2d e9 f0 4f'],
    'app_caller': ['98 47 b5 42 f8 d1'],
    'arch_clean_invalidate_cache_range': ['01 20 80 e0 3f 00 c0 e3 3e 0f 07 ee'],
    'dprintf': ['0f b4 2a 4b f0 b5'],
    'fastboot_enter': ['10 b5 4f f0 ff 30'],
    'fastboot_fail': ['01 46 02 48 78 44 ff f7 97 bf'],
    'fastboot_info': ['13 4a 7a 44 12 68'],
    'fastboot_okay': ['01 46 02 48 78 44 ff f7 db be'],
    'fastboot_register': ['2d e9 f0 41 05 46 18 20'],
    'free': ['d8 b1 38 b5 05 46'],
    'malloc': ['00 21 ff f7 XX bb 00 bf'],
    'mt_part_get_device': ['10 b5 07 4c 7c 44 20 68'],
    'mtk_detect_key': ['47 28 16 d8 08 28 10 b5'],
    'platform_init': ['2d e9 f0 4f 00 20 a5 b0'],
    'video_printf': ['0f b4 30 b5 c9 b0 4c ab']
}

def get_load_address(image: str) -> int:
    '''
    Gets the load address from the given LK image.
    :param image: The LK image to get the load address from.
    :return: The load address.
    '''
    image = LkImage(image)
    partition = (
        image
        .get_partition_by_name('lk')
    )
    return (
        partition
        .header
        .memory_address
    )

def p2r(p: str) -> re.Pattern:
    '''
    Convert a pattern to a regex pattern.
    :param p: The pattern to convert.
    :return: The regex pattern.
    '''
    return re.compile(
        bytes(
            ''.join(
                '.' if b == 'XX' else f"\\x{b}"
                for b in p.split()
            ),
            encoding='latin1'
        ),
        re.DOTALL
    )

def find_offsets(lk: bytes, p: dict, base: int) -> None:
    '''
    Find the offsets of the given patterns in the LK image.
    :param lk: The LK image.
    :param p: The patterns to find.
    :param base: The base address of the LK image.
    :return: None
    '''
    for n, pl in p.items():
        fo = []
        for pattern in pl:
            fo.extend([
                match.start() + base
                for match in p2r(pattern).finditer(lk)
            ])
        if fo:
            print("%s: %s" % (n, ', '.join(f"0x{offset:08x}"
                                           for offset in fo)))
        else:
            print("%s: not found" % n)

def main():
    parser = ArgumentParser()
    parser.add_argument('lk', help='LK file')
    args = parser.parse_args()

    base = get_load_address(args.lk)
    print("load address: 0x%08x" % base)
    with open(args.lk, 'rb') as fp:
        lk = mmap.mmap(fp.fileno(), 0, access=mmap.ACCESS_READ)

    stack_end = (int.from_bytes(
        lk[0x330:0x334], byteorder='little'))
    print("stack end: 0x%08x" % stack_end)
    print("injection point: 0x%08x" % (stack_end + 0x7C))

    find_offsets(lk, patterns, base - 0x200)

if __name__ == "__main__":
    main()