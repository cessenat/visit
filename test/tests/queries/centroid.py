# ----------------------------------------------------------------------------
#  CLASSES: nightly
#
#  Test Case:  centroid.py
#  Tests:      queries     - centroid and moment of inertia.
#
#  Defect ID:  VisIt00006273, '6588
#
#  Programmer: Hank Childs
#  Date:       May 19, 2005
#
#  Modifications:
#
#    Hank Childs, Thu Sep 15 16:33:48 PDT 2005
#    Added tests for centroids of surfaces.
#
#    Mark C. Miller, Wed Jan 20 07:37:11 PST 2010
#    Added ability to swtich between Silo's HDF5 and PDB data.
# ----------------------------------------------------------------------------

# Test that we can do an l2norm of a degenerate ultra file.
TurnOnAllAnnotations()
OpenDatabase("../data/silo_%s_test_data/globe.silo"%SILO_MODE)
AddPlot("Mesh", "mesh1")
DrawPlots()

Query("Moment of Inertia")
text = GetQueryOutputString()
# The answer should be ~167,547 along the diagonals.  But we are off ~10%
# because of mesh granularity.
TestText("centroid_01", text)

# The sphere is perfectly symmetric, so we should get the same answer,
# even if we rotate it.
AddOperator("Transform")
trans = TransformAttributes()
trans.doRotate = 1
trans.rotateAxis = (0.707, 0, -0.707)
trans.rotateAmount = 150
SetOperatorOptions(trans)
Query("Moment of Inertia")
text = GetQueryOutputString()
TestText("centroid_02", text)

Query("Centroid")
text = GetQueryOutputString()
TestText("centroid_03", text)

# Translate and see that the centroid moves.
trans.doTranslate = 1
trans.translateX = 0.5
SetOperatorOptions(trans)

Query("Centroid")
text = GetQueryOutputString()
TestText("centroid_04", text)

DeleteAllPlots()

AddPlot("Pseudocolor", "u")
DrawPlots()
Query("Centroid")
text = GetQueryOutputString()
TestText("centroid_05", text)

Query("Moment of Inertia")
text = GetQueryOutputString()
TestText("centroid_06", text)

DeleteAllPlots()

OpenDatabase("../data/silo_%s_test_data/rect2d.silo"%SILO_MODE)
AddPlot("Mesh", "quadmesh2d")
DrawPlots()

Query("Centroid")
text = GetQueryOutputString()
TestText("centroid_07", text)

DeleteAllPlots()
AddPlot("Pseudocolor", "d")
DrawPlots()
Query("Centroid")
text = GetQueryOutputString()
TestText("centroid_08", text)

AddOperator("Revolve")

Query("Centroid")
text = GetQueryOutputString()
TestText("centroid_09", text)
Query("Moment of Inertia")
text = GetQueryOutputString()
TestText("centroid_10", text)

DeleteAllPlots()

OpenDatabase("../data/silo_%s_test_data/rect3d.silo"%SILO_MODE)
AddPlot("Mesh", "quadmesh3d")
DrawPlots()

Query("Centroid")
text = GetQueryOutputString()
TestText("centroid_11", text)
Query("Moment of Inertia")
text = GetQueryOutputString()
TestText("centroid_12", text)

OpenDatabase("../data/silo_%s_test_data/rect2d.silo"%SILO_MODE)
AddPlot("Pseudocolor", "d")
AddOperator("Elevate")
DrawPlots()

Query("Centroid")
text = GetQueryOutputString()
TestText("centroid_13", text)

Exit()
