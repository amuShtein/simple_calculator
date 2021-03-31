#include <stdio.h>
#include <stdlib.h>

short is_op(int c) {
    return (c == '+' || c == '-' || c == '*' || c == '/');
}

int is_number (int c) {
    return (c >= '0') && (c <= '9');
}

int read_number (int* c, FILE* in) {
    int n = (*c) - '0';
    *c = fgetc(in);
    while (*c != EOF && is_number(*c)) {
        n = n*10 + (*c) - '0';

        *c = fgetc(in);
    }
    return n;
}

struct stack {
    int data;
    struct stack* next;
};

struct queue {
    short is_operator;
    int data;
    struct queue* next;
};

struct answer_stack {
    long long int data;
    struct answer_stack* next;
};

void push_queue(struct queue** head, struct queue** end, int data, int is_op) {
    if(*head == NULL) {
        *head = (struct queue*)malloc(sizeof(struct queue));
        (*head)->is_operator = is_op;
        (*head)->data = data;
        (*head)->next = NULL;
        *end = *head;
        return;
    }
    (*end)->next = (struct queue*)malloc(sizeof(struct queue));
    *end = (*end)->next;
    (*end)->is_operator = is_op;;
    (*end)->data = data;
    (*end)->next = NULL;
}

void push_stack(struct stack** head, int data) {
    struct stack* tmp = *head;
    *head = (struct stack*)malloc(sizeof(struct stack));
    (*head)->data = data;
    (*head)->next = tmp;
}

void push_answer_stack(struct answer_stack** head, long long int data) {
    struct answer_stack* tmp = *head;
    *head = (struct answer_stack*)malloc(sizeof(struct answer_stack));
    (*head)->data = data;
    (*head)->next = tmp;
}

int pop_queue (struct queue** head, short* is_op) {
    struct queue * tmp = *head;
    *head = (*head)->next;
    int data = tmp->data;
    *is_op = tmp->is_operator;
    free(tmp);
    return data;
};

int pop_stack(struct stack** head) {
    struct stack * tmp = *head;
    *head = (*head)->next;
    int data = tmp->data;
    free(tmp);
    return data;
}

long long int pop_answer_stack(struct answer_stack** head) {
    struct answer_stack * tmp = *head;
    *head = (*head)->next;
    long long int data = tmp->data;
    free(tmp);
    return data;
}

int get_priority (int c) {
    if(c == '*' || c == '/') {
        return 2;
    }
    if(c == '+' || c == '-') {
        return 1;
    }
    return 0;
}

void print_stack(struct stack* head) {
    struct stack* cur = head;
    while (cur) {
        printf("[%c] ", (char)(cur->data));
        cur = cur->next;
    }
    putchar('\n');
}

void print_queue(struct queue* head) {
    struct queue* cur = head;
    while (cur) {
        if(cur->is_operator) {
            printf("[%c!] ", (char) (cur->data));
        } else {
            printf("[%d] ",cur->data);
        }
        cur = cur->next;
    }
    putchar('\n');
}



int main (void) {
    struct queue* qhead = NULL;
    struct queue* end = qhead;
    struct stack* ops = NULL;

    FILE* in = fopen("input.txt", "r");
    int c;


    while( (c = fgetc(in)) != EOF) {
        if(c == ' ') {
            continue;
        }
        if(is_number(c)) {
            int x = read_number(&c, in);
          //  printf("{x = %d}", x);
            push_queue(&qhead, &end, x, 0);
            if (c == EOF || c == '\n') {
                break;
            }
        }
        if(is_op(c)) {
            while (ops) {
                int op = pop_stack(&ops);
                if (get_priority(c) <= get_priority(op)) {
                    push_queue(&qhead, &end, op, 1);
                } else {
                    push_stack(&ops, op);
                    break;
                }
            }
            push_stack(&ops, c);
        }
        if (c == '(') {
            push_stack(&ops, c);
        }
        if(c == ')') {
            for (int op = pop_stack(&ops); op != '('; op = pop_stack(&ops)) {
                push_queue(&qhead, &end, op, 1);
            }
        }
    }
    while (ops) {
        int op = pop_stack(&ops);
        push_queue(&qhead, &end, op, 1);
    }

    int data;
    long long int res = 0, x, y;
    short is_op;

    struct answer_stack* ahead = NULL;

    while(qhead) {
        data = pop_queue(&qhead, &is_op);
        if(is_op) {
            y = pop_answer_stack(&ahead);
            x = pop_answer_stack(&ahead);

            switch (data) {
                case '+': res = x + y; break;
                case '-': res = x - y; break;
                case '*': res = x * y; break;
                case '/': res = x / y; break;
                default: res = -1; printf("ERROR"); return 0;
            }
            push_answer_stack(&ahead, res);

        } else {
            push_answer_stack(&ahead, data);
        }


    }

    fclose(in);
    FILE* out = fopen("output.txt", "w");

    if(ahead) {
        fprintf(out, "%lld", ahead->data);
    } else {
        printf("ERROR!");
    }

    return 0;
}