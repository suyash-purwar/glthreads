# GLThread – Glue-Based (Intrusive) Linked List for C

## Introduction

GLThread is a lightweight implementation of an intrusive (glue-based) doubly linked list in C. This design is widely used in kernels, schedulers, memory allocators, and systems software.

Unlike traditional linked lists that wrap user data inside list nodes, GLThread embeds the list node directly inside the user’s structure. This enables:

- Zero extra allocations per node
- Cache-friendly memory layout
  - Payload and node live adjacent to each other
- Type-safe traversal without void * abuse
- Efficient object membership in multiple data structures

## What Is an Intrusive (aka Glue-Based) List?

In a glue-based list:

> The data structure links live inside the object itself.

```c++
typedef struct employee {
    char name[30];
    int age;
    glthread_node_t node;   // intrusive link
} employee_t;
```

The list does not own memory for elements — the user does.

GLThread only manages linkage, not allocation.

## Core Concepts

`glthread_node_t`
```c++
typedef struct glthread_node {
    struct glthread_node *prev;
    struct glthread_node *next;
} glthread_node_t;
```

This is the intrusive node embedded inside user objects.

`glthread_t`
```c++
typedef struct {
    glthread_node_t *head;
    unsigned int offset;
} glthread_t;
```

- head → pointer to the first node of the list

- offset → byte offset of `glthread_node_t` inside the container structure. It is used to recover the parent object via pointer arithmetic. Read about [`offsetof`](https://en.cppreference.com/w/cpp/types/offsetof.html) macro. 

## Initialization

#### Initialize the list

```c++
glthread_t *glthread_init(glthread_node_t *head, size_t offset);
```

Example:

```c++
glthread_t *list =
glthread_init(NULL, offsetof(employee_t, node));
```

#### Initialize a node

```c++
void glthread_init_node(glthread_node_t *node);
```
Must be called before inserting a node.

### Adding Nodes
#### Generic add
```c++
int glthread_add_node(glthread_t *list, glthread_node_t *node, Position position);
```

Where Position is:

```c++
typedef enum {
    HEAD
    TAIL
} Position;
```

#### Convenience APIs
```c++
void glthread_add_node_at_head(glthread_t *, glthread_node_t *);
void glthread_add_node_at_tail(glthread_t *, glthread_node_t *);
```

### Removing Nodes
#### Generic remove

```c++
int glthread_remove_node(glthread_t *, Position);
```

#### Convenience APIs
```c++
void glthread_remove_node_at_head(glthread_t *);
void glthread_remove_node_at_tail(const glthread_t *);
```

**These functions free memory. GLThread assumes ownership of the container object.**

### Traversal and Utilities

#### Length
```c++
size_t glthread_len(const glthread_t *);
```

#### Iteration
```c++
void glthread_foreach(const glthread_t *, void (*cb)(void *arg, size_t index));
```

Callback receives:

- Pointer to container object
- Index in list

#### Filtering (`glthread_where`)
```c++
void **glthread_where(const glthread_t *, unsigned char (*cb)(const void *, size_t), size_t *out_list_size);
```

- Returns dynamically allocated array of matching container pointers
- Grows automatically using exponential resizing
- **Caller must free the returned array**

#### Printing a Node
```c++
void glthread_print_node(const glthread_t *, const glthread_node_t *, void (*print)(void *));
```

### Memory Management

#### Free entire list
```c++
void glthread_free(const glthread_t *);
```

- Frees all container objects
- Frees the list structure itself
- **Use only if GLThread fully owns the objects**

### How Container Recovery Works

GLThread uses an offsetof-based mechanism similar to Linux’s container_of:

```c++
void glthread_foreach(const glthread_t *, void (*cb)(void *arg, size_t index));
```

Internally:

```c++
container_address = node_address - offset
```

This works only because the node is embedded, not a pointer.

### Example Usage

```c++
typedef struct employee {
    char name[30];
    int age;
    glthread_node_t node;
} employee_t;

employee_t *e = malloc(sizeof(employee_t));
strcpy(e->name, "Darwin");
e->age = 23;
glthread_init_node(e->node);

glthread_t *list = glthread_init(NULL, offsetof(employee_t, node));
glthread_add_node(list, e->node, HEAD);
```

Checkout [`main.c`](https://github.com/suyash-purwar/glthreads/blob/master/src/main.c) for more thorough examples.

### Limitations & Caveats

- Not thread-safe
- Incorrect offsets cause undefined behavior
- Requires discipline in memory ownership

### Intended Use Cases

- Operating systems / kernels
- Schedulers and task queues
- Game engines
- Networking stacks
- Embedded systems
- High-performance infrastructure code
