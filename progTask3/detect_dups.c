// add any other includes in the detetc_dups.h file
#include "detect_dups.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>

// define any other global variable you may need over here

// open ssl, this will be used to get the hash of the file
EVP_MD_CTX *mdctx;
const EVP_MD *EVP_md5(); // use md5 hash!!


typedef struct {
    char *path;
    char *Inode;
} fileData;
typedef struct {
    fileData *data;
    size_t size;
    size_t capacity;
} FileDataArray;

void initFileDataArray(FileDataArray *arr) {
    arr->size = 0;
    arr->capacity = 4;
    arr->data = malloc(arr->capacity * sizeof(fileData));
    if (!arr->data) {
        perror("malloc failed");
        exit(EXIT_FAILURE);
    }
}

void appendFileData(FileDataArray *arr, fileData value) {
    if (arr->size >= arr->capacity) {
        arr->capacity *= 2;
        fileData *newData = realloc(arr->data, arr->capacity * sizeof(fileData));
        if (!newData) {
            perror("realloc failed");
            free(arr->data);
            exit(EXIT_FAILURE);
        }
        arr->data = newData;
    }
    arr->data[arr->size++] = value;
}

void freeFileDataArray(FileDataArray *arr) {
    // Free each string inside fileData
    for (size_t i = 0; i < arr->size; i++) {
        free(arr->data[i].path);
        free(arr->data[i].Inode);
    }
    free(arr->data);
    arr->data = NULL;
    arr->size = 0;
    arr->capacity = 0;
}
int symbCount = 0;
int regCount = 0;

void printSymlinkTarget(const char *linkPath) {
    char target[4096];
    // Change this to your prefix when you run this
    const char *prefix = "/root/systems-prog/progTask3/";

    ssize_t len = readlink(linkPath, target, sizeof(target) - 1);
    if (len == -1) {
        perror("readlink failed");
        return;
    }

    target[len] = '\0';  // Null-terminate

    // Trim the prefix if it exists
    const char *trimmed = target;
    if (strncmp(target, prefix, strlen(prefix)) == 0) {
        trimmed = target + strlen(prefix);
    }

    printf("Symlink: %s -> %s\n", linkPath, trimmed);
}
void proccessSymbolic(char *Filename, struct stat fileInfo) {
    printSymlinkTarget(Filename);
    //printf("Symlink: %s (inode: %lu)\n", Filename, fileInfo.st_ino);
    symbCount++;
}

void proccessFile(char *Filename, struct stat fileInfo) {
    printf("File: %s (inode: %lu)\n", Filename, fileInfo.st_ino);
    regCount++;
}
void proccessDir(char *dirName){
    DIR *dir = opendir(dirName);
    if (!dir) {
        perror("opendir failed");
        return;
    }

    struct dirent *entry;

    while ((entry = readdir(dir)) != NULL) {
        // Skip "." and ".."
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        // Build full path to the entry
        char fullPath[4096];
        snprintf(fullPath, sizeof(fullPath), "%s/%s", dirName, entry->d_name);

        struct stat fileInfo;
        if (lstat(fullPath, &fileInfo) == -1) {
            perror("stat failed");
            continue;
        }
        // if directory recusivly go through its folder contents
       if (S_ISDIR(fileInfo.st_mode)) {
            printf("Directory: %s (inode: %lu)\n", fullPath, fileInfo.st_ino);
            proccessDir(fullPath);
        } else if (S_ISLNK(fileInfo.st_mode)) {
            proccessSymbolic(fullPath, fileInfo);
        } else {
            proccessFile(fullPath, fileInfo);
        } 
        
        
    }

    closedir(dir); 
}

int main(int argc, char *argv[]) {
    // check that diretory was passed in
    if (argc < 2){
        printf("failure\nUsage: ./detect_dups <directory>\n");
        //perror("failure");
        return -1;
    }
    struct stat checkDir;
    if (stat(argv[1], &checkDir) != 0 || !S_ISDIR(checkDir.st_mode)) {
        printf("failure\nError 1: %s is not a valid directory\n", argv[1]);
        return -1;
    } 
    proccessDir(argv[1]);
    printf("Done proccessing %s -> Contained %d symblink  && %d hardLinks\n",argv[1],symbCount,regCount);
    //FileDataArray files;
    //initFileDataArray(&files);

    // Need to pass in directory into recusive function 

    // Need map to keep tracks of Hard Link and SoftLinks
    /*struct stat file_stat;
    printf("\nDirectory passed in -> %s\n",argv[1]);
    if (stat(argv[1], &file_stat) == 0) {
        if (S_ISDIR(file_stat.st_mode)){
        printf("This file is a directory \n");
        }else{
            printf("This file is not a directory \n");
        }
        printf("File size: %ld bytes\n", file_stat.st_size);
        printf("Inode: %ld\n", file_stat.st_ino);
        printf("Count of links to file: %ld\n", file_stat.st_nlink);
    } else {
        perror("stat");
        return 1;
    }
        */
    // perform error handling, "exit" with failure incase an error occurs

    // initialize the other global variables you have, if any

    // add the nftw handler to explore the directory
    // nftw should invoke the render_file_info function
}

// render the file information invoked by nftw
static int render_file_info(const char *fpath, const struct stat *sb, int tflag, struct FTW *ftwbuf) {
    // perform the inode operations over here

    // invoke any function that you may need to render the file information
}

// add any other functions you may need over here

/*
dev_t     st_dev     ID of device containing file
ino_t     st_ino     file serial number
mode_t    st_mode    mode of file (see below)
nlink_t   st_nlink   number of links to the file
uid_t     st_uid     user ID of file
gid_t     st_gid     group ID of file
dev_t     st_rdev    device ID (if file is character or block special)
off_t     st_size    file size in bytes (if file is a regular file)
time_t    st_atime   time of last access
time_t    st_mtime   time of last data modification
time_t    st_ctime   time of last status change
blksize_t st_blksize a filesystem-specific preferred I/O block size for
                     this object.  In some filesystem types, this may
                     vary from file to file
blkcnt_t  st_blocks  number of blocks allocated for this object


*/