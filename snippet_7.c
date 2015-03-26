/******************************************************************************
   Author: Manfred Rebentisch, 2004, <mareb@comparat.de>

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the
   Free Software Foundation, Inc.
   59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
   Or have a look to:
   http://www.opensource.org/licenses/gpl-license.php
 ******************************************************************************/
/*
    compile with gcc -o discutil -pedantic -DDEBUG_STATVFS discutil.c
    written for Linux
*/
#include <sys/types.h>
#include <limits.h>
#include <malloc.h>
#include <stdint.h>  /* integer types */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>  /* strdup() */
#include <sys/statvfs.h>    /* statvfs() */

#define SUCCESS 0
#define FAILURE 1
enum { DI_Error_No = 0, DI_Error_ParamNull = -1, DI_Error_Overflow = 330 };
typedef uint32_t        uint32;
typedef uint64_t        uint64;
struct _tagdiscinfo;
typedef struct _tagdiscinfo TDiscInfo, *PTDiscInfo;

struct _tagdiscinfo {
    void*       internalM;
        /* Available count of bytes, print with printf("Freie Bytes: %llu\n", availBytes)
           See 'man 3 printf', search 'long long' */
    uint64      (*availBytes)(PTDiscInfo pdi);
        /* Count of available blocks: */
    uint32      (*availBlocks)(PTDiscInfo pdi);
            /* How many blocks need a file? 'checksize' is the size
               of a file. The result is the count of blocks, this
               file need on the given pathM.  */
    uint32      (*neededBlocksFor)(PTDiscInfo pdi, off_t checksize);
            /* refresh() actualisize internal data for pathM */
    int         (*refresh)(PTDiscInfo pdi);
    void        (*free)(PTDiscInfo pdi);
    int         (*error)(PTDiscInfo pdi);
    uint32      (*blocksize)(PTDiscInfo pdi);
};

/*
    param const char* path    Kein Device angeben, sondern Namen wie "/home/mareb"
*/
PTDiscInfo CPDiscInfoCreate(const char* path);
/* ------- End of Headerpart ------------*/

/* ------- Begin implementation file ----*/
typedef struct _tagInternal {
    int                 errorM;
    struct statvfs      vfsM;
    char*               pathM;
    uint32              kBlockSizeCountM;       /* count of 1024-blocks of block size */
    uint32              kBlockCountFreeM;       /* count 1024-blocks */
    uint64              bytesFreeM;             /* count of bytes */
} TInternal, *PTInternal;
static int    st_DI_refresh(PTDiscInfo pdi);
static void   st_DI_free(PTDiscInfo pdi);
static uint32 st_DI_neededBlocksFor(PTDiscInfo pdi, off_t checksize);
static uint64 st_DI_availBytes(PTDiscInfo pdi);
static uint32 st_DI_availBlocks(PTDiscInfo pdi);
static int    st_DI_error(PTDiscInfo pdi);
static uint32 st_DI_blocksize(PTDiscInfo pdi);

PTDiscInfo
CPDiscInfoCreate(const char* path)
{
    PTDiscInfo  pdi;
    PTInternal  ind;
    int         status;
    if( ! path ) {
        return NULL;
    }
    pdi = calloc( 1, sizeof(TDiscInfo));
    if(! pdi ) {
        return NULL;
    }
    ind = calloc( 1, sizeof(TInternal));
    if( ! ind ) {
        free(pdi);
        return NULL;
    }
    pdi->neededBlocksFor        = st_DI_neededBlocksFor;
    pdi->free                   = st_DI_free;
    pdi->availBytes             = st_DI_availBytes;
    pdi->availBlocks            = st_DI_availBlocks;
    pdi->refresh                = st_DI_refresh;
    pdi->error                  = st_DI_error;
    pdi->blocksize              = st_DI_blocksize;
    ind->pathM                  = strdup(path);
    pdi->internalM              = ind;

    status = pdi->refresh(pdi);
    if( status == FAILURE ) {
        pdi->free(pdi);
        return NULL;
    }
    return pdi;
}

static int
st_DI_refresh(PTDiscInfo pdi)
{
    PTInternal  ind;
    int         status;
    ind = pdi->internalM;
    status = statvfs(ind->pathM, &ind->vfsM);
    if( status < 0 )  {
        return FAILURE;
    }
    ind->kBlockSizeCountM       = ind->vfsM.f_bsize / 1024;
    ind->kBlockCountFreeM       = ind->kBlockSizeCountM * ind->vfsM.f_bavail;
    ind->bytesFreeM             = (uint64)ind->vfsM.f_bavail * (uint64)ind->vfsM.f_bsize;
    /* overflow??  */
    if( ind->bytesFreeM / (uint64)ind->vfsM.f_bsize != (uint64)ind->vfsM.f_bavail ) {
        fprintf(stderr, "st_DI_refresh:bytesFreeM: %llu, bsize: %lu, bavail: %lu, free/bsize=%lu\n",
            ind->bytesFreeM, ind->vfsM.f_bsize, ind->vfsM.f_bavail, ind->bytesFreeM / ind->vfsM.f_bsize);
        ind->errorM     = DI_Error_Overflow;
        ind->bytesFreeM = 0;
    }
#ifdef DEBUG_STATVFS
    printf("statvfs: status: %d\nf_bsize  : %lu\n"
                      "f_frsize : %lu\n"
                      "f_blocks : %lu\n"
                      "f_bfree  : %lu\n"
                      "f_bavail : %lu (%lu bei 1024-Bloecken)\n"
                      "f_namemax: %lu\n",
        status,
        ind->vfsM.f_bsize,
        ind->vfsM.f_frsize,
        ind->vfsM.f_blocks,
        ind->vfsM.f_bfree,
        ind->vfsM.f_bavail, ind->kBlockCountFreeM,
        ind->vfsM.f_namemax);
#endif

    return SUCCESS;
}

static void
st_DI_free(PTDiscInfo pdi)
{
    PTInternal  ind;
    if(pdi) {
        ind = pdi->internalM;
        free(ind->pathM);
        free(ind);
        free(pdi);
    }
}

static uint32
st_DI_neededBlocksFor(PTDiscInfo pdi, off_t checksize)
{
    PTInternal  ind;
    if(pdi) {
        ind = pdi->internalM;
        return (checksize / ind->vfsM.f_bsize) + ind->vfsM.f_bsize;
    }
    return 0;
}

static uint64
st_DI_availBytes(PTDiscInfo pdi)
{
    PTInternal  ind;
    if(pdi) {
        ind = pdi->internalM;
        return ind->bytesFreeM;
    }
    return 0;
}

static uint32
st_DI_availBlocks(PTDiscInfo pdi)
{
    PTInternal  ind;
    if(pdi) {
        ind = pdi->internalM;
        return ind->vfsM.f_bavail;
    }
    return 0;
}

static int
st_DI_error(PTDiscInfo pdi)
{
    PTInternal  ind;
    if(pdi) {
        ind = pdi->internalM;
        return ind->errorM;
    }
    return DI_Error_ParamNull;
}

static uint32
st_DI_blocksize(PTDiscInfo pdi)
{
    PTInternal  ind;
    if(pdi) {
        ind = pdi->internalM;
        return ind->vfsM.f_bsize;
    }
    return 0;
}

#ifdef DEBUG_STATVFS
int main(int argc, char** argv)
{
    PTDiscInfo  p;
    if(argc < 2 ) {
        printf("Need a path like '/home/mareb' as parameter\n");
        return 0;
    }
    p = CPDiscInfoCreate( argv[1] );
    if(p) {
        printf("CPDiscInfoCreate success\n"
            "Free bytes.........: %llu\n"
            "Free blockcount....: %lu\n",
            p->availBytes(p),
            p->availBlocks(p));
    }
    p->free(p);
    return 0;
}
#endif
