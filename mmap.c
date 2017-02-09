#include 	"mmap.h"

int main (int argc, char *argv[])
{
        struct stat sb; //struct stat with sb.st_size to know the size of the file
        off_t len;
        char *p;
        int fd;
        
        if (argc < 2) {
                fprintf (stderr, "usage: %s <file>\n", argv[0]);
                return 1;
        }

        fd = open (argv[1], O_RDONLY);
        if (fd == -1) {
                perror ("open");
                return 1;
        }

        if (fstat (fd, &sb) == -1) { //fstat => size of file
                perror ("fstat");
                return 1;
        }

        if (!S_ISREG (sb.st_mode)) { //verif if the file is valid
                fprintf (stderr, "%s is not a file\n", argv[1]);
                return 1;
        }

        p = mmap (0, sb.st_size, PROT_READ, MAP_SHARED, fd, 0); //map the file in memory for further access
        if (p == MAP_FAILED) {
                perror ("mmap");
                return 1;
        }

        if (close (fd) == -1) {
                perror ("close");
                return 1;
        }

        for (len = 0; len < sb.st_size; len++)
                putchar (p[len]); //print every char in the mapped area p

        if (munmap (p, sb.st_size) == -1) {
                perror ("munmap");
                return 1;
        }

        return 0;
}