#include <linux/moudule.h>
#include <linux/errno.h>
#include <linux/interrupt.h>
#include <linux/mm.h>
#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/timer.h>
#include <linux/genhd.h>
#include <linux/hdreg.h>
#include <linux/ioport.h>
#include <linux/init.h>
#include <linux/wait.h>
#include <linux/blkdev.h>
#include <linux/blkpg.h>
#include <linux/delay.h>
#include <linux/io.h>
#include <asm/system.h>
#include <asm/uaccess.h>
#include <asm/dma.h>

static struct gendisk *ramblock_disk;
static struct request_queue_t *ramblock_queue;
static DEFINE_SPINLOCK(xd_lock);
static int major;


static int ramblock_getgeo(struct block_device *bdev, struct hd_geometry *geo)
{
   geo->heads = 2;
   geo->cyclinders = 32;
   geo->sectors =  1024*1024/2/32/512;      /* capacity = heads * cylinder * 512*/

  return 0;
}

static struct block_device_operations ram_fops = {
   .owner = THIS_MODULE,
   .getgeo = ramblock_getgeo,

};

static void do_request_func(request_queue_t* q)
{
  struct request *req; 
  while((req = elv_next_request(q))!=NULL)
   {
     unsigned long offset = req->sector * 512;
     

     unsigned long len = req->current_nr_sectors * 512;
     
     if(rq_data_dir(req) == READ)
     {
       memcpy(req->buffer, ram_block_buff + offset, len);

     }
     else
     {

       memcpy(ram_block_buff, + offset req->buffer, len);
     }



     end_request(req, 1);

   }


}

static unsigned char* ram_block_buff;
static int ramblock_init(void)
{
    /*alloc gendisk structure*/
    ramblock_disk = alloc_disk(16);
    /*config*/
    ramdblock_queue = block_init_queue(do_request_func, &xd_lock);
    /*build a queue*/
    major = register_blkdev(0, "ramblock");
    ramblock_disk->major = major;
    ramblock_disk->first_minor = 0;
    ramblock_disk->name = "XXX";
    ramblock_disk->fops = &ram_fops;
    ramblock_disk->queue = ramdblock_queue;
    set_capacity(ramblock_disk, 1024*1024/512);
    /*set another configuration*/
    add_disk(ramblock_disk);

    /*hardware*/
    ram_block_buff = kzalloc(1024*1024, GFP_KERNEL);

  return 0;
}

static void ramblock_exit(void)
{
    unregister_blkdev(major, "ramblock_disk");
    del_gendisk(ramblock_disk);
    put_disk(ramblock_disk);
    blk_clean_queue(ramdblock_queue);

}

module_init(ramblock_init);
module_exit(ramblock_exit);
module_license("GPL");



