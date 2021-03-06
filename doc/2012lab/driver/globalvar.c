#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <asm/uaccess.h>
#include <linux/moduleparam.h>

MODULE_LICENSE("GPL");

int globalvar_open(struct inode * inode, struct file * filp);
int globalvar_release(struct inode *, struct file *);
int globalvar_read(struct file *, char *, size_t, loff_t *) ;
int globalvar_write(struct file *, char *, size_t, loff_t *) ;
int globalvar_ioctl( struct file * filp, unsigned int cmd, unsigned long args);


int dev_major = 50;
int dev_minor = 0;

struct file_operations globalvar_fops = 
{
	owner:THIS_MODULE,
      open:globalvar_open,
      release:globalvar_release,
      read:globalvar_read,
      write:globalvar_write,
      unlocked_ioctl:globalvar_ioctl,
};

struct globalvar_dev
{
	int global_var;
	struct cdev cdev;
};

struct globalvar_dev *my_dev;

static void __exit globalvar_exit(void)
{
	dev_t devno= MKDEV(dev_major, dev_minor);
	cdev_del(&my_dev->cdev);
	kfree(my_dev);
	unregister_chrdev_region(devno, 1);
	printk("globalvar_exit called.\r\n");
	return;
}
static int test_var = 0xFF;
module_param(test_var, int, 0644);
static int __init globalvar_init(void )
{
	int ret, err;
	dev_t devno = MKDEV(dev_major, dev_minor);
	ret = alloc_chrdev_region(&devno, dev_minor, 1, "globalvar");
	dev_major = MAJOR(devno);
	if (ret < 0)
	{
		printk("register failed.\r\n");
		globalvar_exit();
		return ret;
	}
	else
	{
		printk("globalvar init succeed\r\n");
	}
	my_dev = kmalloc(sizeof(struct globalvar_dev), GFP_KERNEL);
	if (my_dev == NULL)
	{
		printk("kmalloc failed.\r\n");
	}
	else
	{
		printk("globalvar kmalloc succeed.\r\n");
		my_dev->global_var = 0;
		cdev_init(&my_dev->cdev, &globalvar_fops);
		my_dev->cdev.owner = THIS_MODULE;
		err = cdev_add(&my_dev->cdev, devno, 1);
		if (err < 0)
		{
			printk("add dev failed.\r\n");	
		}
		printk("globalvar cdev_add succeed.\r\n");
	}
	printk("test_var = 0x%x.\r\n", test_var);
	return ret;
}

int globalvar_open(struct inode * inode, struct file * filp)
{
	struct globalvar_dev *dev;
	dev = container_of(inode->i_cdev, struct globalvar_dev, cdev);
	filp->private_data = dev;
	printk("globalvar open called\r\n");
	return 0;
}
int globalvar_release(struct inode * inode, struct file * filp)
{
	printk("globalvar release called\r\n");
	return 0;
}
int globalvar_read(struct file * filp, char * buf, size_t len, loff_t * off) 
{
	struct globalvar_dev *dev = filp->private_data;
	if (copy_to_user((void *)buf, (const void *)&dev->global_var, sizeof(int)) < 0 )
	{
		return -EFAULT;
	}
	printk("globalvar read called, global_var = 0x%x.\r\n", dev->global_var);
	return sizeof(int);
}
int globalvar_write(struct file * filp, char * buf, size_t len , loff_t * off) 
{
	struct globalvar_dev *dev = filp->private_data;
	if (copy_from_user((void *)&dev->global_var,  (const void *)(buf), sizeof(int)) < 0 )
	{
		return -EFAULT;
	}
	printk("globalvar write called\r\n");
	return sizeof(int);
}
int globalvar_ioctl( struct file * filp, unsigned int cmd, unsigned long args)
{
	printk("cmd = 0x%x\r\n", cmd);
	return 0;
}
module_init(globalvar_init);
module_exit(globalvar_exit);


















