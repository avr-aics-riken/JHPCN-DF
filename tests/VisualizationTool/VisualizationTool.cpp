/*
 * JHPCN-DF - Data compression library based on
 *            Jointed Hierarchical Precision Compression Number Data Format
 *
 * Copyright (c) 2014-2015 Advanced Institute for Computational Science, RIKEN.
 * All rights reserved.
 *
 */

// @file VisualizationTool.cpp

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdlib>
#include "jhpcndf.h"

#include <vtkVersion.h>
#include <vtkCellArray.h>
#include <vtkPoints.h>
#include <vtkXMLPolyDataWriter.h>
#include <vtkPolyData.h>
#include <vtkSmartPointer.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkPointSource.h>
#include <vtkCleanPolyData.h>
#include <vtkProperty.h>

int main(int argc, char *argv[])
{
    const size_t num_data=std::atol(argv[1])*1024/sizeof(float);
    float* work=new float [num_data];

    std::vector<std::string> filenames;
    for (int i=2; i<argc; i++)
    {
        filenames.push_back(argv[i]);
    }

    for(std::vector<std::string>::iterator it=filenames.begin(); it!=filenames.end(); ++it)
    {
        int key=JHPCNDF::fopen(*it, "rb");
        JHPCNDF::fread(work, sizeof(float), num_data, key);
        JHPCNDF::fclose(key);

        vtkSmartPointer<vtkSphereSource> sphereSource = vtkSmartPointer<vtkSphereSource>::New();
        sphereSource->SetCenter(0.0, 0.0, 0.0);
        sphereSource->SetRadius(5.0);
        vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
        vtkSmartPointer<vtkCellArray> vertices = vtkSmartPointer<vtkCellArray>::New();
        vtkIdType pid[1];
        for ( size_t i = 0; i < num_data; i+=3 )
        {
            pid[0] = points->InsertNextPoint ( &(work[i]) );
            vertices->InsertNextCell(1,pid);
        }
        vtkSmartPointer<vtkPolyData> polydata = vtkSmartPointer<vtkPolyData>::New();
        polydata->SetPoints(points);
        polydata->SetVerts(vertices);

        /*
        //down sampling
        vtkSmartPointer<vtkCleanPolyData> cleanPolyData = vtkSmartPointer<vtkCleanPolyData>::New();
        cleanPolyData->SetInputData(polydata);
        cleanPolyData->SetTolerance(0.1);
        cleanPolyData->Update();
        */

        // Visualize
        // mapper
        vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
#if VTK_MAJOR_VERSION <= 5
        mapper->SetInput(polydata);
#else
        mapper->SetInputData(polydata);
#endif

        //actor
        vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
        actor->SetMapper(mapper);
        actor->GetProperty()->SetPointSize(10.0);
        actor->GetProperty()->SetColor(0.0, 0.0, 0.0);

        //renderer
        vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
        renderer->AddActor(actor);
        renderer->SetBackground(1.0, 1.0, 1.0);

        //renderWindow
        vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
        renderWindow->AddRenderer(renderer);
        renderWindow->SetSize(600, 600);

        //renderWindowInteractor
        vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
        renderWindowInteractor->SetRenderWindow(renderWindow);


        renderWindow->Render();
        renderWindowInteractor->Start();

    }


    return 0;
}
