/* This file attempts to test the L1 data cache reads          */
/* performance counter on various architectures, as            */
/* implemented by the perf_events generalized event            */
/*    L1-dcache-loads                                          */

/* by Vince Weaver, vincent.weaver _at_ maine.edu                 */

/* Sometimes the C compiler doesn't help us, we should use asm */

char test_string[]="Testing \"L1-dcache-loads\" generalized event...";
int quiet=0;
int fd;

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>
#include <errno.h>

#include "perf_event.h"
#include "test_utils.h"
#include "perf_helpers.h"

#define ARRAYSIZE 1000

int main(int argc, char **argv) {

  int num_runs=100,i,j,read_result;
   long long high=0,low=0,average=0;
   double error,total_sum=0.0;
   struct perf_event_attr pe;

   long long count,total=0;

   quiet=test_quiet();

   if (!quiet) {
      printf("\n");

      printf("Testing a loop writing %d doubles %d times\n",
          ARRAYSIZE,num_runs);
   }

   memset(&pe,0,sizeof(struct perf_event_attr));

   pe.type=PERF_TYPE_HW_CACHE;
   pe.size=sizeof(struct perf_event_attr);
   pe.config=PERF_COUNT_HW_CACHE_L1D |
     (PERF_COUNT_HW_CACHE_OP_READ<<8) |
     (PERF_COUNT_HW_CACHE_RESULT_ACCESS <<16);
   pe.disabled=1;
   pe.exclude_kernel=1;
   pe.exclude_hv=1;

   arch_adjust_domain(&pe,quiet);

   fd=perf_event_open(&pe,0,-1,-1,0);
   if (fd<0) {
     if (!quiet) {
       fprintf(stderr,"Error opening leader %llx %d: %s\n",
		pe.config,errno,strerror(errno));
     }
     if (errno==2) {
        test_skip(test_string);
     } else {
        test_fail(test_string);
     }
   }

   for(i=0;i<num_runs;i++) {

      /*******************************************************************/
      /* Test if the C compiler uses a sane number of data cache acceess */
      /*******************************************************************/

      double array[ARRAYSIZE];
      double aSumm = 0.0;

      for(j=0; j<ARRAYSIZE; j++) {
	array[j]=(double)j;
      }

      ioctl(fd, PERF_EVENT_IOC_RESET, 0);
      ioctl(fd, PERF_EVENT_IOC_ENABLE, 0);

      for(j=0; j<ARRAYSIZE; j++) {
	aSumm+=array[j];
      }

      ioctl(fd, PERF_EVENT_IOC_DISABLE, 0);
      read_result=read(fd,&count,sizeof(long long));


      total_sum+=aSumm;

      //      if (result==CODE_UNIMPLEMENTED) {
      //	 test_skip(test_string);
      //	 fprintf(stdout,"\tNo test code for this architecture\n");
      // }

      if (read_result!=sizeof(long long)) {
	 test_fail(test_string);
         fprintf(stdout,"Error extra data in read %d\n",read_result);
      }

      if (count>high) high=count;
      if ((low==0) || (count<low)) low=count;
      total+=count;
   }

   average=(total/num_runs);

   if (!quiet) printf("Keep compiler from optimizing: %lf\n",total_sum);

   error=display_error(average,high,low,ARRAYSIZE,quiet);

   if ((error > 5.0) || (error<-5.0)) {
     if (!quiet) {
        printf("\nInstruction count off by more than 5%%\n");
      	printf("This test is known to be a factor of 3 too much with gcc 4.1\n\n");
     }
     test_fail(test_string);
   }
   if (!quiet) printf("\n");

   test_pass( test_string );

   return 0;
}
