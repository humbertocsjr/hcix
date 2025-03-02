#include <hcix.h>

#define THREADS_MAX 512

static cothread_t _threads[THREADS_MAX];
static cothread_t *_current = 0;
extern registers_t _cothread_curr_regs;
extern void cothread_switch();
extern void cothread_init_regs();

void cothread_register()
{
    for(size_t i = 0; i < THREADS_MAX; i++)
    {
        _threads[i].status = COTHREAD_NONE;
    }
    _current = &_threads[0];
    _current->status = COTHREAD_RUNNING;
    _current->next = _current;
}

void cothread_exit(int refvalue)
{
    for(size_t i = 0; i < THREADS_MAX; i++)
    {
        if(_threads[i].status != COTHREAD_NONE && _threads[i].next == _current)
        {
            _threads[i].next = _current->next;
        }
    }
    _current->status = COTHREAD_NONE;
    yield();
}

void cothread_direct_exit()
{
    cothread_exit(-1);
}

status_t cothread_new(void *ip, int argc, char ** argv)
{
    status_t st;
    uint32_t *stack;
    size_t stack_top = 32*1024;
    for(size_t i = 0; i < THREADS_MAX; i++)
    {
        if(_threads[i].status == COTHREAD_NONE)
        {
            _threads[i].status = COTHREAD_RUNNING;
            cothread_init_regs();
            stack = (size_t)((size_t)memmgr_alloc(stack_top) + (size_t)stack_top);
            stack--;
            *stack = cothread_direct_exit;
            stack--;
            *stack = ip;
            _cothread_curr_regs.esp = (size_t)stack;
            mem_copy(&_threads[i].registers, &_cothread_curr_regs, sizeof(registers_t));
            _threads[i].next = _current->next;
            _current->next = &_threads[i];
            SET_STATUS(st, STATUS_OK, 0, i);
            return st;
        }
    }
    SET_STATUS(st, STATUS_CAPACITY_OVERFLOW, 0, 0);
    return st;
}

void _cothread_do_switch()
{
    mem_copy(&_current->registers, &_cothread_curr_regs, sizeof(registers_t));
    _current = _current->next;
    mem_copy(&_cothread_curr_regs, &_current->registers, sizeof(registers_t));
}

void yield()
{
    cothread_switch();
}
