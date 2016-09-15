#include "lru-cache.h"

node *my_head = NULL;
int *num_of_elements = NULL;
pthread_mutex_t *mtx;


void *get_cache() {

    node *ptr = NULL;
    int shm_id, num = 0;
    key_t key_node, key_num_element, key_mtx;

    key_num_element = ftok(".", 'a');
    key_node = ftok(".", 'b');
    key_mtx = ftok(".", 'c');

    if ((shm_id = shmget(key_node, sizeof(node) * MAX_SHM_ID_NUM, IPC_CREAT | 0666)) < 0) {
        perror("shmget error.");
        printf("errno= %d EINVAL=%d \n ", errno, EINVAL);
        return NULL;
    }

    if ((my_head = shmat(shm_id, NULL, 0)) == (void *) -1) {
        perror("shmat error");
        //exit(1);
        return NULL;
    }


    /* count how many element are in shared memory */
    ptr = my_head;
    while ((ptr = ptr->next) != NULL)
        num++;


    if ((shm_id = shmget(key_num_element, sizeof(int), IPC_CREAT | 0666)) < 0) {
        perror("shmget error.");
        printf("errno= %d EINVAL=%d \n ", errno, EINVAL);
        return NULL;
    }

    if ((num_of_elements = shmat(shm_id, NULL, 0)) == (void *) -1) {
        perror("shmat error");
        return NULL;
    }

    if ((shm_id = shmget(key_mtx, sizeof(pthread_mutex_t), IPC_CREAT | 0666)) < 0) {
        perror("shmget error.");
        printf("errno= %d EINVAL=%d \n ", errno, EINVAL);
        return NULL;
    }

    if ((mtx = shmat(shm_id, NULL, 0)) == (void *) -1) {
        perror("shmat error");
        return NULL;
    }

    if (pthread_mutex_init(mtx, NULL) != 0) {
        fprintf(stderr, "Error in pthread_mutex_init()\n");
        return NULL;
    }


    (*num_of_elements) = num;

    return my_head;
}

node *get_last() {

    struct node *ptr;
    for (ptr = my_head; ptr->next != NULL; ptr = ptr->next);

    return ptr;

}

/*
 * fail if get_cache is not called before
 */
void put_in_cache(converted_image *img) {

    node *ptr, *node;

    if (pthread_mutex_lock(mtx) != 0) {
        perror("pthread_mutex_lock");
        pthread_exit(NULL);
    }

    if ((*num_of_elements) < MAX_SHM_ID_NUM) {

        node = my_head + (*num_of_elements);

        /* set node's field */
        node->img = *img;

        node->prev = NULL;

        if ((*num_of_elements) == 0) {
            node->next = NULL;          /* first node */
        } else
            node->next = my_head;

        /* put it on head */
        my_head->prev = node;
        my_head = node;

        (*num_of_elements)++;

    } else if ((*num_of_elements) == MAX_SHM_ID_NUM) {

        /* remove least used image: it is on tail of linked list */
        ptr = get_last();
        if (unlink(ptr->img.temp_file) == -1) {
            fprintf(stderr, "error in unlink");
            pthread_exit(NULL);
        }

        ptr->prev->next = NULL;     /* last but one become the last */
        my_head->prev = ptr;        /* the first become the second */
        ptr->next = my_head;        /* the last become the first */
        my_head = ptr;              /* make my_head point to the first */


        /* set field of new node */
        my_head->img = *img;
    }

    if (pthread_mutex_unlock(mtx) != 0) {
        perror("pthread_mutex_unlock");
        pthread_exit(NULL);
    }
}


void move_to_head(node *pnode) {

    node *ptr;

    /* node is already in the head */
    if (pnode == NULL) {
        printf("null");
        return;
    }

    ptr = pnode->next;            /* node to move */
    pnode->next = ptr->next;

    /* check if it is the last */
    if (ptr->next != NULL)
        ptr->next->prev = pnode;

    ptr->next = my_head;
    my_head->prev = ptr;
    ptr->prev = NULL;

    my_head = ptr;
}

char *find_in_cache(converted_image *img) {

    node *ptr = NULL;

    if (pthread_mutex_lock(mtx) != 0) {
        perror("pthread_mutex_lock");
        pthread_exit(NULL);
    }

    if (strcmp(my_head->img.name, img->name) == 0 && my_head->img.quality_factor == img->quality_factor) {

        if (pthread_mutex_unlock(mtx) != 0) {
            perror("pthread_mutex_unlock");
            pthread_exit(NULL);
        }

        return my_head->img.temp_file;
    }


    for (ptr = my_head; ptr->next != NULL; ptr = ptr->next) {

        if (strcmp(ptr->img.name, img->name) == 0 && ptr->img.quality_factor == img->quality_factor) {
            /* found in cache */
            move_to_head(ptr->prev);

            if (pthread_mutex_unlock(mtx) != 0) {
                perror("pthread_mutex_unlock");
                pthread_exit(NULL);
            }

            return ptr->img.name;
        }
    }

    if (pthread_mutex_unlock(mtx) != 0) {
        perror("pthread_mutex_unlock");
        pthread_exit(NULL);
    }

    return NULL;
}