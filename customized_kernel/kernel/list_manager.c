//
// Created by omer.reuveni on 6/2/2021.
//

#include <linux/list_manager.h>

//// "global" list properties
int files_paths_count;
int privileged_procs_count;
int was_initialized = 0;
list_t file_paths_list_head;

void init_list() {
    if (was_initialized == 0)
    {
        printk("init - list initializing\n");
        printk("init - run #\n");
        INIT_LIST_HEAD(&file_paths_list_head);
        privileged_procs_count = 0;
        files_paths_count = 0;
        was_initialized = 1;
    }
}


// free the list and all path entries
void destroy_list() {
    init_list();
    if (was_initialized == 0)
    {
        // list wasn't initialized yet, nothing to do
        return;
    }
    list_t *pos;
    list_t *temp;
    list_for_each_safe(pos, temp, &file_paths_list_head)
    {
        Path_node_p
        a_node = list_entry(pos,
        struct path_node, list_pointer);  // returns pointer to our struct
        list_del(pos); // delete list_t object
        kfree(a_node);  // free what we dynamically allocated
    }
    // destroy list ended
    printk("DESTROY: list destroyed\n");

}


// if change is 1 or -1 it's changing the counter.
// if it's 0- it just returns the current count.
int set_files_paths_count(int change) {
    files_paths_count += change;
    return files_paths_count;
}

// if change is 1 or -1 it's changing the counter.
// if it's 0- it just returns the current count.
int set_privileged_procs_count(int change) {
    privileged_procs_count += change;
    return privileged_procs_count;
}

int check_list_for_path(const char *pathName) {
    if (was_initialized == 0)
    {
        printk("SEARCH: list is empty\n");
        return 0;
    }
    list_t *pos;
    list_for_each(pos, &file_paths_list_head)
    {
        //Path_node_p itt= list_entry(pos,struct path_node, file_paths_list_head);
        Path_node_p
        a_node = list_entry(pos,
        struct path_node, list_pointer);  // returns pointer to our struct



        if (!strcmp(a_node->file_path, pathName))
        {
            printk("SEARCH: file is blocked \n");
            return 1;
        }
    }
    return 0;
}



///**
// * finds the "oldest" process p_jiffies wise waiting in the  PRIVILEGED_PRIO queue
// * @param task_que
// * @return
// */
//task_t* check_queue_for_senior_process(list_t task_que) {
//    printk("GET SENIOR: ENTERED\n");
//
//    // if the queue isn't empty
//    // scan the line for the oldest process waiting.
//
//    struct task_struct *kthreadd_task;
//    struct list_head *list;
//
//    /* Find kthreadd task by PID */
//    kthreadd_task = pid_task(find_vpid(KTHREADD_PID), PIDTYPE_PID);
//    pr_debug("Process name: %s\n", kthreadd_task->comm);
//
//    /* Iterate over all children of kthreadd_task */
//    list_for_each(list, &kthreadd_task->children) {
//        struct task_struct *task;
//
//        /* Get next child */
//        task = list_entry(list, struct task_struct, sibling);
//        pr_debug("  Child name: %s\n", task->comm);
//    }
//
//    return 0;
//}





//
//// Assuming permission checked before. Make sure check permissions before
//int proc_upgrade_queue(pid_t proc_pid){
//    printk("UPGRADE PROCESS: ENTERED\n");
//
//}