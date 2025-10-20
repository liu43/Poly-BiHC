#pragma once
#include "boost/container/small_vector.hpp"

#include <QString>
#include <QEvent>
#include <QMouseEvent>
#include "QGLViewerWidget.h"
#include <OpenMesh/Core/Utils/PropertyManager.hh>
#include "MeshDefinition.h"

#include "BezierCurve.h"
#include "MVC.h"
#include "Gradient.h"
#include "blendt2s.h"
#include "toolbox\Math\TriangleInterface.h"
#include <QOpenGLFunctions>
//#include <omp.h>
class MeshViewerWidget : public QGLViewerWidget/*, protected QOpenGLFunctions*/
{
	Q_OBJECT
public:
	enum SelectMode {
		NoSelect,
		SelectAdjust,
		SelectCustom,
		Move,
		SelectAdjustMesh
	};
	struct Triangle {
		std::array<OpenMesh::Vec3f, 3> vertices;
		std::array<OpenMesh::Vec2f, 3> texcoords;
		float zCenter;
	};

	template <size_t N>
	struct myArray {
		std::array<double, N> data;
		// 添加下标运算符重载
		double& operator[](size_t index) {
			return data[index];
		}
		const double& operator[](size_t index) const {
			return data[index];
		};

			// 新增 size() 成员函数
		constexpr size_t size() const noexcept {
			return N;
		};


		// 重载负号运算符
		myArray operator-() const {
			myArray result;
			for (size_t i = 0; i < N; ++i) {
				result.data[i] = -data[i];
			}
			return result;
		}
	};
	MeshViewerWidget(QWidget* parent = 0);
	virtual ~MeshViewerWidget(void);
	bool LoadMesh(const std::string & filename);
	void Clear(void);
	void UpdateMesh(void);
	bool SaveMesh(const std::string & filename);
	bool ScreenShot(void);
	void SetDrawBoundingBox(bool b);
	void SetDrawBoundary(bool b);
	void EnableLighting(bool b);
	void EnableDoubleSide(bool b);
	void ResetView(void);
	void ViewCenter(void);
	void CopyRotation(void);
	void LoadRotation(void);
signals:
	void LoadMeshOKSignal(bool, QString);
public slots:
	void PrintMeshInfo(void);
	void SelectSMAdjust(void);
	void SetSMCustom(void);
	void SetSMMove(void);
	void SetSMSelectAdjustMesh(void);
	void SetSMUpdegree(void);
	void SetSMChangedegree(void);
	void SetSMNodrawpoint(void);
	void SetSMAddpoints(void);
	void SetSMAddhandles(void);
	void SetSMDrawDistort(void);
	void SetSMMovehandles(void);
	void SetSMNoSelect(void);
	void ClearSelected(void);//变形 

	void setBezierRGB(QRgb rgb);
	void setPointsRGB(QRgb rgb);
	void setBezierSubdiv(int);
	void setBezierPolyPointWidth(double);
	void setBezierPolyLineWidth(double);
	void setBezierLineWidth(double);
	void setBezierPointSize(double);
	void setGreenWeightsBih(double);
protected:
	virtual bool event(QEvent* _event) override;
	virtual void mouseDoubleClickEvent(QMouseEvent* _event) override;
	virtual void DrawScene(void) override;
	void DrawSceneMesh(void);
	OpenMesh::Vec3d Vec223(const OpenMesh::Vec2d& vec2d) {
		return OpenMesh::Vec3d(vec2d[0], vec2d[1], 0.0);
	}
	OpenMesh::Vec2d Vec322(const OpenMesh::Vec3d& vec3d) {
		return OpenMesh::Vec2d(vec3d[0], vec3d[1]);
	}
	std::vector<double> calculateDistortARAP(const Mesh& src, const Mesh& dst);
	std::vector<double> calculateDistortSymDirichlet(const Mesh& src, const Mesh& dst);
	bool NearestVertex(OpenMesh::Vec3d objCor, OpenMesh::VertexHandle& minVh);
	std::vector<OpenMesh::Vec2d> offsetCage(const std::vector<OpenMesh::Vec2d>& linear_cage, double displacement);
	std::vector<OpenMesh::Vec2d> offsetCage2(const std::vector<OpenMesh::Vec2d>& curve_cage2, double displacement);
	std::vector<OpenMesh::Vec2d> offsetCage3(const std::vector<OpenMesh::Vec2d>& curve_cage2, double displacement);
	std::vector<OpenMesh::Vec2d> offsetCage_samples(const std::vector<OpenMesh::Vec2d>& curve_cage3, double displacement, int n_samples);
	std::vector<OpenMesh::Vec2d> offsetCage_samples_adapt(const std::vector<OpenMesh::Vec2d>& curve_cage3, double displacement, std::vector<int> n_samples);
	std::vector<OpenMesh::Vec3d> rationalPtsfrom5points(const std::vector<OpenMesh::Vec2d>& points5);
	void createSimpleMesh(void);
	bool GetBoundaryVertices();
	VectorXd testmain(Eigen::MatrixXd &f, Eigen::MatrixXd &d);
	VectorXd testmain2(Eigen::MatrixXd& f, Eigen::MatrixXd& d);
	VectorXd testmain2AAAP(Eigen::MatrixXd& f, Eigen::MatrixXd& d);
	std::vector<OpenMesh::Vec3d> GetSamplePointsWithOffset();
	bool GetCBoundaryVertices();
	void ConstructMatrix3DOffset();
	void ConstructMatrix4();
	void ConstructMatrix4OffSetTwice();
	void ConstructMatrix4OffSetThird();
	void ConstructMatrix4OffSetTwiceVirtual();
	void ConstructMatrix4OffSetTwiceVirtualNew();
	void ConstructMatrix4OffSetTwiceVirtualNewWithDer();
	void ConstructMatrix4OffSetTwiceVirtual2();
	void ConstructMatrix4OffSetThirdVirtual();
	void ConstructMatrix4OffSetThirdVirtualNew();
	void ConstructMatrix4OffSetFourthVirtualNew();
	void ConstructMatrix4OffSet();
	void BH2ConstructMatrix4OffSet();//2到2的构造矩阵
	void BH2ConstructMatrix4OffSetVirtual();//2到2的构造矩阵,增加虚拟点
	void BH2ConstructMatrix4OffSetVirtual_edge();//2到2的构造矩阵,增加虚拟点，遍历边构造矩阵
	void BH223ConstructMatrix4OffSetVirtual_edge();//2到3的构造矩阵,增加虚拟点，遍历边构造矩阵
	void BH3ConstructMatrix4OffSet();//3到3的构造矩阵
	void BH3ConstructMatrix4OffSetVirtual();//3到3的构造矩阵,增加虚拟点
	void Cal222BiharmonicBasis(OpenMesh::Vec2d& v0, OpenMesh::Vec2d& v01, OpenMesh::Vec2d& v1, OpenMesh::Vec2d& v12, OpenMesh::Vec2d& v2,
		OpenMesh::Vec2d& eta, std::vector<double>& values);
	void Cal222BiharmonicBasis_edge( OpenMesh::Vec2d& v1, OpenMesh::Vec2d& v12, OpenMesh::Vec2d& v2,
		OpenMesh::Vec2d& eta, std::vector<double>& values);
	void Cal222BiharmonicBasis_edge_test(OpenMesh::Vec2d& v1, OpenMesh::Vec2d& v12, OpenMesh::Vec2d& v2,
		OpenMesh::Vec2d& eta, std::vector<double>& values);
	void Cal323BiharmonicBasis(OpenMesh::Vec2d& v0, OpenMesh::Vec2d& v01, OpenMesh::Vec2d& v10, OpenMesh::Vec2d& v1, OpenMesh::Vec2d& v12, OpenMesh::Vec2d& v21, OpenMesh::Vec2d& v2,
		OpenMesh::Vec2d& eta, std::vector<double>& values);
	void Cal323BiharmonicBasis_edge(OpenMesh::Vec2d& v1, OpenMesh::Vec2d& v12, OpenMesh::Vec2d& v21, OpenMesh::Vec2d& v2,
		OpenMesh::Vec2d& eta, std::vector<double>& values);
	void Cal323BiharmonicBasis_edge_limit(OpenMesh::Vec2d& v1, OpenMesh::Vec2d& v12, OpenMesh::Vec2d& v21, OpenMesh::Vec2d& v2,
		OpenMesh::Vec2d& eta, double t0, std::vector<double>& values);
	void Cal323BiharmonicBasisDebug1(OpenMesh::Vec2d& v0, OpenMesh::Vec2d& v01, OpenMesh::Vec2d& v10, OpenMesh::Vec2d& v1, OpenMesh::Vec2d& v12, OpenMesh::Vec2d& v21, OpenMesh::Vec2d& v2,
		OpenMesh::Vec2d& eta, std::vector<double>& values);
	void ConstructMatrix4OffSetTest();
	void NewConstructMatrix4OffSet();
	void ConstructMatrix4OffSetTestGauss();//大失败
	void ConstructMatrix4OffSetTestVirtual();
	void ConstructMatrix4OffSetTestVirtual2();
	void ConstructMatrix4OffSetTestVirtualGauss();
	void ConstructMatrix4OffSetVirtual();
	void ConstructMatrix4OffSetVirtual2();
	void ConstructMatrix4OffSetVirtual3();
	bool NearestCageVertex(OpenMesh::Vec3d objCor, OpenMesh::VertexHandle& minVh);
	bool NearestCageVertex(OpenMesh::Vec3d objCor, Mesh CC_meshX, OpenMesh::VertexHandle& minVh);
	template <typename VecType>
	void Bezier1Poly1(std::vector<std::vector<VecType>> curvecage2, std::vector<std::vector<VecType>>& curvecage2poly);
	template <typename VecType>
	void Bezier2Poly2(std::vector<std::vector<VecType>> curvecage2, std::vector<std::vector<VecType>>& curvecage2poly);
	void Bezier2Poly2(myArray<3> curvecage2, myArray<3>& curvecage2poly);
	template <typename VecType>
	void Bezier2Poly3(std::vector<std::vector<VecType>> curvecage2, std::vector<std::vector<VecType>>& curvecage2poly);
	template <typename VecType>
	void Bezier2Bezier7(std::vector<std::vector<VecType>> curvecage2, std::vector<std::vector<VecType>>& curvecage7);
	template <typename VecType>
	void Bezier2Bezier227(std::vector<std::vector<VecType>> curvecage2, std::vector<std::vector<VecType>>& curvecage3);
	template <typename VecType>
	void Bezier2Bezier223(std::vector<std::vector<VecType>> curvecage2, std::vector<std::vector<VecType>>& curvecage3);
	template <typename VecType>
	void Bezier2Bezier221(std::vector<std::vector<VecType>> curvecage2, std::vector<std::vector<VecType>>& curvecage7);
	template <typename VecType>
	void Bezier2Bezier321(std::vector<std::vector<VecType>> curvecage2, std::vector<std::vector<VecType>>& curvecage7);
	template <typename VecType>
	void Bezier2Bezier322(std::vector<std::vector<VecType>> curvecage2, std::vector<std::vector<VecType>>& curvecage7);
	template <typename VecType>
	void Bezier2Poly7(std::vector<std::vector<VecType>> curvecage7, std::vector<std::vector<VecType>>& curvecage72poly);
	void calculate_specialgreen_weight123(const std::vector<int>& sp_id);
	void calculate_green_weight123(void);
	void calculate_green_weight121(void);
	void calculate_green_weight222(void);
	void calculate_greenD2_weight222(void);
	void calculate_cauchy_weight121(void);
	void calculate_cauchy_weight121(std::vector<std::vector<Mesh::Point>> curvecage, std::vector<std::vector<complex<double>>>& weights);
	void calculate_cauchy_weight222(void);
	void calculate_cauchy_weight222_rational(void);
	void calculate_cauchy_weight123(void);
	void calculate_cauchy_weight123(std::vector<std::vector<Mesh::Point>> curvecage, std::vector<std::vector<complex<double>>>& weights);
	void calculate_cauchy_weight323(void);
	void calculate_cauchy_weight121(std::vector<complex<double>>cps);
	void calculate_cauchy_weight123(std::vector<complex<double>>cps);
	void calculate_cauchy_weight222(std::vector<complex<double>>cps);
	void calculate_cauchy_weight323(std::vector<complex<double>>cps);
	void calculate_D2cauchy_weight222(void);
	void calculate_D2cauchy_weight_CB121(void);
	void calculate_D2cauchy_weight_CB123(void);
	void calculate_D2cauchy_weight_CB222(void);
	void calculate_D2cauchy_weight_CB323(void);
	void calculate_green_weight221(void);
	void calculate_green_weight223(void);
	void calculate_green_weight227(void);
	void calculate_green_weight321(void);
	void calculate_green_weight322(void);
	void calculate_green_weight323(void);
	void calculate_green_weight327(void);
	void calculate_bih_weight121(void);
	void calculate_bih_weight121test(void);
	void calculate_bih_weight122(void);//计算1到2次的双调和坐标
	void calculate_bih_weight122New(void);//计算1到2次的双调和坐标,切向使用边界的另一种表示
	void calculate_bih_weight222New(void);//计算2到2次的双调和坐标,切向使用边界的另一种表示
	void calculate_bih_weight323New(void);//计算3到3次的双调和坐标,切向使用边界的另一种表示
	void calculate_bih_weight123(void);//计算1到3次的双调和坐标
	void calculate_bih_weight123New(void);//计算1到3次的双调和坐标
	void calculate_bih_weight124New(void);//计算1到3次的双调和坐标
	void calculate_bih_weight1213D(void);
	void ccPoints2BezierSegs(void);
	void deform_mesh_from_cc(Mesh& deformedmesh);
	void deform_mesh_from_cc(Mesh& deformedmesh, std::vector<Mesh::Point > CC_pointsX, std::vector<std::vector<complex<double>>>& weights);
	void deform_cage_from_handles(void);
	void Set_Texture_coord();

private:
	void DrawPoints(void);
	void DrawWireframe(void);
	void DrawHiddenLines(void);
	void DrawFlatLines(void);
	void DrawFlat(void);
	void DrawSmooth(void);
	void DrawSmoothDistort(void);
	void DrawBezierCurve(const BezierCurve& bezierCurve, float r = 1.0f, float g = 0.0f, float b = 0.0f, float linewidth = 1.0, float pointSize = 5.0, int subdiv = 50);
	void DrawBezierCurves();
	void DrawBezierCurves(std::vector<std::vector<Mesh::Point>> curvecage);
	void DrawBezierControlPolylines();
	void DrawBezierControlPolylines(std::vector<std::vector<Mesh::Point>> curvecage);
	void DrawTexture();
	void DrawCageWireframe(void);
	void DrawLinearCage(void);
	void DrawCagePoints(void);
	void DrawHandlePoints(void);
	void DrawCurveCage(void);
	void DrawBoundingBox(void) const;
	void DrawBoundary(void) const;
	/*void InitHighResFBO();
	void RenderToFBO();
	void RenderToFBO(const std::vector<BezierCurve>& curves, int maxCurveIndex, int currentCurveIndex, double t_end);
	void SaveImage(const QString& filename);
	void ExportHighResImage(const QString& filename);
	void ExportHighResImage(const QString& filename, const std::vector<BezierCurve>& curves, int maxIndex, int curIndex, double t_end);*/
protected:
	Mesh mesh;
	Mesh mesh2;//绘制video时的第二个mesh
	Mesh mesh3;//绘制video时的第三个mesh
	Mesh mesh_beifen;
	Mesh CC_mesh;//控制点组成的网格，由CC_points初始化，在move过程中顶点位置发生改变
	Mesh CC_mesh22;
	Mesh CC_mesh33;
	Mesh Handle_mesh;
	bool draw_mesh2=true;//绘制第二个网格
	int degree = 3;
	int dividecages =2;//2
	int todegree = 7;
	bool highdegree = false;
	bool usecvm = false;
	bool useGBC = false;
	bool usebih = false;
	bool useCauchy = false;
	bool useHandle = false;
	bool drawpoints = false;
	bool drawDistort = false;
	bool drawDistortDirichlet = false;
	bool use3D = false;
	bool move_mesh2_flag = false;
	std::vector<std::vector<Mesh::Point>> curvecage2;//若干段bezier曲线的控制点，曲线按逆时针顺序排序
	std::vector<std::vector<Mesh::Point>> curvecage22;
	std::vector<std::vector<Mesh::Point>> curvecage33;
	std::vector<std::vector<Mesh::Point>> curvecage2_real;//若干段bezier曲线的控制点，曲线按逆时针顺序排序
	std::vector<Mesh::Point > CC_points;//初始化：将curvecage2中的控制点逆时针排序进一个数组内，改变：costume在move时
	std::vector<Mesh::Point > CC_points22;
	std::vector<Mesh::Point > CC_points33;
	std::vector < OpenMesh::Vec2d > CC_points2d;
	std::vector<Mesh::Point > Handle_points;
	std::vector<Mesh::Point> mvcGn;
	std::vector<Mesh::Point> mvcGt;
	std::vector<OpenMesh::Vec2d> linear_cage;
	std::vector<OpenMesh::Vec2d> LapLspoints;
	std::vector<double> linear_length;
	std::vector<double> sj;
	double p2pweight = 0.1;
	Eigen::MatrixXd A, B, C, D,NewCL,Chalf;//Chalf表示将每条边的每第一和最后一个点分别表示出来，并且每个边上有四个系数，sj1234,他们代表了
	Eigen::MatrixXd AAAB, AAAC;//AAAP变形里的矩阵
	bool use_new=false;
	bool use_aaap = false;
	bool use_rational = false;
	std::vector<double> mvcL;
	std::vector<std::vector<double>> weights;
	std::vector<std::vector<double>> BIH_green_weights;
	std::vector<std::vector<double>> BIH_else_weights;
	std::vector<std::vector<double>> fake_weights;
	std::vector<std::vector<double>> D2weight;
	std::vector<std::vector<complex<double>>> cauchyweights;
	std::vector<std::vector<complex<double>>> cauchyweights_2;
	std::vector<std::vector<complex<double>>> cauchyweights_3;
	std::vector<std::vector<complex<double>>> standad_cauchyweights;
	std::vector<std::vector<complex<double>>> D2cauchyweights;
	std::vector<std::vector<complex<double>>> cp_cauchyweights;
	std::vector<std::complex<double>> deform_cps_t;
	std::vector<std::complex<double>> deform_cps;
	std::vector<Mesh::Point> boundary_vertices;
	std::vector<Mesh::Point> Cboundary_vertices;
	QString strMeshFileName;
	QString strMeshBaseName;
	QString strMeshPath;
	Mesh::Point ptMin;
	Mesh::Point ptMax;
	double avgEdgeLength;
	SelectMode selectMode;
	bool isMovable;
	double moveDepth;
	OpenMesh::Vec3d lastObjCor;
	bool isEnableLighting;
	bool isTwoSideLighting;
	bool isDrawBoundingBox;
	bool isDrawBoundary;

	float bezierLineWidth = 0.055f;//haixing 0.02
	float bezierPolyLineWidth = 2.2f;//控制网厚度haixing 2.1
	float bezierPolyPointWidth = 10.0f;//控制网控制点厚度 haixing 20
	float bezierPointSize = 1.0f;//改成权重系数
	int NormalControl = 1;
	float greenWeightBih = 1.f;
	float bezierR = 1.;//188./255;//80./255;
	float bezierG = 0.8;//128./255;//150./255;
	float bezierB = 0.;// 1.f;//1.f;
	float pointsR = 0.9;//163./255;//134./255;
	float pointsG = 0.6;// 160. / 255;//183./255;
	float pointsB = 0.;//1.f;//1.f;
	int bezierSubdiv = 50;


	struct MoveInfo
	{
		std::vector<OpenMesh::VertexHandle> vertices;
		// in rotation mode, this represent to the rotation center
		OpenMesh::Vec3d move;
		bool isRotation = false;
		double angle;
		double scale;
	};
	bool isRotationMode = false;
	OpenMesh::Vec3d rotationCenter;
	OpenMesh::Vec3d currentMoveStart;
	int currentHistoryPos = 0; // next move history index in list
	std::vector<MoveInfo> moveHistories;

	//保存高质量图片
	GLuint fbo_ = 0;
	GLuint colorTex_ = 0;
	GLuint depthRb_ = 0;
	int exportWidth_ = 3200;   // 导出尺寸
	int exportHeight_ = 2400;
	double movemesh2 = -6;
	double movemesh3 = -12;


public slots:
	void BiharmonicCurve_Test(void); 
	void straight_Bih_Test(void);
	void CurveCage_Test(void);
	void PolyGC_Test(void);
	void CalculateCMVCWeight_Test(void);
	void CoonsWeight_Test(void);
	

protected:
	bool mode_drawCurveCage = false;
};
