#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <vtkSmartPointer.h>
#include <vtkCubeSource.h>
#include <vtkActor.h>
#include <vtkProperty.h>
#include <vtkCamera.h>
#include <vtkPolyData.h>
#include <vtkDataSet.h>
#include <vtkDataSetMapper.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkDataSetReader.h>
#include <vtkNamedColors.h>
#include <vtkNew.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkLight.h>
#include <vtkPlane.h>
#include <vtkShrinkFilter.h>
#include <vtkClipDataSet.h>
#include <vtkSTLReader.h>
#include <QMainWindow>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QColorDialog>
#include <QColor>
#include <vtkLightCollection.h>
#include <vtkGeometryFilter.h>

#include <vtkPyramid.h>
#include <vtkPoints.h>
#include <vtkUnstructuredGrid.h>
#include <vtkCellArray.h>
#include <vtkAlgorithm.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

private slots:
   void handleClipBox(bool checked);
   void handleShrinkBox(bool checked);
public:
    explicit MainWindow(QWidget *parent = 0);
    void addDataSet(vtkSmartPointer<vtkSTLReader> reader);
    void addDataSet(vtkSmartPointer<vtkCubeSource> cubeSource);
    void addDataSet(vtkSmartPointer<vtkUnstructuredGrid> ug);
    void removeDataSet();
    ~MainWindow();
    void GenerateCube();
    void GeneratePyramid();
    void ChModelColour();
	void ChBgColour();
	void LightSlider(int n);
	vtkSmartPointer<vtkLight> light = vtkSmartPointer<vtkLight>::New();
	vtkSmartPointer<vtkAlgorithm> source;
	vtkSmartPointer<vtkShrinkFilter> shrinkfilter = vtkSmartPointer<vtkShrinkFilter>::New();
	vtkSmartPointer<vtkPlane> planeLeft = vtkSmartPointer<vtkPlane>::New();
    vtkSmartPointer<vtkClipDataSet> clipfilter = vtkSmartPointer<vtkClipDataSet>::New();
    //vtkSmartPointer<vtkDataSetMapper> mapper;
    //vtkSmartPointer<vtkActor> actor;
public slots:

    
protected:
	void handleOpenAction();
	void handleModelView();
    
private:
    Ui::MainWindow *ui;
    vtkSmartPointer<vtkRenderer> renderer;
    //vtkSmartPointer<vtkSTLReader> reader;
};

#endif // MAINWINDOW_H



