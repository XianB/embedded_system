#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/mm.h>
#include <asm/uaccess.h>
#include <linux/init.h>
#include <linux/poll.h>
#include <linux/slab.h>


#define READ_BUF_SIZE	1024
#define WRITE_BUF_SIZE	1024
#define MAX_DATABUF_SIZE 1024

//为了便于参数传递宏的使用,将主设备号和设备名进行了相应的修改
static int DEV_MAJOR = 0;
static char *DEV_NAME = "char_dev";
static char* DataBuf = NULL;
int major;

//设定参数传递宏
module_param(DEV_MAJOR,int,0);
module_param(DEV_NAME,charp,0);

struct Mydevice 
{
	const char *name;		/* DEV name */	
	unsigned int major;		/* Major num */	
	unsigned int minor;		/* Minor num */
	unsigned char *read_buffer;	/* Read Buffer area */
	unsigned char *write_buffer;	/* Write Buffer area */
	wait_queue_head_t read_queue;   /* Read Queue */
	wait_queue_head_t write_queue;  /* дWrite Queue */
	struct semaphore	sem;	/* Semaphore for lock */
};

int my_open(struct inode *inode,struct file *filp)
{
	struct Mydevice	*dev = kmalloc(sizeof(struct Mydevice), GFP_KERNEL);
	if(dev == NULL) {
		printk(" KERN_ALERT allocate device memory failed.\n");
		return(-ENOMEM);
	} 
	dev->name = DEV_NAME;
	dev->major = MAJOR(inode->i_rdev);
	dev->minor = MINOR(inode->i_rdev);
	dev->read_buffer = kmalloc(READ_BUF_SIZE,GFP_KERNEL);
	if(dev->read_buffer == NULL)
		printk(" KERN_ALERT allocate read buffer memory failed.\n");
	dev->write_buffer = kmalloc(WRITE_BUF_SIZE,GFP_KERNEL);
	if(dev->read_buffer == NULL)
		printk(" KERN_ALERT allocate write buffer memory failed.\n");
	init_waitqueue_head(&dev->read_queue);
	init_waitqueue_head(&dev->write_queue);
	if(filp->private_data == NULL)
		filp->private_data = dev;

	DataBuf = kmalloc(MAX_DATABUF_SIZE,GFP_KERNEL);
	if(DataBuf == NULL)
		printk(" KERN_ALERT allocate DataBuf memory failed.\n");
	printk("The function of my_open has been called!\n");

	return 0;
}

int my_release(struct inode *inode,struct file *filp)
{
	struct Mydevice *dev = filp->private_data;
	if(dev->read_buffer != NULL)
		kfree(dev->read_buffer);
	if(dev->write_buffer != NULL)
		kfree(dev->write_buffer);
	kfree(dev); 		
	printk("The function of my_release has been called £¡\n");
	return 0;
}

ssize_t my_read(struct file *filp,char *buf,size_t count,loff_t *offp)
{
	int accountread = 0;
	char *pdata = kmalloc(count,GFP_KERNEL);
	if(pdata == NULL)
		return (-ENOMEM);
	//防止copy_to_user的警告
	accountread = copy_to_user(buf,DataBuf,count);
	*offp += count;
	printk("The function of my_read has been called £¡\n");
	return count;	
}

ssize_t my_write(struct file *filp,char *buf,size_t count,loff_t *offp)
{
	int accountwrite = 0;
	char *pdata = kmalloc(count,GFP_KERNEL);
	if(pdata == NULL)
		return (-ENOMEM);
	//防止copy_to_user的警告
	accountwrite = copy_from_user(DataBuf,buf,count);
	*offp += count;
	printk("The function of my_write has been called £¡\n");
	return count;	
}
#define DRIVER_EVENT_BIT	13
int my_poll(struct file *filp, poll_table *wait)
{
    unsigned int mask = 0;

    mask |= 1<<DRIVER_EVENT_BIT;
    return mask;
}
//int  my_ioctl (struct inode *inode,struct file *filp,unsigned int cmd,unsigned long arg)
int  my_ioctl (struct file *filp,unsigned int cmd,unsigned long arg)
{
	switch(cmd){
		case 1 :{
				printk("This is command 1 !\n");
				break;
			}
		case 2 :{
				printk("This is command 2 !\n");
				break;
			}
		case 3 :{
				printk("This is command 3 !\n");
				break;
			}
		default :{
				 printk("There is no such command !\n");
				 return -1;
			 }
	}
	printk("The function of my_ioctl has been called %d",cmd);
	return 0;
}

struct file_operations fops = {
		open  :    my_open,			  
		release:	my_release,	   
		read:	my_read,		
		write:	my_write,		
		unlocked_ioctl:	my_ioctl,		
		poll:	my_poll,		
 
};

int my_init(void)
{
	/*注册设备*/
	int res = register_chrdev(DEV_MAJOR,DEV_NAME,&fops);
	if(res < 0){
		printk("My device register failed !\n");
		return res;
	}
	if(res > 0) major = res;
	printk("My device register success !, major = %d name = %s\n", major,DEV_NAME);	
	return 0;
}

int my_cleanup(void)
{
	/*卸载设备*/
	unregister_chrdev(major,DEV_NAME);
	printk("My device release success !\n");
	return 0;
}

module_init(my_init);
module_exit(my_cleanup);
MODULE_LICENSE("GPL");//为了避免”no license”警告

