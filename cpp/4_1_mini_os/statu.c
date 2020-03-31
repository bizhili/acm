
#include "statu.h"
void  display(struct pcb *p){
	fprintf(pFile,"name    cputime    needtime    priority    state\n");
	while(p){
		fprintf(pFile,"%s", p->name);
		fprintf(pFile,"        ");
		fprintf(pFile,"%d", p->cputime);
		fprintf(pFile,"        ");
		fprintf(pFile,"%d", p->needtime);
		fprintf(pFile,"        ");
		fprintf(pFile,"%d", p->priority);
		fprintf(pFile,"        ");
		switch(p->process){
			case ready:
				fprintf(pFile,"ready\n");
				break;
			case execute:
				fprintf(pFile,"execute\n");
				break;
			case block:
				fprintf(pFile,"block\n");
				break;
			case finish:
				fprintf(pFile,"finish\n");
				break;
		}
		p=p->next;
	}
}
void display_round(struct pcb *p){
	fprintf(pFile,"NAME  CPUTIME  NEEDTIME  COUNT  ROUND  STATE\n");
	while(p){
		fprintf(pFile,"%s",p->name);
		fprintf(pFile,"        ");
		fprintf(pFile,"%d",p->cputime);
		fprintf(pFile,"        ");
		fprintf(pFile,"%d",p->needtime);
		fprintf(pFile,"        ");
		fprintf(pFile,"%d",p->count);
		fprintf(pFile,"        ");
		fprintf(pFile,"%d",p->round);
		fprintf(pFile,"        ");
		switch(p->process){
				case ready:
				fprintf(pFile,"ready\n");
				break;
			case execute:
				fprintf(pFile,"execute\n");
				break;
			case finish:
				fprintf(pFile,"finish\n");
				break;
		}
		p=p->next;
	}
}

void display_menu(){
	fprintf(pFile,"CHOOSE THE ALGORITHM:\n" );
	fprintf(pFile,"1 PRIORITY\n" );
	fprintf(pFile,"2 ROUNDROBIN\n" );
	fprintf(pFile,"3 EXIT\n" );
}