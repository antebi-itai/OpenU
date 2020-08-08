/*
 * mkbkp.h
 *  Created on: Jan 31, 2017
 *      Author: Itai Antebi
 *      ID	  : 204817498
 */


#ifndef MKBKP_H_
#define MKBKP_H_

#include <fcntl.h>
#include <ftw.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <utime.h>
#include <time.h>

char USAGE[] = "Usage: mkbkp <-c|-x> <backup_file> [file_to_backup|directory_to_backup]\n\n";
int MAX_PATH_SIZE = 1000;

int compare_strings(char a[], char b[]);
int delete_first_bytes(char *file_name, int n_bytes);
int doesFileExist(const char *filename);

int backup_sl(const char *rel_path, const struct stat *status);
int backup_file(const char *rel_path, const struct stat *status);
int backup_dir(const char *rel_path, const struct stat *status);
int backup_address(const char *rel_path, const struct stat *status, int type);
int backup_recursive_address(char *backup_file, char *backuped_fd);

int restore_file(char *backup_file);
int restore_sl(char *backup_file);
int restore_dir(char *backup_file);
int restore(char *backup_file);

#endif /* MKBKP_H_ */
