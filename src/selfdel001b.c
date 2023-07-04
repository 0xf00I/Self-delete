void 
CommitSuicide() {
    char path[256];
    int status;

    // Get the path of the current program
    status = readlink("/proc/self/exe", path, sizeof(path) - 1);
    if (status == -1) {
        exit(EXIT_FAILURE);
    }
    path[status] = '\0';

    // Copy the executable to a temporary file
    char tmp_path[] = "/tmp/XXXXXX";
    int fd = mkstemp(tmp_path);
    if (fd == -1) {
        exit(EXIT_FAILURE);
    }
    if (write(fd, path, status) != status) {
        exit(EXIT_FAILURE);
    }
    close(fd);

    // Launch the temporary executable as a new process
    if (fork() == 0) {
        execl(tmp_path, "", NULL);
    }

    // Delete the original executable
    unlink(path);

    // Exit the current process
    exit(EXIT_SUCCESS);
}

int main() {
    CommitSuicide();
    return 0;
}
