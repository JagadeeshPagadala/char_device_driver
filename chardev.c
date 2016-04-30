/*
*	Simple char device driver.
*
*
*/
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/slab.h>

static dev_t major;
static struct cdev char_dev;
static struct class *class;

int char_open(struct inode* inode, struct file* file)
{
	printk(KERN_EMERG"its open");
	return 0;
}

int char_close(struct inode* inode, struct file* file)
{
	printk(KERN_EMERG"its close");
	return 0;
}

ssize_t char_write(struct file* file, const  char __user *data,size_t num, loff_t* offset)
{
	printk(KERN_EMERG"its write");
	return num;
}

ssize_t char_read(struct file* file, char __user *data,size_t num, loff_t* offset)
{
	printk(KERN_EMERG"its read");
	return num;
}

struct file_operations fileops=
{
	.owner = THIS_MODULE,
	.open = char_open,
	.release = char_close,
	.write = char_write,
	.read = char_read,
};

static int char_init(void)
{

	printk(KERN_EMERG"init module");

	if( alloc_chrdev_region(&major, 0, 2, "testdevice") < 0)
		return -1;

	if( (class = class_create(THIS_MODULE, "test_class")) == NULL) {
		unregister_chrdev_region(major, 2);
		return -1;
	}

	cdev_init(&char_dev, &fileops);
	if (cdev_add(&char_dev, major, 1) == -1) {
		device_destroy(class, major);
		class_destroy(class);
		unregister_chrdev_region(major, 2);
		return -1;
	}

	if (device_create(class, NULL, major+0, NULL, "testdevice0") == NULL) {
		device_destroy(class, major);
		class_destroy(class);
		unregister_chrdev_region(major, 2);
		return -1;
	}

	return 0;
}

static void char_exit(void)
{
	cdev_del(&char_dev);
	device_destroy(class, major);
	class_destroy(class);
	unregister_chrdev_region(major, 2);
	printk("exit module ");
}

module_init(char_init);
module_exit(char_exit);
MODULE_AUTHOR("Jagadeesh Pagadala");
MODULE_DESCRIPTION("Pseudo Device driver");
MODULE_LICENSE("GPL");
