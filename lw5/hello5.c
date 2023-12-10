// SPDX-License-Identifier: GPL-2.0+
#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/printk.h>
#include <linux/types.h>
#include <linux/ktime.h>
#include <linux/slab.h>
#include <linux/list.h>

struct time_record {
    struct list_head next;
    ktime_t start_time;
    ktime_t finish_time;
};

MODULE_AUTHOR("IO-11 Chernyshevych Myroslava");
MODULE_DESCRIPTION("Laboratorna 5");
MODULE_LICENSE("GPL");

static LIST_HEAD(head);

static uint count = 1;
module_param(count, uint, 0444);
MODULE_PARM_DESC(count, "A count parameter");

static int __init hello_init(void)
{
    uint i = 0;
    struct time_record *tail;

    pr_info("Count: %d\n", count);

    BUG_ON(count >= 5 && count <= 10);

    for (i = 0; i < count; i++) {
        tail = kmalloc(sizeof(struct time_record), GFP_KERNEL);

        if (i == 5)
            tail = 0;

        if (ZERO_OR_NULL_PTR(tail))
            goto exception;

        tail->start_time = ktime_get();
        pr_info("Hello Linux AK!\n");
        tail->finish_time = ktime_get();

        list_add_tail(&(tail->next), &head);
    }

    return 0;

exception:
    {
        struct time_record *md, *tmp;

        pr_err("The end of memory...\n");

        list_for_each_entry_safe(md, tmp, &head, next) {
            list_del(&md->next);
            kfree(md);
        }

        BUG();
        return -ENOMEM;
    }
}


static void __exit hello_exit(void)
{
    struct time_record *md, *tmp;

    list_for_each_entry_safe(md, tmp, &head, next) {
        pr_info("Time: %lld",
        md->finish_time - md->start_time);

        list_del(&md->next);
        kfree(md);
    }

    BUG_ON(!list_empty(&head));
}

module_init(hello_init);
module_exit(hello_exit);

