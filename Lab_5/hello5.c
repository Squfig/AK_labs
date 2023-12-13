// SPDX-License-Identifier: GPL-2.0+
#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/printk.h>
#include <linux/types.h>
#include <linux/ktime.h>
#include <linux/slab.h>
#include <linux/list.h>

struct time_measurement {
    struct list_head list;
    ktime_t start;
    ktime_t end;
};

MODULE_AUTHOR("IO-11 Goncharov");
MODULE_DESCRIPTION("Laba 5");
MODULE_LICENSE("GPL");


static LIST_HEAD(time_list);

static uint counter = 1;
module_param(counter, uint, 0444);
MODULE_PARM_DESC(counter, "A counter parameter");

static int __init my_hello_init(void)
{
    uint idx = 0;
    struct time_measurement *current_record;

    pr_info("Counter: %d\n", counter);

    BUG_ON(counter >= 6);

    for (idx = 0; idx < counter; idx++) {
        current_record = kmalloc(sizeof(struct time_measurement), GFP_KERNEL);

        if (idx == 5)
            current_record = NULL;

        if (ZERO_OR_NULL_PTR(current_record))
            goto error_handle;

        current_record->start = ktime_get();
        pr_info("Hello!\n");
        current_record->end = ktime_get();

        list_add_tail(&current_record->list, &time_list);
    }

    return 0;

error_handle:
    {
        struct time_measurement *temp_record, *safe_record;

        pr_err("Out of memory...\n");

        list_for_each_entry_safe(temp_record, safe_record, &time_list, list) {
            list_del(&temp_record->list);
            kfree(temp_record);
        }

        BUG();
        return -ENOMEM;
    }
}

static void __exit my_hello_exit(void)
{
    struct time_measurement *temp_record, *safe_record;

    list_for_each_entry_safe(temp_record, safe_record, &time_list, list) {
        pr_info("Execution Time: %lld",
        temp_record->end - temp_record->start);

        list_del(&temp_record->list);
        kfree(temp_record);
    }

    BUG_ON(!list_empty(&time_list));
}

module_init(my_hello_init);
module_exit(my_hello_exit);
