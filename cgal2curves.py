import sys
import vtk
import math
from numpy import genfromtxt

def vtkReader(file_name):
 reader = vtk.vtkSTLReader()
 reader.SetFileName(file_name)
 reader.Update()

 polydata = reader.GetOutput()

 return polydata

Vessel = vtkReader(sys.argv[2])

numberOfBranches = 0

f=open(sys.argv[1])
ar=f.read().split('\n')
polyData_Container = []
for i in range(0,len(ar)):
    v=ar[i].split(' ')
    if(v[0]):
        points = vtk.vtkPoints()
        polyLine = vtk.vtkPolyLine()
        polyLine.GetPointIds().SetNumberOfIds(int(v[0]))
        cells = vtk.vtkCellArray()
        polyData = vtk.vtkPolyData()
        vtkFloatArray = vtk.vtkFloatArray()
        vtkFloatArray.SetName("BranchIds")
        vtkFloatArray.SetNumberOfTuples(int(v[0]))

        for j in range(0, int(v[0])):
            vtkFloatArray.SetTuple1(j,numberOfBranches)

        for j in range(0,int(v[0])):
            points.InsertNextPoint(float(v[j*3+1]),float(v[j*3+2]),float(v[j*3+3]))

        for j in range(0,int(v[0])):
            polyLine.GetPointIds().SetId(j,j)

        cells.InsertNextCell(polyLine)
        polyData.SetPoints(points)
        polyData.SetLines(cells)
        polyData.GetPointData().AddArray(vtkFloatArray)
        polyData.Modified()
        polyData_Container.append(polyData)
        numberOfBranches = numberOfBranches + 1

polyDataAppend = vtk.vtkAppendPolyData()
for polyDataV in polyData_Container:
 polyDataAppend.AddInputData(polyDataV)
polyDataAppend.Update()

Centerline = polyDataAppend.GetOutput()

implicitPolyDataDistance = vtk.vtkImplicitPolyDataDistance()

implicitPolyDataDistance.SetInput(Vessel)

implicitPolyDataDistance.SetTolerance(1e-6)

centerlineNumberOfPoints = Centerline.GetNumberOfPoints()

resDistance = []

for i in range(centerlineNumberOfPoints):
 p=[0,0,0]
 Centerline.GetPoint(i, p)
 signedDistance = abs(implicitPolyDataDistance.EvaluateFunction(p))
 resDistance.append(signedDistance)

vtkFloatArray = vtk.vtkFloatArray()
vtkFloatArray.SetName("Radius")
vtkFloatArray.SetNumberOfTuples(centerlineNumberOfPoints)
for i in range(centerlineNumberOfPoints):
  vtkFloatArray.SetTuple1(i,resDistance[i])

Centerline.GetPointData().AddArray(vtkFloatArray)
Centerline.GetPointData().SetActiveScalars("Radius")

writer = vtk.vtkXMLPolyDataWriter()  
writer.SetFileName("centerline.vtp")
writer.SetInputData(Centerline)
writer.Write()
