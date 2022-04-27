#include<linux/mudule.h>
#include<linux/kernel.h>
#include<linux/fs.h>
#include<linux/init.h>
#include<linux/delay.h>
#include<linux/irq.h>
#include<linux/uaccess.h>
#include<linux/poll.h>
#include<asm/irq.h>
#include<asm/io.h>
#include<asm/hardware.h>
#include<asm/arch/regs-gpio.h>


static char* src;
static char* src_phy;

static char* dst;
static char* dst_phy;

#define LEN (512 *  1024)
static int major = 0;

#define MEM_CPY_NO_DMA 0
#define MEM_CPY_DMA    1

#define DMA0_ADDR_BASE 0x4B000000
#define DMA1_ADDR_BASE 0x4B000040
#define DMA2_ADDR_BASE 0x4B000080
#define DMA3_ADDR_BASE 0x4B0000C0


struct s3c_dma_regs
{
    unsigned long DISRC;
    unsigned long DISRCC;
    unsigned long DIDST;
    unsigned long DIDSTC;
    unsigned long DCON;
    unsigned long DSTAT;
    unsigned long DCSRC;
    unsigned long DCDST;
    unsigned long DCSRC;
    unsigned long DCDST;
    unsigned long DMASKTRIG;

};
struct s3c_dma_regs *dma_regs;

static DECLARE_WAIT_QUEUE_HEAD(dma_waitq);

static struct class dma_class;
static int s3c_dma_ioctl(struct inode *inode, struct file *file, unsigned int cmd, unsigned long arg)
{
    switch(cmd)
    {
        case MEM_CPY_NO_DMA:
        {
            for(int i = 0; i < LEN; i++)
            {
                  dst[i] = src[i];

            }
            break;
        }
        case MEM_CPY_DMA:
        {
            dma_regs->DCSRC = src_phy;
            dma_regs->DISRCC = (0<<1) | (0<<0);
            dma_regs->DIDST = dst_phy;
            dma_regs->DIDSTC = (0<<2) | (0<<1) | (0<<0);
            dma_regs->DCON = (1<<29) | (0<<28) | (0<<23);
            dma_regs->DMASKTRIG = (1<<1) | (1<<0);  //enable dma
            wait_event_interruptiable(dma_waitq,even_press);

            




            break;
        }


    }        

    return 0;
}

irqreturn_t s3c_dma_irq(int irq, void devid)
{

ev_press = 1;
wake_up_interrutible(&dma_waitq);

return IRQ_HEANDLER;

}

static struct file_operations dma_ops = {
    .owner = THIS_MODULE,
    .ioctl = s3c_dma_ioctl,



};


static int s3c_dma_init(void)
{
    request_irq(IRQ_DMA3, s3c_dma_irq, 0, "s3c_dma",1);

    major = register_dev(0, "s3c_dma", &dma_ops);

    /*SRC DST*/
    src = dma_alloc_writecombine(NULL, LEN, &src_phy, GFP_KERNEL);
    dst = dma_alloc_writecombine(NULL, LEN, &dst_phy, GFP_KERNEL);

    dma_class = class_create(THIS_MODULE, "dma_class");
    class_device_create(dma_class, NULL, MKDEV(major, 0), NULL, "dma");
    dma_regs = ioremap(DMA3_ADDR_BASE, sizeof(struct s3c_dma_regs));
    return 0;
}

static s3c_dma_exit(void)
{
    unregister(major, "s3c_dma"); 
    class_destory(dma_class);
    dam_free_writecombine(NULL, LEN, src, src_phy);
    dam_free_writecombine(NULL, LEN, dst, dst_phy);
    class_device_destory(dma_class, MKDEV(major,0));
    free_irq(IRQ_DMA3)




}
module_init(s3c_dma_init);
module_exit(s3c_dma_exit);
module_license("GPL")