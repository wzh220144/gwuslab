#include <stdio.h>
#include "kmem_cache.h"
#include <time.h>
#include <stdlib.h>

int main() {
	startup();		//init 32 cache, from 8 bytes to 12288 bytes(3*page)

	printf("************************test slab*******************************\n");
	printf("\nSmall object!!!\n");
	/******************************************small object*******************************************************************/


	// test the speed of allocate and free small object(less than or equal to 512byes)
	// in this program will test 128, 256 and 512 bytes cache
	// in here will allocate 100 objects at one time each cache and free them, do it 100000 times
	// and get the average as the result
	int stime, etime, freetime, allocatetime, free1, free2, free3, allocate1, allocate2, allocate3;
	int i, j;
	int **a=(int **)malloc(sizeof(int *)*100); //in order not to affect the behaviour, here will not use slab

	//**************************128 bytes cache*******************************************************************//

	int t=cache_id_estimate(128, 0);	//this routine replace kmem_cache_ceate, it will return the id instead of the address of cache
	freetime=0, allocatetime=0;
	for(j=0; j<100000; j++) {
		stime=clock();
		for(i=0; i<100; i++) {
			a[i]=(int *)kmem_cache_alloc(&kcache[t], KM_SLEEP);
		
		}
		etime=clock();
		allocatetime+=etime-stime;
		stime=clock();
		for(i=0; i<100; i++) {
			kmem_cache_free(&kcache[t], a[i]);
		}
		etime=clock();
		freetime+=etime-stime;
	}
	free1=freetime;
	allocate1=allocatetime;
	printf("128 bytes cache:\n");
	printf("                allocate: %lfms\n", ((double)allocate1)/10000000.0);
	printf("                free:     %lfms\n", ((double)free1)/10000000.0);
	//**************************256 bytes cache*******************************************************************//
	
	t=cache_id_estimate(256, 0);	//this routine replace kmem_cache_ceate, it will return the id instead of the address of cache
	freetime=0, allocatetime=0;
	for(j=0; j<100000; j++) {
		stime=clock();
		for(i=0; i<100; i++) {
			a[i]=(int *)kmem_cache_alloc(&kcache[t], KM_SLEEP);
		
		}
		etime=clock();
		allocatetime+=etime-stime;
		stime=clock();
		for(i=0; i<100; i++) {
			kmem_cache_free(&kcache[t], a[i]);
		}
		etime=clock();
		freetime+=etime-stime;
	}
	free2=freetime;
	allocate2=allocatetime;
	printf("256 bytes cache:\n");
	printf("                allocate: %lfms\n", ((double)allocate2)/10000000.0);
	printf("                free:     %lfms\n", ((double)free2)/10000000.0);
	//**************************128 bytes cache*******************************************************************//
	
	t=cache_id_estimate(512, 0);	//this routine replace kmem_cache_ceate, it will return the id instead of the address of cache
	freetime=0, allocatetime=0;
	for(j=0; j<100000; j++) {
		stime=clock();
		for(i=0; i<100; i++) {
			a[i]=(int *)kmem_cache_alloc(&kcache[t], KM_SLEEP);
		
		}
		etime=clock();
		allocatetime+=etime-stime;
		stime=clock();
		for(i=0; i<100; i++) {
			kmem_cache_free(&kcache[t], a[i]);
		}
		etime=clock();
		freetime+=etime-stime;
	}
	free3=freetime;
	allocate3=allocatetime;
	printf("512 bytes cache:\n");
	printf("                allocate: %lfms\n", ((double)allocate3)/10000000.0);
	printf("                free:     %lfms\n", ((double)free3)/10000000.0);
	//****************************************************************************************************************//
	
	printf("\nLarge object!!!\n");
	/****************************************large object********************************************************/
	
	//**************************513 bytes cache*******************************************************************//
	
	t=cache_id_estimate(513, 0);	//this routine replace kmem_cache_ceate, it will return the id instead of the address of cache
	freetime=0, allocatetime=0;
	for(j=0; j<100000; j++) {
		stime=clock();
		for(i=0; i<100; i++) {
			a[i]=(int *)kmem_cache_alloc(&kcache[t], KM_SLEEP);
		
		}
		etime=clock();
		allocatetime+=etime-stime;
		stime=clock();
		for(i=0; i<100; i++) {
			kmem_cache_free(&kcache[t], a[i]);
		}
		etime=clock();
		freetime+=etime-stime;
	}
	free1=freetime;
	allocate1=allocatetime;
	printf("513 bytes cache:\n");
	printf("                allocate: %lfms\n", ((double)allocate1)/10000000.0);
	printf("                free:     %lfms\n", ((double)free1)/10000000.0);
	//**************************4096 bytes cache*******************************************************************//
	
	t=cache_id_estimate(4096, 0);	//this routine replace kmem_cache_ceate, it will return the id instead of the address of cache
	freetime=0, allocatetime=0;
	for(j=0; j<100000; j++) {
		stime=clock();
		for(i=0; i<100; i++) {
			a[i]=(int *)kmem_cache_alloc(&kcache[t], KM_SLEEP);
		
		}
		etime=clock();
		allocatetime+=etime-stime;
		stime=clock();
		for(i=0; i<100; i++) {
			kmem_cache_free(&kcache[t], a[i]);
		}
		etime=clock();
		freetime+=etime-stime;
	}
	free2=freetime;
	allocate2=allocatetime;
	printf("512 bytes cache:\n");
	printf("                allocate: %lfms\n", ((double)allocate2)/10000000.0);
	printf("                free:     %lfms\n", ((double)free2)/10000000.0);
	//**************************8192 bytes cache*******************************************************************//
	
	t=cache_id_estimate(8192, 0);	//this routine replace kmem_cache_ceate, it will return the id instead of the address of cache
	freetime=0, allocatetime=0;
	for(j=0; j<100000; j++) {
		stime=clock();
		for(i=0; i<100; i++) {
			a[i]=(int *)kmem_cache_alloc(&kcache[t], KM_SLEEP);
		
		}
		etime=clock();
		allocatetime+=etime-stime;
		stime=clock();
		for(i=0; i<100; i++) {
			kmem_cache_free(&kcache[t], a[i]);
		}
		etime=clock();
		freetime+=etime-stime;
	}
	free3=freetime;
	allocate3=allocatetime;
	printf("8192 bytes cache:\n");
	printf("                allocate: %lfms\n", ((double)allocate3)/10000000.0);
	printf("                free:     %lfms\n", ((double)free3)/10000000.0);
	printf("********************************************************************\n");
	
	printf("************************compare with malloc*************************\n");
	/**********************************************************************************************************/
	//**************************128 bytes cache*******************************************************************//

	freetime=0, allocatetime=0;
	for(j=0; j<100000; j++) {
		stime=clock();
		for(i=0; i<100; i++) {
			a[i]=(int *)malloc(sizeof(int)*128);
		
		}
		etime=clock();
		allocatetime+=etime-stime;
		stime=clock();
		for(i=0; i<100; i++) {
			free(a[i]);
		}
		etime=clock();
		freetime+=etime-stime;
	}
	free1=freetime;
	allocate1=allocatetime;
	printf("128 cache:\n");
	printf("                allocate: %lfms\n", ((double)allocate1)/10000000.0);
	printf("                free:     %lfms\n", ((double)free1)/10000000.0);
	//**************************256 bytes cache*******************************************************************//
	
	freetime=0, allocatetime=0;
	for(j=0; j<100000; j++) {
		stime=clock();
		for(i=0; i<100; i++) {
			a[i]=(int *)malloc(sizeof(int)*256);
		
		}
		etime=clock();
		allocatetime+=etime-stime;
		stime=clock();
		for(i=0; i<100; i++) {
			free(a[i]);
		}
		etime=clock();
		freetime+=etime-stime;
	}
	free2=freetime;
	allocate2=allocatetime;
	printf("256 cache:\n");
	printf("                allocate: %lfms\n", ((double)allocate2)/10000000.0);
	printf("                free:     %lfms\n", ((double)free2)/10000000.0);
	//**************************128 bytes cache*******************************************************************//
	
	freetime=0, allocatetime=0;
	for(j=0; j<100000; j++) {
		stime=clock();
		for(i=0; i<100; i++) {
			a[i]=(int *)malloc(sizeof(int)*512);
		
		}
		etime=clock();
		allocatetime+=etime-stime;
		stime=clock();
		for(i=0; i<100; i++) {
			free(a[i]);
		}
		etime=clock();
		freetime+=etime-stime;
	}
	free3=freetime;
	allocate3=allocatetime;
	printf("512 cache:\n");
	printf("                allocate: %lfms\n", ((double)allocate3)/10000000.0);
	printf("                free:     %lfms\n", ((double)free3)/10000000.0);
	//****************************************************************************************************************//
	
	printf("\nLarge object!!!\n");
	/****************************************large object********************************************************/
	
	//**************************513 bytes cache*******************************************************************//
	
	freetime=0, allocatetime=0;
	for(j=0; j<100000; j++) {
		stime=clock();
		for(i=0; i<100; i++) {
			a[i]=(int *)malloc(sizeof(int)*513);
		
		}
		etime=clock();
		allocatetime+=etime-stime;
		stime=clock();
		for(i=0; i<100; i++) {
			free(a[i]);
		}
		etime=clock();
		freetime+=etime-stime;
	}
	free1=freetime;
	allocate1=allocatetime;
	printf("513 bytes:\n");
	printf("                allocate: %lfms\n", ((double)allocate1)/10000000.0);
	printf("                free:     %lfms\n", ((double)free1)/10000000.0);
	//**************************4096 bytes cache*******************************************************************//
	
	freetime=0, allocatetime=0;
	for(j=0; j<100000; j++) {
		stime=clock();
		for(i=0; i<100; i++) {
			a[i]=(int *)malloc(sizeof(int)*4096);
		
		}
		etime=clock();
		allocatetime+=etime-stime;
		stime=clock();
		for(i=0; i<100; i++) {
			free(a[i]);
		}
		etime=clock();
		freetime+=etime-stime;
	}
	free2=freetime;
	allocate2=allocatetime;
	printf("4096 bytes:\n");
	printf("                allocate: %lfms\n", ((double)allocate2)/10000000.0);
	printf("                free:     %lfms\n", ((double)free2)/10000000.0);
	//**************************8192 bytes cache*******************************************************************//
	
	freetime=0, allocatetime=0;
	for(j=0; j<100000; j++) {
		stime=clock();
		for(i=0; i<100; i++) {
			a[i]=(int *)malloc(sizeof(int)*8192);
		
		}
		etime=clock();
		allocatetime+=etime-stime;
		stime=clock();
		for(i=0; i<100; i++) {
			free(a[i]);
		}
		etime=clock();
		freetime+=etime-stime;
	}
	free3=freetime;
	allocate3=allocatetime;
	printf("8192 bytes:\n");
	printf("                allocate: %lfms\n", ((double)allocate3)/10000000.0);
	printf("                free:     %lfms\n", ((double)free3)/10000000.0);
	printf("********************************************************************\n");

	return 0;
}
