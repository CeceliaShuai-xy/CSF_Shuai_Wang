#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <assert.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

// // // helper function to sequential sort element
// int compare_i64(const void *a, const void *b) {
//   return (*(int64_t*)a - *(int64_t*)b);
// }
int compare_i64(const void * a, const void * b) {
  if (*(int64_t*)a > *(int64_t*)b) return 1;
  if (*(int64_t*)a < *(int64_t*)b) return -1;
  return 0;
}

// Merge the elements in the sorted ranges [begin, mid) and [mid, end),
// copying the result into temparr.
void merge(int64_t *arr, size_t begin, size_t mid, size_t end, int64_t *temparr) {
  int64_t *endl = arr + mid;
  int64_t *endr = arr + end;
  int64_t *left = arr + begin, *right = arr + mid, *dst = temparr;

  for (;;) {
    int at_end_l = left >= endl;
    int at_end_r = right >= endr;

    if (at_end_l && at_end_r) break;

    if (at_end_l)
      *dst++ = *right++;
    else if (at_end_r)
      *dst++ = *left++;
    else {
      int cmp = compare_i64(left, right);
      if (cmp <= 0)
        *dst++ = *left++;
      else
        *dst++ = *right++;
    }
  }
}

// void merge_sort(int64_t *arr, size_t begin, size_t end, size_t threshold) {
//   // TODO: implement
//   if(end-begin < threshold) {
//     qsort(arr + begin, end-begin, sizeof(int64_t), compare_i64);
//     exit(0);
//   } else {
//     pid_t left_child_pid = fork();
//     if (left_child_pid == -1) {
//     // fork failed to start a new process
//     // handle the error and exit
//       fprintf(stderr, "Error: could not fork left child process.\n");
//       exit(1);
//     } else if (left_child_pid == 0) {
//     // this is now in the child process
//       merge_sort(arr,begin, (begin+end)/2, threshold);
//       exit(0);
//     } else {
//       // now in the parent process
//       pid_t right_child_pid = fork();
//       if (right_child_pid == -1) {
//       // fork failed to start a new process
//       // handle the error and exit
//         fprintf(stderr, "Error: could not fork right child process.\n");
//         exit(1);  
//       } else if (right_child_pid == 0) {
//       // this is now in the child process
//         merge_sort(arr,(begin+end)/2 + 1, end, threshold);
//         exit(0);
//       } 

//       // wait for both child process to finish and merge
//       int wstatus_left;
//       int wstatus_right;
//       // blocks until the process indentified by pid_to_wait_for completes
//       pid_t actual_pid_l = waitpid(left_child_pid, &wstatus_left, 0);
//       pid_t actual_pid_r = waitpid(right_child_pid, &wstatus_right, 0);
      
//       if (actual_pid_l == -1 || actual_pid_r == -1) {
//         // handle waitpid failure
//         fprintf(stderr, "Error: waitpid failure.\n");
//         exit(1);
//       }
//       if (!WIFEXITED(wstatus_left) || !WIFEXITED(wstatus_right)) {
//         // subprocess crashed, was interrupted, or did not exit normally
//         // handle as error
//         fprintf(stderr, "Error: subprocess crashed, was interrupted, or did not exit normally.\n");
//         exit(1);
//       }
//       if (WEXITSTATUS(wstatus_left) != 0 || WEXITSTATUS(wstatus_right) != 0) {
//         // subprocess returned a non-zero exit code
//         // if following standard UNIX conventions, this is also an error
//         fprintf(stderr, "Error: subprocess returned a non-zero exit code.\n");
//         exit(1);
//       }

//       int64_t *temp_arr = malloc((end-begin)*sizeof(int64_t));
//       merge(arr, begin, (begin+end)/2, end,temp_arr);

//       for (int i = 0; i < end-begin; i++) {
//         arr[i+begin] = temp_arr[i];
//       }

//       free(temp_arr);

//       }
//   }
// }

void print_arr(int64_t *arr, size_t begin, size_t end) {
  for (size_t i = begin; i < end; i++){
    printf("%d", arr[i]);
    printf(" ");
  }
  printf("\n");
}

void merge_sort(int64_t *arr, size_t begin, size_t end, size_t threshold) {
  // TODO: implement
  if(end-begin <= threshold) {
    qsort(arr + begin, (end-begin), sizeof(int64_t), compare_i64);
  } else {
    pid_t left_child_pid = fork();
    if (left_child_pid == -1) {
    // fork failed to start a new process
    // handle the error and exit
      fprintf(stderr, "Error: could not fork left child process.\n");
      exit(1);
    } else if (left_child_pid == 0) {
    // this is now in the child process
      merge_sort(arr,begin, (begin+end)/2, threshold);
      exit(0);
    } else {
      // now in the parent process
      merge_sort(arr,(begin+end)/2, end, threshold);

      // wait for both child process to finish and merge
      int wstatus_left;
      // blocks until the process indentified by pid_to_wait_for completes
      pid_t actual_pid_l = waitpid(left_child_pid, &wstatus_left, 0);
      
      if (actual_pid_l == -1) {
        // handle waitpid failure
        fprintf(stderr, "Error: waitpid failure.\n");
        exit(1);
      }
      if (!WIFEXITED(wstatus_left)) {
        // subprocess crashed, was interrupted, or did not exit normally
        // handle as error
        fprintf(stderr, "Error: subprocess crashed, was interrupted, or did not exit normally.\n");
        exit(1);
      }
      if (WEXITSTATUS(wstatus_left) != 0) {
        // subprocess returned a non-zero exit code
        // if following standard UNIX conventions, this is also an error
        fprintf(stderr, "Error: subprocess returned a non-zero exit code.\n");
        exit(1);
      }
    
      int64_t *temp_arr = malloc((end-begin)*sizeof(int64_t));
      merge(arr, begin, (begin+end)/2, end, temp_arr);

      for (int i = 0; i < end-begin; i++) {
        arr[i+begin] = temp_arr[i];
      }

      free(temp_arr);
    }
  }
  return;
}

int main(int argc, char **argv) {
  // check for correct number of command line arguments
  if (argc != 3) {
    fprintf(stderr, "Usage: %s <filename> <sequential threshold>\n", argv[0]);
    return 1;
  }

  // process command line arguments
  const char *filename = argv[1];
  char *end;
  size_t threshold = (size_t) strtoul(argv[2], &end, 10);
  if (end != argv[2] + strlen(argv[2])) {
    // report an error (threshold value is invalid)
    fprintf(stderr, "Error: threshold value is invalid.\n");
    return 1;
  }
  // open the file
  int fd = open(filename, O_RDWR);
  if (fd < 0) {
  // file couldn't be opened: handle error and exit
    fprintf(stderr, "Error: failure to open the file with the integers to be sorted.\n");
    close(fd);
    return 1;
  }

  // TODO: use fstat to determine the size of the file
  struct stat statbuf;
  int rc = fstat(fd, &statbuf);
  if (rc != 0) {
    fprintf(stderr, "Error: failure to get file stats.\n");
    close(fd);
    return 1;
  }

  size_t file_size_in_bytes = statbuf.st_size;

  // TODO: map the file into memory using mmap
  int64_t *data = mmap(NULL, file_size_in_bytes, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  // printf("Here's data: ");
  // // int num_elements = file_size_in_bytes/sizeof(int64_t);
  // printf("%d\n", file_size_in_bytes/sizeof(int64_t));
  // print_arr(data, 0, file_size_in_bytes/sizeof(int64_t));
  
  if (data == MAP_FAILED) {
    // handle mmap error and exit
    fprintf(stderr, "Error: failure to mmap the file data.\n"); 
    close(fd);
    return 1;
  }
  //merge_sort(data, 0, file_size_in_bytes/sizeof(int64_t)-1, threshold);

  //TODO: sort the data!
  pid_t pid = fork();
  if (pid == -1) {
    // fork failed to start a new process
    // handle the error and exit
    fprintf(stderr, "Error: could not fork the child process to do mergesort.\n");
    munmap(data, file_size_in_bytes);
    close(fd);
    return 1;
  } else if (pid == 0) {
    // this is now in the child process
    printf("In child process\n");
    merge_sort(data, 0, file_size_in_bytes/sizeof(int64_t), threshold);
    exit(0);
  } else {
    //now back in parent
    int wstatus;
      // blocks until the process indentified by pid_to_wait_for completes
    pid_t actual_pid = waitpid(pid, &wstatus, 0);
    if (actual_pid == -1 ) {
        // handle waitpid failure
      fprintf(stderr, "Error: waitpid failure.\n");
      munmap(data, file_size_in_bytes);
      close(fd);
      return 1;
    }
    if (!WIFEXITED(wstatus)) {
      // subprocess crashed, was interrupted, or did not exit normally
      // handle as error
      fprintf(stderr, "Error: subprocess crashed, was interrupted, or did not exit normally.\n");
      munmap(data, file_size_in_bytes);
      close(fd);
      return 1;
    }
    if (WEXITSTATUS(wstatus) != 0) {
      // subprocess returned a non-zero exit code
      // if following standard UNIX conventions, this is also an error
      fprintf(stderr, "Error: subprocess returned a non-zero exit code.\n");
      munmap(data, file_size_in_bytes);
      close(fd);
      return 1;
    }

  //   printf("**********After sorting all **********\n");
  // // int num_elements = file_size_in_bytes/sizeof(int64_t);
  //   print_arr(data, 0, file_size_in_bytes/sizeof(int64_t));
    // TODO: unmap and close the file
    munmap(data, file_size_in_bytes);
    close(fd);
    return 0;
  }
  // sanity check
  // pid_t pid = fork();
  // if (pid == 0) {
  //   printf("In child process\n");
  // } else {
  //   printf("In parent process\n");
  // }

  // TODO: exit with a 0 exit code if sort was successful
}