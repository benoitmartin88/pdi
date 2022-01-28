/**
 *  Copyright (c) 2017 Leonardo A. Bautista-Gomez
 *  All rights reserved
 *
 *  FTI - A multi-level checkpointing library for C/C++/Fortran applications
 *
 *  Revision 1.0 : Fault Tolerance Interface (FTI)
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are met:
 *
 *  1. Redistributions of source code must retain the above copyright notice, this
 *  list of conditions and the following disclaimer.
 *
 *  2. Redistributions in binary form must reproduce the above copyright notice,
 *  this list of conditions and the following disclaimer in the documentation
 *  and/or other materials provided with the distribution.
 *
 *  3. Neither the name of the copyright holder nor the names of its contributors
 *  may be used to endorse or promote products derived from this software without
 *  specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 *  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 *  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 *  FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 *  DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 *  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 *  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 *  OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *  @file   dcp.c
 *  @date   October, 2017
 *  @brief  Checkpointing functions for the FTI library.
 */

#include "dcp.h"

static size_t _mb = 1024L*1024L;
static size_t _gb = 1024L*1024L*1024L;
static size_t _tb = 1024L*1024L*1024L*1024L;

/**
 *  Calculates the most adequate metric in bytes for N among TB, GB and MB
 *  @param n A number N
 *  @param s A string buffer to print the result, must not be null
 *  @remark Sets s to be either TB, GB or MB
 *  @return The adequate metric size in bytes
 **/
static inline size_t get_metric(size_t n, char* s) {
    s[1] = 'B';
    if (n > _tb) {
        s[0] = 'T';
        return _tb;
    }
    if (n > _gb) {
        s[0] = 'G';
        return _gb;
    }
    s[0] = 'M';
    return _mb;
}

void FTI_PrintDcpStats(FTIT_configuration FTI_Conf, FTIT_execution FTI_Exec,
 FTIT_topology FTI_Topo) {
    char str[FTI_BUFS];
    char data_metric[3] = "_B";
    char dcp_metric[3] = "_B";
    char cp_print_mode[6];

    size_t norder_data, norder_dcp, pureDataSize, dcpSize;

    if (FTI_Conf.dcpFtiff) {
        dcpSize = FTI_Exec.FTIFFMeta.dcpSize;
        pureDataSize = FTI_Exec.FTIFFMeta.pureDataSize;
    } else {
        dcpSize = FTI_Exec.dcpInfoPosix.dcpSize;
        pureDataSize = FTI_Exec.dcpInfoPosix.dataSize;
    }

    norder_data = get_metric(pureDataSize, data_metric);
    norder_dcp = get_metric(dcpSize, dcp_metric);

    // If not head
    if (FTI_Topo.splitRank)
        snprintf(cp_print_mode, sizeof(cp_print_mode), "Local");
    else
        snprintf(cp_print_mode, sizeof(cp_print_mode), "Total");

    snprintf(str, FTI_BUFS, "%s CP data: %.2lf %s, %s dCP update:"
            " %.2lf %s, dCP share: %.2lf%%",
            cp_print_mode,
            (double)pureDataSize/norder_data,
            data_metric,
            cp_print_mode,
            (double)dcpSize/norder_dcp,
            dcp_metric,
            ((double)dcpSize/pureDataSize)*100);

    // If not head
    if (FTI_Topo.splitRank)
        FTI_Print(str, FTI_DBUG);
    FTI_Print(str, FTI_IDCP);
}
