// Copyright (c) Lawrence Livermore National Security, LLC and other VisIt
// Project developers.  See the top-level LICENSE file for dates and other
// details.  No copyright assignment is required to contribute to VisIt.

// ************************************************************************* //
//                           avtGeometryDrawable.C                           //
// ************************************************************************* //

#include <avtGeometryDrawable.h>

#include <vtkActor.h>
#include <vtkActor2D.h>
#include <vtkProp.h>
#include <vtkProperty.h>
#include <vtkRenderer.h>
#include <vtkVisItFullFrameMapper2D.h>

#include <avtMapperBase.h>
#include <avtTransparencyActor.h>

#include <ColorAttribute.h>

#include <ImproperUseException.h>


// ****************************************************************************
//  Method: avtGeometryDrawable constructor
//
//  Arguments:
//      nA       The number of actors.
//      a        A list of actors.
//
//  Programmer:  Hank Childs
//  Creation:    December 21, 2000
//
//  Modifications:
//    Kathleen Bonnell, Thu Mar 15 10:51:59 PST 2001
//    Added member actorsVisibility, used to store visibility of passed
//    actors, so that this visibility value can be restored to individual
//    actors in the VisbilityOn method.
//
//    Kathleen Bonnell, Tue Oct 12 16:18:37 PDT 2004
//    Set all actor's Pickable flag to OFF by default.  Can be turned on/off
//    as need by MakePickable and MakeUnPickable.
//
// ****************************************************************************

avtGeometryDrawable::avtGeometryDrawable(int nA, vtkActor **a)
{
    nActors = nA;
    actors  = new vtkProp*[nActors];
    actorsVisibility  = new int[nActors];

    for (int i = 0 ; i < nActors ; i++)
    {
        actors[i] = a[i];
        if (actors[i] != NULL)
        {
            actors[i]->Register(NULL);
            actorsVisibility[i] = actors[i]->GetVisibility();
            actors[i]->PickableOff();
        }
    }

    renderer = NULL;
    mapper = NULL;
}


// ****************************************************************************
//  Method: avtGeometryDrawable constructor
//
//  Arguments:
//      nA       The number of 2D actors.
//      a        A list of 2D actors.
//
//  Programmer:  Kathleen Biagas
//  Creation:    April 13, 2017
//
//  Modifications:
//
// ****************************************************************************

avtGeometryDrawable::avtGeometryDrawable(int nA, vtkActor2D **a)
{
    nActors = nA;
    actors  = new vtkProp*[nActors];
    actorsVisibility  = new int[nActors];

    for (int i = 0 ; i < nActors ; i++)
    {
        actors[i] = a[i];
        if (actors[i] != NULL)
        {
            actors[i]->Register(NULL);
            actorsVisibility[i] = actors[i]->GetVisibility();
            actors[i]->PickableOff();
        }
    }

    renderer = NULL;
    mapper = NULL;
}


// ****************************************************************************
//  Method: avtGeometryDrawable destructor
//
//  Programmer: Hank Childs
//  Creation:   December 21, 2000
//
// ****************************************************************************

avtGeometryDrawable::~avtGeometryDrawable()
{
    //
    // If we are still attached to a renderer, remove ourselves.
    //
    if (renderer != NULL)
    {
        Remove(renderer);
    }

    if (actors != NULL)
    {
        for (int i = 0 ; i < nActors ; i++)
        {
            if (actors[i] != NULL)
            {
                actors[i]->Delete();
                actors[i] = NULL;
            }
        }
    }
    delete [] actors;
    delete [] actorsVisibility;
}


// ****************************************************************************
//  Method: avtGeometryDrawable::SetMapper
//
//  Purpose:
//      Tells the geometry drawable which mapper it comes from.
//
//  Programmer: Hank Childs
//  Creation:   May 24, 2002
//
// ****************************************************************************

void
avtGeometryDrawable::SetMapper(avtMapperBase *m)
{
    mapper = m;
}


// ****************************************************************************
//  Method: avtGeometryDrawable::Add
//
//  Purpose:
//      Adds this drawable to a renderer.
//
//  Arguments:
//      ren     The renderer to add ourselves to.
//
//  Programmer: Hank Childs
//  Creation:   December 21, 2000
//
// ****************************************************************************

void
avtGeometryDrawable::Add(vtkRenderer *ren)
{
    renderer = ren;
    for (int i = 0 ; i < nActors ; i++)
    {
        if (actors[i] != NULL)
        {
            ren->AddActor(actors[i]);
        }
    }
}


// ****************************************************************************
//  Method: avtGeometryDrawable::Remove
//
//  Purpose:
//      Removes this drawable from a renderer.
//
//  Arguments:
//      ren     The renderer to remove ourselves from.
//
//  Programmer: Hank Childs
//  Creation:   December 21, 2000
//
//  Modifications:
//
//    Hank Childs, Fri May  3 15:14:59 PDT 2002
//    Added a trick/hack to allow actors to be removed from a window without
//    affecting their display lists.
//
//    Kathleen Bonnell, Wed May 17 15:08:39 PDT 2006
//    GetProps->RemoveItem has been deprecated, use GetViewProps->RemoveItem.
// ****************************************************************************

void
avtGeometryDrawable::Remove(vtkRenderer *ren)
{
    if (ren != renderer)
    {
        EXCEPTION0(ImproperUseException);
    }

    for (int i = 0 ; i < nActors ; i++)
    {
        if (actors[i] != NULL)
        {
            //
            // This is supposed to approximate the RemoveActor call of
            // vtkRenderer.  That call also tells the actor to release its
            // graphics resources, which does not work well for us, since
            // we remove the actors every time we add new plots (the viewer
            // does a ClearPlots) and also when the vis window re-orders the
            // actors.
            //
            // THIS IS A MAINTENANCE ISSUE.  This routine should be the same
            // as vtkRenderer::RemoveActor, but does not call
            // ReleaseGraphicsResources (which is actually called indirectly
            // through vtkViewport::RemoveProp).
            //
            //ren->RemoveActor(actors[i]);
            //
            ren->GetActors()->RemoveItem(actors[i]);
            actors[i]->RemoveConsumer(ren);
            ren->GetViewProps()->RemoveItem(actors[i]);
        }
    }
    renderer = NULL;
}


// ****************************************************************************
//  Method: avtGeometryDrawable::VisibilityOn
//
//  Purpose:
//      Turns the visibility of the drawable back on (this is the default).
//
//  Programmer: Hank Childs
//  Creation:   December 28, 2000
//
//  Modifications:
//    Kathleen Bonnell, Thu Mar 15 10:51:59 PST 2001
//    Changed to set visibility of individual actors based on stored
//    visibility created in constructor.
//
// ****************************************************************************

void
avtGeometryDrawable::VisibilityOn(void)
{
    for (int i = 0 ; i < nActors ; i++)
    {
        if (actors[i] != NULL)
        {
            actors[i]->SetVisibility(actorsVisibility[i]);
        }
    }
}


// ****************************************************************************
//  Method: avtGeometryDrawable::VisibilityOff
//
//  Purpose:
//      Turns off the visibility of the drawable (for bounding box mode, etc).
//
//  Programmer: Hank Childs
//  Creation:   December 28, 2000
//
//  Modifications:
//
//    Hank Childs, Mon Jul 22 13:17:35 PDT 2002
//    Give ourselves another chance to check out the actor's visibility.
//    This is because the transparency actor may have modified it.
//
// ****************************************************************************

void
avtGeometryDrawable::VisibilityOff(void)
{
    for (int i = 0 ; i < nActors ; i++)
    {
        if (actors[i] != NULL)
        {
            actorsVisibility[i] = actors[i]->GetVisibility();
            if (actorsVisibility[i] != 0)
            {
                actors[i]->VisibilityOff();
            }
        }
    }
}


// ****************************************************************************
//  Method: avtGeometryDrawable::ShiftByVector
//
//  Purpose:
//      Shifts the drawable by a vector.
//
//  Arguments:
//      vec    The vector to shift by.
//
//  Programmer: Hank Childs
//  Creation:   March 12, 2001
//
//  Modifications:
//    Kathleen Biagas, Thu Apr 13 10:27:54 PDT 2017
//    Only set position for vtkActor (not vtkActor2D).
//
// ****************************************************************************

void
avtGeometryDrawable::ShiftByVector(const double vec[3])
{
    for (int i = 0 ; i < nActors ; i++)
    {
        if (actors[i] != NULL && actors[i]->IsA("vtkActor") &&
            mapper != NULL && mapper->ActorIsShiftable(i))
        {
            // VTK is ridiculous -- needs const.
            double v[3];
            v[0] = vec[0];
            v[1] = vec[1];
            v[2] = vec[2];
            ((vtkActor*)actors[i])->SetPosition(v);
        }
    }
}


// ****************************************************************************
//  Method: avtGeometryDrawable::ScaleByVector
//
//  Purpose:
//      Scales the drawable by a vector.
//
//  Arguments:
//      vec    The vector to scale by.
//
//  Programmer: Kathleen Bonnell
//  Creation:   July 12, 2002
//
//  Modifications:
//    Kathleen Biagas, Thu Apr 13 10:27:54 PDT 2017
//    Only set scale for vtkActor (not vtkActor2D).
//
//    Alister Maguire, Fri May 21 15:11:53 PDT 2021
//    Handle the vtkLabelMapper.
//
// ****************************************************************************

void
avtGeometryDrawable::ScaleByVector(const double vec[3])
{
    for (int i = 0 ; i < nActors ; i++)
    {
        if (actors[i] != NULL && actors[i]->IsA("vtkActor"))
        {
            // VTK is ridiculous -- needs const.
            double v[3];
            v[0] = vec[0];
            v[1] = vec[1];
            v[2] = vec[2];
            ((vtkActor*)actors[i])->SetScale(v);
        }
        else if (actors[i] != NULL && actors[i]->IsA("vtkActor2D"))
        {
            //
            // The vtkLabelMapper is a special case. It's a vtkActor2D, which
            // means that it doesn't have a "SetScale" method, but we need to
            // be able to actively adjust the full frame scaling. We can
            // instead rely on the mapper to handle this adjustment.
            //
            if (((vtkActor2D*)actors[i])->GetMapper()->IsA("vtkLabelMapper"))
            {
                ((vtkVisItFullFrameMapper2D*)((vtkActor2D*)actors[i])->
                    GetMapper())->SetFullFrameScaling(vec);
            }
        }
    }
}


// ****************************************************************************
//  Method: avtGeometryDrawable::GetDataObject
//
//  Purpose:
//      Gets the datasets associated with this geometry drawable.
//
//  Programmer: Hank Childs
//  Creation:   May 24, 2002
//
//  Modifications:
//
//    Hank Childs, Sun May 26 18:37:23 PDT 2002
//    Changed interface to return an avtDataObject directly.  This is a more
//    general interface and allows us to not modify the tree structure for an
//    avtDataset.
//
// ****************************************************************************

avtDataObject_p
avtGeometryDrawable::GetDataObject(void)
{
    if (mapper != NULL)
    {
        return mapper->GetInput();
    }

    return NULL;
}


// ****************************************************************************
//  Method: avtGeometryDrawable::SetTransparencyActor
//
//  Purpose:
//      Sets the mapper's transparency actor.
//
//  Arguments:
//      act      The transparency actor.
//
//  Returns:     The index for this actor with the transparency actor.
//
//  Programmer:  Hank Childs
//  Creation:    July 7, 2002
//
// ****************************************************************************

int
avtGeometryDrawable::SetTransparencyActor(avtTransparencyActor *act)
{
    int rv = -1;
    if (mapper != NULL)
    {
        rv = mapper->SetTransparencyActor(act);
    }

    return rv;
}

// ****************************************************************************
//  Method: avtGeometryDrawable::UpdateScaleFactor
//
//  Purpose:
//      Allow the actors to update their scale.
//
//  Programmer:  Kathleen Bonnell
//  Creation:    July 19, 2002
//
// ****************************************************************************

void
avtGeometryDrawable::UpdateScaleFactor()
{
    ;
}


// ****************************************************************************
//  Method: avtGeometryDrawable::TurnLightingOn
//
//  Purpose:
//      Allow the mapper to update the lighting coefficients.
//
//  Programmer:  Kathleen Bonnell
//  Creation:    August 13, 2002
//
//  Modifications:
//    Dave Bremer, Fri Mar  7 10:43:49 PST 2008
//    Added a NULL check just for consistency with other methods.
// ****************************************************************************

void
avtGeometryDrawable::TurnLightingOn()
{
    if (mapper != NULL)
        mapper->GlobalLightingOn();
}


// ****************************************************************************
//  Method: avtGeometryDrawable::TurnLightingOff
//
//  Purpose:
//      Allow the mapper to update the lighting coefficients.
//
//  Programmer:  Kathleen Bonnell
//  Creation:    August 13, 2002
//
//  Modifications:
//    Dave Bremer, Fri Mar  7 10:43:49 PST 2008
//    Added a NULL check just for consistency with other methods.
// ****************************************************************************

void
avtGeometryDrawable::TurnLightingOff()
{
    if (mapper != NULL)
        mapper->GlobalLightingOff();
}

// ****************************************************************************
//  Method: avtGeometryDrawable::SetAmbientCoefficient
//
//  Purpose:
//      Allow the mapper to update the ambient lighting coefficient.
//
//  Programmer:  Kathleen Bonnell
//  Creation:    August 13, 2002
//
//  Modifications:
//    Dave Bremer, Fri Mar  7 10:43:49 PST 2008
//    Added a NULL check just for consistency with other methods.
// ****************************************************************************

void
avtGeometryDrawable::SetAmbientCoefficient(const double amb)
{
    if (mapper != NULL)
        mapper->GlobalSetAmbientCoefficient(amb);
}

// ****************************************************************************
//  Method: avtGeometryDrawable::SetSurfaceRepresentation
//
//  Purpose:
//      Sets the drawable's surface representation.
//
//  Arguments:
//      rep : The new surface representation.
//
//  Programmer: Brad Whitlock
//  Creation:   Mon Sep 23 15:58:48 PST 2002
//
//  Modifications:
//    Kathleen Bonnell, Sat Oct 19 15:07:04 PDT 2002
//    Disable lighting for Wireframe and Points representation.
//
//    Kathleen Bonnell, Thu Sep  2 11:44:09 PDT 2004
//    Moved logic into avtMapper so that derived mappers may have a chance
//    to override the behavior.
//
// ****************************************************************************

void
avtGeometryDrawable::SetSurfaceRepresentation(int rep)
{
    if (mapper != NULL)
        mapper->SetSurfaceRepresentation(rep);
}

// ****************************************************************************
//  Method: avtGeometryDrawable::SetSpecularProperties
//
//  Purpose:
//      Sets the drawable's surface representation.
//
//  Arguments:
//      flag  :  true to enable specular, false otherwise
//      coeff :  the new specular coefficient
//      power :  the new specular power
//      color :  the new specular color
//
//  Programmer: Jeremy Meredith
//  Creation:   November 14, 2003
//
//  Modifications:
//    Kathleen Bonnell, Thu Sep  2 08:52:56 PDT 2004
//    Moved logic into avtMapper so that derived mappers may have a chance
//    to override the behavior.
//
// ****************************************************************************

void
avtGeometryDrawable::SetSpecularProperties(bool flag, double coeff,
                                           double power,
                                           const ColorAttribute &color)
{
    if (mapper != NULL)
        mapper->SetSpecularProperties(flag, coeff, power, color);
}

// ****************************************************************************
// Method: avtGeometryDrawable::SetColorTexturingFlag
//
// Purpose:
//   Sets the mapper's color texturing flag.
//
// Arguments:
//   val : The new color texturing mode.
//
// Programmer: Brad Whitlock
// Creation:   Mon Sep 18 11:24:19 PDT 2006
//
// Modifications:
//
// ****************************************************************************

void
avtGeometryDrawable::SetColorTexturingFlag(bool val)
{
    if (mapper != NULL)
        mapper->SetColorTexturingFlag(val);
}


// ****************************************************************************
//  Method: avtGeometryDrawable::MakePickable
//
//  Purpose:
//    Turns on the pickability of the vtkActors.
//
//  Programmer: Kathleen Bonnell
//  Creation:   September 27, 2004
//
// ****************************************************************************

void
avtGeometryDrawable::MakePickable()
{
    for (int i = 0 ; i < nActors ; i++)
    {
        if (actors[i] != NULL)
        {
            actors[i]->PickableOn();
        }
    }
}


// ****************************************************************************
//  Method: avtGeometryDrawable::MakeUnPickable
//
//  Purpose:
//    Turns off the pickability of the vtkActors.
//
//  Programmer: Kathleen Bonnell
//  Creation:   September 27, 2004
//
// ****************************************************************************

void
avtGeometryDrawable::MakeUnPickable()
{
    for (int i = 0 ; i < nActors ; i++)
    {
        if (actors[i] != NULL)
        {
            actors[i]->PickableOff();
        }
    }
}


// ****************************************************************************
//  Method: avtGeometryDrawable::GetZPosition
//
//  Purpose:
//    Retrieves the z-position of the vtk actor.
//
//  Programmer: Kathleen Bonnell
//  Creation:   June 27, 2005
//
//  Modifications:
//    Kathleen Biagas, Thu Apr 13 10:27:54 PDT 2017
//    Only get position for vtkActor (not vtkActor2D).
//
// ****************************************************************************

double
avtGeometryDrawable::GetZPosition()
{
    double zpos = 0.;
    for (int i = 0 ; i < nActors ; i++)
    {
        if (actors[i] != NULL && actors[i]->IsA("vtkActor"))
        {
            zpos = static_cast<vtkActor*>(actors[i])->GetPosition()[2];
            break;
        }
    }
    return zpos;
}

// ****************************************************************************
// Method: avtGeometryDrawable::ReducedDetailModeOn
//
// Purpose:
//   Turn on reduced detail mode.
//
// Programmer: Brad Whitlock
// Creation:   Wed Aug 22 11:30:08 PDT 2007
//
// Modifications:
//
// ****************************************************************************

void
avtGeometryDrawable::ReducedDetailModeOn()
{
    if(mapper != NULL)
        mapper->ReducedDetailModeOn();
}

// ****************************************************************************
// Method: avtGeometryDrawable::ReducedDetailModeOff
//
// Purpose:
//   Turn off reduced detail mode.
//
// Programmer: Brad Whitlock
// Creation:   Wed Aug 22 11:30:08 PDT 2007
//
// Modifications:
//
// ****************************************************************************

bool
avtGeometryDrawable::ReducedDetailModeOff()
{
    bool retval = false;
    if(mapper != NULL)
        retval = mapper->ReducedDetailModeOff();
    return retval;
}

