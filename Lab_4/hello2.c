// SPDX-License-Identifier: GPL-2-Clause
#include <hello1.h>

MODULE_AUTHOR("IO-11 Goncharov");
MODULE_DESCRIPTION("Laba 4");
MODULE_LICENSE("GPL");

static uint counter = 1;

module_param(counter, uint, 0);
MODULE_PARM_DESC(counter, "Counter for my laba 4\n");

static int __init init_hello(void)
{
 pr_info("Success! hello2 has been initiated\n");
 display_hello(counter);
 return 0;
}

static void __exit exit_hello(void)
{
 pr_info("Success! hello2 has been unloaded\n");
}

module_init(init_hello);
module_exit(exit_hello);