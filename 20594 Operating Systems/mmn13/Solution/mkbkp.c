/*
 * mkbkp.c
 * This function can create a backuped file for a folder, file or soft-link (using the flag -c)
 * and can later be used to restore the information from that backuped file (using the flad -x)
 *
 *  Created on: Jan 31, 2017
 *      Author: Itai Antebi
 *      ID	  : 204817498
 */

#include "mkbkp.h"

FILE *buf;
char *buf_path;

int compare_strings(char a[], char b[]){
   int c = 0;

   while (a[c] == b[c]) {
      if (a[c] == '\0' || b[c] == '\0')
         break;
      c++;
   }

   if (a[c] == '\0' && b[c] == '\0')
      return 0;
   else
      return -1;
}

int delete_first_bytes(char *file_name, int n_bytes){
	FILE *file;
	struct stat	st;
	void *file_content;
	int new_size;
	if (lstat(file_name, &st) < 0) {
		fprintf(stderr,"error: lstat error");
		return -1;
	}
	new_size = st.st_size - n_bytes;
	// read file from n_bytes to end into file_content
	file = fopen(file_name, "rb");
	if (file == NULL) {
		fprintf(stderr,"error: fopen error");
		return -1;
	}
	file_content = malloc(new_size);
	if (file_content == NULL) {
		fprintf(stderr,"error: malloc error");
		return -1;
	}
	if(fseek(file, n_bytes, SEEK_SET) != 0){
		fprintf(stderr,"error: fseek error");
		return -1;
	}
	fread(file_content, new_size, 1, file);
	if(fclose(file) == -1){
		fprintf(stderr,"error: fclose error");
		return -1;
	}
	// overwrite the file with file_content
	file = fopen(file_name, "w+");
	if (file == NULL) {
		fprintf(stderr,"error: fopen error");
		return -1;
	}
	fwrite(file_content, new_size , 1, file);
	if(fclose(file) == -1){
		fprintf(stderr,"error: fclose error");
		return -1;
	}
	free(file_content);
	return 0;
}

int doesFileExist(const char *filename) {
    struct stat st;
    int result = stat(filename, &st);
    return result == 0;
}


/*
 * type, stat, path_len, content_len, path, content
 */
int backup_sl(const char *rel_path, const struct stat *status){
	struct stat	st;
	int rel_path_len, content_len, len;
	char *file_content;
	// file type
	char *type = "slnk";
	if(fputs(type, buf) == -1){
		fprintf(stderr,"error: fputs error");
		return -1;
	}
//	printf("type 'slnk'. 	4\n");
	// file stat
	if (lstat(rel_path, &st) < 0) {
		fprintf(stderr,"error: lstat error");
		return -1;
	}
	fwrite(&st, sizeof(struct stat), 1, buf);
//	printf("slnk stat. 	%d\n", sizeof(struct stat));
	// slnk name length
	rel_path_len = (int)strlen(rel_path);
	fwrite(&rel_path_len, sizeof(rel_path_len), 1, buf);
//	printf("name length.	%d\n", sizeof(rel_path_len));
	// slnk "content" len
	file_content = malloc(MAX_PATH_SIZE);
	if (file_content == NULL) {
		fprintf(stderr,"error: malloc error");
		return -1;
	}
	len = readlink(rel_path, file_content, MAX_PATH_SIZE);
	if(len == -1){
		fprintf(stderr,"error: readlink error");
		return -1;
	}
	file_content[len] = 0;
	content_len = (int)strlen(file_content);
	fwrite(&content_len, sizeof(content_len), 1, buf);
//	printf("content length.	%d\n", sizeof(content_len));
	// slnk name
	fwrite(rel_path, rel_path_len, 1, buf);
//	printf("name.		%d\n", rel_path_len);
	// slnk "content"
	fwrite(file_content, content_len, 1, buf);
//	printf("slnk content.	%s\n", file_content);
	free(file_content);
	return 0;
}

/*
 * 'file', stat, file_name_len, file_name, file_content
 */
int backup_file(const char *rel_path, const struct stat *status){
	struct stat	st;
	int rel_path_len;
	FILE *file;
	void *file_content;
	// file type
	char *type = "file";
//	printf("type 'file'. 	4\n");
	if(fputs(type, buf) == -1){
		fprintf(stderr,"error: fputs error");
		return -1;
	}
	// file stat
	if (lstat(rel_path, &st) < 0) {
		fprintf(stderr,"error: lstat error");
		return -1;
	}
	fwrite(&st, sizeof(struct stat), 1, buf);
//	printf("file stat. 	%d\n", sizeof(struct stat));
	// file name length
	rel_path_len = (int)strlen(rel_path);
	fwrite(&rel_path_len, sizeof(rel_path_len), 1, buf);
//	printf("name length.	%d\n", sizeof(rel_path_len));
	// file name
	fwrite(rel_path, rel_path_len, 1, buf);
//	printf("name.		%d\n", rel_path_len);
	// (file size)
	file = fopen(rel_path, "rb");
	if (file == NULL) {
		fprintf(stderr,"error: fopen error");
		return -1;
	}
	file_content = malloc(st.st_size);
	if (file_content == NULL) {
		fprintf(stderr,"error: malloc error");
		return -1;
	}
	fread(file_content, st.st_size , 1, file);
	if(fclose(file) == -1){
		fprintf(stderr,"error: fclose error");
		return -1;
	}
	// file content
//	printf("file content.	%d\n", file_size);
	fwrite(file_content, st.st_size, 1, buf);
	free(file_content);
	return 0;
}

/*
 * 'dire', stat, dir_name_len, dir_name
 */
int backup_dir(const char *rel_path, const struct stat *status){
	struct stat	st;
	int rel_path_len;
	// dir type
	char *type = "dire";
//	printf("type 'dire'. 	4\n");
	if(fputs(type, buf) == -1){
		fprintf(stderr,"error: fputs error");
		return -1;
	}
	// dir stat
	if (lstat(rel_path, &st) < 0) {
		fprintf(stderr,"error: lstat error");
		return -1;
	}
	fwrite(&st, sizeof(struct stat), 1, buf);
//	printf("dire stat. 	%d\n", sizeof(struct stat));
	// dir name length
	rel_path_len = (int)strlen(rel_path);
//	printf("rel_path =		%s\n", rel_path);
//	printf("rel_path_len =	%d\n", rel_path_len);
	fwrite(&rel_path_len, sizeof(rel_path_len), 1, buf);
//	printf("name length.	%d\n", sizeof(rel_path_len));
	// dir name
	fwrite(rel_path, rel_path_len, 1, buf);
//	printf("name.		%d\n", rel_path_len);
	return 0;
}

int backup_address(const char *rel_path, const struct stat *status, int type){
	// check if there is an attempt to backup the backup file
	if(strcmp(rel_path, buf_path) == 0){
		fprintf(stderr,"error: tried to backup the backup file");
		return -1;
	}

	struct stat	st;
	if (lstat(rel_path, &st) < 0) {
		fprintf(stderr,"error: lstat error");
		return -1;
	}
	printf("%s",rel_path);
	printf("\n");
#ifdef	S_ISLNK
	if(S_ISLNK(st.st_mode)){
		printf("\tEntering Symbolic Link backup...\n");
		return backup_sl(rel_path, status);
	}
#endif
	if(S_ISREG(st.st_mode)){
		printf("\tEntering File backup...\n");
		return backup_file(rel_path, status);
	}

	if(S_ISDIR(st.st_mode) && strcmp(".", rel_path) != 0){
		printf("\tEntering Directory backup...\n");
		return backup_dir(rel_path, status);
	}
	return 0;
}

int backup_recursive_address(char *backup_file, char *backuped_fd){
	printf("\nbacking up...\n\n");
	if(doesFileExist(backup_file)){
		if(unlink(backup_file) == -1){
			fprintf(stderr,"error: unlink error");
			return -1;
		}
	}
	buf_path = backup_file;
	buf = fopen(backup_file, "ab");
	if (buf == NULL) {
		fprintf(stderr,"error: fopen error");
		return -1;
	}
	//backup backuped_fd
	ftw(backuped_fd, backup_address, 1);
	if(fclose(buf) == -1){
		fprintf(stderr,"error: fclose error");
		return -1;
	}
	printf("finished backing up!\n");
	return 0;
}

/*
 * 'file', stat, file_name_len, file_name, file_content
 */
int restore_file(char *backup_file){
	FILE *new_file;
	int bytes_read = 4;
	struct stat *st;
	int *rel_path_len;
	char *rel_path, *file_content;
	struct utimbuf file_times;
	size_t temp;
	// retract information from buf
	buf = fopen(backup_file, "rb");
	if (buf == NULL) {
		fprintf(stderr,"error: fopen error");
		return -1;
	}
	if(fseek(buf, 4, SEEK_SET) != 0){
		fprintf(stderr,"error: fseek error");
		return -1;
	}
//	printf("position %ld\n",ftell(buf));
	// file stat
	st = malloc(sizeof(struct stat));
	if (st == NULL) {
		fprintf(stderr,"error: malloc error");
		return -1;
	}
	fread(st, sizeof(struct stat), 1, buf);
	bytes_read += sizeof(struct stat);
//	printf("stat read\n");
//	printf("position %ld\n",ftell(buf));
	// file name length
	rel_path_len = malloc(sizeof(int));
	if (rel_path_len == NULL) {
		fprintf(stderr,"error: malloc error");
		return -1;
	}
	fread(rel_path_len, sizeof(int), 1, buf);
	bytes_read += sizeof(int);
//	printf("rel_path_len =	%d\n", *rel_path_len);
//	printf("position %ld\n",ftell(buf));
	// file name
	temp = (size_t)*rel_path_len;
	rel_path = malloc(temp + 1);
	if (rel_path == NULL) {
		fprintf(stderr,"error: malloc error");
		return -1;
	}
	fread(rel_path, temp, 1, buf);
	rel_path[temp] = 0;
	bytes_read += temp;
//	printf("rel_path =	%s\n", rel_path);
//	printf("position %ld\n",ftell(buf));
	// file content
	file_content = malloc(st->st_size);
	if (file_content == NULL) {
		fprintf(stderr,"error: malloc error");
		return -1;
	}
	fread(file_content, st->st_size, 1, buf);
	bytes_read += st->st_size;
//	printf("file content =	%s", file_content);
	if(fclose(buf) == -1){
		fprintf(stderr,"error: fclose error");
		return -1;
	}

	// check if file existed
	if(doesFileExist(rel_path)){
		fprintf(stderr,"error: tried to restore an existing file");
		return -1;
	}

	// restore the file from the information
	// permissions
//	printf("permissions\n");
	umask(0);
	if(creat(rel_path, st->st_mode) < 0){
		fprintf(stderr,"error: creat error");
		return -1;
	}
	// content
//	printf("content\n");
	new_file = fopen(rel_path, "wb");
	if (new_file == NULL) {
		fprintf(stderr,"error: fopen error");
		return -1;
	}
//	printf("content\n");
	fwrite(file_content, st->st_size, 1, new_file);
//	printf("content\n");
	if(fclose(new_file) == -1){
		fprintf(stderr,"error: fclose error");
		return -1;
	}
	// uid gid
//	printf("uid gid\n");
	if(lchown(rel_path, st->st_uid, st->st_gid) == -1){
		fprintf(stderr,"error: lchown error");
		return -1;
	}
	// modification time
//	printf("mod time	\n");
	file_times.modtime = st->st_mtim.tv_sec;
	if(utime(rel_path, &file_times) == -1){
		fprintf(stderr,"error: utime error");
		return -1;
	}
//	printf("end of file bkp\n");
	free(st);
	free(rel_path_len);
	free(rel_path);
	free(file_content);
	return bytes_read;
}

/*
 * type, stat, path_len, content_len, path, content
 */
int restore_sl(char *backup_file){
	int bytes_read = 4;
	struct stat *st;
	int *rel_path_len, *content_path_len;

	char *rel_path, *file_content;
	size_t temp;

	// retract information from buf
	buf = fopen(backup_file, "rb");
	if (buf == NULL) {
			fprintf(stderr,"error: fopen error");
			return -1;
	}
	if(fseek(buf, 4, SEEK_SET) != 0){
		fprintf(stderr,"error: fseek error");
		return -1;
	}
//	printf("position %ld\n",ftell(buf));
	// file stat
	st = malloc(sizeof(struct stat));
	if (st == NULL) {
		fprintf(stderr,"error: malloc error");
		return -1;
	}
	fread(st, sizeof(struct stat), 1, buf);
	bytes_read += sizeof(struct stat);
//	printf("stat read\n");
//	printf("position %ld\n",ftell(buf));
	// slnk name length
	rel_path_len = malloc(sizeof(int));
	if (rel_path_len == NULL) {
		fprintf(stderr,"error: malloc error");
		return -1;
	}
	fread(rel_path_len, sizeof(int), 1, buf);
	bytes_read += sizeof(int);
//	printf("rel_path_len =	%d\n", *rel_path_len);
//	printf("position %ld\n",ftell(buf));
	// content length
	content_path_len = malloc(sizeof(int));
	if (content_path_len == NULL) {
		fprintf(stderr,"error: malloc error");
		return -1;
	}
	fread(content_path_len, sizeof(int), 1, buf);
	bytes_read += sizeof(int);
//	printf("content_path_len =	%d\n", *content_path_len);
//	printf("position %ld\n",ftell(buf));
	// file name
	temp = (size_t)*rel_path_len;
	rel_path = malloc(temp + 1);
	if (rel_path == NULL) {
		fprintf(stderr,"error: malloc error");
		return -1;
	}
	fread(rel_path, temp, 1, buf);
	rel_path[temp] = 0;
	bytes_read += temp;
//	printf("rel_path =	%s\n", rel_path);
//	printf("position %ld\n",ftell(buf));
	// file content
	temp = (size_t)*content_path_len;
	file_content = malloc(temp + 1);
	if (file_content == NULL) {
		fprintf(stderr,"error: malloc error");
		return -1;
	}
	fread(file_content, temp, 1, buf);
	file_content[temp] = 0;
	bytes_read += temp;
//	printf("file_content =	%s\n", file_content);
//	printf("position %ld\n",ftell(buf));
	if(fclose(buf) == -1){
		fprintf(stderr,"error: fclose error");
		return -1;
	}

	// check if file existed
	if(doesFileExist(rel_path)){
		fprintf(stderr,"error: tried to restore an existing spft link");
		return -1;
	}

	// restore the file from the information
	if(symlink(file_content, rel_path) == -1){
		fprintf(stderr,"error: symlink error");
		return -1;
	}
	// permissions
//	printf("permissions\n");
	umask(0);
// CHECK LCHMOD
	lchmod(rel_path, st->st_mode);
	// uid gid
//	printf("uid gid\n");
	if(lchown(rel_path, st->st_uid, st->st_gid) == -1){
		fprintf(stderr,"error: lchown error");
		return -1;
	}
//	printf("end of file bkp\n");
	free(st);
	free(rel_path_len);
	free(content_path_len);
	free(rel_path);
	free(file_content);
	return bytes_read;
}

int restore_dir(char *backup_file){
	int bytes_read = 4;
	struct stat *st;
	int *rel_path_len;
	size_t temp;
	char *rel_path;
	struct utimbuf file_times;

	// retract information from buf
	buf = fopen(backup_file, "rb");
	if (buf == NULL) {
			fprintf(stderr,"error: fopen error");
			return -1;
	}
	if(fseek(buf, 4, SEEK_SET) != 0){
		fprintf(stderr,"error: fseek error");
		return -1;
	}
//	printf("position %ld\n",ftell(buf));
	// dir stat
	st = malloc(sizeof(struct stat));
	if (st == NULL) {
		fprintf(stderr,"error: malloc error");
		return -1;
	}
	fread(st, sizeof(struct stat), 1, buf);
	bytes_read += sizeof(struct stat);
//	printf("stat read\n");
//	printf("position %ld\n",ftell(buf));
	// dir name length
	rel_path_len = malloc(sizeof(int));
	if (rel_path_len == NULL) {
		fprintf(stderr,"error: malloc error");
		return -1;
	}
	fread(rel_path_len, sizeof(int), 1, buf);
	bytes_read += sizeof(int);
//	printf("rel_path_len =	%d\n", *rel_path_len);
//	printf("position %ld\n",ftell(buf));
	// dir name
	temp = (size_t)*rel_path_len;
	rel_path = malloc(temp+1);
	if (rel_path == NULL) {
		fprintf(stderr,"error: malloc error");
		return -1;
	}
	fread(rel_path, temp, 1, buf);
	rel_path[temp] = 0;
	bytes_read += (size_t)*rel_path_len;
//	printf("rel_path =	%s\n", rel_path);
//	printf("position %ld\n",ftell(buf));
	if(fclose(buf) == -1){
		fprintf(stderr,"error: fclose error");
		return -1;
	}

	// check if file existed
	if(doesFileExist(rel_path)){
		fprintf(stderr,"error: tried to restore an existing folder");
		return -1;
	}

	// restore the dir from the information
	// permissions
	umask(0);
	if(mkdir(rel_path, st->st_mode) == -1){
		fprintf(stderr,"error: mkdir error");
		return -1;
	}
	// uid gid
	if(lchown(rel_path, st->st_uid, st->st_gid) == -1){
		fprintf(stderr,"error: lchown error");
		return -1;
	}
	// modification time
	file_times.modtime = st->st_mtim.tv_sec;
	if(utime(rel_path, &file_times) == -1){
		fprintf(stderr,"error: utime error");
		return -1;
	}
	free(st);
	free(rel_path_len);
	free(rel_path);
	return bytes_read;
}

int restore(char *backup_file){
	/* This is a recursive function
	 * If 		- backup_file is empty - it will return successfully
	 * Else 	- it will create the first file/folder (ff) in backupfile, and return restore(backupfile - ff)
	 */
	printf("\nrestoring...\n");
	struct stat	st;
	char type[4];
	int bytes_read;
	if (lstat(backup_file, &st) < 0) {
		fprintf(stderr,"error: lstat error");
		return -1;
	}
	// finished restoring
	if(st.st_size == 0){
		if(unlink(backup_file) == -1){
			fprintf(stderr,"error: unlink error");
			return -1;
		}
		printf("restoring completed successfully\n");
		return 0;
	}
	// restoring first element
	buf = fopen(backup_file, "rb");
	if (buf == NULL) {
			fprintf(stderr,"error: fopen error");
			return -1;
	}
	fread(&type, 4, 1, buf);
	if(fclose(buf) == -1){
		fprintf(stderr,"error: fclose error");
		return -1;
	}
//	printf("TYPE = %s\n", type);
	if(compare_strings(type, "file") == 0){
		printf("restoring a file\n");
		bytes_read = restore_file(backup_file);
		printf("file restored successfully\n");
	}
	else if(compare_strings(type, "slnk") == 0){
		printf("restoring a soft link\n");
		bytes_read = restore_sl(backup_file);
		printf("soft link restored successfully\n");
	}
	else if(compare_strings(type, "dire") == 0){
		printf("restoring a dir\n");
		bytes_read = restore_dir(backup_file);
		printf("dir restored successfully\n");
	}
	else{
		fprintf(stderr,"damaged backup file\n");
		return -1;
	}
	// MAKE BUF BYTES_READ LESS
	delete_first_bytes(backup_file, bytes_read);
	// continue recursively
	return restore(backup_file);
}

int main(int argc, char *argv[]){
	int opt;
	if((opt = getopt(argc, argv, "cx")) != -1){
		switch(opt){
			case 'c':
				printf("c flag received\n");
				if(argc!=4){
					printf("%s",USAGE);
					return 1;
				}
				return backup_recursive_address(argv[2], argv[3]);
				break;
			case 'x':
				printf("x flag received\n");
				if(argc!=3){
					printf("%s",USAGE);
					return 1;
				}
				return restore(argv[2]);
				break;
		}
	}
	printf("%s",USAGE);
	return 0;
}
