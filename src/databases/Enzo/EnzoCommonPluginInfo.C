// Copyright (c) Lawrence Livermore National Security, LLC and other VisIt
// Project developers.  See the top-level LICENSE file for dates and other
// details.  No copyright assignment is required to contribute to VisIt.

#include <EnzoPluginInfo.h>
#include <avtEnzoFileFormat.h>
#include <avtSTMDFileFormatInterface.h>
#include <avtGenericDatabase.h>

// ****************************************************************************
//  Method:  EnzoCommonPluginInfo::GetDatabaseType
//
//  Purpose:
//    Returns the type of a Enzo database.
//
//  Programmer:  generated by xml2info
//  Creation:    omitted
//
// ****************************************************************************
DatabaseType
EnzoCommonPluginInfo::GetDatabaseType()
{
    return DB_TYPE_STMD;
}

// ****************************************************************************
//  Method: EnzoCommonPluginInfo::SetupDatabase
//
//  Purpose:
//      Sets up a Enzo database.
//
//  Arguments:
//      list    A list of file names.
//      nList   The number of timesteps in list.
//      nBlocks The number of blocks in the list.
//
//  Returns:    A Enzo database from list.
//
//  Programmer: generated by xml2info
//  Creation:   omitted
//
// ****************************************************************************
avtDatabase *
EnzoCommonPluginInfo::SetupDatabase(const char *const *list,
                                   int nList, int nBlock)
{
    avtSTMDFileFormat **ffl = new avtSTMDFileFormat*[nList];
    for (int i = 0; i < nList; i++)
    {
        ffl[i] = new avtEnzoFileFormat(list[i]);
    }
    avtSTMDFileFormatInterface *inter
           = new avtSTMDFileFormatInterface(ffl, nList);
    return new avtGenericDatabase(inter);
}
