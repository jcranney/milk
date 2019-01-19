
/**
 * @file function_parameters.c
 * @brief Tools to help expose and control function parameters
 * 
 * 
 * # Description
 * 
 * Function parameters are saved in directory ./conf/KEYW0/KEYW1/KEYW2.txt
 * 
 * 
 * # Use Template
 * 
 * @code
 * 
 * #include 
 * 
 * 	// allocate and initialize function parameter array
 * NBparam = FUNCTION_PARAMETER_NBPARAM_DEFAULT;
 * FUNCTION_PARAMETER *funcparam = (FUNCTION_PARAMETER *)malloc(sizeof(FUNCTION_PARAMETER)*NBparam);
 * function_parameter_initarray(funcparam, NBparam);
 * 
 * // add parameter (integer)
 * 
 * 
 * @endcode
 * 
 * 
 * 
 */




#define _GNU_SOURCE


/* =============================================================================================== */
/* =============================================================================================== */
/*                                        HEADER FILES                                             */
/* =============================================================================================== */
/* =============================================================================================== */


#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include <math.h>
#include <limits.h>

#include <errno.h>

#include <time.h>

#include <sys/types.h>

#include <ncurses.h>
#include <dirent.h>

#include <pthread.h>
#include <fcntl.h> // for open
#include <unistd.h> // for close
#include <sys/mman.h> // mmap
#include <sys/stat.h> // fstat

#include <00CORE/00CORE.h>
#include <CommandLineInterface/CLIcore.h>
#include "info/info.h"

/* =============================================================================================== */
/* =============================================================================================== */
/*                                      DEFINES, MACROS                                            */
/* =============================================================================================== */
/* =============================================================================================== */


#define NB_FPS_MAX 100

#define MAXNBLEVELS 20


// regular color codes
#define RESET   "\033[0m"
#define BLACK   "\033[0;30m"      /* Black */
#define RED     "\033[0;31m"      /* Red */
#define GREEN   "\033[0;32m"      /* Green */
#define YELLOW  "\033[0;33m"      /* Yellow */
#define BLUE    "\033[0;34m"      /* Blue */
#define MAGENTA "\033[0;35m"      /* Magenta */
#define CYAN    "\033[0;36m"      /* Cyan */
#define WHITE   "\033[0;37m"      /* White */

// Bold
#define BOLDBLACK   "\033[1;30m"      /* Bold Black */
#define BOLDRED     "\033[1;31m"      /* Bold Red */
#define BOLDGREEN   "\033[1;32m"      /* Bold Green */
#define BOLDYELLOW  "\033[1;33m"      /* Bold Yellow */
#define BOLDBLUE    "\033[1;34m"      /* Bold Blue */
#define BOLDMAGENTA "\033[1;35m"      /* Bold Magenta */
#define BOLDCYAN    "\033[1;36m"      /* Bold Cyan */
#define BOLDWHITE   "\033[1;37m"      /* Bold White */

// Blink
#define BLINKBLACK   "\033[5;30m"      /* Blink Black */
#define BLINKRED     "\033[5;31m"      /* Blink  Red */
#define BLINKGREEN   "\033[5;32m"      /* Blink  Green */
#define BLINKYELLOW  "\033[5;33m"      /* Blink  Yellow */
#define BLINKBLUE    "\033[5;34m"      /* Blink  Blue */
#define BLINKMAGENTA "\033[5;35m"      /* Blink  Magenta */
#define BLINKCYAN    "\033[5;36m"      /* Blink  Cyan */
#define BLINKWHITE   "\033[5;37m"      /* Blink  White */


// Blink High Intensity
#define BLINKHIBLACK   "\033[5;90m"      /* Blink Black */
#define BLINKHIRED     "\033[5;91m"      /* Blink  Red */
#define BLINKHIGREEN   "\033[5;92m"      /* Blink  Green */
#define BLINKHIYELLOW  "\033[5;93m"      /* Blink  Yellow */
#define BLINKHIBLUE    "\033[5;94m"      /* Blink  Blue */
#define BLINKHIMAGENTA "\033[5;95m"      /* Blink  Magenta */
#define BLINKHICYAN    "\033[5;96m"      /* Blink  Cyan */
#define BLINKHIWHITE   "\033[5;97m"      /* Blink  White */


// Underline
#define UNDERLINEBLACK   "\033[4;30m"      /* Bold Black */
#define UNDERLINERED     "\033[4;31m"      /* Bold Red */
#define UNDERLINEGREEN   "\033[4;32m"      /* Bold Green */
#define UNDERLINEYELLOW  "\033[4;33m"      /* Bold Yellow */
#define UNDERLINEBLUE    "\033[4;34m"      /* Bold Blue */
#define UNDERLINEMAGENTA "\033[4;35m"      /* Bold Magenta */
#define UNDERLINECYAN    "\033[4;36m"      /* Bold Cyan */
#define UNDERLINEWHITE   "\033[4;37m"      /* Bold White */

// High Intensity
#define HIBLACK   "\033[0;90m"      /* Black */
#define HIRED     "\033[0;91m"      /* Red */
#define HIGREEN   "\033[0;92m"      /* Green */
#define HIYELLOW  "\033[0;93m"      /* Yellow */
#define HIBLUE    "\033[0;94m"      /* Blue */
#define HIMAGENTA "\033[0;95m"      /* Magenta */
#define HICYAN    "\033[0;96m"      /* Cyan */
#define HIWHITE   "\033[0;97m"      /* White */

// Bold High Intensity
#define BOLDHIBLACK   "\033[1;90m"      /* Black */
#define BOLDHIRED     "\033[1;91m"      /* Red */
#define BOLDHIGREEN   "\033[1;92m"      /* Green */
#define BOLDHIYELLOW  "\033[1;93m"      /* Yellow */
#define BOLDHIBLUE    "\033[1;94m"      /* Blue */
#define BOLDHIMAGENTA "\033[1;95m"      /* Magenta */
#define BOLDHICYAN    "\033[1;96m"      /* Cyan */
#define BOLDHIWHITE   "\033[1;97m"      /* White */





// Background
#define BACKGROUNDBLACK   "\033[40m"      /* Black */
#define BACKGROUNDRED     "\033[41m"      /* Red */
#define BACKGROUNDGREEN   "\033[42m"      /* Green */
#define BACKGROUNDYELLOW  "\033[43m"      /* Yellow */
#define BACKGROUNDBLUE    "\033[44m"      /* Blue */
#define BACKGROUNDMAGENTA "\033[45m"      /* Magenta */
#define BACKGROUNDCYAN    "\033[46m"      /* Cyan */
#define BACKGROUNDWHITE   "\033[47m"      /* White */

// High Intensity background
#define BACKGROUNDHIBLACK   "\033[0;100m"      /* Black */
#define BACKGROUNDHIRED     "\033[0;101m"      /* Red */
#define BACKGROUNDHIGREEN   "\033[0;102m"      /* Green */
#define BACKGROUNDHIYELLOW  "\033[0;103m"      /* Yellow */
#define BACKGROUNDHIBLUE    "\033[0;104m"      /* Blue */
#define BACKGROUNDHIMAGENTA "\033[0;105m"      /* Magenta */
#define BACKGROUNDHICYAN    "\033[0;106m"      /* Cyan */
#define BACKGROUNDHIWHITE   "\033[0;107m"      /* White */




/* =============================================================================================== */
/* =============================================================================================== */
/*                                  GLOBAL DATA DECLARATION                                        */
/* =============================================================================================== */
/* =============================================================================================== */




static int wrow, wcol;


typedef struct
{
    char keywordfull[FUNCTION_PARAMETER_KEYWORD_STRMAXLEN*FUNCTION_PARAMETER_KEYWORD_MAXLEVEL];
    char keyword[FUNCTION_PARAMETER_KEYWORD_MAXLEVEL][FUNCTION_PARAMETER_KEYWORD_STRMAXLEN];
    int keywordlevel;

    int parent_index;

    int NBchild;
    int child[500];

    int leaf; // 1 if this is a leaf (no child)
    int fpsindex;
    int pindex;

} KEYWORD_TREE_NODE;




/* =============================================================================================== */
/* =============================================================================================== */
/*                                    FUNCTIONS SOURCE CODE                                        */
/* =============================================================================================== */
/* =============================================================================================== */





int function_parameter_struct_create(
    int NBparam,
    char *name
)
{
    int index;
    char *mapv;
    FUNCTION_PARAMETER_STRUCT  funcparamstruct;

    //  FUNCTION_PARAMETER_STRUCT_MD *funcparammd;
    //  FUNCTION_PARAMETER *funcparamarray;

    char SM_fname[200];
    size_t sharedsize = 0; // shared memory size in bytes
    int SM_fd; // shared memory file descriptor

    snprintf(SM_fname, sizeof(SM_fname), "%s/%s.fps.shm", SHAREDMEMDIR, name);
    remove(SM_fname);

    printf("Creating file %s\n", SM_fname);
    fflush(stdout);

    sharedsize = sizeof(FUNCTION_PARAMETER_STRUCT_MD);
    sharedsize += sizeof(FUNCTION_PARAMETER)*NBparam;

    SM_fd = open(SM_fname, O_RDWR | O_CREAT | O_TRUNC, (mode_t)0600);
    if (SM_fd == -1) {
        perror("Error opening file for writing");
        exit(0);
    }

    int result;
    result = lseek(SM_fd, sharedsize-1, SEEK_SET);
    if (result == -1) {
        close(SM_fd);
        printf("ERROR [%s %s %d]: Error calling lseek() to 'stretch' the file\n", __FILE__, __func__, __LINE__);
        exit(0);
    }

    result = write(SM_fd, "", 1);
    if (result != 1) {
        close(SM_fd);
        perror("Error writing last byte of the file");
        exit(0);
    }

    funcparamstruct.md = (FUNCTION_PARAMETER_STRUCT_MD*) mmap(0, sharedsize, PROT_READ | PROT_WRITE, MAP_SHARED, SM_fd, 0);
    if (funcparamstruct.md == MAP_FAILED) {
        close(SM_fd);
        perror("Error mmapping the file");
        exit(0);
    }
    //funcparamstruct->md = funcparammd;

    mapv = (char*) funcparamstruct.md;
    mapv += sizeof(FUNCTION_PARAMETER_STRUCT_MD);
    funcparamstruct.parray = (FUNCTION_PARAMETER*) mapv;



    printf("shared memory space = %ld bytes\n", sharedsize); //TEST


    funcparamstruct.md->NBparam = NBparam;

    for(index=0; index<NBparam; index++)
    {
        funcparamstruct.parray[index].status = 0; // not active
        funcparamstruct.parray[index].cnt0 = 0;   // update counter
    }

    strcpy(funcparamstruct.md->name, name);

    funcparamstruct.md->signal     = (uint64_t) FUNCTION_PARAMETER_STRUCT_SIGNAL_CONFRUN;
    funcparamstruct.md->confwaitus = (uint64_t) 1000; // 1 kHz default
    funcparamstruct.md->msgcnt = 0;

    munmap(funcparamstruct.md, sharedsize);


    return 0;
}







long function_parameter_struct_connect(
    char *name,
    FUNCTION_PARAMETER_STRUCT *fps
)
{
    char SM_fname[200];
    int SM_fd; // shared memory file descriptor
    int NBparam;
    char *mapv;

    snprintf(SM_fname, sizeof(SM_fname), "%s/%s.fps.shm", SHAREDMEMDIR, name);
    printf("File : %s\n", SM_fname);
    SM_fd = open(SM_fname, O_RDWR);
    if(SM_fd==-1)
    {
        printf("ERROR [%s %s %d]: cannot connect to %s\n", __FILE__, __func__, __LINE__, SM_fname);
        return(-1);
    }


    struct stat file_stat;
    fstat(SM_fd, &file_stat);


    fps->md = (FUNCTION_PARAMETER_STRUCT_MD*) mmap(0, file_stat.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, SM_fd, 0);
    if (fps->md == MAP_FAILED) {
        close(SM_fd);
        perror("Error mmapping the file");
        exit(0);
    }

    mapv = (char*) fps->md;
    mapv += sizeof(FUNCTION_PARAMETER_STRUCT_MD);
    fps->parray = (FUNCTION_PARAMETER*) mapv;

    //	NBparam = (int) (file_stat.st_size / sizeof(FUNCTION_PARAMETER));
    NBparam = fps->md->NBparam;
    printf("Connected to %s, %d entries\n", SM_fname, NBparam);
    fflush(stdout);


    // decompose full name into pname and indices
    int NBi = 0;
    char tmpstring[200];
    char tmpstring1[100];
    char *pch;

    strncpy(tmpstring, name, 200);
    NBi = -1;
    pch = strtok (tmpstring, "-");
    while (pch != NULL)
    {
        strncpy(tmpstring1, pch, 100);

        if(NBi==-1)
            strncpy(fps->md->pname, tmpstring1, 100);

        if((NBi>=0)&&(NBi<10))
            fps->md->nameindex[NBi] = atoi(tmpstring1);

        NBi++;
        pch = strtok (NULL, "-");
    }
    fps->md->NBnameindex = NBi;

    function_parameter_printlist(fps->parray, NBparam);

    return(NBparam);
}





int function_parameter_struct_disconnect(FUNCTION_PARAMETER_STRUCT *funcparamstruct)
{
    int NBparam;

    NBparam = funcparamstruct->md->NBparam;
    munmap(funcparamstruct, sizeof(FUNCTION_PARAMETER_STRUCT_MD)+sizeof(FUNCTION_PARAMETER)*NBparam);

    return(0);
}






//
// stand-alone function to set parameter value
//
int function_parameter_SetValue_int64(char *keywordfull, long val)
{
    FUNCTION_PARAMETER_STRUCT fps;
    char tmpstring[FUNCTION_PARAMETER_KEYWORD_STRMAXLEN*FUNCTION_PARAMETER_KEYWORD_MAXLEVEL];
    char keyword[FUNCTION_PARAMETER_KEYWORD_MAXLEVEL][FUNCTION_PARAMETER_KEYWORD_STRMAXLEN];
    int keywordlevel = 0;
    char *pch;


    // break full keyword into keywords
    strncpy(tmpstring, keywordfull, FUNCTION_PARAMETER_KEYWORD_STRMAXLEN*FUNCTION_PARAMETER_KEYWORD_MAXLEVEL);
    keywordlevel = 0;
    pch = strtok (tmpstring, ".");
    while (pch != NULL)
    {
        strncpy(keyword[keywordlevel], pch, FUNCTION_PARAMETER_KEYWORD_STRMAXLEN);
        keywordlevel++;
        pch = strtok (NULL, ".");
    }

    function_parameter_struct_connect(keyword[9], &fps);

    int pindex = functionparameter_GetParamIndex(&fps, keywordfull);


    fps.parray[pindex].val.l[0] = val;

    function_parameter_struct_disconnect(&fps);

    return 0;
}









int function_parameter_printlist(
    FUNCTION_PARAMETER  *funcparamarray,
    int NBparam
)
{
    int pindex = 0;
    int pcnt = 0;

    printf("\n");
    for(pindex=0; pindex<NBparam; pindex++)
    {
        if(funcparamarray[pindex].status & FUNCTION_PARAMETER_STATUS_ACTIVE)
        {
            int kl;

            printf("Parameter %4d : %s\n", pindex, funcparamarray[pindex].keywordfull);
            /*for(kl=0; kl< funcparamarray[pindex].keywordlevel; kl++)
            	printf("  %s", funcparamarray[pindex].keyword[kl]);
            printf("\n");*/
            printf("    %s\n", funcparamarray[pindex].description);

            // STATUS FLAGS
            printf("    STATUS FLAGS (0x%02hhx) :", (int) funcparamarray[pindex].status);
            if(funcparamarray[pindex].status & FUNCTION_PARAMETER_STATUS_ACTIVE)
                printf(" ACTIVE");
            if(funcparamarray[pindex].status & FUNCTION_PARAMETER_STATUS_USED)
                printf(" USED");
            if(funcparamarray[pindex].status & FUNCTION_PARAMETER_STATUS_VISIBLE)
                printf(" VISIBLE");
            if(funcparamarray[pindex].status & FUNCTION_PARAMETER_STATUS_WRITE)
                printf(" WRITE");
            if(funcparamarray[pindex].status & FUNCTION_PARAMETER_STATUS_WRITECONF)
                printf(" WRITECONF");
            if(funcparamarray[pindex].status & FUNCTION_PARAMETER_STATUS_WRITERUN)
                printf(" WRITERUN");
            if(funcparamarray[pindex].status & FUNCTION_PARAMETER_STATUS_LOG)
                printf(" LOG");
            if(funcparamarray[pindex].status & FUNCTION_PARAMETER_STATUS_SAVEONCHANGE)
                printf(" SAVEONCHANGE");
            if(funcparamarray[pindex].status & FUNCTION_PARAMETER_STATUS_SAVEONCLOSE)
                printf(" SAVEONCLOSE");
            if(funcparamarray[pindex].status & FUNCTION_PARAMETER_STATUS_MINLIMIT)
                printf(" MINLIMIT");
            if(funcparamarray[pindex].status & FUNCTION_PARAMETER_STATUS_MAXLIMIT)
                printf(" MAXLIMIT");
            if(funcparamarray[pindex].status & FUNCTION_PARAMETER_STATUS_CHECKSTREAM)
                printf(" CHECKSTREAM");
            if(funcparamarray[pindex].status & FUNCTION_PARAMETER_STATUS_IMPORTED)
                printf(" IMPORTED");
            if(funcparamarray[pindex].status & FUNCTION_PARAMETER_STATUS_CURRENT)
                printf(" CURRENT");
            if(funcparamarray[pindex].status & FUNCTION_PARAMETER_STATUS_ERROR)
                printf(" ERROR");
            if(funcparamarray[pindex].status & FUNCTION_PARAMETER_STATUS_ONOFF)
                printf(" ONOFF");
            printf("\n");

            // DATA TYPE
            //			printf("    TYPE : 0x%02hhx\n", (int) funcparamarray[pindex].type);
            if(funcparamarray[pindex].type & FUNCTION_PARAMETER_TYPE_UNDEF)
                printf("    TYPE = UNDEF\n");
            if(funcparamarray[pindex].type & FUNCTION_PARAMETER_TYPE_INT64)
            {
                printf("    TYPE  = INT64\n");
                printf("    VALUE = %ld\n", (long) funcparamarray[pindex].val.l[0]);
            }
            if(funcparamarray[pindex].type & FUNCTION_PARAMETER_TYPE_FLOAT64)
                printf("    TYPE = FLOAT64\n");
            if(funcparamarray[pindex].type & FUNCTION_PARAMETER_TYPE_PID)
                printf("    TYPE = PID\n");
            if(funcparamarray[pindex].type & FUNCTION_PARAMETER_TYPE_TIMESPEC)
                printf("    TYPE = TIMESPEC\n");
            if(funcparamarray[pindex].type & FUNCTION_PARAMETER_TYPE_FILENAME)
                printf("    TYPE = FILENAME\n");
            if(funcparamarray[pindex].type & FUNCTION_PARAMETER_TYPE_DIRNAME)
                printf("    TYPE = DIRNAME\n");
            if(funcparamarray[pindex].type & FUNCTION_PARAMETER_TYPE_STREAMNAME)
                printf("    TYPE = STREAMNAME\n");
            if(funcparamarray[pindex].type & FUNCTION_PARAMETER_TYPE_STRING)
                printf("    TYPE = STRING\n");
            if(funcparamarray[pindex].type & FUNCTION_PARAMETER_TYPE_ONOFF)
                printf("    TYPE = ONOFF\n"); 

            pcnt ++;
        }
    }
    printf("\n");
    printf("%d parameters\n", pcnt);
    printf("\n");

    return 0;
}




int functionparameter_GetFileName(FUNCTION_PARAMETER *fparam, char *outfname)
{
    char fname[500];
    char fname1[500];
    char command[1000];
    int ret;
    int l;


    sprintf(fname, "./fpconf");
    sprintf(command, "mkdir -p %s", fname);
    ret = system(command);

    for(l=0; l<fparam->keywordlevel-1; l++)
    {
        sprintf(fname1, "/%s", fparam->keyword[l]);
        strcat(fname, fname1);
        sprintf(command, "mkdir -p %s", fname);
        ret = system(command);
    }

    sprintf(fname1, "/%s.txt", fparam->keyword[l]);
    strcat(fname, fname1);
    strcpy(outfname, fname);

    return 0;
}





int functionparameter_GetParamIndex(
    FUNCTION_PARAMETER_STRUCT *fps,
    char *paramname
)
{
    int index = -1;
    int pindex = 0;
    int pcnt = 0;

    int NBparam = fps->md->NBparam;

    int found = 0;
    for(pindex=0; pindex<NBparam; pindex++)
    {
        if(found==0)
        {
            if(fps->parray[pindex].status & FUNCTION_PARAMETER_STATUS_ACTIVE)
            {
                if(strstr(fps->parray[pindex].keywordfull, paramname) != NULL)
                {
                    index = pindex;
                    found = 1;
                }
            }
        }
    }

    if (index == -1)
    {
        printf("ERROR: cannot find parameter \"%s\" in structure\n", paramname);
        exit(0);
    }

    return index;
}







/**
 * ## Purpose
 *
 * Add parameter to database with default settings
 *
 * If entry already exists, do not modify it
 *
 */

int function_parameter_add_entry(
    FUNCTION_PARAMETER  *funcparamarray,
    char                *keywordstring,
    char                *descriptionstring,
    uint64_t             type,
    int                  NBparam,
    void *               valueptr
)
{
    int pindex = 0;
    char *pch;
    char tmpstring[FUNCTION_PARAMETER_KEYWORD_STRMAXLEN*FUNCTION_PARAMETER_KEYWORD_MAXLEVEL];



    // scan for existing keyword
    int scanOK = 0;
    int pindexscan;
    for(pindexscan=0; pindexscan<NBparam; pindexscan++)
    {
        if(strcmp(keywordstring,funcparamarray[pindexscan].keywordfull)==0)
        {
            pindex = pindexscan;
            scanOK = 1;
        }
    }

    if(scanOK==0) // not found
    {
        // scan for first available entry
        pindex = 0;
        while((funcparamarray[pindex].status & FUNCTION_PARAMETER_STATUS_ACTIVE)&&(pindex<NBparam))
            pindex++;

        if(pindex == NBparam)
        {
            printf("ERROR [%s line %d]: NBparam limit reached\n", __FILE__, __LINE__);
            fflush(stdout);
            exit(0);
        }
    }
    else
    {
        printf("Found matching keyword: applying values to existing entry\n");
    }


    funcparamarray[pindex].status |= FUNCTION_PARAMETER_STATUS_ACTIVE;
    funcparamarray[pindex].status |= FUNCTION_PARAMETER_STATUS_USED;
    funcparamarray[pindex].status |= FUNCTION_PARAMETER_STATUS_VISIBLE;
    funcparamarray[pindex].status |= FUNCTION_PARAMETER_STATUS_WRITE;
    funcparamarray[pindex].status |= FUNCTION_PARAMETER_STATUS_WRITECONF;
    funcparamarray[pindex].status |= FUNCTION_PARAMETER_STATUS_SAVEONCHANGE;


    // break full keyword into keywords
    strncpy(funcparamarray[pindex].keywordfull, keywordstring, FUNCTION_PARAMETER_KEYWORD_STRMAXLEN*FUNCTION_PARAMETER_KEYWORD_MAXLEVEL);
    strncpy(tmpstring, keywordstring, FUNCTION_PARAMETER_KEYWORD_STRMAXLEN*FUNCTION_PARAMETER_KEYWORD_MAXLEVEL);
    funcparamarray[pindex].keywordlevel = 0;
    pch = strtok (tmpstring, ".");
    while (pch != NULL)
    {
        strncpy(funcparamarray[pindex].keyword[funcparamarray[pindex].keywordlevel], pch, FUNCTION_PARAMETER_KEYWORD_STRMAXLEN);
        funcparamarray[pindex].keywordlevel++;
        pch = strtok (NULL, ".");
    }


    // Write description
    strncpy(funcparamarray[pindex].description, descriptionstring, FUNCTION_PARAMETER_DESCR_STRMAXLEN);

    // type
    funcparamarray[pindex].type = type;



    // Allocate value
    funcparamarray[pindex].cnt0 = 0; // not allocated

    // Default values
    switch (funcparamarray[pindex].type) {
    case FUNCTION_PARAMETER_TYPE_INT64 :
        funcparamarray[pindex].val.l[0] = 0;
        funcparamarray[pindex].val.l[1] = 0;
        funcparamarray[pindex].val.l[2] = 0;
        funcparamarray[pindex].val.l[3] = 0;
        break;
    case FUNCTION_PARAMETER_TYPE_FLOAT64 :
        funcparamarray[pindex].val.f[0] = 0.0;
        funcparamarray[pindex].val.f[1] = 0.0;
        funcparamarray[pindex].val.f[2] = 0.0;
        funcparamarray[pindex].val.f[3] = 0.0;
        break;
    case FUNCTION_PARAMETER_TYPE_PID :
        funcparamarray[pindex].val.pid[0] = 0;
        funcparamarray[pindex].val.pid[1] = 0;
        break;
    case FUNCTION_PARAMETER_TYPE_TIMESPEC :
        funcparamarray[pindex].val.ts[0].tv_sec = 0;
        funcparamarray[pindex].val.ts[0].tv_nsec = 0;
        funcparamarray[pindex].val.ts[1].tv_sec = 0;
        funcparamarray[pindex].val.ts[1].tv_nsec = 0;
        break;
    case FUNCTION_PARAMETER_TYPE_FILENAME :
        sprintf(funcparamarray[pindex].val.string[0], "NULL");
        sprintf(funcparamarray[pindex].val.string[1], "NULL");
        break;
    case FUNCTION_PARAMETER_TYPE_DIRNAME :
        sprintf(funcparamarray[pindex].val.string[0], "NULL");
        sprintf(funcparamarray[pindex].val.string[1], "NULL");
        break;
    case FUNCTION_PARAMETER_TYPE_STREAMNAME :
        sprintf(funcparamarray[pindex].val.string[0], "NULL");
        sprintf(funcparamarray[pindex].val.string[1], "NULL");
        break;
    case FUNCTION_PARAMETER_TYPE_STRING :
        sprintf(funcparamarray[pindex].val.string[0], "NULL");
        sprintf(funcparamarray[pindex].val.string[1], "NULL");
        break;
    }

    if(valueptr == NULL)
    {
        // attempt to read value for filesystem
        char fname[200];
        FILE *fp;


        functionparameter_GetFileName(&funcparamarray[pindex], fname);
        //sprintf(fname, "FPCONF/%s", funcparamarray[pindex].keywordfull);
        printf(" FILE NAME = %s\n", fname);
        fflush(stdout);

        if ( (fp = fopen(fname, "r")) != NULL)
        {
            switch (funcparamarray[pindex].type) {
            case FUNCTION_PARAMETER_TYPE_INT64 :
                if ( fscanf(fp, "%ld", &funcparamarray[pindex].val.l[0]) == 1)
                    funcparamarray[pindex].cnt0++;
                break;
            case FUNCTION_PARAMETER_TYPE_FLOAT64 :
                if ( fscanf(fp, "%lf", &funcparamarray[pindex].val.f[0]) == 1)
                    funcparamarray[pindex].cnt0++;
                break;
            case FUNCTION_PARAMETER_TYPE_PID :
                if ( fscanf(fp, "%d", &funcparamarray[pindex].val.pid[0]) == 1)
                    funcparamarray[pindex].cnt0++;
                break;
            case FUNCTION_PARAMETER_TYPE_TIMESPEC :
                if ( fscanf(fp, "%ld %ld", &funcparamarray[pindex].val.ts[0].tv_sec, &funcparamarray[pindex].val.ts[0].tv_nsec) == 2)
                    funcparamarray[pindex].cnt0++;
                break;
            case FUNCTION_PARAMETER_TYPE_FILENAME :
                if ( fscanf(fp, "%s", funcparamarray[pindex].val.string[0]) == 1)
                    funcparamarray[pindex].cnt0++;
                break;
            case FUNCTION_PARAMETER_TYPE_DIRNAME :
                if ( fscanf(fp, "%s", funcparamarray[pindex].val.string[0]) == 1)
                    funcparamarray[pindex].cnt0++;
                break;
            case FUNCTION_PARAMETER_TYPE_STREAMNAME :
                if ( fscanf(fp, "%s", funcparamarray[pindex].val.string[0]) == 1)
                    funcparamarray[pindex].cnt0++;
                break;
            case FUNCTION_PARAMETER_TYPE_STRING :
                if ( fscanf(fp, "%s", funcparamarray[pindex].val.string[0]) == 1)
                    funcparamarray[pindex].cnt0++;
                break;
            }
            fclose(fp);
        }
    }
    else // allocate value requested by function call
    {
        int64_t *valueptr_INT64;
        double *valueptr_FLOAT64;
        struct timespec *valueptr_ts;

        switch (funcparamarray[pindex].type) {

        case FUNCTION_PARAMETER_TYPE_INT64 :
            valueptr_INT64 = (int64_t *) valueptr;
            funcparamarray[pindex].val.l[0] = *valueptr_INT64;
            funcparamarray[pindex].cnt0++;
            break;

        case FUNCTION_PARAMETER_TYPE_FLOAT64 :
            valueptr_FLOAT64 = (double *) valueptr;
            funcparamarray[pindex].val.f[0] = *valueptr_FLOAT64;
            funcparamarray[pindex].cnt0++;
            break;

        case FUNCTION_PARAMETER_TYPE_PID :
            valueptr_INT64 = (int64_t *) valueptr;
            funcparamarray[pindex].val.pid[0] = (pid_t) (*valueptr_INT64);
            funcparamarray[pindex].cnt0++;
            break;

        case FUNCTION_PARAMETER_TYPE_TIMESPEC:
            valueptr_ts = (struct timespec *) valueptr;
            funcparamarray[pindex].val.ts[0] = *valueptr_ts;
            funcparamarray[pindex].cnt0++;
            break;

        case FUNCTION_PARAMETER_TYPE_FILENAME :
            strncpy(funcparamarray[pindex].val.string[0], (char*) valueptr,  FUNCTION_PARAMETER_STRMAXLEN);
            funcparamarray[pindex].cnt0++;
            break;

        case FUNCTION_PARAMETER_TYPE_DIRNAME :
            strncpy(funcparamarray[pindex].val.string[0], (char*) valueptr,  FUNCTION_PARAMETER_STRMAXLEN);
            funcparamarray[pindex].cnt0++;
            break;

        case FUNCTION_PARAMETER_TYPE_STREAMNAME :
            strncpy(funcparamarray[pindex].val.string[0], (char*) valueptr,  FUNCTION_PARAMETER_STRMAXLEN);
            funcparamarray[pindex].cnt0++;
            break;

        case FUNCTION_PARAMETER_TYPE_STRING :
            strncpy(funcparamarray[pindex].val.string[0], (char*) valueptr,  FUNCTION_PARAMETER_STRMAXLEN);
            funcparamarray[pindex].cnt0++;
            break;
        }
    }



    return pindex;
}







// ======================================== GUI FUNCTIONS =======================================



/**
 * INITIALIZE ncurses
 *
 */
static int initncurses()
{
    if ( initscr() == NULL ) {
        fprintf(stderr, "Error initialising ncurses.\n");
        exit(EXIT_FAILURE);
    }
    getmaxyx(stdscr, wrow, wcol);		/* get the number of rows and columns */
    cbreak();
    keypad(stdscr, TRUE);		/* We get F1, F2 etc..		*/
    nodelay(stdscr, TRUE);
    curs_set(0);
    noecho();			/* Don't echo() while we do getch */


    start_color();

    init_color(COLOR_GREEN, 400, 1000, 400);

    //  colored background
    init_pair(1, COLOR_BLACK, COLOR_WHITE);
    init_pair(2, COLOR_BLACK, COLOR_GREEN);  // all good
    init_pair(3, COLOR_BLACK, COLOR_YELLOW); // parameter out of sync
    init_pair(4, COLOR_WHITE, COLOR_RED);
    init_pair(5, COLOR_WHITE, COLOR_BLUE); // DIRECTORY
    init_pair(9, COLOR_BLACK, COLOR_RED);
    init_pair(10, COLOR_BLACK, COLOR_CYAN);

    init_pair(6, COLOR_GREEN, COLOR_BLACK);
    init_pair(7, COLOR_YELLOW, COLOR_BLACK);
    init_pair(8, COLOR_RED, COLOR_BLACK);

    return 0;
}





int functionparameter_WritePararameterToDisk(FUNCTION_PARAMETER_STRUCT *fpsentry, int pindex)
{
    char fname[200];
    FILE *fp;

    functionparameter_GetFileName(&(fpsentry->parray[pindex]), fname);

    printf("\nSaving to %s\n", fname);
    fflush(stdout);

    fp = fopen(fname, "w");

    switch (fpsentry->parray[pindex].type) {

    case FUNCTION_PARAMETER_TYPE_INT64:
        fprintf(fp, "%10ld   # Set value\n", fpsentry->parray[pindex].val.l[0]);
        fprintf(fp, "%10ld   # Min value\n", fpsentry->parray[pindex].val.l[1]);
        fprintf(fp, "%10ld   # Max value\n", fpsentry->parray[pindex].val.l[2]);
        fprintf(fp, "%10ld   # Current value\n", fpsentry->parray[pindex].val.l[3]);
        break;

    case FUNCTION_PARAMETER_TYPE_FLOAT64:
        fprintf(fp, "%18f   # Set value\n", fpsentry->parray[pindex].val.f[0]);
        fprintf(fp, "%18f   # Min value\n", fpsentry->parray[pindex].val.f[1]);
        fprintf(fp, "%18f   # Max value\n", fpsentry->parray[pindex].val.f[2]);
        fprintf(fp, "%18f   # Current value\n", fpsentry->parray[pindex].val.f[3]);
        break;

    case FUNCTION_PARAMETER_TYPE_PID:
        fprintf(fp, "%18ld   # Set value\n", (long) fpsentry->parray[pindex].val.pid[0]);
        break;

    case FUNCTION_PARAMETER_TYPE_TIMESPEC:
        fprintf(fp, "%15ld %09ld   # Set value\n", (long) fpsentry->parray[pindex].val.ts[0].tv_sec, (long) fpsentry->parray[pindex].val.ts[0].tv_nsec);
        break;

    case FUNCTION_PARAMETER_TYPE_FILENAME:
        fprintf(fp, "%s\n", fpsentry->parray[pindex].val.string[0]);
        break;

    case FUNCTION_PARAMETER_TYPE_DIRNAME:
        fprintf(fp, "%s\n", fpsentry->parray[pindex].val.string[0]);
        break;

    case FUNCTION_PARAMETER_TYPE_STREAMNAME:
        fprintf(fp, "%s\n", fpsentry->parray[pindex].val.string[0]);
        break;

    case FUNCTION_PARAMETER_TYPE_STRING:
        fprintf(fp, "%s\n", fpsentry->parray[pindex].val.string[0]);
        break;

    }

    fprintf(fp, "%10ld   # parameter status\n", (long) fpsentry->parray[pindex].status);
    fprintf(fp, "%10ld   # parameter type\n",  (long) fpsentry->parray[pindex].type);
    fprintf(fp, "%s\n", fpsentry->parray[pindex].description);
    fprintf(fp, "%10ld   # counter\n", fpsentry->parray[pindex].cnt0);

    fclose(fp);

    return 0;
}










int functionparameter_CheckParameter(FUNCTION_PARAMETER_STRUCT *fpsentry, int pindex)
{
    int err = 0;
    int msglen;
    char msgadd[200];

    // if entry is not active, no error reported
    if( !(fpsentry->parray[pindex].status & FUNCTION_PARAMETER_STATUS_ACTIVE ) )
    {
        return 0;
    }

    // if entry is not used, no error reported
    if( !(fpsentry->parray[pindex].status & FUNCTION_PARAMETER_STATUS_USED ) )
    {
        return 0;
    }


    if(fpsentry->parray[pindex].cnt0 == 0)
    {
        msglen = strlen(fpsentry->md->message);
        sprintf(msgadd, "%s not initialized\n", fpsentry->parray[pindex].keywordfull);
        strncat(fpsentry->md->message, msgadd, FUNCTION_PARAMETER_STRUCT_MSG_SIZE-msglen-1);
        err = 1;
    }

    if(err == 0)
    {
        // Check min value
        if( fpsentry->parray[pindex].type & FUNCTION_PARAMETER_TYPE_INT64 )
            if( fpsentry->parray[pindex].status & FUNCTION_PARAMETER_STATUS_MINLIMIT )
                if ( fpsentry->parray[pindex].val.l[0] < fpsentry->parray[pindex].val.l[1] )
                {
                    msglen = strlen(fpsentry->md->message);
                    sprintf(msgadd, "%s value below min limit\n", fpsentry->parray[pindex].keywordfull);
                    strncat(fpsentry->md->message, msgadd, FUNCTION_PARAMETER_STRUCT_MSG_SIZE-msglen-1);
                    err = 1;
                }

        if( fpsentry->parray[pindex].type & FUNCTION_PARAMETER_TYPE_FLOAT64 )
            if( fpsentry->parray[pindex].status & FUNCTION_PARAMETER_STATUS_MINLIMIT )
                if ( fpsentry->parray[pindex].val.f[0] < fpsentry->parray[pindex].val.f[1] )
                {
                    msglen = strlen(fpsentry->md->message);
                    sprintf(msgadd, "%s value below min limit\n", fpsentry->parray[pindex].keywordfull);
                    strncat(fpsentry->md->message, msgadd, FUNCTION_PARAMETER_STRUCT_MSG_SIZE-msglen-1);
                    err = 1;
                }
    }

    if(err == 0)
    {
        // Check max value
        if( fpsentry->parray[pindex].type & FUNCTION_PARAMETER_TYPE_INT64 )
            if( fpsentry->parray[pindex].status & FUNCTION_PARAMETER_STATUS_MAXLIMIT )
                if ( fpsentry->parray[pindex].val.l[0] > fpsentry->parray[pindex].val.l[2] )
                {
                    msglen = strlen(fpsentry->md->message);
                    sprintf(msgadd, "%s value above max limit\n", fpsentry->parray[pindex].keywordfull);
                    strncat(fpsentry->md->message, msgadd, FUNCTION_PARAMETER_STRUCT_MSG_SIZE-msglen-1);
                    err = 1;
                }

        if( fpsentry->parray[pindex].type & FUNCTION_PARAMETER_TYPE_FLOAT64 )
            if( fpsentry->parray[pindex].status & FUNCTION_PARAMETER_STATUS_MAXLIMIT )
                if ( fpsentry->parray[pindex].val.f[0] > fpsentry->parray[pindex].val.f[2] )
                {
                    msglen = strlen(fpsentry->md->message);
                    sprintf(msgadd, "%s value above max limit\n", fpsentry->parray[pindex].keywordfull);
                    strncat(fpsentry->md->message, msgadd, FUNCTION_PARAMETER_STRUCT_MSG_SIZE-msglen-1);
                    err = 1;
                }
    }

    if (err == 1)
        fpsentry->parray[pindex].status |= FUNCTION_PARAMETER_STATUS_ERROR;
    else
        fpsentry->parray[pindex].status &= ~FUNCTION_PARAMETER_STATUS_ERROR;

    return err;
}







int functionparameter_CheckParametersAll(FUNCTION_PARAMETER_STRUCT *fpsentry)
{
    int NBparam;
    int pindex;
    int errcnt = 0;


    strcpy(fpsentry->md->message, "\0");
    NBparam = fpsentry->md->NBparam;

    printf("Checking %d parameter entries\n", NBparam);
    for(pindex=0; pindex<NBparam; pindex++)
        errcnt += functionparameter_CheckParameter(fpsentry, pindex);

    fpsentry->md->errcnt = errcnt;
    fpsentry->md->msgcnt ++;

    if(errcnt==0)
        fpsentry->md->status |= FUNCTION_PARAMETER_STRUCT_STATUS_CHECKOK;
    else
        fpsentry->md->status &= ~FUNCTION_PARAMETER_STRUCT_STATUS_CHECKOK;

    return 0;
}













int functionparameter_PrintParameterInfo(FUNCTION_PARAMETER_STRUCT *fpsentry, int pindex)
{
    printf("%s\n", fpsentry->parray[pindex].description);
    printf("\n");


    printf("------------- FUNCTION PARAMETER STRUCTURE\n");
    printf("FPS name       : %s\n", fpsentry->md->name);
    printf("   %s ", fpsentry->md->pname);
    int i;
    for(i=0; i< fpsentry->md->NBnameindex; i++)
        printf(" [%02d]", fpsentry->md->nameindex[i]);
    printf("\n\n");

    if( fpsentry->md->status & FUNCTION_PARAMETER_STRUCT_STATUS_CHECKOK )
    {
        printf("[%ld] %sScan OK%s\n", fpsentry->md->msgcnt, BLINKHIGREEN, RESET);
    }
    else
    {
        printf("[%ld] %s%ld ERROR(s)%s\n", fpsentry->md->msgcnt, BLINKHIRED, fpsentry->md->errcnt, RESET);
        printf("%s%s%s\n", BOLDHIRED, fpsentry->md->message, RESET);
    }

    printf("\n");



    printf("------------- FUNCTION PARAMETER \n");
    printf("[%d] Parameter name : %s\n", pindex, fpsentry->parray[pindex].keywordfull);

    if( fpsentry->parray[pindex].type & FUNCTION_PARAMETER_TYPE_UNDEF )
        printf("  TYPE UNDEF\n");
    if( fpsentry->parray[pindex].type & FUNCTION_PARAMETER_TYPE_INT64 )
        printf("  TYPE INT64\n");
    if( fpsentry->parray[pindex].type & FUNCTION_PARAMETER_TYPE_FLOAT64 )
        printf("  TYPE FLOAT64\n");
    if( fpsentry->parray[pindex].type & FUNCTION_PARAMETER_TYPE_PID )
        printf("  TYPE PID\n");
    if( fpsentry->parray[pindex].type & FUNCTION_PARAMETER_TYPE_TIMESPEC )
        printf("  TYPE TIMESPEC\n");
    if( fpsentry->parray[pindex].type & FUNCTION_PARAMETER_TYPE_FILENAME )
        printf("  TYPE FILENAME\n");
    if( fpsentry->parray[pindex].type & FUNCTION_PARAMETER_TYPE_DIRNAME )
        printf("  TYPE DIRNAME\n");
    if( fpsentry->parray[pindex].type & FUNCTION_PARAMETER_TYPE_STREAMNAME )
        printf("  TYPE STREAMNAME\n");
    if( fpsentry->parray[pindex].type & FUNCTION_PARAMETER_TYPE_STRING )
        printf("  TYPE STRING\n");
    if( fpsentry->parray[pindex].type & FUNCTION_PARAMETER_TYPE_UNDEF )
        printf("  TYPE UNDEF\n");
    if( fpsentry->parray[pindex].type & FUNCTION_PARAMETER_TYPE_UNDEF )
        printf("  TYPE UNDEF\n");

    printf("\n");
    printf("------------- FLAGS \n");

    if( fpsentry->parray[pindex].status & FUNCTION_PARAMETER_STATUS_ACTIVE )
        printf("   FUNCTION_PARAMETER_STATUS_ACTIVE\n");
    if( fpsentry->parray[pindex].status & FUNCTION_PARAMETER_STATUS_VISIBLE )
        printf("   FUNCTION_PARAMETER_STATUS_VISIBLE\n");
    if( fpsentry->parray[pindex].status & FUNCTION_PARAMETER_STATUS_WRITECONF )
        printf("   FUNCTION_PARAMETER_STATUS_WRITECONF\n");
    if( fpsentry->parray[pindex].status & FUNCTION_PARAMETER_STATUS_WRITERUN )
        printf("   FUNCTION_PARAMETER_STATUS_WRITERUN\n");
    if( fpsentry->parray[pindex].status & FUNCTION_PARAMETER_STATUS_LOG )
        printf("   FUNCTION_PARAMETER_STATUS_LOG\n");
    if( fpsentry->parray[pindex].status & FUNCTION_PARAMETER_STATUS_SAVEONCHANGE )
        printf("   FUNCTION_PARAMETER_STATUS_SAVEONCHANGE\n");
    if( fpsentry->parray[pindex].status & FUNCTION_PARAMETER_STATUS_SAVEONCLOSE )
        printf("   FUNCTION_PARAMETER_STATUS_SAVEONCLOSE\n");
    if( fpsentry->parray[pindex].status & FUNCTION_PARAMETER_STATUS_MINLIMIT )
        printf("   FUNCTION_PARAMETER_STATUS_MINLIMIT\n");
    if( fpsentry->parray[pindex].status & FUNCTION_PARAMETER_STATUS_MAXLIMIT )
        printf("   FUNCTION_PARAMETER_STATUS_MAXLIMIT\n");
    if( fpsentry->parray[pindex].status & FUNCTION_PARAMETER_STATUS_CHECKSTREAM )
        printf("   FUNCTION_PARAMETER_STATUS_CHECKSTREAM\n");
    if( fpsentry->parray[pindex].status & FUNCTION_PARAMETER_STATUS_IMPORTED )
        printf("   FUNCTION_PARAMETER_STATUS_IMPORTED\n");
    if( fpsentry->parray[pindex].status & FUNCTION_PARAMETER_STATUS_CURRENT )
        printf("   FUNCTION_PARAMETER_STATUS_CURRENT\n");

    printf("\n");
    printf("\n");
    printf("cnt0 = %ld\n", fpsentry->parray[pindex].cnt0);

    printf("\n");

    printf("Current value : ");

    if(fpsentry->parray[pindex].type & FUNCTION_PARAMETER_TYPE_UNDEF)
        printf("  %s", "-undef-");

    if(fpsentry->parray[pindex].type & FUNCTION_PARAMETER_TYPE_INT64)
        printf("  %10d", (int) fpsentry->parray[pindex].val.l[0]);

    if(fpsentry->parray[pindex].type & FUNCTION_PARAMETER_TYPE_FLOAT64)
        printf("  %10f", (float) fpsentry->parray[pindex].val.f[0]);

    if(fpsentry->parray[pindex].type & FUNCTION_PARAMETER_TYPE_PID)
        printf("  %10d", (int) fpsentry->parray[pindex].val.pid[0]);

    if(fpsentry->parray[pindex].type & FUNCTION_PARAMETER_TYPE_TIMESPEC)
        printf("  %10s", "-timespec-");

    if(fpsentry->parray[pindex].type & FUNCTION_PARAMETER_TYPE_FILENAME)
        printf("  %10s", fpsentry->parray[pindex].val.string[0]);

    if(fpsentry->parray[pindex].type & FUNCTION_PARAMETER_TYPE_DIRNAME)
        printf("  %10s", fpsentry->parray[pindex].val.string[0]);

    if(fpsentry->parray[pindex].type & FUNCTION_PARAMETER_TYPE_STREAMNAME)
        printf("  %10s", fpsentry->parray[pindex].val.string[0]);

    if(fpsentry->parray[pindex].type & FUNCTION_PARAMETER_TYPE_STRING)
        printf("  %10s", fpsentry->parray[pindex].val.string[0]);

    printf("\n");
    printf("\n");

    return 0;

}





/**
 *
 * ## PURPOSE
 *
 * Enter new value for parameter
 *
 *
 */


int functionparameter_UserInputSetParamValue(FUNCTION_PARAMETER_STRUCT *fpsentry, int pindex)
{
    int inputOK;
    int strlenmax = 20;
    char buff[100];
    char c;

    functionparameter_PrintParameterInfo(fpsentry, pindex);

    int writeOK; // do we have write permission ?

    // by default, adopt FUNCTION_PARAMETER_STATUS_WRITE flag
    if ( fpsentry->parray[pindex].status & FUNCTION_PARAMETER_STATUS_WRITE )
        writeOK = 1;
    else
        writeOK = 0;
    printf("     -> %d\n", writeOK);


    // if CONF running
    if( fpsentry->md->status & FUNCTION_PARAMETER_STRUCT_STATUS_CONF )
    {
        printf("CONF -> ");
        if ( fpsentry->parray[pindex].status & FUNCTION_PARAMETER_STATUS_WRITECONF )
            writeOK = 1;
        else
            writeOK = 0;
        printf("%d\n", writeOK);
    }

    // if RUN running
    if( fpsentry->md->status & FUNCTION_PARAMETER_STRUCT_STATUS_RUN )
    {
        printf("RUN  -> ");
        if ( fpsentry->parray[pindex].status & FUNCTION_PARAMETER_STATUS_WRITERUN )
            writeOK = 1;
        else
            writeOK = 0;
        printf("%d\n", writeOK);
    }

    if(writeOK == 0)
    {
        printf("%s Value cannot be modified %s\n", BOLDHIRED, RESET);
        c = getchar();
    }
    else
    {
        inputOK = 0;
        fflush(stdout);

        while(inputOK == 0)
        {
            printf ("\nESC or update value : ");
            fflush(stdout);

            int stringindex = 0;
            c = getchar();
            while( (c != 27) && (c != 10) && (c != 13) && (stringindex<strlenmax-1) )
            {
                buff[stringindex] = c;
                if(c == 127) // delete key
                {
                    putchar (0x8);
                    putchar (' ');
                    putchar (0x8);
                    stringindex --;
                }
                else
                {
                    putchar(c);  // echo on screen
                    // printf("[%d]", (int) c);
                    stringindex++;
                }
                if(stringindex<0)
                    stringindex = 0;
                c = getchar();
            }
            buff[stringindex] = '\0';
            inputOK = 1;
        }

        if(c != 27) // do not update value if escape key
        {

            long lval;
            double fval;
            char *endptr;
            char c;
            int vOK = 1;

            switch (fpsentry->parray[pindex].type) {

            case FUNCTION_PARAMETER_TYPE_INT64:
                errno = 0;    /* To distinguish success/failure after call */
                lval = strtol(buff, &endptr, 10);

                /* Check for various possible errors */
                if ((errno == ERANGE && (lval == LONG_MAX || lval == LONG_MIN))
                        || (errno != 0 && lval == 0)) {
                    perror("strtol");
                    vOK = 0;
                    sleep(1);
                }

                if (endptr == buff) {
                    fprintf(stderr, "\nERROR: No digits were found\n");
                    vOK = 0;
                    sleep(1);
                }

                if(vOK == 1)
                    fpsentry->parray[pindex].val.l[0] = lval;
                break;

            case FUNCTION_PARAMETER_TYPE_FLOAT64:
                errno = 0;    /* To distinguish success/failure after call */
                fval = strtod(buff, &endptr);

                /* Check for various possible errors */
                if ( (errno == ERANGE)
                        || (errno != 0 && lval == 0)) {
                    perror("strtod");
                    vOK = 0;
                    sleep(1);
                }

                if (endptr == buff) {
                    fprintf(stderr, "\nERROR: No digits were found\n");
                    vOK = 0;
                    sleep(1);
                }

                if(vOK == 1)
                    fpsentry->parray[pindex].val.f[0] = fval;
                break;


            case FUNCTION_PARAMETER_TYPE_PID :
                errno = 0;    /* To distinguish success/failure after call */
                lval = strtol(buff, &endptr, 10);

                /* Check for various possible errors */
                if ((errno == ERANGE && (lval == LONG_MAX || lval == LONG_MIN))
                        || (errno != 0 && lval == 0)) {
                    perror("strtol");
                    vOK = 0;
                    sleep(1);
                }

                if (endptr == buff) {
                    fprintf(stderr, "\nERROR: No digits were found\n");
                    vOK = 0;
                    sleep(1);
                }

                if(vOK == 1)
                    fpsentry->parray[pindex].val.pid[0] = (pid_t) lval;
                break;


            case FUNCTION_PARAMETER_TYPE_FILENAME :
                snprintf(fpsentry->parray[pindex].val.string[0], FUNCTION_PARAMETER_STRMAXLEN, "%s", buff);
                break;

            case FUNCTION_PARAMETER_TYPE_DIRNAME :
                snprintf(fpsentry->parray[pindex].val.string[0], FUNCTION_PARAMETER_STRMAXLEN, "%s", buff);
                break;

            case FUNCTION_PARAMETER_TYPE_STREAMNAME :
                snprintf(fpsentry->parray[pindex].val.string[0], FUNCTION_PARAMETER_STRMAXLEN, "%s", buff);
                break;

            case FUNCTION_PARAMETER_TYPE_STRING :
                snprintf(fpsentry->parray[pindex].val.string[0], FUNCTION_PARAMETER_STRMAXLEN, "%s", buff);
                break;


            }

            fpsentry->parray[pindex].cnt0++;

            // notify GUI
            fpsentry->md->signal |= FUNCTION_PARAMETER_STRUCT_SIGNAL_UPDATE;


            // Save to disk
            if( fpsentry->parray[pindex].status & FUNCTION_PARAMETER_STATUS_SAVEONCHANGE)
            {
                functionparameter_WritePararameterToDisk(fpsentry, pindex);
            }
        }
    }



    return 0;
}







/**
 * ## Purpose
 *
 * Automatically build simple ASCII GUI from function parameter structure (fps) name mask
 *
 *
 *
 */
int_fast8_t functionparameter_CTRLscreen(char *fpsnamemask)
{
    // function parameter structure(s)
    int NBfps;
    int fpsindex;
    FUNCTION_PARAMETER_STRUCT fps[NB_FPS_MAX];
    int fps_symlink[NB_FPS_MAX];

    // display index
    long NBindex;

    // function parameters
    long NBpindex = 0;
    long pindex;
    int *p_fpsindex; // fps index for parameter
    int *p_pindex;   // index within fps

    // keyword tree
    int kwnindex;
    int NBkwn = 0;
    KEYWORD_TREE_NODE keywnode[100];

    int l;

    char  monstring[200];
    int loopOK = 1;
    long long loopcnt = 0;


    int nodechain[MAXNBLEVELS];

    int iSelected[MAXNBLEVELS];


    // current selection
    int fpsindexSelected = 0;
    int pindexSelected = 0;

    for(l=0; l<MAXNBLEVELS; l++)
    {
        nodechain[l] = 0;
        iSelected[l] = 0;
    }


    //    NBparam = function_parameter_struct_connect(fpsname, &fps[fpsindex]);


    // create ROOT node (invisible)
    keywnode[0].keywordlevel = 0;
    sprintf(keywnode[0].keyword[0], "ROOT");
    keywnode[0].leaf = 0;
    keywnode[0].NBchild = 0;
    NBkwn = 1;


    // scan filesystem for fps entries
    DIR *d;
    struct dirent *dir;


    d = opendir("/tmp/");
    if(d)
    {
        fpsindex = 0;
        pindex = 0;
        while(((dir = readdir(d)) != NULL))
        {
            char *pch = strstr(dir->d_name, ".fps.shm");

            int matchOK = 0;
            // name filtering
            if(strcmp(fpsnamemask, "ALL") == 0)
                matchOK = 1;
            else
            {
                if(strncmp(dir->d_name, fpsnamemask, strlen(fpsnamemask)) == 0)
                    matchOK = 1;
            }


            if((pch) && (matchOK == 1))
            {
                // is file sym link ?
                struct stat buf;
                int retv;
                char fullname[200];

                sprintf(fullname, "/tmp/%s", dir->d_name);
                retv = lstat (fullname, &buf);
                if (retv == -1 ) {
                    endwin();
                    printf("File \"%s\"", dir->d_name);
                    perror("Error running lstat on file ");
                    exit(0);
                }

                if (S_ISLNK(buf.st_mode)) // resolve link name
                {
                    char fullname[200];
                    char linknamefull[200];
                    char linkname[200];
                    int nchar;
                    int ret;

                    fps_symlink[fpsindex] = 1;
                    sprintf(fullname, "/tmp/%s", dir->d_name);
                    ret = readlink (fullname, linknamefull, 200-1);

                    strcpy(linkname, basename(linknamefull));

                    int lOK = 1;
                    int ii = 0;
                    while((lOK == 1)&&(ii<strlen(linkname)))
                    {
                        if(linkname[ii] == '.')
                        {
                            linkname[ii] = '\0';
                            lOK = 0;
                        }
                        ii++;
                    }

                    //                        strncpy(streaminfo[sindex].linkname, linkname, nameNBchar);
                }
                else
                    fps_symlink[fpsindex] = 0;


                char fpsname[200];
                strncpy(fpsname, dir->d_name, strlen(dir->d_name)-strlen(".fps.shm"));

                int NBparamMAX = function_parameter_struct_connect(fpsname, &fps[fpsindex]);
                int i;
                for(i=0; i<NBparamMAX; i++)
                {
                    if(fps[fpsindex].parray[i].status & FUNCTION_PARAMETER_STATUS_ACTIVE)  // if entry is active
                    {
                        // find or allocate keyword node
                        int level;
                        for(level=1; level < fps[fpsindex].parray[i].keywordlevel+1; level++)
                        {

                            // does node already exist ?
                            int scanOK = 0;
                            for(kwnindex=0; kwnindex<NBkwn; kwnindex++) // scan existing nodes looking for match
                            {
                                if(keywnode[kwnindex].keywordlevel == level) // levels have to match
                                {
                                    int match = 1;
                                    for(l=0; l<level; l++) // keywords at all levels need to match
                                    {
                                        if( strcmp(fps[fpsindex].parray[i].keyword[l], keywnode[kwnindex].keyword[l]) != 0 )
                                            match = 0;
                                    }
                                    if(match == 1) // we have a match
                                        scanOK = 1;
                                }
                            }



                            if(scanOK == 0) // node does not exit -> create it
                            {

                                // look for parent
                                int scanparentOK = 0;
                                int kwnindexp = 0;
                                keywnode[kwnindex].parent_index = 0; // default value, not found -> assigned to ROOT

                                while ((kwnindexp<NBkwn) && (scanparentOK==0))
                                {
                                    if(keywnode[kwnindexp].keywordlevel == level-1) // check parent has level-1
                                    {
                                        int match = 1;

                                        for(l=0; l<level-1; l++) // keywords at all levels need to match
                                        {
                                            if( strcmp(fps[fpsindex].parray[i].keyword[l], keywnode[kwnindexp].keyword[l]) != 0 )
                                                match = 0;
                                        }
                                        if(match == 1) // we have a match
                                            scanparentOK = 1;
                                    }
                                    kwnindexp++;
                                }

                                if(scanparentOK == 1)
                                {
                                    keywnode[kwnindex].parent_index = kwnindexp-1;
                                    int cindex;
                                    cindex = keywnode[keywnode[kwnindex].parent_index].NBchild;
                                    keywnode[keywnode[kwnindex].parent_index].child[cindex] = kwnindex;
                                    keywnode[keywnode[kwnindex].parent_index].NBchild++;
                                }




                                printf("CREATING NODE ");
                                keywnode[kwnindex].keywordlevel = level;

                                for(l=0; l<level; l++) {
                                    strcpy(keywnode[kwnindex].keyword[l], fps[fpsindex].parray[i].keyword[l]);
                                    printf(" %s", keywnode[kwnindex].keyword[l]);
                                }
                                printf("   %d %d\n", keywnode[kwnindex].keywordlevel, fps[fpsindex].parray[i].keywordlevel);

                                if(keywnode[kwnindex].keywordlevel == fps[fpsindex].parray[i].keywordlevel)
                                {
                                    keywnode[kwnindex].leaf = 1;
                                    keywnode[kwnindex].fpsindex = fpsindex;
                                    keywnode[kwnindex].pindex = i;
                                }
                                else
                                {
                                    keywnode[kwnindex].leaf = 0;
                                }




                                kwnindex ++;
                                NBkwn = kwnindex;
                            }




                        }

                        pindex++;

                    }
                }

                printf("Found fps %-20s %d parameters\n", fpsname, fps[fpsindex].md->NBparam);

                fpsindex ++;
            }
        }
    }
    else
    {
        printf("ERROR: missing /tmp/ directory\n");
        exit(0);
    }

    NBfps = fpsindex;
    NBpindex = pindex;


    // print keywords
    printf("Found %d keyword node(s)\n", NBkwn);
    int level;
    for(level=0; level<FUNCTION_PARAMETER_KEYWORD_MAXLEVEL; level++)
    {
        printf("level %d :\n", level);
        for(kwnindex=0; kwnindex<NBkwn; kwnindex++)
        {
            if(keywnode[kwnindex].keywordlevel == level)
            {
                printf("   %3d->[%3d]->x%d   (%d)", keywnode[kwnindex].parent_index, kwnindex, keywnode[kwnindex].NBchild, keywnode[kwnindex].leaf);
                printf("%s", keywnode[kwnindex].keyword[0]);

                for(l=1; l<level; l++)
                    printf(".%s", keywnode[kwnindex].keyword[l]);
                printf("\n");
            }
        }
    }

    printf("%d function parameter structure(s) imported, %ld parameters\n", NBfps, NBpindex);






    // INITIALIZE ncurses
    initncurses();
    clear();


    int currentnode = 0;
    int currentlevel = 0;
    NBindex = 0;

    while( loopOK == 1 )
    {
        int i;
        int fpsindex;
        int pindex;
        long pcnt;
        char command[500];
        char tmuxname[500];
        int nameindex;
        long icnt = 0;


        usleep(10000); // 100 Hz display



        int ch = getch();

        switch (ch)
        {
        case 'x':     // Exit control screen
            loopOK=0;
            break;

        case KEY_UP:
            iSelected[currentlevel] --;
            if(iSelected[currentlevel]<0)
                iSelected[currentlevel] = 0;
            break;

        case KEY_DOWN:
            iSelected[currentlevel] ++;
            if(iSelected[currentlevel] > NBindex-1)
                iSelected[currentlevel] = NBindex-1;
            break;

        case KEY_PPAGE:
            iSelected[currentlevel] -= 10;
            if(iSelected[currentlevel]<0)
                iSelected[currentlevel] = 0;
            break;

        case KEY_NPAGE:
            iSelected[currentlevel] += 10;
            if(iSelected[currentlevel] > NBindex-1)
                iSelected[currentlevel] = NBindex-1;
            break;


        case KEY_RIGHT:
            if(keywnode[keywnode[currentnode].child[iSelected[currentlevel]]].leaf == 0)
            {
                //iSelected = 0;
                currentnode = keywnode[currentnode].child[iSelected[currentlevel]];
            }
            break;

        case KEY_LEFT:
            if(currentnode != 0) // ROOT has no parent
                currentnode = keywnode[currentnode].parent_index;
            break;


        case 10 : // set value (if allowed)
            endwin();
            if(system("clear") != 0) // clear screen
                printERROR(__FILE__,__func__,__LINE__, "system() returns non-zero value");
            functionparameter_UserInputSetParamValue(&fps[fpsindexSelected], pindexSelected);
            initncurses();
            break;



        case '>' : // start run process
            sprintf(command, "tmux new-session -d -s %s-run &> /dev/null", fps[keywnode[iSelected[currentlevel]].fpsindex].md->name);
            system(command);

            sprintf(command, "tmux send-keys -t %s-run \"./cmdproc/%s-run-start", fps[keywnode[iSelected[currentlevel]].fpsindex].md->name, fps[keywnode[iSelected[currentlevel]].fpsindex].md->pname);
            for(nameindex=0; nameindex<fps[keywnode[iSelected[currentlevel]].fpsindex].md->NBnameindex; nameindex++)
            {
                char tmpstring[20];

                sprintf(tmpstring, " %02d", fps[keywnode[iSelected[currentlevel]].fpsindex].md->nameindex[nameindex]);
                strcat(command, tmpstring);
            }
            strcat(command, "\" C-m");
            system(command);
            break;

        case '<' : // stop run process
            sprintf(command, "./cmdproc/%s-run-stop", fps[keywnode[iSelected[currentlevel]].fpsindex].md->pname);
            for(nameindex=0; nameindex<fps[keywnode[iSelected[currentlevel]].fpsindex].md->NBnameindex; nameindex++)
            {
                char tmpstring[20];

                sprintf(tmpstring, " %02d", fps[keywnode[iSelected[currentlevel]].fpsindex].md->nameindex[nameindex]);
                strcat(command, tmpstring);
            }
            strcat(command, " &> /dev/null");
            system(command);
            break;


        case ')' : // start conf process

            sprintf(command, "tmux new-session -d -s %s-conf &> /dev/null", fps[keywnode[iSelected[currentlevel]].fpsindex].md->name);
            system(command);

            sprintf(command, "tmux send-keys -t %s-conf \"./cmdproc/%s-conf-start", fps[keywnode[iSelected[currentlevel]].fpsindex].md->name, fps[keywnode[iSelected[currentlevel]].fpsindex].md->pname);
            for(nameindex=0; nameindex<fps[keywnode[iSelected[currentlevel]].fpsindex].md->NBnameindex; nameindex++)
            {
                char tmpstring[20];

                sprintf(tmpstring, " %02d", fps[keywnode[iSelected[currentlevel]].fpsindex].md->nameindex[nameindex]);
                strcat(command, tmpstring);
            }
            strcat(command, "\" C-m");
            system(command);

            break;

        case '(': // kill conf process
            fps[keywnode[iSelected[currentlevel]].fpsindex].md->signal &= ~FUNCTION_PARAMETER_STRUCT_SIGNAL_CONFRUN;
            sprintf(command, "tmux send-keys -t %s-conf C-c &> /dev/null", fps[keywnode[iSelected[currentlevel]].fpsindex].md->name);
            system(command);
            break;

        }

        erase();

        attron(A_BOLD);
        sprintf(monstring, "FUNCTION PARAMETER MONITOR: PRESS (x) TO STOP, (h) FOR HELP");
        print_header(monstring, '-');
        attroff(A_BOLD);
        printw("\n");

        printw("currentlevel = %d   Selected = %d/%d   Current node [%3d]: ", currentlevel, iSelected[currentlevel], NBindex, currentnode);

        if(currentnode==0)
        {
            printw("ROOT");
        }
        else
        {
            for(l=0; l<keywnode[currentnode].keywordlevel; l++)
                printw("%s.", keywnode[currentnode].keyword[l]);
        }
        printw("  NBchild = %d\n", keywnode[currentnode].NBchild);

        printw("tmux sessions :  %s-conf  %s-run\n", fps[keywnode[iSelected[currentlevel]].fpsindex].md->name, fps[keywnode[iSelected[currentlevel]].fpsindex].md->name);

        printw("\n");


        currentlevel = keywnode[currentnode].keywordlevel;
        int imax = keywnode[currentnode].NBchild; // number of lines to be displayed

        nodechain[currentlevel] = currentnode;
        l = currentlevel-1;
        while(l>0)
        {
            nodechain[l] = keywnode[nodechain[l+1]].parent_index;
            l--;
        }
        nodechain[0] = 0; // root


        pcnt = 0;


        int i1 = 0;
        for(i=0; i<imax; i++) // i is the line number on GUI display
        {

            for(l=0; l<currentlevel; l++)
            {
                if(keywnode[nodechain[l]].NBchild > imax)
                    imax = keywnode[nodechain[l]].NBchild;

                if(i<keywnode[nodechain[l]].NBchild)
                {
                    int snode = 0; // selected node
                    int ii;

                    if(keywnode[nodechain[l]].child[i] == nodechain[l+1])
                        snode = 1;

                    attron(A_REVERSE);
                    printw(" ");
                    attroff(A_REVERSE);

                    if(snode == 1)
                        attron(A_REVERSE);


                    ii = keywnode[nodechain[l]].child[i];

                    if(keywnode[ii].leaf == 0) // directory
                        attron(COLOR_PAIR(5));

                    printw("%-10s ", keywnode[keywnode[nodechain[l]].child[i]].keyword[l]);

                    if(keywnode[ii].leaf == 0) // directory
                        attroff(COLOR_PAIR(5));

                    if(snode == 1)
                        attroff(A_REVERSE);



                }
                else
                    printw("            ");
            }









            int ii;
            ii = keywnode[currentnode].child[i1];
            fpsindex = keywnode[ii].fpsindex;
            pindex = keywnode[ii].pindex;

            while ( (! ( fps[fpsindex].parray[pindex].status & FUNCTION_PARAMETER_STATUS_VISIBLE )) && (i1 < keywnode[currentnode].NBchild) ) // if not visible, advance to next one
            {
                i1++;
                ii = keywnode[currentnode].child[i1];
                fpsindex = keywnode[ii].fpsindex;
                pindex = keywnode[ii].pindex;
            }

            if(i1 < keywnode[currentnode].NBchild)
            {
                attron(A_REVERSE);
                printw(" ");
                attroff(A_REVERSE);


                if(keywnode[ii].leaf == 0) // Directory
                {
                    if(i == iSelected[currentlevel])
                        attron(A_REVERSE);

                    attron(COLOR_PAIR(5));
                    l = keywnode[ii].keywordlevel;
                    printw("%s", keywnode[ii].keyword[l-1]);
                    attroff(COLOR_PAIR(5));

                    if(i == iSelected[currentlevel])
                        attroff(A_REVERSE);

                }
                else // parameter
                {
                    fpsindex = keywnode[ii].fpsindex;
                    pindex = keywnode[ii].pindex;


                    if( fps[fpsindex].parray[pindex].status & FUNCTION_PARAMETER_STATUS_VISIBLE )
                    {
                        int kl;

                        if(i == iSelected[currentlevel])
                        {
                            pindexSelected = keywnode[ii].pindex;
                            fpsindexSelected = keywnode[ii].fpsindex;

                            attron(COLOR_PAIR(10)|A_BOLD);
                        }


                        l = keywnode[ii].keywordlevel;
                        printw("%-16s", fps[fpsindex].parray[pindex].keyword[l-1]);

                        if(i == iSelected[currentlevel])
                            attroff(COLOR_PAIR(10));

                        printw("   ");

                        pid_t pid;
                        pid = fps[fpsindex].md->confpid;
                        if((getpgid(pid) >= 0)&&(pid>0))
                        {
                            attron(COLOR_PAIR(2));
                            printw("%5d ", (int) pid);
                            attroff(COLOR_PAIR(2));
                        }
                        else
                            printw("----- ");

                        pid = fps[fpsindex].md->runpid;
                        if((getpgid(pid) >= 0)&&(pid>0))
                        {
                            attron(COLOR_PAIR(2));
                            printw("%5d ", (int) pid);
                            attroff(COLOR_PAIR(2));
                        }
                        else
                            printw("----- ");


                        // VALUE

                        int paramsync = 1; // parameter is synchronized

                        if( fps[fpsindex].parray[pindex].status & FUNCTION_PARAMETER_STATUS_ERROR ) // parameter setting error
                            attron(COLOR_PAIR(4));

                        if(fps[fpsindex].parray[pindex].type & FUNCTION_PARAMETER_TYPE_UNDEF)
                            printw("  %s", "-undef-");



                        if(fps[fpsindex].parray[pindex].type & FUNCTION_PARAMETER_TYPE_INT64)
                        {
                            if( ! (fps[fpsindex].parray[pindex].status & FUNCTION_PARAMETER_STATUS_ERROR) )
                                if( fps[fpsindex].parray[pindex].val.l[0] != fps[fpsindex].parray[pindex].val.l[3] )
                                    paramsync = 0;

                            if(paramsync == 0)
                                attron(COLOR_PAIR(3));

                            printw("  %10d", (int) fps[fpsindex].parray[pindex].val.l[0]);

                            if(paramsync == 0)
                                attroff(COLOR_PAIR(3));
                        }



                        if(fps[fpsindex].parray[pindex].type & FUNCTION_PARAMETER_TYPE_FLOAT64)
                        {
                            if( ! (fps[fpsindex].parray[pindex].status & FUNCTION_PARAMETER_STATUS_ERROR) )
                            {
                                double absdiff;
                                double abssum;
                                double epsrel = 1.0e-6;
                                double epsabs = 1.0e-10;

                                absdiff = fabs(fps[fpsindex].parray[pindex].val.f[0] - fps[fpsindex].parray[pindex].val.f[3]);
                                abssum = fabs(fps[fpsindex].parray[pindex].val.f[0]) + fabs(fps[fpsindex].parray[pindex].val.f[3]);


                                if( (absdiff < epsrel*abssum) || (absdiff < epsabs) )
                                    paramsync = 1;
                                else
                                    paramsync = 0;
                            }

                            if(paramsync == 0)
                                attron(COLOR_PAIR(3));

                            printw("  %10f", (float) fps[fpsindex].parray[pindex].val.f[0]);

                            if(paramsync == 0)
                                attroff(COLOR_PAIR(3));
                        }



                        if(fps[fpsindex].parray[pindex].type & FUNCTION_PARAMETER_TYPE_PID)
                        {
                            if( ! (fps[fpsindex].parray[pindex].status & FUNCTION_PARAMETER_STATUS_ERROR) )
                                if( fps[fpsindex].parray[pindex].val.pid[0] != fps[fpsindex].parray[pindex].val.pid[1] )
                                    paramsync = 0;

                            if(paramsync == 0)
                                attron(COLOR_PAIR(3));

                            printw("  %10d", (float) fps[fpsindex].parray[pindex].val.pid[0]);

                            if(paramsync == 0)
                                attroff(COLOR_PAIR(3));

                            printw("  %10d", (int) fps[fpsindex].parray[pindex].val.pid[0]);
                        }




                        if(fps[fpsindex].parray[pindex].type & FUNCTION_PARAMETER_TYPE_TIMESPEC)
                            printw("  %10s", "-timespec-");


                        if(fps[fpsindex].parray[pindex].type & FUNCTION_PARAMETER_TYPE_FILENAME)
                        {
                            if( ! (fps[fpsindex].parray[pindex].status & FUNCTION_PARAMETER_STATUS_ERROR) )
                                if( strcmp(fps[fpsindex].parray[pindex].val.string[0], fps[fpsindex].parray[pindex].val.string[1] ))
                                    paramsync = 0;

                            if(paramsync == 0)
                                attron(COLOR_PAIR(3));

                            printw("  %10s", fps[fpsindex].parray[pindex].val.string[0]);

                            if(paramsync == 0)
                                attroff(COLOR_PAIR(3));
                        }


                        if(fps[fpsindex].parray[pindex].type & FUNCTION_PARAMETER_TYPE_DIRNAME)
                        {
                            if( ! (fps[fpsindex].parray[pindex].status & FUNCTION_PARAMETER_STATUS_ERROR) )
                                if( strcmp(fps[fpsindex].parray[pindex].val.string[0], fps[fpsindex].parray[pindex].val.string[1] ))
                                    paramsync = 0;

                            if(paramsync == 0)
                                attron(COLOR_PAIR(3));

                            printw("  %10s", fps[fpsindex].parray[pindex].val.string[0]);

                            if(paramsync == 0)
                                attroff(COLOR_PAIR(3));
                        }


                        if(fps[fpsindex].parray[pindex].type & FUNCTION_PARAMETER_TYPE_STREAMNAME)
                        {
                            if( ! (fps[fpsindex].parray[pindex].status & FUNCTION_PARAMETER_STATUS_ERROR) )
                                if( strcmp(fps[fpsindex].parray[pindex].val.string[0], fps[fpsindex].parray[pindex].val.string[1] ))
                                    paramsync = 0;

                            if(paramsync == 0)
                                attron(COLOR_PAIR(3));

                            printw("  %10s", fps[fpsindex].parray[pindex].val.string[0]);

                            if(paramsync == 0)
                                attroff(COLOR_PAIR(3));
                        }


                        if(fps[fpsindex].parray[pindex].type & FUNCTION_PARAMETER_TYPE_STRING)
                        {
                            if( ! (fps[fpsindex].parray[pindex].status & FUNCTION_PARAMETER_STATUS_ERROR) )
                                if( strcmp(fps[fpsindex].parray[pindex].val.string[0], fps[fpsindex].parray[pindex].val.string[1] ))
                                    paramsync = 0;

                            if(paramsync == 0)
                                attron(COLOR_PAIR(3));

                            printw("  %10s", fps[fpsindex].parray[pindex].val.string[0]);

                            if(paramsync == 0)
                                attroff(COLOR_PAIR(3));
                        }


                        if( fps[fpsindex].parray[pindex].status & FUNCTION_PARAMETER_STATUS_ERROR ) // parameter setting error
                            attroff(COLOR_PAIR(4));

                        printw("    %s", fps[fpsindex].parray[pindex].description);



                        if(i == iSelected[currentlevel])
                            attroff(A_BOLD);


                        pcnt++;

                    }
                }



                icnt++;

                i1++;

            }



            printw("\n");
        }

        NBindex = icnt;

        printw("\n");
        printw("%d parameters\n", pcnt);
        printw("\n");


        printw("------------- FUNCTION PARAMETER STRUCTURE   %s\n", fps[fpsindexSelected].md->name);
        if( fps[fpsindexSelected].md->status & FUNCTION_PARAMETER_STRUCT_STATUS_CHECKOK )
        {
            attron(COLOR_PAIR(2));
            printw("[%ld] PARAMETERS OK - RUN function good to go\n", fps[fpsindexSelected].md->msgcnt);
            attroff(COLOR_PAIR(2));
        }
        else
        {
            attron(COLOR_PAIR(4));
            printw("[%ld] %d PARAMETER SETTINGS ERROR(s) :\n", fps[fpsindexSelected].md->msgcnt, fps[fpsindexSelected].md->errcnt);
            attroff(COLOR_PAIR(4));

            attron(COLOR_PAIR(8));
            printw("%s",fps[fpsindexSelected].md->message);
            attroff(COLOR_PAIR(8));
        }




        refresh();


        loopcnt++;

        if( (data.signal_TERM == 1) || (data.signal_INT == 1) || (data.signal_ABRT == 1) || (data.signal_BUS == 1) || (data.signal_SEGV == 1) || (data.signal_HUP == 1) || (data.signal_PIPE == 1))
            loopOK = 0;

    }
    endwin();

    for(fpsindex=0; fpsindex<NBfps; fpsindex++)
    {
        function_parameter_struct_disconnect(&fps[fpsindex]);
    }


    return 0;
}


