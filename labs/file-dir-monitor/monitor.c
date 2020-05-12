#include <stdio.h>
#include <stdlib.h>
#include "logger.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <ftw.h>
#include <sys/inotify.h>
#include <string.h>

#define MSKSLEN 3 // MASKS array length
#define FDMAX 100 // max length of file descriptors
#define NAMEMAX 1000 // maximum name of a filename
#define DIRMAX 100
#define BUFLEN (10 *(sizeof(struct inotify_event) + NAMEMAX + 1)) // len for buffer

struct wds {
    char name[NAMEMAX];
    int wd;
};

struct wds wd_structs[(10 * (sizeof(struct wds) + NAMEMAX + 1))];
int wd_index = 0;
int wd_size = 0;

int inotifyfd;
// const uint32_t masks[MSKSLEN] = {IN_CREATE, IN_DELETE, IN_MODIFY};

char * findNameByWd(int wd) {
    int i;
    for (i = 0; i < wd_size; i++) {
        if (wd_structs[i].wd == wd) {
            return wd_structs[i].name;
        }
    }

    return NULL;
}

void newStructWd(const char * name, int wd) {
    struct wds wd_struct;
    strcpy(wd_struct.name, name);
    wd_struct.wd = wd;
    wd_structs[wd_index++] = wd_struct;
    wd_size++;
}


static int add_watchers(
    const char *fpath, 
    const struct stat *sb, 
    int typeflag) {
    
    // Every dir entry needs to have its watchers defined with the masks
    // infof("typeflag: %d == %d", typeflag, FTW_D);
    if (typeflag != FTW_D) {
        return 0;
    }
    int wd;

    if ((wd = inotify_add_watch(inotifyfd, fpath, IN_CREATE | IN_DELETE | IN_MODIFY)) == -1) {
        errorf("fail to add watch for file %s at flag num (%zu)", fpath, IN_CREATE);
        return -1;
    }
    infof("succesfully added wd: %d in %s mask %zu", wd, fpath, IN_CREATE | IN_DELETE | IN_MODIFY);

    newStructWd(fpath, wd);
    
    return 0;
}

static void displayInotifyEvent(struct inotify_event *i) {

    infof("wd: %d detected event: %s in file %s", i->wd, 
        i->mask & IN_CREATE ? "IN CREATE" : i->mask & IN_DELETE ? "IN DELETE" :
        i->mask & IN_MODIFY ? "IN MODIFY" : "???", i->name);   

    if (i->mask & IN_CREATE) {
        
        if (ftw(findNameByWd(i->wd), add_watchers, FDMAX) == -1) {
            errorf("there was an error with the ftw function, exiting now");
            exit(1);
        }
    }
}

int main(int argc, char **argv){
    // Place your magic here
    const char *pathname;
    char buf[BUFLEN];
    ssize_t numRead;
    char *p;
    struct inotify_event *event;

    if (argc < 2) {
        errorf("usage: ./monitor <path>");
        exit(1);
    }

    infof("Initializing implementation of inotify");
    pathname = argv[1];


    inotifyfd = inotify_init();
    if (inotifyfd == -1) {
        errorf("inotify_init failure, exiting now");
        exit(1);
    }

    if (ftw(pathname, add_watchers, FDMAX) == -1) {
        errorf("there was an error with the ftw function, exiting now");
        exit(1);
    }

    infof("watchers successfully added for all files and subdirectories of %s", pathname);

    for (;;) {                                  /* Read events forever */
        if ((numRead = read(inotifyfd, buf, BUFLEN)) == 0) {
            errorf("numread returned 0, exiting");
            exit(1);
        }

        if (numRead == -1) {
            errorf("error reading numread, exiting");
            exit(1);
        }

        /* Process all of the events in buffer returned by read() */

        for (p = buf; p < buf + numRead; ) {
            event = (struct inotify_event *) p;
            displayInotifyEvent(event);

            p += sizeof(struct inotify_event) + event->len;
        }
    }

    return 0;
}
