#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <ucontext.h>
#include <signal.h>
#include <vector>

#define MEM 64000

//#include <thread>
//#include <chrono>
//std::this_thread::sleep_for(std::chrono::milliseconds(2000));

using namespace std;

enum thread_state
{
    RUNNING,
    READY,
    SLEEPING,
    SUSPENDED,
    FINISHED
};

class myThread
{
    private:

        ucontext_t new_context;
        ucontext_t old_context;
        thread_state state;

    public:

        myThread(void (*ptr_function)())
        {
            getcontext(&old_context);
            getcontext(&new_context);
            new_context.uc_link = &old_context;
            new_context.uc_stack.ss_sp = malloc(MEM);
            new_context.uc_stack.ss_size = MEM;
            new_context.uc_stack.ss_flags = 0;

            makecontext(&new_context, (void (*)())ptr_function, 0);

        }

        void run()
        {
            swapcontext(&old_context, &new_context);
        }

        void set_state(thread_state new_state)
        {
            state = new_state;
        }

        thread_state get_state()
        {
            return state;
        }
};


class ManageThreads
{
    private:

        vector<myThread*> list_of_thread;

    public:

        unsigned create(void (*ptr_function)())
        {
            myThread *ob_thread = new myThread(ptr_function);
            ob_thread->set_state(thread_state::READY);

            list_of_thread.insert(list_of_thread.end(), ob_thread);
            return list_of_thread.size() - 1;
        }

        void start_thread_by_id(unsigned id)
        {
            if (list_of_thread[id]->get_state() == thread_state::READY)
            {
                list_of_thread[id]->run();
            }
        }

};



void fn1() {
    cout << "From new thread. Manage works quite good !!!" << endl;
}


int main(int argc, char *argv[]) {

    ManageThreads *m = new ManageThreads();
    unsigned id = m->create(fn1);
    m->start_thread_by_id(id);

    cout << "In Main" << endl;
}
















































// Thread Status structure

//typedef struct status_t
//{
//    int id;
//    enum {RUNNING, READY, SLEEPING, SUSPENDED, FINISHED} state;
//    unsigned no_of_bursts;
//    unsigned total_exec_time;
//    unsigned total_sleep_time;
//    unsigned total_wait_time;
//    unsigned avg_exec_time;
//    unsigned avg_wait_time;
//    unsigned wake_time;
//} status_t;
//
//// Thread Structure
//typedef struct thread_t
//{
//    char *stack;
//    status_t *status;
//    int priority;
//    sigjmp_buf jbuf;
//    address_t pc;
//    address_t sp;
//} thread_t;



//Приклад створення потоку:

//int CreateThread(void (*f) (void), int priority)
//{
//    // Return -1 if fail
//    if (thread_list->size + 1 > MAX_NO_OF_THREADS)
//    {
//        printf("Too many threads\n");
//        return -1;
//    }
//    thread_t *thread = malloc(sizeof(thread_t));
//
//    thread->status = malloc(sizeof(status_t));
//    thread->stack = malloc(STACK_SIZE);
//    thread->status->id = next_thread;
//    thread->priority = priority;
//    next_thread++;
//    thread->status->state = READY;
//
//    thread->sp = (address_t)thread->stack + STACK_SIZE - sizeof(address_t);
//    thread->pc = (address_t)f;
//    sigsetjmp(thread->jbuf, 1);
//    (thread->jbuf->__jmpbuf)[JB_SP] = translate_address(thread->sp);
//    (thread->jbuf->__jmpbuf)[JB_PC] = translate_address(thread->pc);
//    sigemptyset(&thread->jbuf->__saved_mask);
//
//    thread_enqueue(thread, ready_list);
//    thread_enqueue(thread, thread_list);
//    return thread->status->id;
//}
//
//
//int ResumeThread(int thread_id)
//{
//    thread_t *t = GetThread(thread_id);
//    if (t == NULL)
//        return -1;
//    if (t->status->state != SUSPENDED)
//        return thread_id;
//    thread_enqueue(t, ready_list);
//    t->status->state = READY;
//    return thread_id;
//}
//
//void SleepThread(int sec)
//{
//    current->status->state = SLEEPING;
//    current->status->wake_time = (GetCurrentTime() + (sec * 1000));
//    current->status->total_sleep_time += (sec * 1000);
//    YieldCPU();
//}
//
//void YieldCPU()
//{
//    raise(SIGVTALRM);
//}
//
//int GetStatus(int thread_id, status_t *status)
//{
//    thread_t *t = GetThread(thread_id);
//    if (t == NULL)
//        return -1;
//    status->id = t->status->id;
//    status->state = t->status->state;
//    status->no_of_bursts = t->status->no_of_bursts;
//    status->total_exec_time = t->status->total_exec_time;
//    status->total_sleep_time = t->status->total_sleep_time;
//    status->total_wait_time = t->status->total_wait_time;
//    status->avg_exec_time = t->status->avg_exec_time;
//    status->avg_wait_time = t->status->avg_wait_time;
//    return t->status->id;
//}
