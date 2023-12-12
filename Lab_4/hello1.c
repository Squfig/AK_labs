// SPDX-License-Identifier: GPL-2-Clause
#include <hello1.h>

MODULE_AUTHOR("IO-11 Goncharov");
MODULE_DESCRIPTION("Laba 4");
MODULE_LICENSE("GPL");

struct time_track_list {
 struct list_head node;
 ktime_t before;
 ktime_t after;
};

static struct list_head list_head_node = LIST_HEAD_INIT(list_head_node);


int display_hello(uint counter)
{
 struct time_track_list *list, *tmp;
 uint i;

 if (counter < 0) {
  pr_err("Fail! The parameter's less than 0\n");
 } else if (counter == 0) {
  pr_warn("Warning! The parameter's 0\n\n");
 } else if (counter >= 5 && counter <= 10) {
  pr_warn("Warning! The parameter's between 5 and 10\n");
 } else if (counter > 10) {
  pr_err("Fail! The parameter's > 10\n");
  return -EINVAL;
 }

 for (i = 0; i < counter; i++) {
  list = kmalloc(sizeof(struct time_track_list), GFP_KERNEL);
  if (ZERO_OR_NULL_PTR(list))
   goto tidy_away;
  list->before = ktime_get();
  pr_info("Hello!\n");
  list->after = ktime_get();
  list_add_tail(&list->node, &list_head_node);
 }
 return 0;

tidy_away:
 list_for_each_entry_safe(list, tmp, &list_head_node, node) {
  list_del(&list->node);
  kfree(list);
 }
 pr_err("Fail! No memory\n");
 return -1;
}
EXPORT_SYMBOL(display_hello);


static int __init init_hello(void)
{
 pr_info("Success! hello1 has been initiated\n");
 return 0;
}


static void __exit exit_hello(void)
{
 struct time_track_list *list, *tmp;

 list_for_each_entry_safe(list, tmp, &list_head_node, node) {
  pr_info("Time: %lld", list->after - list->before);
  list_del(&list->node);
  kfree(list);
 }
 BUG_ON(!list_empty(&list_head_node));
 pr_info("Success! hello1 has been unloaded\n");
}


module_init(init_hello);
module_exit(exit_hello);