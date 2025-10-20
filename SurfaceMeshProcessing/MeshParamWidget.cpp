#include "MeshParamWidget.h"

MeshParamWidget::MeshParamWidget(QWidget *parent)
	: QWidget(parent)
{
	CreateTabWidget();
	CreateLayout();
}

MeshParamWidget::~MeshParamWidget()
{
}

void MeshParamWidget::CreateTabWidget(void)
{
	pbPrintInfo = new QPushButton(tr("Cubic MVC"));
	connect(pbPrintInfo, SIGNAL(clicked()), SIGNAL(PrintInfoSignal()));

	noSelectBtn = new QPushButton(tr("No Select"));
	SelectAdjustBtn = new QPushButton(tr("Adjust Cage"));
	selectCustomBtn = new QPushButton(tr("Select Custom"));
	moveBtn = new QPushButton(tr("Move"));
	selectMoveBtn = new QPushButton(tr("Select & Move"));
	clearBtn = new QPushButton(tr("Deform"));
	updegreeBtn = new QPushButton(tr("Updegree"));
	changedegreeBtn = new QPushButton(tr("Changedegree"));
	nodrawpointBtn = new QPushButton(tr("No drawpoint"));
	addpointsBtn = new QPushButton(tr("Add points"));
	/*addhandlesBtn = new QPushButton(tr("Show Distort"));*/
	addhandlesBtn = new QPushButton(tr("Add handle"));
	selectMoveHandlesBtn = new QPushButton(tr("Cal coor H"));

	pointsChooseColor = new QPushButton(tr("Choose Point Color"), this);
	bezierChooseColor = new QPushButton(tr("Choose Color"), this);
	bezierPolyPointWidth = new QDoubleSpinBox(this);
	bezierPolyLineWidth = new QDoubleSpinBox(this);
	bezierLineWidth = new QDoubleSpinBox(this);
	bezierPointSize = new QDoubleSpinBox(this);
	greenWeightBih = new QDoubleSpinBox(this);
	bezierSubdiv = new QSpinBox(this);
	
	connect(SelectAdjustBtn, SIGNAL(clicked()), SIGNAL(SelectAdjustSignal()));
	connect(selectCustomBtn, SIGNAL(clicked()), SIGNAL(SelectCustomSignal()));
	connect(selectMoveBtn, SIGNAL(clicked()), SIGNAL(SelectMoveSignal()));
	connect(moveBtn, SIGNAL(clicked()), SIGNAL(MoveSignal()));
	connect(clearBtn, SIGNAL(clicked()), SIGNAL(ClearSignal()));
	connect(clearBtn, SIGNAL(clicked()), SLOT(ResetCheck()));
	connect(updegreeBtn, SIGNAL(clicked()), SIGNAL(UpdegreeSignal()));
	connect(changedegreeBtn, SIGNAL(clicked()), SIGNAL(ChangedegreeSignal()));
	connect(nodrawpointBtn, SIGNAL(clicked()), SIGNAL(NodrawpointSignal()));
	connect(addpointsBtn, SIGNAL(clicked()), SIGNAL(AddpointsSignal()));
	connect(addhandlesBtn, SIGNAL(clicked()), SIGNAL(AddhandlesSignal()));
	connect(selectMoveHandlesBtn, SIGNAL(clicked()), SIGNAL(SelectMoveHandlesSignal()));

	connect(pointsChooseColor, SIGNAL(clicked()), this, SLOT(setPointColorClicked()));
	connect(bezierChooseColor, SIGNAL(clicked()), this, SLOT(setBezierColorClicked()));
	connect(bezierSubdiv, SIGNAL(valueChanged(int)), SIGNAL(ChangeBezierSubdiv(int)));
	connect(bezierPolyPointWidth, SIGNAL(valueChanged(double)), SIGNAL(ChangeBezierPolyPointWidth(double)));
	connect(bezierPolyLineWidth, SIGNAL(valueChanged(double)), SIGNAL(ChangeBezierPolyLineWidth(double)));
	connect(bezierLineWidth, SIGNAL(valueChanged(double)), SIGNAL(ChangeBezierLineWidth(double)));
	connect(bezierPointSize, SIGNAL(valueChanged(double)), SIGNAL(ChangeBezierPointSize(double)));
	connect(greenWeightBih, SIGNAL(valueChanged(double)), SIGNAL(ChangeGreenWeightBih(double)));

	bezierSubdiv->setValue(50);
	bezierPolyPointWidth->setValue(10.00);
	bezierPolyLineWidth->setValue(10.00);
	bezierLineWidth->setValue(0.01);
	bezierPointSize->setValue(5.0);
	greenWeightBih->setValue(0.10000);

	bezierSubdiv->setRange(10, 1000);

	bezierPolyPointWidth->setRange(0.0, 20.0);
	bezierPolyPointWidth->setDecimals(4);
	bezierPolyPointWidth->setSingleStep(0.100);
	bezierPolyPointWidth->setPrefix(tr("PolyPoint width: "));

	bezierPolyLineWidth->setRange(0.0, 20.0);
	bezierPolyLineWidth->setDecimals(4);
	bezierPolyLineWidth->setSingleStep(0.100);
	bezierPolyLineWidth->setPrefix(tr("Polyline width: "));

	bezierLineWidth->setRange(0.0, 2.0);
	bezierLineWidth->setDecimals(4);
	bezierLineWidth->setSingleStep(0.001);
	bezierLineWidth->setPrefix(tr("line width: "));
	bezierPointSize->setRange(0.0, 1.0);
	bezierPointSize->setDecimals(4);
	bezierPointSize->setSingleStep(0.01);
	bezierPointSize->setPrefix(tr("weights: "));
	greenWeightBih->setRange(0, 100);
	greenWeightBih->setDecimals(10);
	greenWeightBih->setSingleStep(0.001);
	greenWeightBih->setPrefix(tr("p2p weight: "));

	QVBoxLayout *layout = new QVBoxLayout();
	layout->addWidget(pbPrintInfo);
	layout->addWidget(noSelectBtn);
	layout->addWidget(SelectAdjustBtn);
	layout->addWidget(selectCustomBtn);
	layout->addWidget(moveBtn);
	layout->addWidget(selectMoveBtn);
	layout->addWidget(clearBtn);
	layout->addWidget(updegreeBtn);
	layout->addWidget(changedegreeBtn);
	layout->addWidget(nodrawpointBtn);
	layout->addWidget(addpointsBtn);
	layout->addWidget(addhandlesBtn);
	layout->addWidget(selectMoveHandlesBtn);
	layout->addStretch();
	layout->addWidget(pointsChooseColor);
	layout->addWidget(bezierChooseColor);
	layout->addWidget(bezierSubdiv);
	layout->addWidget(bezierPolyPointWidth);
	layout->addWidget(bezierPolyLineWidth);
	layout->addWidget(bezierLineWidth);
	layout->addWidget(bezierPointSize);
	layout->addWidget(greenWeightBih);
	wParam = new QWidget();
	wParam->setLayout(layout);
	saParam = new QScrollArea();
	saParam->setFocusPolicy(Qt::NoFocus);
	saParam->setFrameStyle(QFrame::NoFrame);
	saParam->setWidget(wParam);
	saParam->setWidgetResizable(true);
}


void MeshParamWidget::CreateLayout(void)
{
	twParam = new QTabWidget();
	twParam->addTab(saParam, "Tab");
	QGridLayout *layout = new QGridLayout();
	layout->addWidget(twParam, 0, 0, 1, 1);
	
	this->setLayout(layout);
}


void MeshParamWidget::ResetCheck()
{
	noSelectBtn->setChecked(true);
	SelectAdjustBtn->setChecked(false);
	selectCustomBtn->setChecked(false);
	moveBtn->setChecked(false);
	selectMoveBtn->setChecked(false);
	updegreeBtn->setChecked(false);
	changedegreeBtn->setChecked(false);
	nodrawpointBtn->setChecked(false);
	addpointsBtn->setChecked(false);
	addhandlesBtn->setChecked(false);
	selectMoveHandlesBtn->setChecked(false);
}

void MeshParamWidget::setBezierColorClicked()
{
	QColor color = QColorDialog::getColor(Qt::blue, this);
	if (!color.isValid()) {
		return;
	}
	else {
		QRgb rgb = color.rgb();
		emit ChangeBezierRGB(rgb);
	}
}

void MeshParamWidget::setPointColorClicked()
{
	QColor color = QColorDialog::getColor(Qt::blue, this);
	if (!color.isValid()) {
		return;
	}
	else {
		QRgb rgb = color.rgb();
		emit ChangePointRGB(rgb);
	}
}
