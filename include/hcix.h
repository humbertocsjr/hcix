#ifndef HCIX_H
#define HCIX_H

typedef   signed char   int8_t;
typedef unsigned char  uint8_t;
typedef   signed short  int16_t;
typedef unsigned short uint16_t;
typedef   signed int    int32_t;
typedef unsigned int   uint32_t;
typedef unsigned int    size_t;
typedef unsigned short  process_id_t;


typedef struct status_t
{
    enum
    {
        STATUS_OK,
        STATUS_NOT_FOUND,
        STATUS_NOT_IMPLEMENTED,
        STATUS_ALREADY_EXISTS,
        STATUS_CAPACITY_OVERFLOW,
        STATUS_INVALID_POINTER
    } status;
    int value;
    size_t size;
    void *pointer;
} status_t;

#define DEVICE_NAME_SIZE 32
#define DEVICE_PARAMS_MAX 32

typedef enum
{
    DEVICE_NONE,
    DEVICE_UNKNOWN,
    DEVICE_CONSOLE,
    DEVICE_DISK,
    DEVICE_KEYBOARD,
    DEVICE_MEMORY_MANAGER
} device_type_t;

enum
{
    CONSOLE_CONF_X,
    CONSOLE_CONF_Y,
    CONSOLE_CONF_WIDTH,
    CONSOLE_CONF_HEIGHT,
    CONSOLE_CONF_FOREGROUND,
    CONSOLE_CONF_BACKGROUND,
};

enum
{
    MEMMGR_CONF_LOWMEM_CAPACITY,
    MEMMGR_CONF_HIGHMEM_CAPACITY,
    MEMMGR_CONF_FREE_PAGES,
    MEMMGR_CONF_TOTAL_PAGES
};

typedef struct device_t
{
    uint32_t id;
    char name[DEVICE_NAME_SIZE];
    device_type_t type;
    status_t (*init)(struct device_t *dev);
    status_t (*read)(struct device_t *dev, void *data, size_t size);
    status_t (*write)(struct device_t *dev, void *data, size_t size);
    status_t (*set_position)(struct device_t *dev, size_t pos);
    status_t (*get_position)(struct device_t *dev);
    status_t (*get_block_size)(struct device_t *dev);
    status_t (*set_conf)(struct device_t *dev, int id, int value);
    status_t (*get_conf)(struct device_t *dev, int id);
    int params[DEVICE_PARAMS_MAX];
    void *buffer;
    size_t buffer_size;
} device_t;

typedef struct registers_t
{
    uint32_t cr3;
    uint32_t edi;
    uint32_t esi;
    uint32_t ebp;
    uint32_t esp;
    uint32_t ebx;
    uint32_t edx;
    uint32_t ecx;
    uint32_t eax;
    uint32_t eflags;
} registers_t;

typedef struct cothread_t
{
    registers_t registers;
    process_id_t process_id;
    enum
    {
        COTHREAD_NONE,
        COTHREAD_RUNNING,
        COTHREAD_SENDING_REQ,
        COTHREAD_SENDING_RESP,
        COTHREAD_WAITING_REQ,
        COTHREAD_WAITING_RESP
    } status;
    struct cothread_t *next;
} cothread_t;

// libhcix
int char_is_new_line(char c);
int char_is_space(char c);
void *mem_clear(void *dest, size_t size);
char *mem_copy(char *dest, char *src, size_t capacity);
void *mem_set_words(void *dest, uint16_t value, size_t size);
char *string_concat_char(char *dest, char c, size_t capacity);
char *string_copy(char *dest, char *src, size_t capacity);
int string_equal(char *dest, char *src, size_t capacity);
size_t string_len(char *src, size_t capacity);
int32_t string_to_int32(char *src, size_t capacity);
uint32_t string_to_uint32(char *src, size_t capacity);

// libdevice
status_t device_alloc(device_type_t type, char *name);
status_t device_init(device_t *dev);
status_t device_init_by_name(char *name);
status_t device_find_by_name(char *name);
status_t device_get_conf(device_t *dev, int id);
status_t device_get_conf_by_name(char *name, int id);
status_t device_set_conf(device_t *dev, int id, int value);
status_t device_set_conf_by_name(char *name, int id, int value);
status_t device_read(device_t *dev, void *data, size_t size);
status_t device_read_by_name(char *name, void *data, size_t size);
status_t device_write(device_t *dev, void *data, size_t size);
status_t device_write_by_name(char *name, void *data, size_t size);

// libio
void io_write_uint8(uint16_t port, uint8_t value);
void io_write_uint16(uint16_t port, uint16_t value);
void io_write_uint32(uint16_t port, uint32_t value);
uint8_t io_read_uint8(uint16_t port);
uint16_t io_read_uint16(uint16_t port);
uint32_t io_read_uint32(uint16_t port);

// libconsole
status_t console_set_phys(device_t *dev);
void console_write_const(const char *str);
void console_write_uint32(uint32_t value);
void console_write_hex32(uint32_t value);
uint16_t console_get_width();
uint16_t console_get_height();
uint16_t console_get_x();
void console_set_position(uint16_t x, uint16_t y);

// libcothread
void yield();
void cothread_idle();
status_t cothread_new(void *ip, int argc, char ** argv);

// libmemmgr
void *memmgr_alloc(size_t size);

#define SET_STATUS(obj, st, ptr, val) obj.status = st; obj.pointer = ptr; obj.value = val;
#define OK(obj) (obj.status == STATUS_OK)
#define ERROR(obj) (obj.status != STATUS_OK)
#define GET_VALUE(obj, default_value) (obj.status == STATUS_OK ? obj.value : default_value)
#define GET_POINTER(obj, default_value) (obj.status == STATUS_OK ? obj.pointer : default_value)

#endif