#include <scheduler.h>
#include <output.h>

static Scheduler scheduler;
static int schedulerInitiated = 0;

Process getProcessWithSpecifiedQueue(int pid, SchedulerLL q);

void schedulerInit(){
	scheduler = kmalloc(sizeof(struct scheduler));
	scheduler -> waitingpq = queueInit();
	scheduler -> blockedpq = queueInit();
	schedulerInitiated = 1;
}

Process schedule(){

	if(scheduler -> waitingpq -> current !=NULL){
		scheduler -> waitingpq -> current = scheduler -> waitingpq-> current -> next;
		return scheduler -> waitingpq -> current -> process;	
	}
	return NULL;
}

SchedulerLL queueInit(){
	SchedulerLL q = kmalloc(sizeof(struct schedulerLL));
	q -> size = 0;
	q -> current = NULL;
	return q;
}

void printProcesses(){
	LLnode first = scheduler->waitingpq->current;
	LLnode current = scheduler->waitingpq->current;
	
	out_printf ("Processes that are waiting:\n");
	do {
		out_printf("%s: pid %d", current->process->name, current->process->pid);
		current = current -> next;
	}while(current != first);
	
	current = scheduler->blockedpq->current;
	first = scheduler->blockedpq->current;
	
	out_printf("Processes that are blocked:\n");
	do {
		out_printf("%s: pid %d", current->process->name, current->process->pid);
		current = current -> next;
	}while(current != first);
	
}

int addProcess(Process p, SchedulerLL q){
	if(p == NULL)
		return ERROR;
	if(q == NULL)
		return ERROR;
	LLnode node = kmalloc(sizeof(struct llnode));
	if(node == NULL)
		return ERROR; //error

	//out_printf("Nuevo Proceso\nID: %d\nName: %s", p->pid,p->name);

	if(q->size == 0){
		q->current = node;
		node->process = p;
		node -> next = node;
		node -> prev = node;
		q->size++;
		return 1;
	}
	q->current->prev ->next = node;
	node->process = p;
	node -> prev = q -> current -> prev;
	node -> next = q -> current;
	q -> current -> prev = node;
	q->size++;
	return 1; 
}

int addProcessWaiting(Process p){
	return addProcess(p, scheduler -> waitingpq);
}

int addProcessBlocked(Process p){
	return addProcess(p, scheduler -> blockedpq);
}

Process removeProcess(uint64_t pid, SchedulerLL q){
	int found = 0;
	LLnode node = q -> current;
	Process p = NULL;
	if(q->size < 1){
		return 0;
	}
	do{
		if(node->process->pid == pid)
			found = 1;
	}while(node != q ->current && !found);

	if(q->size == 1 && found){
		p = node -> process; 
		q->size --;
		q -> current = NULL;
		return p;
	}
	if(found){
		p = node->process;
		node -> prev -> next = node -> next;
		node -> next -> prev = node -> prev;
	}
	return 0;
}

Process removeProcessWaiting(uint64_t pid){
	return removeProcess(pid, scheduler -> waitingpq);
}

Process removeProcessBlocked(uint64_t pid){
	return removeProcess(pid, scheduler -> blockedpq);
}

Process getCurrentWaiting(){
	if(schedulerInitiated == 0)
		return NULL;
	return getCurrent(scheduler -> waitingpq);
}

Process getCurrentBlocked(){
	if(schedulerInitiated == 0)
		return NULL;
	return getCurrent(scheduler -> blockedpq);
}

Process getCurrent(SchedulerLL q){
	if(q->size == 0)
		return NULL;
	return q -> current -> process;
}

Process getProcessWithSpecifiedQueue(int pid, SchedulerLL q){
	int found = 0;
	LLnode node = q -> current;
	if(q->size < 1){
		return NULL;
	}
	do{
		if(node->process->pid == (uint64_t)pid)
			found = 1;
	}while(node != q ->current && !found);

	if(found)
		return node->process;
	return NULL;
}

Process getProcess(int pid){
	Process ret = getProcessWithSpecifiedQueue(pid, scheduler->waitingpq);
	if(ret != NULL)
		return ret;
	return getProcessWithSpecifiedQueue(pid, scheduler->blockedpq);
}

int isWaiting(int pid){
	return getProcessWithSpecifiedQueue(pid, scheduler->waitingpq) != NULL;
}

int isBlocked(int pid){
	return getProcessWithSpecifiedQueue(pid, scheduler->blockedpq) != NULL;
}