// ************************************************************************* //
//                              BadNodeException.h                           //
// ************************************************************************* //

#ifndef BAD_NODE_EXCEPTION_H
#define BAD_NODE_EXCEPTION_H

#include <avtexception_exports.h>

#include <PipelineException.h>
#include <vector>

// ****************************************************************************
//  Class: BadNodeException
//
//  Purpose:
//      The exception that should be called when a bad node is encountered.
//
//  Programmer: Kathleen Bonnell 
//  Creation:   January 13, 2004 
//
//  Modifications:
//
// ****************************************************************************

class AVTEXCEPTION_API BadNodeException : public PipelineException
{
  public:
                    BadNodeException(int, int);
                    BadNodeException(std::vector<int> &);
    virtual        ~BadNodeException() VISIT_THROW_NOTHING {;};
};


#endif


