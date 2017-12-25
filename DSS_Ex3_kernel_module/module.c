#include <linux/module.h>
#include <linux/vmalloc.h>
#include <linux/blkdev.h>
#include <linux/hdreg.h>

static int major_num = 0; //Required to register and unregister device
static int logical_block_size = 512;
static int nsectors = 204800; //How big the drive is, equals 100mb

module_param(major_num, int, 0);
module_param(logical_block_size, int, 0);
module_param(nsectors, int, 0);

//Legacy size is 512b
#define KERNEL_SECTOR_SIZE 512

static struct request_queue *Queue;

//Internal representation of device
static struct ram_disk {
        unsigned long size;
        spinlock_t lock;
        u8 *data;
        struct gendisk *gd;
} Device;

//Read or write data to disk
static void transfer_data(struct ram_disk *dev, sector_t sector,
                unsigned long nsect, char *buffer, int write) {

        unsigned long offset = sector * logical_block_size;
        unsigned long nbytes = nsect * logical_block_size;

        if ((offset + nbytes) > dev->size) {
                printk (KERN_NOTICE "Not enought space on disk\n");
                return;
        }
        if (write)
                memcpy(dev->data + offset, buffer, nbytes);
        else
                memcpy(buffer, dev->data + offset, nbytes);
}

//Handle request
static void handle_request(struct request_queue *q) {
        struct request *req;

        req = blk_fetch_request(q);
        while (req != NULL) {

                if (req == NULL || (req->cmd_type != REQ_TYPE_FS)) {
                        printk (KERN_NOTICE "Wrong request\n");
                        __blk_end_request_all(req, -EIO);
                        continue;
                }

                transfer_data(&Device, blk_rq_pos(req), blk_rq_cur_sectors(req),
                                bio_data(req->bio), rq_data_dir(req));

                if ( ! __blk_end_request_cur(req, 0) ) {
                        req = blk_fetch_request(q);
                }
        }
}

//We need to create hd_geometry to use disk normally with system tools
int create_geo(struct block_device * block_device, struct hd_geometry * geo) {
        long size;

        //Some values from internet
        size = Device.size * (logical_block_size / KERNEL_SECTOR_SIZE);
        geo->cylinders = (size & ~0x3f) >> 6;
        geo->heads = 4;
        geo->sectors = 16;
        geo->start = 0;
        return 0;
}

static struct block_device_operations ops_struct = {
                .owner  = THIS_MODULE,
                .getgeo = create_geo
};


//Init module and create disk
static int __init_module(void) {

        //Allocate memory
        Device.size = nsectors * logical_block_size;
        spin_lock_init(&Device.lock);
        Device.data = vmalloc(Device.size);
        if (Device.data == NULL)
                return -ENOMEM;

        //Get request queue
        Queue = blk_init_queue(handle_request, &Device.lock);
        if (Queue == NULL) {
            vfree(Device.data);
            return -ENOMEM;
        }
        blk_queue_logical_block_size(Queue, logical_block_size);

        //Register device
        major_num = register_blkdev(major_num, "ramdisk");
        if (major_num < 0) {
                printk(KERN_WARNING "Can't register device\n");
                vfree(Device.data);
                return -ENOMEM;
        }

        //Create gendisk structure
        Device.gd = alloc_disk(16);
        if (!Device.gd)
                unregister_blkdev(major_num, "ramdisk");

        Device.gd->major = major_num;
        Device.gd->first_minor = 0;
        Device.gd->fops = &ops_struct;
        Device.gd->private_data = &Device;
        strcpy(Device.gd->disk_name, "ramdisk0");
        set_capacity(Device.gd, nsectors);
        Device.gd->queue = Queue;
        add_disk(Device.gd);

        return 0;
}

//Remove module and disk
static void __cleanup_module(void)
{
        del_gendisk(Device.gd);
        put_disk(Device.gd);
        unregister_blkdev(major_num, "ramdisk");
        blk_cleanup_queue(Queue);
        vfree(Device.data);
}

module_init(__init_module);
module_exit(__cleanup_module);


MODULE_AUTHOR("Max Kuzmin");
MODULE_LICENSE("GPL");
