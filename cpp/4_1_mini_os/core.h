#ifndef CORE_H
#define CORE_H
#include <time.h>
#include "head.h"
#include <sys/time.h>



extern bool_t os_delay_ms(u32 delay_time);
extern void round_cal();
extern void set_state(struct pcb *p);
extern struct pcb * get_next(struct pcb * k, struct pcb * head);
extern void cpu_round(struct pcb *q);
extern struct pcb * get_process_round();
extern void priority_init();
extern void cpuexe(struct pcb *q);
extern int process_finish(struct pcb *q);


#endif