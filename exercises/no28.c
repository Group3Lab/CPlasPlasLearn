
struct ListNode {
     int val;
     struct ListNode *next;
};
 
void deleteNode(struct ListNode *node)
{

    struct ListNode *t2 = node->next,*t1 = node;

    t1->val = t2->val;
    t1->next = t2->next;//到t3

    free(t2);
}