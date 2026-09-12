#include<stdlib.h>

struct ListNode {
     int val;
     struct ListNode *next;
 };
 
struct ListNode *reverseList(struct ListNode *head)
{
    struct ListNode *t1, *t2;   

    if (head == NULL)
        return NULL;

    if (head->next == NULL)
        return head;

    t1 = head;
    t2 = head->next;
    head->next = NULL;           

    while (t2 != NULL)
    {
        struct ListNode *t3 = t2->next;  
        t2->next = t1;   
                         
        t1 = t2;                         
        t2 = t3;                         
    }

    return t1;               
}