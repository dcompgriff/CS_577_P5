#ifndef _FS_H_
#define _FS_H_

// Checksum implementation code.
#define ADLER32_BASE 65521U

static inline uint adler32(void* data, uint len)
{
  uint i, a = 1, b = 0;

  for (i = 0; i < len; i++) {
    a = (a + ((uchar*)data)[i]) % ADLER32_BASE;
    b = (b + a) % ADLER32_BASE;
  }

  return (b << 16) | a;
}

// On-disk file system format.
// Both the kernel and user programs use this header file.

// Block 0 is unused.
// Block 1 is super block.
// Inodes start at block 2.

#define ROOTINO 1  // root i-number
#define BSIZE 512  // block size

// File system super block
struct superblock {
  uint size;         // Size of file system image (blocks)
  uint nblocks;      // Number of data blocks
  uint ninodes;      // Number of inodes.
};

// Orig 12, but divide by 2 (minus 1 since indirect no longer in array)
//   since twice as many bytes per node.
#define NDIRECT 6
#define NINDIRECT (BSIZE / (2*sizeof(uint)))
#define MAXFILE (NDIRECT + NINDIRECT)

// On-disk inode structure
struct dinode {
  short type;           // File type
  short major;          // Major device number (T_DEV only)
  short minor;          // Minor device number (T_DEV only)
  short nlink;          // Number of links to inode in file system
  uint size;            // Size of file (bytes)
  uint addrs[NDIRECT];  // Data block addresses
  uint chksm[NDIRECT];  // Checksums for direct blocks
  uint indirect;
};

// Inodes per block.
#define IPB           (BSIZE / sizeof(struct dinode))

// Block containing inode i
#define IBLOCK(i)     ((i) / IPB + 2)

// Bitmap bits per block
#define BPB           (BSIZE*8)

// Block containing bit for block b
#define BBLOCK(b, ninodes) (b/BPB + (ninodes)/IPB + 3)

// Directory is a file containing a sequence of dirent structures.
#define DIRSIZ 14

struct dirent {
  ushort inum;
  char name[DIRSIZ];
};

#endif // _FS_H_
