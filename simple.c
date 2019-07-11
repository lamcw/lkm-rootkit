#include <linux/module.h>

MODULE_DESCRIPTION("Testing kernel module");
MODULE_AUTHOR("Thomas Lam");
MODULE_LICENSE("GPL");

static int __init dummy_init(void)
{
	printk(KERN_INFO "Hi\n");
	return 0;
}

static void __exit dummy_exit(void)
{
	printk(KERN_INFO "Bye\n");
}

module_init(dummy_init);
module_exit(dummy_exit);
