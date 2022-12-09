#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/debugfs.h>
#include <linux/fs.h>
#include <linux/netdevice.h>
#include <linux/namei.h>

#define BUFFER_SIZE 1024

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Vladislav Kharin");
MODULE_DESCRIPTION("Lab");
MODULE_VERSION("1.0");

static struct dentry *kmod_dir;
static struct dentry *args_file;
static void print_net_device(struct seq_file *file);
static int print_struct(struct seq_file *file, void *data);
static void print_inode(struct seq_file *file, struct inode *inode);

struct net_device *net_device;
struct inode *inode;

static struct inode* get_inode(char * path_name) {
    static struct path path;
    kern_path(path_name, LOOKUP_FOLLOW, &path);
    return path.dentry->d_inode;
}

static ssize_t kmod_write(struct file *file, const char __user *buffer, size_t length, loff_t *ptr_offset) {
    printk(KERN_INFO "Getting arguments...\n");
    char user_data[BUFFER_SIZE];
    copy_from_user(user_data, buffer, length);
    char path[BUFFER_SIZE];
    sscanf(user_data, "path: %s", &path);
    inode = get_inode(path);
    single_open(file, print_struct, NULL);
    return strlen(user_data);
}

static struct file_operations fops = {
        .read = seq_read,
        .write = kmod_write,
};

static int __init mod_init(void) {
    printk(KERN_INFO "kmod inited.\n");
    kmod_dir = debugfs_create_dir("kharin", NULL);
    args_file = debugfs_create_file("lab2", 0644, kmod_dir, NULL, &fops);
    return 0;
}

static void __exit mod_exit(void) {
    debugfs_remove_recursive(kmod_dir);
    printk(KERN_INFO "kmod unloaded\n");
}

static int print_struct(struct seq_file *file, void *data) {
    print_inode(file, inode);
    print_net_device(file);
    return 0;
}

static void print_inode(struct seq_file *file, struct inode *inode) {
    // char *path_name = "/home/vladislav/Downloads/lab2os/src/dfsn/Makefile";
    if (inode == NULL) {
        seq_printf(file, "Inode was not find with this path\n");
    } else {
        seq_printf(file, "Inode number is %lu\n", inode->i_ino);
    }
}

static void print_net_device(struct seq_file *file) {
    net_device = first_net_device(&init_net);

    while (net_device) {
        char *name = net_device -> name;
        seq_printf(file, "net_device: %s\n", net_device -> name);
        net_device = next_net_device(net_device);
    }
}

module_init(mod_init);
module_exit(mod_exit);
