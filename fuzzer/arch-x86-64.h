#define PAGE_OFFSET	0xffff880000000000L
#define KERNEL_ADDR	0xffffffff81000000L
#define MODULE_ADDR	0xffffffffa0000000L
#define VDSO_ADDR	0xffffffffff600000L

#define TASK_SIZE       (0x800000000000UL - 4096)

#define PAGE_SHIFT 12

#define PTE_FILE_MAX_BITS 32
