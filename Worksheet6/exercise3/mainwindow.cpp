#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    // standard call to setup Qt UI (same as previously)
    ui->setupUi( this );
	connect( ui->actionFileOpen, &QAction::triggered, this, &MainWindow::handleOpenAction);
	connect( ui->ModelView, &QPushButton::released, this, &MainWindow::handleModelView);
	connect( ui->ClipBox, &QCheckBox::clicked, this, &MainWindow::handleClipBox);
	connect( ui->ShrinkBox, &QCheckBox::clicked, this, &MainWindow::handleShrinkBox);
	connect( ui->Cube, &QPushButton::released, this, &MainWindow::GenerateCube);
	connect( ui->Pyramid, &QPushButton::released, this, &MainWindow::GeneratePyramid);
	connect( ui->ModelColour, &QPushButton::released, this, &MainWindow::ChModelColour);
	connect( ui->BgColour, &QPushButton::released, this, &MainWindow::ChBgColour);
	connect( ui->Slider, &QSlider::sliderMoved, this, &MainWindow::LightSlider);
	
	// Now need to create a VTK render window and link it to the QtVTK widget	vtkNew<vtkGenericOpenGLRenderWindow> renderWindow;
	vtkNew<vtkGenericOpenGLRenderWindow> renderWindow;
	ui->qvtkWidget->SetRenderWindow( renderWindow );			// note that vtkWidget is the name I gave to my QtVTKOpenGLWidget in Qt creator
	renderer = vtkSmartPointer<vtkRenderer>::New();
    renderer->SetBackground(0.5, 0.5, 0.5);
    ui->qvtkWidget->GetRenderWindow()->AddRenderer(renderer);
    renderer->ResetCamera();
	renderer->GetActiveCamera()->Azimuth(30);
	renderer->GetActiveCamera()->Elevation(30);
	renderer->ResetCameraClippingRange();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::addDataSet(vtkSmartPointer<vtkSTLReader> reader)
{
    // Mapper
    vtkSmartPointer<vtkDataSetMapper> mapper = vtkSmartPointer<vtkDataSetMapper>::New();
    mapper->SetInputConnection(reader->GetOutputPort());
    
    // Actor
    vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
	actor->GetProperty()->EdgeVisibilityOn();

    
    actor->SetMapper(mapper);
	
	ui->qvtkWidget->GetRenderWindow()->AddRenderer( renderer );	
	
    renderer->AddActor(actor);
	renderer->ResetCamera();
	renderer->SetBackground(0.5, 0.5, 0.5);
	
	light->SetLightTypeToSceneLight();
	light->SetPosition( 5, 5, 15 );
	light->SetPositional( true );
	light->SetConeAngle( 10 );
	light->SetFocalPoint( 0, 0, 0 );
	light->SetDiffuseColor( 1, 1, 1 );
	light->SetAmbientColor( 1, 1, 1 );
	light->SetSpecularColor( 1, 1, 1 );
	light->SetIntensity( 0.5 );
	
	renderer->SetBackground(0.5, 0.5, 0.5);
	source=reader;
    ui->qvtkWidget->GetRenderWindow()->Render();
	renderer->AddLight( light );
}


void MainWindow::addDataSet(vtkSmartPointer<vtkCubeSource> cubeSource)
{
    vtkSmartPointer<vtkDataSetMapper> mapper = vtkSmartPointer<vtkDataSetMapper>::New();
    mapper->SetInputConnection(cubeSource->GetOutputPort());
    
    // Actor
    vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
	actor->GetProperty()->EdgeVisibilityOn();

    actor->SetMapper(mapper);
	
	ui->qvtkWidget->GetRenderWindow()->AddRenderer( renderer );	
	
    renderer->AddActor(actor);
    renderer->SetBackground(0.5, 0.5, 0.5);
	renderer->ResetCamera();
	
	light->SetLightTypeToSceneLight();
	light->SetPosition( 5, 5, 15 );
	light->SetPositional( true );
	light->SetConeAngle( 10 );
	light->SetFocalPoint( 0, 0, 0 );
	light->SetDiffuseColor( 1, 1, 1 );
	light->SetAmbientColor( 1, 1, 1 );
	light->SetSpecularColor( 1, 1, 1 );
	light->SetIntensity( 0.5 );
	source=cubeSource;
    ui->qvtkWidget->GetRenderWindow()->Render();
    
	renderer->AddLight( light );
	
}

void MainWindow::addDataSet(vtkSmartPointer<vtkUnstructuredGrid> ug)
{
    vtkSmartPointer<vtkDataSetMapper> mapper = vtkSmartPointer<vtkDataSetMapper>::New();
    mapper->SetInputData(ug);
    
    // Actor
    vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
	actor->GetProperty()->EdgeVisibilityOn();

    
    actor->SetMapper(mapper);
	
	ui->qvtkWidget->GetRenderWindow()->AddRenderer( renderer );	
	
    renderer->AddActor(actor);
	renderer->ResetCamera();
	renderer->SetBackground(0.5, 0.5, 0.5);
	
	light->SetLightTypeToSceneLight();
	light->SetPosition( 5, 5, 15 );
	light->SetPositional( true );
	light->SetConeAngle( 10 );
	light->SetFocalPoint( 0, 0, 0 );
	light->SetDiffuseColor( 1, 1, 1 );
	light->SetAmbientColor( 1, 1, 1 );
	light->SetSpecularColor( 1, 1, 1 );
	light->SetIntensity( 0.5 );
	
	vtkSmartPointer<vtkGeometryFilter> filter = vtkSmartPointer<vtkGeometryFilter>::New();
	filter->SetInputData(ug);
	source=filter;
    ui->qvtkWidget->GetRenderWindow()->Render();
	renderer->AddLight( light );
}

void MainWindow::removeDataSet()
{
    vtkActor* actor = renderer->GetActors()->GetLastActor();
    if (actor != nullptr) {
        renderer->RemoveActor(actor);
    }
    ui->qvtkWidget->GetRenderWindow()->Render();
}

void MainWindow::handleOpenAction() 
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open STL File"), "./", tr("STL Files(*.stl)"));
	std::string str = fileName.toUtf8().constData();
	vtkSmartPointer<vtkSTLReader> reader = vtkSmartPointer<vtkSTLReader>::New();
	reader->SetFileName(str.data());
	reader->Update();
	removeDataSet();
    vtkSmartPointer<vtkDataSet> dataSet = reader->GetOutput();
    if (dataSet != nullptr) 
	{
        addDataSet(reader);
    }
}

void MainWindow::handleModelView()
{
	vtkSmartPointer<vtkActor> actor = renderer->GetActors()->GetLastActor();
    if (actor != nullptr) {
        renderer->ResetCamera(actor->GetBounds());
    }
    ui->qvtkWidget->GetRenderWindow()->Render();
}

void MainWindow::handleClipBox(bool checked)
{
    vtkMapper * mapper = renderer->GetActors()->GetLastActor()->GetMapper();
    //vtkDataSet* dataSet = renderer->GetActors()->GetLastActor()->GetMapper()->GetInput();
    //vtkSmartPointer<vtkGeometryFilter> filter = vtkSmartPointer<vtkGeometryFilter>::New();
	if (ui->ClipBox->isChecked()) 
	{
		if (ui->ShrinkBox->isChecked()) 
		{	
			planeLeft->SetOrigin(0.0, 0.0, 0.0);
			planeLeft->SetNormal(-1.0, 0.0, 0.0);
			clipfilter->SetInputConnection(source->GetOutputPort());
			clipfilter->SetClipFunction(planeLeft.Get());
			shrinkfilter->SetInputConnection(clipfilter->GetOutputPort());
			shrinkfilter->SetShrinkFactor(.8);
			shrinkfilter->Update();
			mapper->SetInputConnection(shrinkfilter->GetOutputPort());
		}
		else 
		{
			planeLeft->SetOrigin(0.0, 0.0, 0.0);
			planeLeft->SetNormal(-1.0, 0.0, 0.0);
			clipfilter->SetInputConnection(source->GetOutputPort());
			clipfilter->SetClipFunction(planeLeft.Get());
			mapper->SetInputConnection(clipfilter->GetOutputPort());
		}
	
    }
	else 
	{
		if (ui->ShrinkBox->isChecked()) 
		{	
			shrinkfilter->SetInputConnection(source->GetOutputPort());
			shrinkfilter->SetShrinkFactor(.8);
			shrinkfilter->Update();
			mapper->SetInputConnection(shrinkfilter->GetOutputPort());
		}
		else
		{
			mapper->SetInputConnection(source->GetOutputPort());
		}
    }
    ui->qvtkWidget->GetRenderWindow()->Render();
}

void MainWindow::handleShrinkBox(bool checked) 
{
	vtkMapper * mapper = renderer->GetActors()->GetLastActor()->GetMapper();
	//vtkDataSet* dataSet = renderer->GetActors()->GetLastActor()->GetMapper()->GetInput();
    //vtkSmartPointer<vtkGeometryFilter> filter = vtkSmartPointer<vtkGeometryFilter>::New();
	
	if (ui->ClipBox->isChecked()) 
	{
		if (ui->ShrinkBox->isChecked()) 
		{	
			planeLeft->SetOrigin(0.0, 0.0, 0.0);
			planeLeft->SetNormal(-1.0, 0.0, 0.0);
			clipfilter->SetInputConnection(source->GetOutputPort());
			clipfilter->SetClipFunction(planeLeft.Get());
			shrinkfilter->SetInputConnection(clipfilter->GetOutputPort());
			shrinkfilter->SetShrinkFactor(.8);
			shrinkfilter->Update();
			mapper->SetInputConnection(shrinkfilter->GetOutputPort());
		}
		else 
		{
			planeLeft->SetOrigin(0.0, 0.0, 0.0);
			planeLeft->SetNormal(-1.0, 0.0, 0.0);
			clipfilter->SetInputConnection(source->GetOutputPort());
			clipfilter->SetClipFunction(planeLeft.Get());
			mapper->SetInputConnection(clipfilter->GetOutputPort());
		}
	
    }
	else 
	{
		if (ui->ShrinkBox->isChecked()) 
		{	
			shrinkfilter->SetInputConnection(source->GetOutputPort());
			shrinkfilter->SetShrinkFactor(.8);
			shrinkfilter->Update();
			mapper->SetInputConnection(shrinkfilter->GetOutputPort());
		}
		else
		{
			mapper->SetInputConnection(source->GetOutputPort());
		}
    }

	ui->qvtkWidget->GetRenderWindow()->Render();
}


void MainWindow::GenerateCube()
{
	removeDataSet();
    vtkSmartPointer<vtkCubeSource> cubeSource = vtkSmartPointer<vtkCubeSource>::New();
	addDataSet(cubeSource);
}

void MainWindow::GeneratePyramid()
{	
    removeDataSet();
    vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
    float p0[3] = {1.0, 1.0, 1.0};
    float p1[3] = {-1.0, 1.0, 1.0};
    float p2[3] = {-1.0, -1.0, 1.0};
    float p3[3] = {1.0, -1.0, 1.0};
    float p4[3] = {0.0, 0.0, 0.0};

    points->InsertNextPoint(p0);
  	points->InsertNextPoint(p1);
  	points->InsertNextPoint(p2);
  	points->InsertNextPoint(p3);
  	points->InsertNextPoint(p4);
  	
  	vtkSmartPointer<vtkPyramid> pyramid = vtkSmartPointer<vtkPyramid>::New();
  	pyramid->GetPointIds()->SetId(0,0);
  	pyramid->GetPointIds()->SetId(1,1);
  	pyramid->GetPointIds()->SetId(2,2);
  	pyramid->GetPointIds()->SetId(3,3);
  	pyramid->GetPointIds()->SetId(4,4);
  
  	vtkSmartPointer<vtkCellArray> cells = vtkSmartPointer<vtkCellArray>::New();
  	cells->InsertNextCell (pyramid);
  
  	vtkSmartPointer<vtkUnstructuredGrid> ug = vtkSmartPointer<vtkUnstructuredGrid>::New();
    ug->SetPoints(points);
    ug->InsertNextCell(pyramid->GetCellType(),pyramid->GetPointIds());
    addDataSet(ug);
}

void MainWindow::ChModelColour()
{
	int r,g,b,opacity;
	QColor color = QColorDialog::getColor(Qt::white,this,"Choose Model Colour");
	color.getRgb(&r,&g,&b,&opacity);
	vtkActor* actor = renderer->GetActors()->GetLastActor();
	actor->GetProperty()->SetColor(double(r)/256.0,double(g)/256.0,double(b)/256.0);
	actor->GetProperty()->EdgeVisibilityOn();
	ui->qvtkWidget->GetRenderWindow()->Render();
}

void MainWindow::ChBgColour()
{
	int r,g,b,opacity;
	QColor color = QColorDialog::getColor(Qt::white,this,"Choose Background Colour");
	color.getRgb(&r,&g,&b,&opacity);
	renderer->SetBackground(double(r)/256.0,double(g)/256.0,double(b)/256.0);
	ui->qvtkWidget->GetRenderWindow()->Render();
}

void MainWindow::LightSlider(int n)
{
	light->SetIntensity((float)(n)/ 100);
	ui->qvtkWidget->GetRenderWindow()->Render();
}

