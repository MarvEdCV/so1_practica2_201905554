//#include <linux/module.h>
// para usar KERN_INFO
//#include <linux/kernel.h>

//Header para los macros module_init y module_exit
//#include <linux/init.h>
//Header necesario porque se usara proc_fs
//#include <linux/proc_fs.h>
// for copy_from_user 
//#include <asm/uaccess.h>	
//Header para usar la lib seq_file y manejar el archivo en /proc*/
//#include <linux/seq_file.h>

//libreria para cpu
//#include <linux/sched.h>

//#include <linux/sched/signal.h>

#include <linux/module.h> 

#include <linux/kernel.h>

#include <linux/init.h>

#include <linux/list.h>
#include <linux/types.h>
#include <linux/slab.h>
#include <linux/sched.h>
#include <linux/string.h>
#include <linux/fs.h>
#include <linux/seq_file.h>
#include <linux/proc_fs.h>
#include <asm/uaccess.h> 
#include <linux/hugetlb.h>
#include <linux/sched/signal.h>
#include <linux/sched.h>

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Creación de modulo de CPU en Linux, Laboratio Sistemas Operativos 1");
MODULE_AUTHOR("Marvin Eduardo Catalán Véliz");

struct task_struct * cpu;
struct task_struct * child;
struct list_head * lstProcess;


//Funcion que se ejecutara cada vez que se lea el archivo con el comando CAT
static int escribir_archivo(struct seq_file *archivo, void *v)
{   
    int numeropadres = 0;
    int numerohijos = 0;

    seq_printf(archivo,"{\n\"procesos\":\n");
    for_each_process(cpu){
        numeropadres++;
        seq_printf(archivo,"\"pid\":%d\n\"nombre\":%s\n\"usuario\":%d\n\"estado\":%d\n\"hijo\":0\n",cpu->pid,cpu->comm,cpu->cred->uid.val,cpu->__state);
        list_for_each(lstProcess, &(cpu->children)){
            numerohijos++;
            child = list_entry(lstProcess, struct task_struct, sibling);
            seq_printf(archivo,"\"pid\":%d\n\"nombre\":%s\n\"usuario\":%d\n\"estado\":%d\n\"hijo\":1\n",child->pid,child->comm,child->cred->uid.val,child->__state,cpu->pid);
        }
        seq_printf(archivo,"}");
    }
    return 0;
}

//Funcion que se ejecutara cada vez que se lea el archivo con el comando CAT
static int al_abrir(struct inode *inode, struct file *file)
{
    return single_open(file, escribir_archivo, NULL);
}

//Si el kernel es 5.6 o mayor se usa la estructura proc_ops
static struct proc_ops operaciones =
{
    .proc_open = al_abrir,
    .proc_read = seq_read
};

//Funcion a ejecuta al insertar el modulo en el kernel con insmod
static int _insert(void)
{
    proc_create("cpu_201905554", 0, NULL, &operaciones);
    printk(KERN_INFO "MARVIN EDUARDO CATALAN VELIZ\n");
    return 0;
}

//Funcion a ejecuta al remover el modulo del kernel con rmmod
static void _remove(void)
{
    remove_proc_entry("cpu_201905554", NULL);
    printk(KERN_INFO "SEGUNDO SEMESTRE 2022\n");
}

module_init(_insert);
module_exit(_remove);