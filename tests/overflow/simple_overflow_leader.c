/* simple_overflow_leader.c  */
/* by Vince Weaver   vincent.weaver _at_ maine.edu */

/* Just does some tests of the overflow infrastructure */
/*  on event group leaders.                            */

#define _GNU_SOURCE 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <fcntl.h>

#include <errno.h>

#include <signal.h>

#include <sys/mman.h>

#include <sys/ioctl.h>
#include <asm/unistd.h>
#include <sys/prctl.h>

#include "perf_event.h"
#include "test_utils.h"
#include "perf_helpers.h"
#include "instructions_testcode.h"

#define MMAP_PAGES 8

static struct signal_counts {
  int in,out,msg,err,pri,hup,unknown,total;
} count = {0,0,0,0,0,0,0,0};

static int fd1;

static void our_handler(int signum,siginfo_t *oh, void *blah) {
  int ret;

//  ret=ioctl(fd1, PERF_EVENT_IOC_DISABLE, 0);

  switch(oh->si_code) {
     case POLL_IN:  count.in++;  break;
     case POLL_OUT: count.out++; break;
     case POLL_MSG: count.msg++; break;
     case POLL_ERR: count.err++; break;
     case POLL_PRI: count.pri++; break;
     case POLL_HUP: count.hup++; break;
     default: count.unknown++; break;
  }

  count.total++;

//  ret=ioctl(fd1, PERF_EVENT_IOC_REFRESH,1);

  (void) ret;
  
}


int main(int argc, char** argv) {
   
   int ret,quiet;   
   int i;

   struct perf_event_attr pe;

   struct sigaction sa;
   void *our_mmap;
   char test_string[]="Testing overflow on leaders...";
   
   quiet=test_quiet();

   if (!quiet) printf("This tests overflow on leaders.\n");
   
   memset(&sa, 0, sizeof(struct sigaction));
   sa.sa_sigaction = our_handler;
   sa.sa_flags = SA_SIGINFO;

   if (sigaction( SIGIO, &sa, NULL) < 0) {
     fprintf(stderr,"Error setting up signal handler\n");
     exit(1);
   }
   
   memset(&pe,0,sizeof(struct perf_event_attr));

   pe.type=PERF_TYPE_HARDWARE;
   pe.size=sizeof(struct perf_event_attr);
   pe.config=PERF_COUNT_HW_INSTRUCTIONS;
   pe.sample_period=100000;
   pe.sample_type=PERF_SAMPLE_IP;
   pe.read_format=PERF_FORMAT_GROUP|PERF_FORMAT_ID;
   pe.disabled=1;
   pe.pinned=1;
   pe.exclude_kernel=1;
   pe.exclude_hv=1;

   /* Not needed on 3.2? */
   pe.wakeup_events=1;

   arch_adjust_domain(&pe,quiet);

   fd1=perf_event_open(&pe,0,-1,-1,0);
   if (fd1<0) {
     if (!quiet) fprintf(stderr,"Error opening leader %llx\n",pe.config);
     test_fail(test_string);
   }

   /* large enough that threshold not crossed */
   our_mmap=mmap(NULL, (1+MMAP_PAGES)*getpagesize(), 
         PROT_READ|PROT_WRITE, MAP_SHARED, fd1, 0);

   
   fcntl(fd1, F_SETFL, O_RDWR|O_NONBLOCK|O_ASYNC);
   fcntl(fd1, F_SETSIG, SIGIO);
   fcntl(fd1, F_SETOWN,getpid());
   
   ioctl(fd1, PERF_EVENT_IOC_RESET, 0);   

   ret=ioctl(fd1, PERF_EVENT_IOC_ENABLE, 0);

   if (ret<0) {
     if (!quiet) fprintf(stderr,"Error with PERF_EVENT_IOC_ENABLE of group leader: "
	     "%d %s\n",errno,strerror(errno));
     exit(1);
   }

   for (i=0;i<100;i++) {
       instructions_million();
   }
   
   ret=ioctl(fd1, PERF_EVENT_IOC_DISABLE, 0);   
   
   if (!quiet) {
      printf("Counts, using mmap buffer %p\n",our_mmap);
      printf("\tPOLL_IN : %d\n",count.in);
      printf("\tPOLL_OUT: %d\n",count.out);
      printf("\tPOLL_MSG: %d\n",count.msg);
      printf("\tPOLL_ERR: %d\n",count.err);
      printf("\tPOLL_PRI: %d\n",count.pri);
      printf("\tPOLL_HUP: %d\n",count.hup);
      printf("\tUNKNOWN : %d\n",count.unknown);
   }

   close(fd1);
   
   if (count.total==0) {
      if (!quiet) printf("No overflow events generated.\n");
      test_fail(test_string);
   }

   if (count.hup!=0) {
      if (!quiet) printf("Unexpected POLL_HUP overflow.\n");
      test_fail(test_string);
   }
   
   
   if (count.in!=1000) {
      if (!quiet) printf("Error: POLL_IN of %d but %d expected.\n",
			count.in,1000);      
      test_fail(test_string);
   }
   

   test_pass(test_string);
   
   return 0;
}

