#include<linux/init.h>
#include<linux/module.h>
#include<linux/fs.h>
#include<linux/slab.h>
#include<linux/uaccess.h>

#define BUFFER_SIZE 1024

/* Define device_buffer and other global data structures you will need here */
int opened_count = 0;
int closed_count = 0;
static char *device_buffer;
loff_t device_buffer_size;
loff_t new_position;

ssize_t simple_char_driver_read (struct file *pfile, char __user *buffer, size_t length, loff_t *offset)
{
	/* *buffer is the userspace buffer to where you are writing the data you want to be read from the device file*/
	/* length is the length of the userspace buffer*/
	/* offset will be set to current position of the opened file after read*/
	/* copy_to_user function: source is device_buffer and destination is the userspace buffer *buffer */
	copy_to_user(buffer, device_buffer, length);
	printk(KERN_ALERT "Number of bytes read: %d", length);
	return 0;
}

ssize_t simple_char_driver_write (struct file *pfile, const char __user *buffer, size_t length, loff_t *offset)
{
	/* *buffer is the userspace buffer where you are writing the data you want to be written in the device file*/
	/* length is the length of the userspace buffer*/
	/* current position of the opened file*/
	/* copy_from_user function: destination is device_buffer and source is the userspace buffer *buffer */
	copy_from_user(device_buffer + *offset, buffer, length);
	printk(KERN_ALERT "Number of bytes written %d", length);
	return length;
}


int simple_char_driver_open (struct inode *pinode, struct file *pfile)
{
	/* print to the log file that the device is opened and also print the number of times this device has been opened until now*/
	opened_count++;
	printk(KERN_ALERT "Number of times device has been opened: %d \n", opened_count);
	return 0;
}

int simple_char_driver_close (struct inode *pinode, struct file *pfile)
{
	/* print to the log file that the device is closed and also print the number of times this device has been closed until now*/
	closed_count++;
	printk(KERN_ALERT "Number of times device has been closed: %d \n", closed_count);
	return 0;
}

loff_t simple_char_driver_seek (struct file *pfile, loff_t offset, int whence)
{
	/* Update open file position according to the values of offset and whence */
	
	device_buffer_size = strlen(device_buffer);

	if(whence == 0) {
		new_position = offset;
	}
	else if(whence == 1) {
		new_position = pfile->f_pos + offset;
	}
	else if(whence == 2) {
		new_position = strlen(device_buffer) - offset;
	}

	if(new_position > device_buffer_size || new_position < 0 || whence > 2 || whence < 0) {
		return -1;
	}
	else {
		pfile->f_pos = new_position;
		return new_position;
	}
}

struct file_operations simple_char_driver_file_operations = {

	.owner   = THIS_MODULE,
	/* add the function pointers to point to the corresponding file operations. look at the file fs.h in the linux souce code*/
	.open = simple_char_driver_open,
	.release = simple_char_driver_close,
	.llseek = simple_char_driver_seek,
	.read = simple_char_driver_read,
	.write = simple_char_driver_write
};

static int simple_char_driver_init(void)
{
	/* print to the log file that the init function is called.*/
	printk(KERN_ALERT "simple_char_driver init called");
	/* register the device */
	register_chrdev(270, "simple_character_device", &simple_char_driver_file_operations);
	device_buffer = kmalloc(BUFFER_SIZE, GFP_KERNEL);
	memset(device_buffer, '\000', BUFFER_SIZE);
	return 0;
}

static void simple_char_driver_exit(void)
{
	/* print to the log file that the exit function is called.*/
	printk(KERN_ALERT "simple_char_driver exit called");
	/* unregister  the device using the register_chrdev() function. */
	unregister_chrdev(270, "simple_character_device");
	kfree(device_buffer);
}

/* add module_init and module_exit to point to the corresponding init and exit function*/
module_init(simple_char_driver_init);
module_exit(simple_char_driver_exit);
