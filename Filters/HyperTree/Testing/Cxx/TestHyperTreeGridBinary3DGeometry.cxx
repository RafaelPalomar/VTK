/*==================================================================

  Program:   Visualization Toolkit
  Module:    TestHyperTreeGridBinary3DGeometry.cxx

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

===================================================================*/
// .SECTION Thanks
// This test was written by Philippe Pebay, NexGen Analytics 2017
// This work was supported by Commissariat a l'Energie Atomique (CEA/DIF)

#include "vtkHyperTreeGridGeometry.h"
#include "vtkHyperTreeGridSource.h"

#include "vtkCamera.h"
#include "vtkCellData.h"
#include "vtkNew.h"
#include "vtkPolyData.h"
#include "vtkPolyDataMapper.h"
#include "vtkProperty.h"
#include "vtkRegressionTestImage.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRenderer.h"

int TestHyperTreeGridBinary3DGeometry( int argc, char* argv[] )
{
  // Hyper tree grid
  vtkNew<vtkHyperTreeGridSource> htGrid;
  htGrid->SetMaxDepth(6);
  htGrid->SetDimensions( 4, 4, 3 ); //Dimension 3 GridCell 3, 3, 2
  htGrid->SetGridScale( 1.5, 1., .7 );
  htGrid->SetBranchFactor( 2 );
  htGrid->SetDescriptor( "RRR .R. .RR ..R ..R .R.|R....... ........ ........ ...R.... .RRRR.R. RRRRR.RR ........ ........ ........|........ ........ ........ RR.RR.RR ........ RR...... ........ ........ ........ ........ ........ ........ ........ ..RRR...|........ ..R..... ........ ........ ........ ........ ........ ........ ........ ........ ........|........" );

  // Geometry
  vtkNew<vtkHyperTreeGridGeometry> geometry;
  geometry->SetInputConnection( htGrid->GetOutputPort() );
  geometry->Update();
  vtkPolyData* pd = geometry->GetPolyDataOutput();

  // Mappers
  vtkMapper::SetResolveCoincidentTopologyToPolygonOffset();
  vtkNew<vtkPolyDataMapper> mapper1;
  mapper1->SetInputConnection( geometry->GetOutputPort() );
  mapper1->SetScalarRange( pd->GetCellData()->GetScalars()->GetRange() );
  vtkNew<vtkPolyDataMapper> mapper2;
  mapper2->SetInputConnection( geometry->GetOutputPort() );
  mapper2->ScalarVisibilityOff();

  // Actors
  vtkNew<vtkActor> actor1;
  actor1->SetMapper( mapper1 );
  vtkNew<vtkActor> actor2;
  actor2->SetMapper( mapper2 );
  actor2->GetProperty()->SetRepresentationToWireframe();
  actor2->GetProperty()->SetColor( .7, .7, .7 );

  // Camera
  double bd[6];
  pd->GetBounds( bd );
  vtkNew<vtkCamera> camera;
  camera->SetClippingRange( 1., 100. );
  camera->SetFocalPoint( pd->GetCenter() );
  camera->SetPosition( -.8 * bd[1], 2.1 * bd[3], -4.8 * bd[5] );

  // Renderer
  vtkNew<vtkRenderer> renderer;
  renderer->SetActiveCamera( camera );
  renderer->SetBackground( 1., 1., 1. );
  renderer->AddActor( actor1 );
  renderer->AddActor( actor2 );

  // Render window
  vtkNew<vtkRenderWindow> renWin;
  renWin->AddRenderer( renderer );
  renWin->SetSize( 400, 400 );
  renWin->SetMultiSamples( 0 );

  // Interactor
  vtkNew<vtkRenderWindowInteractor> iren;
  iren->SetRenderWindow( renWin );

  // Render and test
  renWin->Render();

  int retVal = vtkRegressionTestImageThreshold( renWin, 90 );
  if ( retVal == vtkRegressionTester::DO_INTERACTOR )
  {
    iren->Start();
  }

  return !retVal;
}
