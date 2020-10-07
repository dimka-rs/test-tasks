#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static void
file_stat(char *fptr)
{
    struct stat buf;
    int res;

    res = stat(fptr, &buf);
    if (res == 0)
        printf("UID:%7d, GID%7d, SZ:%10ld ", buf.st_uid, buf.st_gid, buf.st_size);
}

static void
list_dir(DIR *dirp, int fst)
{
     struct dirent *dent;

     dent = readdir(dirp);
     while (dent != NULL)
     {
        if (fst > 0)
            file_stat(dent->d_name);

         /* print inode */
        printf("%10ld ", dent->d_ino);

        /* print type */
        switch (dent->d_type)
        {
            case DT_BLK:
                printf("BLK ");
                break;
            case DT_CHR:
                printf("CHR ");
                break;
            case DT_DIR:
                printf("DIR ");
                break;
            case DT_FIFO:
                printf("FIF ");
                break;
            case DT_LNK:
                printf("LNK ");
                break;
            case DT_REG:
                printf("REG ");
                break;
            case DT_SOCK:
                printf("SOC ");
                break;
            default:
                printf("??? ");
        }

        /* print name */
        printf("%s\n", dent->d_name);

        /* get next entry */
        dent = readdir(dirp);
     }
}

int
main (int argc, char **argv)
{
    DIR *dirp;
    char *dpath;
    char cwd[PATH_MAX];
    int l = 0;

    if (argc > 1)
    {
        if (strcmp(argv[1], "-l") == 0)
            l = 1;
    }

    if (argc == 1 || (argc == 2 && l == 1))
    {
        /* get current path */
        if (getcwd(cwd, PATH_MAX) == NULL)
            dpath = NULL;
        else
            dpath = cwd;
    }
    else
    {
        /* use provided path */
        if (l == 1)
            dpath = argv[2];
        else
            dpath = argv[1];
    }

    dirp = opendir(dpath);
    if (dirp == NULL)
    {
        perror("Failed to open dir");
        return 1;
    }

    list_dir(dirp, l);
    closedir(dirp);

    return 0;
}
