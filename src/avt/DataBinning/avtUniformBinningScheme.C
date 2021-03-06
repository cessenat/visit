// Copyright (c) Lawrence Livermore National Security, LLC and other VisIt
// Project developers.  See the top-level LICENSE file for dates and other
// details.  No copyright assignment is required to contribute to VisIt.

// ************************************************************************* //
//                          avtUniformBinningScheme.C                        //
// ************************************************************************* //

#include <avtUniformBinningScheme.h>

#include <vtkFloatArray.h>
#include <vtkRectilinearGrid.h>

#include <ImproperUseException.h>


// ****************************************************************************
//  Method: avtUniformBinningScheme constructor
//
//  Programmer: Hank Childs
//  Creation:   February 12, 2006
//
// ****************************************************************************

avtUniformBinningScheme::avtUniformBinningScheme(int nt, const float *r,
                                                 const int *nv)
{
    int  i;

    if (nt <= 0)
    {
        // Must be a positive number of tuples
        EXCEPTION0(ImproperUseException);
    }
    for (i = 0 ; i < nt ; i++)
    {
        if (r[2*i+1] <= r[2*i])
        {
            // Ranges must be ordered correctly.
            EXCEPTION0(ImproperUseException);
        }
        if (nv[i] <= 0)
        {
            // Must have at least one bin.
            EXCEPTION0(ImproperUseException);
        }
    }
    
    ntuples = nt;
    ranges  = new float[2*nt];
    nvals   = new int[nt];
    nBins   = 1;
    for (i = 0 ; i < ntuples ; i++)
    {
        ranges[2*i]   = r[2*i];
        ranges[2*i+1] = r[2*i+1];
        nvals[i]      = nv[i];
        nBins        *= nvals[i];
    }
}

// ****************************************************************************
//  Method: avtUniformBinningScheme destructor
//
//  Programmer: Hank Childs
//  Creation:   February 12, 2006
//
// ****************************************************************************

avtUniformBinningScheme::~avtUniformBinningScheme()
{
    delete [] ranges;
    delete [] nvals;
}


// ****************************************************************************
//  Method: avtUniformBinningScheme::GetBinId
//
//  Purpose:
//      Gets the bin identifier for some set of tuples.
//
//  Programmer: Hank Childs
//  Creation:   February 12, 2006
//
//  Modifications:
//
//    Hank Childs, Thu Mar 30 12:38:14 PST 2006
//    Fix precision issue.
//
//    Hank Childs, Sat Aug 21 14:05:14 PDT 2010
//    Add support for clamping vs discarding (always clamped previously).
//
// ****************************************************************************

int
avtUniformBinningScheme::GetBinId(const float *f) const
{
    int mult = 1;
    int rv = 0;
    for (int i = 0 ; i < ntuples ; i++)
    {
        double span = ranges[2*i+1] - ranges[2*i];
        double dist = f[i] - ranges[2*i];
        int tup_bin = (int)((dist / span)*nvals[i]);
        if (tup_bin < 0)
        {
            if (oobb == ConstructDataBinningAttributes::Discard)
                return -1;
            else
                tup_bin = 0;
        }
        if (tup_bin >= nvals[i])
        {
            if (oobb == ConstructDataBinningAttributes::Discard)
                return -1;
            else
                tup_bin = nvals[i]-1;
        }
         
        rv += tup_bin*mult;
        mult *= nvals[i];
    }

    return rv;
}


// ****************************************************************************
//  Method: avtUniformBinningScheme::CreateGrid
//
//  Purpose:
//      Creates a grid corresponding to the binning scheme that allows for
//      direct visualization.
//
//  Notes:      The X-axis must correspond to tuple 0, the Y- to tuple 1, etc.
//
//  Programmer: Hank Childs
//  Creation:   February 12, 2006
//
//  Modifications:
//
//    Hank Childs, Sat Aug 21 14:05:14 PDT 2010
//    Set up curves for point-centering.
//
// ****************************************************************************

vtkDataSet *
avtUniformBinningScheme::CreateGrid(void) const
{
    int  i, j;

    if (ntuples > 3)
    {
        EXCEPTION0(ImproperUseException);
    }

    vtkRectilinearGrid *rgrid = vtkRectilinearGrid::New();
    int dims[3] = { 1, 1, 1 };

    for (i = 0 ; i < 3 ; i++)
    {
        vtkFloatArray *arr = vtkFloatArray::New();
        if (i < ntuples)
        {
            if (ntuples == 1) // curve ... set up for point data
            {
                arr->SetNumberOfTuples(nvals[i]);
                float start = ranges[2*i];
                float stop  = ranges[2*i+1];
                float step  = (stop - start) / (nvals[i]);
                for (j = 0 ; j < nvals[i] ; j++)
                {
                    arr->SetValue(j, start + j*step + step/2.0);
                }
            }
            else // 2D or 3D ... set up for cell data
            {
                arr->SetNumberOfTuples(nvals[i]+1);
                float start = ranges[2*i];
                float stop  = ranges[2*i+1];
                float step  = (stop - start) / (nvals[i]);
                for (j = 0 ; j < nvals[i]+1 ; j++)
                {
                    arr->SetValue(j, start + j*step);
                }
            }
        }
        else
        {
            arr->SetNumberOfTuples(1);
            arr->SetValue(0, 0.);
        }
        dims[i] = arr->GetNumberOfTuples();
        if (i == 0)
            rgrid->SetXCoordinates(arr);
        else if (i == 1)
            rgrid->SetYCoordinates(arr);
        else if (i == 2)
            rgrid->SetZCoordinates(arr);
        arr->Delete();
    }

    rgrid->SetDimensions(dims);
    return rgrid;
}


