void 
CommitSuicide() {

    char path[256];
    int status;
    status = readlink("/proc/self/exe", path, sizeof(path) - 1);
    if (status == -1) {
        exit(EXIT_FAILURE);
    }

    path[status] = '\0';
    if (unlink(path) != 0) {
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}


int main() {
    CommitSuicide();
    return 0;
}
