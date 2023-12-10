// SPDX-License-Identifier: GPL-2-Clause
#include <hello1.h>

MODULE_AUTHOR("IO-11 Chernyshevych Myroslava\n");
MODULE_DESCRIPTION("Laboratorna 4\n");
MODULE_LICENSE("GPL");

struct timing_list {
	struct list_head node;
	ktime_t start_time;
	ktime_t end_time;
};

static struct list_head head_element = LIST_HEAD_INIT(head_element);


int display_greeting(uint count)
{
	struct timing_list *list, *tmp;
	uint i;

	if (count <= 0) {
	    pr_err("Error: The count is not positive\n");
	} else if (count > 0 && count < 5) {
	    pr_info("Note: The count is less than 5\n");
	} else if (count >= 5 && count <= 10) {
	    pr_warn("Warning: The count is in the range 5 to 10\n");
	} else { // count > 10
	    pr_err("Error: The count exceeds 10\n");
	    return -EINVAL;
	}


	for (i = 0; i < count; i++) {
		list = kmalloc(sizeof(struct timing_list), GFP_KERNEL);
		if (ZERO_OR_NULL_PTR(list))
			goto tidy_away;
		list->start_time = ktime_get();
		pr_info("Hello Linux AK!\n");
		list->end_time = ktime_get();
		list_add_tail(&list->node, &head_element);
	}
	return 0;

tidy_away:
	list_for_each_entry_safe(list, tmp, &head_element, node) {
		list_del(&list->node);
		kfree(list);
	}
	pr_err("Memory not enough\n");
	return -1;
}
EXPORT_SYMBOL(display_greeting);


static int __init init_hello(void)
{
	pr_info("hello1 is initiated\n");
	return 0;
}


static void __exit exit_hello(void)
{
	struct timing_list *list, *tmp;

	list_for_each_entry_safe(list, tmp, &head_element, node) {
		pr_info("Time: %lld", list->end_time - list->start_time);
		list_del(&list->node);
		kfree(list);
	}
	BUG_ON(!list_empty(&head_element));
	pr_info("Success! hello1 has been unloaded\n");
}


module_init(init_hello);
module_exit(exit_hello);
