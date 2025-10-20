#pragma once

#include <QWidget>
#include <QtGui>
#include <QtWidgets>

class MeshParamWidget : public QWidget
{
	Q_OBJECT

public:
	MeshParamWidget(QWidget *parent = 0);
	~MeshParamWidget(void);
private:
	void CreateTabWidget(void);
	void CreateLayout(void);
signals:
	void PrintInfoSignal();
	void NoSelectSignal();
	void SelectAdjustSignal();
	void SelectCustomSignal();
	void SelectMoveSignal();
	void MoveSignal();
	void UpdegreeSignal();
	void ChangedegreeSignal();
	void NodrawpointSignal();
	void AddpointsSignal();
	void AddhandlesSignal();
	void SelectMoveHandlesSignal();
	void ClearSignal();
	void ChangePointRGB(QRgb);
	void ChangeBezierRGB(QRgb);
	void ChangeBezierPolyPointWidth(double);
	void ChangeBezierPolyLineWidth(double);
	void ChangeBezierLineWidth(double);
	void ChangeBezierPointSize(double);
	void ChangeBezierSubdiv(int);
	void ChangeGreenWeightBih(double);
private slots:
	void ResetCheck();
	void setBezierColorClicked();
	void setPointColorClicked();
private:
	QTabWidget *twParam;
	QWidget *wParam;
	QScrollArea *saParam;
	QPushButton *pbPrintInfo;
	QPushButton* noSelectBtn;
	QPushButton* SelectAdjustBtn;
	QPushButton* selectCustomBtn;
	QPushButton* moveBtn;
	QPushButton* selectMoveBtn;
	QPushButton* clearBtn;
	QPushButton* updegreeBtn;
	QPushButton* changedegreeBtn;
	QPushButton* nodrawpointBtn;
	QPushButton* addpointsBtn;
	QPushButton* addhandlesBtn;
	QPushButton* selectMoveHandlesBtn;
	QButtonGroup* deformBtnGroup;

	QPushButton* pointsChooseColor;
	QPushButton* bezierChooseColor;
	QSpinBox* bezierSubdiv;
	QDoubleSpinBox* bezierPolyPointWidth;
	QDoubleSpinBox* bezierPolyLineWidth;
	QDoubleSpinBox* bezierLineWidth;
	QDoubleSpinBox* bezierPointSize;//green Ë«µ÷ºÍweight
	QDoubleSpinBox* greenWeightBih;//p2pweight
	QWidget* deformWidget;
};
