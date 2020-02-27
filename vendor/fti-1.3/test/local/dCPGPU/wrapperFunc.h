#ifndef __WRAPPER__
#ifdef __cplusplus
extern "C" {
#endif


#define BLOCKSIZE (BLKX * BLKY *BLKZ)

#define XSIZE (BLKX*GRDX)
#define YSIZE (BLKY*GRDY)
#define ZSIZE (BLKZ*GRDZ)

void allocateMemory(void **ptr, size_t size);
void cudaCopy(void *str, void *dest, size_t size);
void hostCopy(void *src, void *dest, size_t size);
void executeKernel(int *ptr, int size);
void freeCuda( void *ptr );
int getProperties();
void setDevice(int id);
void deviceMemset(void *ptr, int size);

//int getProperties();
//void setDevice(int id);
#ifdef __cplusplus
}
#endif
#endif



