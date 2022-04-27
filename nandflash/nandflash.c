#include <linux/module.h>
#include <linux/types.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/ioport.h>
#include <linux/platform_device.h>
#include <linux/delay.h>
#include <linux/err.h>
#include <linux/slab.h>
#include <linux/clk.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/nand.h>
#include <linux/mtd/nand_ecc.h>
#include <linux/mtd/partitions.h>
#include <asm/io.h>
#include <asm/arch/regs-nand.h>
#include <asm/arch/nand.h>

static struct nand_chip* s3c_nand;
static struct mtd_info *s3c_mtd;
static struct nand_regs
{
   unsigned long NFCONF;
   unsigned long NFCONT;
   unsigned long NFCMD;
   unsigned long NFDATA;
   unsigned long NFECCD0;
   unsigned long NFECCD1;
   unsigned long NFECCD;
   unsigned long NFSTAT;
   unsigned long NFESTAT0;
   unsigned long NFESTAT1;
   unsigned long NFMECC0;
   unsigned long NFMECC1;
   unsigned long NFSECC;
   unsigned long NFSBLK;
   unsigned long NFEBLK;


};

static struct nand_regs *nand_regs;

static void nand_select_chip(struct mtd_info *mtd, int chipnr)
{
   if(chipnr == -1)
   {

       nand_regs->NFCONT |= (1<<1);
   }
   else
   {
       nand_regs->NFCONT &= ~(1<<1);

   }


}


static void nand_cmd_ctrl(struct mtd_info *mtd, int dat, unsigned int ctrl)
{
    if(ctrl & NAND_CLE)
    {
      nand_regs->NFCMD = dat;
    }
    else
    {
      nand_regs->NFADDR = dat;
   

    }
   

}


int dev_ready(struct mtd_info *mtd)
{



return nand_regs->nfstat & (1<<0);
}

static int s3c_nand_init(void)
{
/*alloc nand chip structure*/
s3c_nand = kzalloc(sizeof(struct nand_chip), GFP_KERNEL);
nand_regs = ioremap(0x4E000000,sizeof(struct nand_regs));


/*configurate*/
s3c_nand->select_chip = nand_select_chip;
s3c_nand->cmd_ctrl = nand_cmd_ctrl;
s3c_nand->IO_ADDR_R = &nand_regs->NFDATA;/*need virtual address*/
s3c_nand->IO_ADDR_W = &nand_regs->NFDATA;
s3c_nand->dev_ready = dev_ready;
s3c_nand->ecc.mode = NAND_ECC_SOFT;


#define TACLS  0
#define TWRPH0 1
#define TWRPH1 0
    nand_regs->nfconf = (TACLS << 12) | (TWRPH0 << 8) | (TWRPH1W << 4);
    nand_regs->NFCONT = (1<<1) | (1<< 0);
    struct clk *clk;
    clk = clk_get(NULL, "nand");
    clk_enable(clk);


/*4. add_mtd_partitions*/
s3c_mtd = kzalloc(sizeof(struct mtd_info, GFP_KERNEL));
mtd->owner = THIS_MODULE;
mtd->priv = s3c_nand;


nand_scan(s3c_mtd, 1)






return 0;
}

static void s3c_nand_exit(void)
{
  kfree(s3c_mtd);
  iounmap(nand_regs);
  kfree(s3c_nand);
}


module_init(s3c_nand_init);
module_exit(s3c_nand_exit);
module_license("GPL");
