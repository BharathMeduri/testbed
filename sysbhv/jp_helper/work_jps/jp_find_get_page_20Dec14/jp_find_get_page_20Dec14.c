/*
 * Jprobes helper framework.
* jp_find_get_page_20Dec14.c 
 * This file is (a copy of) a simple "template" for writing your own JProbes.
 *
 * The basic idea:
 * *Copy* this C source file as is, calling it by a different name of course, 
 * and then modify it as instructed below to attach your jprobe(s).
 *
 * ___The good news___ : this work is automated by the helper script jprobeit.sh .
 * It copies the file appropriately (pathname format: 
 *   work_jps/jp_<func>_<date>/jp_<func>_<date>.c
 *
 * ___ The limitation___ : it (currently) cannot modify the jprobe'd function 
 * handler's signature to exactly match that of the function being jprobed.
 *
 * For more information on theory of operation of jprobes, see
 * Documentation/kprobes.txt
 *
 * Original Author: Kaiwan N Billimoria
 * <kaiwan -at- kaiwantech -dot- com>
 * [L]GPL.
 */
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/kprobes.h>
#include <linux/interrupt.h>
#include "convenient.h"
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

//#include <linux/whatever.h>

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#define MYNAME "jp_find_get_page_20Dec14"

/*
 * Jumper probe template for find_get_page
 *
 * THE THING TO DO:
 * Replace the function signature below such that it exactly matches the 
 * function being jprobed.
 * 
 * Mirror principle enables access to arguments of the probed routine
 * from the probe handler.
 */
//static <ret type> jp_find_get_page( /* ... params ... */ )  //%%%
struct page *jp_find_get_page(struct address_space *mapping, pgoff_t offset)
{
	//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	// Your Jprobe handler code goes here...
	
	PRINT_CTX();
	
	/* Get file pathname using dentry w/ dentry_path_raw();
	 How to get dentry ptr given the inode ??
	 It's not straightforward apparently: see
	 http://stackoverflow.com/questions/8556461/how-to-obtain-a-pathname-or-dentry-or-struct-file-from-a-given-inode
	 */

	if (mapping->host) {
		//printk_ratelimited("host inode : "
		MSG("host inode : "
		"inode # : %ld  "
		"size : %lld\n"
				, mapping->host->i_ino
				, mapping->host->i_size
				);
	}
#if 0
	dump_stack();  // very, *very* verbose; and what's worse, our own prev printks get pretty much subsumed! :( WHY??
#endif
	//show_stack(NULL, NULL); // not exported :(
	//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

	/* Always end with a call to jprobe_return(). */
	jprobe_return(); 
	return ; //%%%
}


static struct jprobe jps_find_get_page = {
	.entry			= jp_find_get_page,
	.kp = {
		.symbol_name	= "find_get_page",
	},
};

static int __init jprobe_init(void)
{
	int ret;

	ret = register_jprobe(&jps_find_get_page);
	if (ret < 0) {
		printk(KERN_INFO "register_jprobe failed, returned %d\n"
		"Check: is the to-be-probed function invalid, static, "
		"inline or attribute-marked '__kprobes' ?\n", ret);
		return ret;
	}
	printk(KERN_INFO "%s: Jprobe(s) registered.\n", MYNAME);
	return 0;
}

static void __exit jprobe_exit(void)
{
	unregister_jprobe(&jps_find_get_page);
	printk(KERN_INFO "%s: Jprobe(s) unregistered\n", MYNAME);
}

module_init(jprobe_init);
module_exit(jprobe_exit);
MODULE_LICENSE("GPL");
