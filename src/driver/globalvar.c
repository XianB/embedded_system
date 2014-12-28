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
int globalvar_read(struct file *, char *, size_t, loff_t *);
int globalvar_write(struct file *, char *, size_t, loff_t *);
int globalvar_ioctl(struct file * filp, unsigned int cmd, unsigned long args);

int dev_major = 50;
int dev_minor = 0;

struct file_operations globalvar_fops = {
	owner: THIS_MODULE,
		   open:globalvar_open,
		   release:globalvar_release,
		   read:globalvar_read,
		   write:globalvar_write,
		   unlocked_ioctl:globalvar_ioctl,

};


/*字符驱动数据结构*/
struct globalvar_dev{
	int global_var;
	struct cdev cdev;
};


/*声明一个该设备*/
struct globalvar_dev *my_dev;

static void __exit globalvar_exit(void)
{
	/*卸载该设备*/
	dev_t devno = MKDEV(dev_major, dev_minor);
	cdev_del(&my_dev->cdev);
	/*释放该设备占用的空间*/
	kfree(my_dev);
	/*清除设备注册信息*/
	unregister_chrdev_region(devno, 1);
	printk("globalvar_exit called.\r\n");
	return;
}

/*测试用的数据*/
static int test_var = 0xFF;
/*参数*/
module_param(test_var, int, 0644);

static int __init globalvar_init(void)
{
	int ret, err;
	dev_t devno = MKDEV(dev_major, dev_minor);
	/*注册一个设备*/
	ret = alloc_chrdev_region(&devno, dev_minor, 1, "globalvar");
	dev_major = MAJOR(devno);

	if (ret < 0) {
		printk("register failed.\r\n");
		globalvar_exit();
		return ret;
	} else {
		printk("globalvar init succeed\r\n");
	}

	/*给设备分配一个空间*/
	my_dev = kmalloc(sizeof(struct globalvar_dev), GFP_KERNEL);
	if (my_dev == NULL) {
		printk("kmalloc failed.\r\n");
	} else {
		printk("globalvar kmalloc succeed.\r\n");
		my_dev->global_var = 12;
		cdev_init(&my_dev->cdev, &globalvar_fops);
		my_dev->cdev.owner = THIS_MODULE;
		err = cdev_add(&my_dev->cdev, devno, 1);
		if (err < 0) {
			printk("add dev failed.\r\n");
		}
		printk("globalvar cdev_add succeed.\r\n");
	}

	printk("test_var = 0x%x.\r\n", test_var);
	return ret;
}

int globalvar_open(struct inode *inode, struct file * filp)
{
	struct globalvar_dev *dev;
	/*container_of获得某结构中某成员的入口地址
	#define container_of(ptr, type, member) ({const typeof( ((type *)0)->member ) *__mptr = (ptr); (type *)( (char *)__mptr - offsetof(type,member) );})
	即由结构体成员指针得到整个结构的指针
	inode->i_cdev即结构体成员的指针
	struct globalvar_dev即欲得到地址的结构的类型
	cdev即该成员的声明
	 */
	dev = container_of(inode->i_cdev, struct globalvar_dev, cdev);
	/*private_data指向自定义的描述设备的结构体的指针
	 在这里的含义是使filp的private指向该设备文件的设备的指针
	 open函数的关键即在此,即赋值private成员,使得设备和文件绑定,以后就不用更改了
	 inode是"文件"
	 file代表的是设备
	 */
	filp->private_data = dev;
	printk("globalvar open called\r\n");
	return 0;
}

int globalvar_release(struct inode * inode, struct file *filp)
{
	printk("globalvar release called\r\n");
	return 0;
}

int globalvar_read(struct file * filp, char *buf, size_t len, loff_t *off)
{
	/*从用内核空间到户空间读数据,由前面可知private_data指向的是该设备自定义设备结构的指针*/
	struct globalvar_dev *dev = filp->private_data;
	if(copy_to_user((void *)buf, (const void*)&dev->global_var, sizeof(int)) < 0) {
		return -EFAULT;
	}

	printk("globalvar read called, global_var = 0x%x.\r\n", dev->global_var);

}

int globalvar_write(struct file *filp, char *buf, size_t len, loff_t *off)
{
	/*从用户空间到内核空间复制数据*/
	struct globalvar_dev *dev = filp->private_data;
	if(copy_from_user((void *)&dev->global_var, (const void *)buf, sizeof(int)) < 0) {
		return -EFAULT;
	}

	printk("globalvar write called\r\n");
	return sizeof(int);
}

int globalvar_ioctl(struct file * filp, unsigned int cmd, unsigned long args)
{
	printk("cmd = 0x%x\r\n", cmd);
	return 0;
}

module_init(globalvar_init);
module_exit(globalvar_exit);
