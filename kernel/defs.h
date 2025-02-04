#ifndef DEFS_H
#define DEFS_H

#include "types.h"
#include "ipc_hash.h"
#include "riscv.h"

#define ASSERT(x, info)\
do\
{\
    if (!(x))\
        panic(info);\
} while(0);

struct buf;
struct context;
struct file;
struct inode;
struct pipe;
struct proc;
struct spinlock;
struct sleeplock;
struct stat;
struct superblock;

#define NULL 0

// bio.c
void            binit(void);
struct buf*     bread(uint, uint);
void            brelse(struct buf*);
void            bwrite(struct buf*);
void            bpin(struct buf*);
void            bunpin(struct buf*);

// console.c
void            consoleinit(void);
void            consoleintr(int);
void            consputc(int);

// exec.c
int             exec(char*, char**);

// file.c
struct file*    filealloc(void);
void            fileclose(struct file*);
struct file*    filedup(struct file*);
void            fileinit(void);
int             fileread(struct file*, uint64, int n);
int             filestat(struct file*, uint64 addr);
int             filewrite(struct file*, uint64, int n);

// fs.c
void            fsinit(int);
int             dirlink(struct inode*, char*, uint);
struct inode*   dirlookup(struct inode*, char*, uint*);
struct inode*   ialloc(uint, short);
struct inode*   idup(struct inode*);
void            iinit();
void            ilock(struct inode*);
void            iput(struct inode*);
void            iunlock(struct inode*);
void            iunlockput(struct inode*);
void            iupdate(struct inode*);
int             namecmp(const char*, const char*);
struct inode*   namei(char*);
struct inode*   nameiparent(char*, char*);
int             readi(struct inode*, int, uint64, uint, uint);
void            stati(struct inode*, struct stat*);
int             writei(struct inode*, int, uint64, uint, uint);
void            itrunc(struct inode*);

// ramdisk.c
void            ramdiskinit(void);
void            ramdiskintr(void);
void            ramdiskrw(struct buf*);

// kalloc.c
void*           kalloc(void);
void            kfree(void *);
void            kinit(void);
void*           kbuddy_alloc(uint32);
void            kbuddy_free(void *, uint32);
int             kaddrefcnt(void*);
void*           cowalloc(pagetable_t, uint64);
int             krefcnt(void*);
int             cowpage(pagetable_t, uint64); 

// log.c
void            initlog(int, struct superblock*);
void            log_write(struct buf*);
void            begin_op(void);
void            end_op(void);

// pipe.c
int             pipealloc(struct file**, struct file**);
void            pipeclose(struct pipe*, int);
int             piperead(struct pipe*, uint64, int);
int             pipewrite(struct pipe*, uint64, int);

// printf.c
int             printf(char*, ...) __attribute__ ((format (printf, 1, 2)));
void            panic(char*) __attribute__((noreturn));
void            printfinit(void);

// proc.c
typedef uint64 pte_t;
typedef uint64 *pagetable_t; 

int             cpuid(void);
void            exit(int);
int             fork(void);
int             vfork(void);
int             growproc(int);
void            proc_mapstacks(pagetable_t);
pagetable_t     proc_pagetable(struct proc *);
void            proc_freepagetable(pagetable_t, uint64);
int             kill(int);
int             killed(struct proc*);
void            setkilled(struct proc*);
struct cpu*     mycpu(void);
struct cpu*     getmycpu(void);
struct proc*    myproc();
void            procinit(void);
void            scheduler(void) __attribute__((noreturn));
void            sched(void);
void            sleep(void*, struct spinlock*);
void            userinit(void);
int             wait(uint64);
void            wakeup(void*);
void            yield(void);
int             either_copyout(int user_dst, uint64 dst, void *src, uint64 len);
int             either_copyin(void *dst, int user_src, uint64 src, uint64 len);
void            procdump(void);
int             set_priority(int, int);
void            update_proc(void);
void            initMlfq(void);
void            printMlfq(void);
int             chrt(int);
void            neofetch(void);

// raw_kmalloc.c
void            raw_kmalloc_init();
void*           raw_kmalloc(uint32 sz, uint32 align);
void            raw_kfree(void* pa);

// swtch.S
void            swtch(struct context*, struct context*);

// spinlock.c
void            acquire(struct spinlock*);
int             holding(struct spinlock*);
void            initlock(struct spinlock*, char*);
void            release(struct spinlock*);
void            push_off(void);
void            pop_off(void);

// slab.c
struct          kmem_cache;
void            slab_init();
void            kmem_cache_create(struct kmem_cache* cache, char* name, uint16 sz, uint16 align, uint16 init_pgnum);
void            kmem_cache_destroy(struct kmem_cache*, int flag);
void*           kmem_cache_alloc(struct kmem_cache*);
void            kmem_cache_free(struct kmem_cache*, void*);

// sleeplock.c
void            acquiresleep(struct sleeplock*);
void            releasesleep(struct sleeplock*);
int             holdingsleep(struct sleeplock*);
void            initsleeplock(struct sleeplock*, char*);

// string.c
int             memcmp(const void*, const void*, uint);
void*           memmove(void*, const void*, uint);
void*           memset(void*, int, uint);
char*           safestrcpy(char*, const char*, int);
int             strlen(const char*);
int             strncmp(const char*, const char*, uint);
char*           strncpy(char*, const char*, int);

// syscall.c
void            argint(int, int*);
int             argstr(int, char*, int);
void            argaddr(int, uint64 *);
int             fetchstr(uint64, char*, int);
int             fetchaddr(uint64, uint64*);
void            syscall();

// trap.c
extern uint     ticks;
void            trapinit(void);
void            trapinithart(void);
extern struct spinlock tickslock;
void            usertrapret(void);
uint            getticks(void);

// uart.c
void            uartinit(void);
void            uartintr(void);
void            uartputc(int);
void            uartputc_sync(int);
int             uartgetc(void);

// vm.c
void            kvminit(void);
void            kvminithart(void);
void            kvmmap(pagetable_t, uint64, uint64, uint64, int);
int             mappages(pagetable_t, uint64, uint64, uint64, int);
pagetable_t     uvmcreate(void);
void            uvmfirst(pagetable_t, uchar *, uint);
uint64          uvmalloc(pagetable_t, uint64, uint64, int);
uint64          uvmdealloc(pagetable_t, uint64, uint64);
int             uvmcopy(pagetable_t, pagetable_t, uint64);
void            uvmfree(pagetable_t, uint64);
void            uvmunmap(pagetable_t, uint64, uint64, int);
void            uvmclear(pagetable_t, uint64);
pte_t *         walk(pagetable_t, uint64, int);
uint64          walkaddr(pagetable_t, uint64);
int             copyout(pagetable_t, uint64, char *, uint64);
int             copyin(pagetable_t, char *, uint64, uint64);
int             copyinstr(pagetable_t, char *, uint64, uint64);
int             uvlazyalloc(pagetable_t, uint64);
void            uvmcopy_shm(struct proc* old, struct proc* new);

// plic.c
void            plicinit(void);
void            plicinithart(void);
int             plic_claim(void);
void            plic_complete(int);

// virtio_disk.c
void            virtio_disk_init(void);
void            virtio_disk_rw(struct buf *, int);
void            virtio_disk_intr(void);

// number of elements in fixed-size array
#define NELEM(x) (sizeof(x)/sizeof((x)[0]))

// time.c
struct timeVal  _gettimeofday(void);
void            cyclesinithart(void);
uint64          getcycles(void);

// trace.c
void            _trace(uint64 mask);

// kmalloc.c
void*           kmalloc(uint64);
void            kmfree(void*, uint64);

// shm.c
struct          shmid_ds;
void            shminit(void);
ipc_id          shmget(key_t key, uint64 size, uint flag);
void*           shmat(int shmid, const void* shmaddr, int shmflag);
void            shmdt(struct proc* p, const void* shmaddr);
int             shmctl(int shmid, int cmd, struct shmid_ds *buf);

// uvapg.c
struct          freeva;
struct          proc_shmblock;
void            init_freeva(struct freeva* head);
void*           uallocva(struct freeva* head, uint64 npg);
void            ufreeva(struct freeva* head, void* va, uint64 npg);
void            init_procshmblock(struct proc_shmblock* head);
void            insert_procshmblock(struct proc_shmblock* head, struct proc_shmblock* newblock);
struct proc_shmblock* findprev_procshmblock(struct proc_shmblock* head, ipc_id shmid, const void* va);

// semaphore.c
struct          sembuf;
union           semun;
struct          proc_semblock;
struct          semblock;
void            seminit();
void            sem_reinit(struct semblock* b);
ipc_id          semget(key_t key, uint64 size, uint flag);
void            init_procsemblock(struct proc_semblock* b);
int             semop(int semid, struct sembuf* sops, uint nsops);
int             semctl(int semid, int semnum, int cmd, uint64 un);

// msg.c
struct          proc_msgblock;
struct          msgbuf;
struct          msqid_ds;
struct          msg_queue;
void            msqinit();
void            msq_reinit(struct msg_queue* msq);
void            init_procmsgblock(struct proc_msgblock* b);
ipc_id          msgget(key_t key, uint32 maxlen, uint flag);
int             msgsnd(int msqid, const struct msgbuf *msgp, int msgflg);
int             msgrcv(int msqid, struct msgbuf* msgp, uint32 msgsz, uint32 msgtype, int msgflg);
int             msgctl(ipc_id msqid, int cmd, struct msqid_ds* buf);

#endif