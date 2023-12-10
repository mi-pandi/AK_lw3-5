// SPDX-License-Identifier: GPL-2-Clause
#include <hello1.h>

MODULE_AUTHOR("IO-11 Chernyshevych Myroslava\n");
MODULE_DESCRIPTION("Laboratorna 4\n");
MODULE_LICENSE("GPL");

static uint count = 1;

module_param(count, uint, 0);
MODULE_PARM_DESC(count, "count for my laboratorna4\n");

static int __init initialize_hello(void)
{
	pr_info("hello2 is initiated\n");
	display_greeting(count);
	return 0;
}

static void __exit cleanup_hello(void)
{
	pr_info("hello2 is unloaded\n");
}

module_init(initialize_hello);
module_exit(cleanup_hello);
