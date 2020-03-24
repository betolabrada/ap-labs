#include <stdio.h>  
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define REPORT_FILE "packages_report.txt"
#define INSTALLED "installed"
#define UPGRADED "upgraded"
#define REMOVED "removed"
#define ALPM "[ALPM]"
#define MYSIZ 100

// My helper struct of package to keep the data
typedef struct {
    char name[MYSIZ];
    char inst_date[MYSIZ];
    char upgd_date[MYSIZ]; /* the last date */
    char rmvd_date[MYSIZ];
    int upgrades;
} mypackage;

// Function declarations
void analizeLog(char *logFile, char *report);
int mygetchar(int filedesc);
int mygetline(char string[], int limit, int filedesc);
mypackage * getstructbyname(char * name, mypackage * begin, mypackage * end);
void writeReport(int fd, mypackage * packages, int size, int installed, int removed, int upgraded);

int main(int argc, char **argv) {

    if (argc < 2) {
	printf("Usage:./pacman-analizer.o pacman.log\n");
	return 1;
    }

    analizeLog(argv[1], REPORT_FILE);

    return 0;
}

void analizeLog(char *logFile, char *report) {
    printf("Generating Report from: [%s] log file\n", logFile);

    // Implement your solution here.

    // Opening logfile
    int fd_log = open(logFile, O_RDONLY);
    if (fd_log < 0) {
	    perror("Error opening logFile");
	    exit(1);
    }

    // Opening report file (for later)
    int fd_report = open(report, O_WRONLY | O_CREAT, 00700);   
    if (fd_report < 0) {
	    perror("Error opening Report");
	    exit(1);
    }

    // A buffer to get our lines
    char buffer[BUFSIZ];
    // char *tokbuf[TKSIZ];
    
    // Keep our found packages in a struct dynamic array
    mypackage *packages;

    // Size configuration
    int maxsize = MYSIZ;
    int size = 0;

    // Allocating an initial size of 100, when full, maxsize will be doubled
    packages = malloc(maxsize * sizeof(*packages));

    // Pointers to our array, current can create new struct or manipulate existing
    // endpkg keeps track of the last pkg of array
    mypackage *current = packages;
    mypackage *beginpkg = packages;
    mypackage *endpkg = packages;

    // Counters for our data
    int installed, upgraded, removed, total;
    installed = upgraded = removed = total = 0;

    // Strings to manipulate our buffer, (might delete later)
    char in_datetime[MYSIZ];
    char in_type[MYSIZ];
    char in_action[MYSIZ];
    char in_name[MYSIZ];


    while (mygetline(buffer, BUFSIZ, fd_log)) {

        // Inserting may fill our array, we need to reallocate and set pointers to new references 
        if (size == maxsize) {
            printf("------REALLOCATING at size = %d---------------\n\n", size);
            maxsize *= 2;
            packages = realloc(packages, maxsize * sizeof(*packages));
            beginpkg = packages;
            endpkg = packages + size;
        }

        // Get datetime
        strcpy(in_datetime, strtok(buffer, " "));
        strcat(in_datetime, " ");
        strcat(in_datetime, strtok(NULL, " "));
        
        // We are only interested in [ALPM] package types
        strcpy(in_type, strtok(NULL, " "));
        if (strcmp(in_type, ALPM) != 0) continue; 

        // Define our action (installed, upgraded or removed)
        strcpy(in_action, strtok(NULL, " "));
        if (strcmp(in_action, INSTALLED) != 0 && strcmp(in_action, UPGRADED) != 0
            && strcmp(in_action, REMOVED) != 0) continue;

        // Our package name
        strcpy(in_name, strtok(NULL, " "));

        // Find if package has been added before
        current = getstructbyname(in_name, beginpkg, endpkg);

        // Pointer current == endpkg means no package with name in_name was found, creating new pkg
        if (current == endpkg) {
            strcpy(current->name, "-");
            strcpy(current->inst_date, "-");
            strcpy(current->upgd_date, "-");
            strcpy(current->rmvd_date, "-");
            current->upgrades = 0;
        }

        // Find which action is performed in package
        if (strcmp(in_action, INSTALLED) == 0) {
            if (strcmp(current->inst_date, "-") != 0) { // Package is reinstalled
                printf("reinstalled %s\n", current->name);
                current->upgrades = 0;
                removed--; 
                upgraded--;
            }
            strcpy(current->inst_date, in_datetime);
        } else if (strcmp(in_action, UPGRADED) == 0) {
            strcpy(current->upgd_date, in_datetime);
            if (current->upgrades++ == 0) upgraded++;           
        } else if (strcmp(in_action, REMOVED) == 0) {
            strcpy(current->rmvd_date, in_datetime);
            removed++;
        } else {
            continue;
        }

        // Insert name into the package
        strcpy(current->name, in_name);

        // If a new pkg was added we need to move our end pointer
        char writbuf[BUFSIZ];
        if (current == endpkg) {
            endpkg++;
            size++;
        }

    }
    installed = size;     
    
    printf("Creating report\n");
    writeReport(fd_report, packages, size, installed, removed, upgraded);
    printf("Report is generated at: [%s]\n", report);


    free(packages);
    close(fd_log);


}

int mygetchar(int fd) {
    char c;
    
    return (read(fd, &c, 1)) ? (unsigned char) c : EOF;
}

int mygetline(char s[], int lim, int fd) {
    
    int i, n;

    for (i = 0; i < lim && (n = mygetchar(fd)) != EOF && n != '\n'; i++) {
        s[i] = n;
    }
    if (n == '\n') {
        s[i] = n;
        i++;
    }
    s[i] = '\0';
    return i;
}

mypackage * getstructbyname(char * name, mypackage * begin, mypackage * end) {

    // find our struct by name if exists
    // if not found, we just return the end pointer to create a new pkg
    mypackage * pc;

    for (pc = begin; pc != end; pc++) {
        if (strcmp(pc->name, name) == 0) {
            return pc;
        }
    }

    return end;

}

void writeReport(int fd, mypackage * pkgs, int sz, int instd, int rmvd, int upgd) {

    char buf[256];
    int n;

    n = sprintf(buf, "Pacman Packages Report\n");
    write(fd, buf, n);
    n = sprintf(buf, "----------------------\n");
    write(fd, buf, n);

    n = sprintf(buf, "%-24s : %-3d\n", "- Installed packages ", instd);
    write(fd, buf, n);
    n = sprintf(buf, "%-24s : %-3d\n", "- Removed packages ", rmvd);
    write(fd, buf, n);
    n = sprintf(buf, "%-24s : %-3d\n", "- Upgraded packages ", upgd);
    write(fd, buf, n);
    n = sprintf(buf, "%-24s : %-3d\n\n", "- Current installed ", instd - rmvd);
    write(fd, buf, n);

    n = sprintf(buf, "List of packages\n");
    write(fd, buf, n);
    n = sprintf(buf, "-----------------\n");
    write(fd, buf, n);

    mypackage * ppkgs;
    for (ppkgs = pkgs; ppkgs != pkgs + sz; ppkgs++) {
        n = sprintf(buf, "%-27s : %-40s\n", "- Package name ", ppkgs->name);
        write(fd, buf, n);
        n = sprintf(buf, "%-20s : %-40s\n", "\t- Last update date ", ppkgs->upgd_date);
        write(fd, buf, n);
        n = sprintf(buf, "%-20s : %-3d\n", "\t- How many updates ", ppkgs->upgrades);
        write(fd, buf, n);
        n = sprintf(buf, "%-20s : %-40s\n\n", "\t- Removal date ", ppkgs->rmvd_date);
        write(fd, buf, n);
    }

    close(fd);

}

