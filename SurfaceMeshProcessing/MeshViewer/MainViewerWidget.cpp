#include "MainViewerWidget.h"
#include "MeshParamWidget.h"
#include "InteractiveViewerWidget.h"
#include <QLayout>
#include <QMessageBox>
#include <iostream>

MainViewerWidget::MainViewerWidget(QWidget* _parent/* =0 */)
	:loadmeshsuccess(false)
{
	InitViewerWindow();
}

MainViewerWidget::~MainViewerWidget(void)
{
}

void MainViewerWidget::InitViewerWindow(void)
{
	CreateViewerDialog();
	CreateParamWidget();

	QHBoxLayout* main_layout = new QHBoxLayout();
	main_layout->addWidget(meshparamwidget);
	main_layout->addWidget(meshviewerwidget, 1);
	this->setLayout(main_layout);
}

void MainViewerWidget::CreateParamWidget(void)
{
	meshparamwidget = new MeshParamWidget();
	connect(meshparamwidget, SIGNAL(PrintInfoSignal()), meshviewerwidget, SLOT(CalculateCMVCWeight_Test()));
	connect(meshparamwidget, SIGNAL(ClearSignal()), meshviewerwidget, SLOT(ClearSelected()));//����
	//connect(meshparamwidget, SIGNAL(ClearSignal()), meshviewerwidget, SLOT(SetSMNoSelect()));

	connect(meshparamwidget, SIGNAL(NoSelectSignal()), meshviewerwidget, SLOT(SetSMNoSelect()));
	connect(meshparamwidget, SIGNAL(SelectAdjustSignal()), meshviewerwidget, SLOT(SelectSMAdjust()));
	connect(meshparamwidget, SIGNAL(SelectCustomSignal()), meshviewerwidget, SLOT(SetSMCustom()));
	connect(meshparamwidget, SIGNAL(SelectMoveSignal()), meshviewerwidget, SLOT(SetSMSelectAdjustMesh()));
	connect(meshparamwidget, SIGNAL(MoveSignal()), meshviewerwidget, SLOT(SetSMMove()));
	connect(meshparamwidget, SIGNAL(UpdegreeSignal()), meshviewerwidget, SLOT(SetSMUpdegree()));
	connect(meshparamwidget, SIGNAL(ChangedegreeSignal()), meshviewerwidget, SLOT(SetSMChangedegree()));
	connect(meshparamwidget, SIGNAL(NodrawpointSignal()), meshviewerwidget, SLOT(SetSMNodrawpoint()));
	connect(meshparamwidget, SIGNAL(AddpointsSignal()), meshviewerwidget, SLOT(SetSMAddpoints()));
	connect(meshparamwidget, SIGNAL(AddhandlesSignal()), meshviewerwidget, SLOT(SetSMAddhandles()));
	//connect(meshparamwidget, SIGNAL(AddhandlesSignal()), meshviewerwidget, SLOT(SetSMDrawDistort()));
	connect(meshparamwidget, SIGNAL(SelectMoveHandlesSignal()), meshviewerwidget, SLOT(SetSMMovehandles()));

	connect(meshparamwidget, SIGNAL(ChangePointRGB(QRgb)), meshviewerwidget, SLOT(setPointsRGB(QRgb)));
	connect(meshparamwidget, SIGNAL(ChangeBezierRGB(QRgb)), meshviewerwidget, SLOT(setBezierRGB(QRgb)));
	connect(meshparamwidget, SIGNAL(ChangeBezierPolyPointWidth(double)), meshviewerwidget, SLOT(setBezierPolyPointWidth(double)));
	connect(meshparamwidget, SIGNAL(ChangeBezierPolyLineWidth(double)), meshviewerwidget, SLOT(setBezierPolyLineWidth(double)));
	connect(meshparamwidget, SIGNAL(ChangeBezierLineWidth(double)), meshviewerwidget, SLOT(setBezierLineWidth(double)));
	connect(meshparamwidget, SIGNAL(ChangeGreenWeightBih(double)), meshviewerwidget, SLOT(setGreenWeightsBih(double)));
	connect(meshparamwidget, SIGNAL(ChangeBezierPointSize(double)), meshviewerwidget, SLOT(setBezierPointSize(double)));
	connect(meshparamwidget, SIGNAL(ChangeBezierSubdiv(int)), meshviewerwidget, SLOT(setBezierSubdiv(int)));
}

void MainViewerWidget::CreateViewerDialog(void)
{
	meshviewerwidget = new InteractiveViewerWidget(NULL);
	meshviewerwidget->setAcceptDrops(true);
	//connect(meshviewerwidget, SIGNAL(LoadMeshOKSignal(bool, QString)), SLOT(LoadMeshFromInner(bool, QString)));
	connect(
		meshviewerwidget, &InteractiveViewerWidget::LoadMeshOKSignal,
		this, &MainViewerWidget::LoadMeshFromInner
	);
}

void MainViewerWidget::OpenMeshGUI(const QString & fname)
{
	if (fname.isEmpty() || !meshviewerwidget->LoadMesh(fname.toStdString()))
	{
		QString msg = "Cannot read mesh from file:\n '" + fname + "'";
		QMessageBox::critical(NULL, windowTitle(), msg);
	}
	else
	{
		loadmeshsuccess = true;
		emit(haveLoadMesh(fname));
	}
}

void MainViewerWidget::SaveMeshGUI(const QString & fname)
{
	if (fname.isEmpty() || !meshviewerwidget->SaveMesh(fname.toStdString()))
	{
		QString msg = "Cannot read mesh from file:\n '" + fname + "'";
		QMessageBox::critical(NULL, windowTitle(), msg);
	}
}

void MainViewerWidget::LoadMeshFromInner(bool OK, QString fname)
{
	loadmeshsuccess = OK;
	emit(haveLoadMesh(fname));
}

void MainViewerWidget::Open(void)
{
	QString fileName = QFileDialog::getOpenFileName(this,
		tr("Open mesh file"),
		tr(""),
		tr("Mesh Files (*.obj *.off *.ply *.stl);;"
		"OFF Files (*.off);;"
		"OBJ Files (*.obj);;"
		"PLY Files (*.ply);;"
		"STL Files (*.stl);;"
		"All Files (*)"));
	if (!fileName.isEmpty())
	{
		OpenMeshGUI(fileName);
	}
}

void MainViewerWidget::Save(void)
{
	QString fileName = QFileDialog::getSaveFileName(this,
		tr("Save mesh file"),
		tr("untitled.obj"),
		tr("OBJ Files (*.obj);;"
		"OFF Files (*.off);;"
		"PLY Files (*.ply);;"
		"STL Files (*.stl);;"
		"All Files (*)"));
	if (!fileName.isEmpty())
	{
		SaveMeshGUI(fileName);
	}
}

void MainViewerWidget::ClearMesh(void)
{
	if (loadmeshsuccess)
	{
		loadmeshsuccess = false;
		meshviewerwidget->Clear();
	}
}

void MainViewerWidget::Screenshot(void)
{
	meshviewerwidget->ScreenShot();
}

void MainViewerWidget::ShowPoints(void)
{
	meshviewerwidget->CoonsWeight_Test();
	meshviewerwidget->SetDrawMode(InteractiveViewerWidget::CURVECAGE);
	//meshviewerwidget->SetDrawMode(InteractiveViewerWidget::POINTS);
}

void MainViewerWidget::ShowWireframe(void)
{
	meshviewerwidget->CoonsWeight_Test();
	meshviewerwidget->SetDrawMode(InteractiveViewerWidget::CURVECAGE);
	//meshviewerwidget->SetDrawMode(InteractiveViewerWidget::WIREFRAME);
}

void MainViewerWidget::ShowHiddenLines(void)
{
	meshviewerwidget->BiharmonicCurve_Test();
	meshviewerwidget->SetDrawMode(InteractiveViewerWidget::CURVECAGE);
}

void MainViewerWidget::ShowFlatLines(void)
{
	meshviewerwidget->SetDrawMode(InteractiveViewerWidget::FLATLINES);
}

void MainViewerWidget::ShowFlat(void)
{
	meshviewerwidget->SetDrawMode(InteractiveViewerWidget::FLAT);
}

void MainViewerWidget::ShowSmooth(void)
{
	meshviewerwidget->SetDrawMode(InteractiveViewerWidget::SMOOTH);
}

void MainViewerWidget::ShowCurveCage(void)
{
	meshviewerwidget->CurveCage_Test();
	meshviewerwidget->SetDrawMode(InteractiveViewerWidget::CURVECAGE);
}
void MainViewerWidget::ShowPolyGC(void)
{
	meshviewerwidget->PolyGC_Test();
	meshviewerwidget->SetDrawMode(InteractiveViewerWidget::CURVECAGE);
}
void MainViewerWidget::ShowCubicMVC(void)
{
	meshviewerwidget->straight_Bih_Test();
	meshviewerwidget->SetDrawMode(InteractiveViewerWidget::CURVECAGE);
}

void MainViewerWidget::DrawVideo1(void)
{
	meshviewerwidget->straight_Bih_Test();
	meshviewerwidget->SetDrawMode(InteractiveViewerWidget::CURVECAGE);
}

void MainViewerWidget::DrawVideo2(void)
{
	meshviewerwidget->straight_Bih_Test();
	meshviewerwidget->SetDrawMode(InteractiveViewerWidget::CURVECAGE);
}

void MainViewerWidget::DrawVideo3(void)
{
	meshviewerwidget->straight_Bih_Test();
	meshviewerwidget->SetDrawMode(InteractiveViewerWidget::CURVECAGE);
}

void MainViewerWidget::Lighting(bool b)
{
	meshviewerwidget->EnableLighting(b);
}

void MainViewerWidget::DoubleSideLighting(bool b)
{
	meshviewerwidget->EnableDoubleSide(b);
}

void MainViewerWidget::ShowBoundingBox(bool b)
{
	meshviewerwidget->SetDrawBoundingBox(b);
}

void MainViewerWidget::ShowBoundary(bool b)
{
	meshviewerwidget->SetDrawBoundary(b);
}

void MainViewerWidget::ResetView(void)
{
	meshviewerwidget->ResetView();
}

void MainViewerWidget::ViewCenter(void)
{
	meshviewerwidget->ViewCenter();
}

void MainViewerWidget::CopyRotation(void)
{
	meshviewerwidget->CopyRotation();
}

void MainViewerWidget::LoadRotation(void)
{
	meshviewerwidget->LoadRotation();
}
