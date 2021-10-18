#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

MODULE_LICENSE("");

static int device_file_major_number = 0;
static const char device_name[] = "ASO";

static ssize_t device_file_read(struct file *file_ptr, char __user *user_buffer, size_t count, loff_t *position) {
	printk(KERN_NOTICE "ASO: device read\n");
	return 0;
}

static ssize_t device_file_write(struct file *file_ptr, const char __user *user_buffer, size_t length, loff_t *offset) {
	printk(KERN_INFO "ASO: device write\n");
	return length;
}

static struct file_operations simple_driver_fops = {
	.owner = THIS_MODULE,
	.read  = device_file_read,
	.write = device_file_write,
};

int register_device(void) {
	int result = 0;
	printk(KERN_NOTICE "ASO: register_device() is called\n");
	result = register_chrdev(0, device_name, &simple_driver_fops);
	if (result < 0) {
		printk( KERN_WARNING "ASO: can\'t register character device with error code = %i\n", result);
		return result;
	}
	device_file_major_number = result;
	printk(KERN_NOTICE "ASO: Device File: %i\n", device_file_major_number);
	return 0;
}

void unregister_device(void) {
	printk(KERN_NOTICE "ASO: unregister_device() is called\n");
	if (device_file_major_number != 0) {
		unregister_chrdev(device_file_major_number, device_name);
	}
}

static int my_init(void) {
	printk(KERN_NOTICE "Init LKM\n");
	register_device();
	return 0;
}

static void my_exit(void) {
	printk(KERN_NOTICE "Exit LKM\n");
	unregister_device();
	return;
}

module_init(my_init);
module_exit(my_exit);
