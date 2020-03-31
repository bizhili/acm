#include "head.h"
struct pcb * init_process(){
	struct pcb *q;
	struct pcb *t;
	struct pcb *p;
	int i=0;
	fprintf(pFile,"input name and needtime, please input %d processes\n,set of time: *10ms",P_NUM);
	while (i<P_NUM){
		q=(struct pcb *)malloc(sizeof(struct pcb));
		scanf("%s", &(q->name) );
		scanf("%d", &(q->startTime) );
		scanf("%d", &(q->needtime));
		q->cputime=0;
		q->priority=0.0f;
		q->process=future;
		q->waitTime=0;
		q->needtime*=10;
		q->startTime*=10;
		q->leastTime=q->needtime;
		q->next=NULL;
		if (i==0){
			p=q;
			t=q;
		}
		else{
			t->next=q;
			t=q;
		}
		i++;
	}/*while*/
	return p;//head process
}