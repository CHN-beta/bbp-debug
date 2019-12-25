#include <linux/module.h>
#include <linux/version.h>
#include <linux/kmod.h>
#include <linux/kernel.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>
#include <linux/ip.h>
#include <linux/tcp.h>
#include <linux/netdevice.h>
#include <linux/slab.h>
#include <linux/types.h>
#include <linux/random.h>
#include <linux/moduleparam.h>
#include <linux/time.h>
#include <linux/mutex.h>

static struct nf_hook_ops nfho;
static DEFINE_MUTEX(lock);
unsigned* junk;

unsigned int hook_funcion(void *priv, struct sk_buff *skb, const struct nf_hook_state *state)
{
	int i, j;
	printk("try to lock.\n");
	mutex_lock(&lock);
	printk("locked.\n");
	printk("counting.\n");
	for(j = 0; j < 100; j++)
		for(i = 0; i < 1000; i++)
			junk[i]++;
	printk("count finish.\n");
	printk("try to unlock.\n");
	mutex_unlock(&lock);
	printk("unlocked.\n");
	return NF_ACCEPT;
}

static int __init hook_init(void)
{
	int ret;
	mutex_init(&lock);
	junk = kmalloc(1000 * sizeof(unsigned), GFP_KERNEL);

	nfho.hook = hook_funcion;
	nfho.pf = NFPROTO_IPV4;
	nfho.hooknum = NF_INET_POST_ROUTING;
	nfho.priority = NF_IP_PRI_RAW;

#if LINUX_VERSION_CODE >= KERNEL_VERSION(4,13,0)
    ret = nf_register_net_hook(&init_net, &nfho);
#else
    ret = nf_register_hook(&nfho);
#endif

	return 0;
}

static void __exit hook_exit(void)
{
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4,13,0)
    nf_unregister_net_hook(&init_net, &nfho);
#else
    nf_unregister_hook(&nfho);
#endif
	kfree(junk);
	mutex_destroy(&lock);
}

module_init(hook_init);
module_exit(hook_exit);