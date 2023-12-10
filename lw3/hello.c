#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/moduleparam.h>
#include <linux/ktime.h>
#include <linux/slab.h>

MODULE_AUTHOR("IO-11 Chernyshevych Myroslava");
MODULE_DESCRIPTION("Modified module for educational purposes");
MODULE_LICENSE("GPL");

static uint count = 1; 

module_param(count, uint, 0444);
MODULE_PARM_DESC(count, "A count parameter");

struct custom_struct {
	struct list_head custom_list;
	ktime_t custom_time;
};

static LIST_HEAD(custom_list_head);

static int __init custom_init(void)
{
	int i;	
	
	if (count == 0) {
		printk(KERN_WARNING "Parameter is 0\n");
	} else if (count > 10) {
		printk(KERN_WARNING "Parameter exceeds 10\n");
		return -EINVAL;
	}

	for (i = 0; i < count; i++) {
		struct custom_struct *cs = kmalloc(sizeof(struct custom_struct), GFP_KERNEL);
		cs->custom_time = ktime_get();
		list_add_tail(&cs->custom_list, &custom_list_head);
		printk(KERN_INFO "Hello Linux AK: %d\n", i);
	}

	return 0;
}

static void __exit custom_exit(void)
{
	struct custom_struct *entry, *temp;
	int idx = 0;

	list_for_each_entry_safe(entry, temp, &custom_list_head, custom_list) {
		pr_info("Index: %d; Time = %lld\n", idx++, ktime_to_ns(entry->custom_time));
		list_del(&entry->custom_list);
		kfree(entry);
	}
	printk(KERN_INFO "Module successfully unloaded\n");
}

module_init(custom_init);
module_exit(custom_exit);

