#pragma once
#include <vector>
#include <cmath>
#include <map>
#include <iostream>
#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>
#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/Utils/PropertyManager.hh>
#include <Eigen/Dense>
#include <Eigen/Sparse>
using namespace std;
class BezierCurve
{
protected:
	int m_Degree;
	vector<OpenMesh::Vec3d> m_CtrlPoints;
	bool m_Rational;
	vector<double> m_Weights;

public:
	BezierCurve() :m_Degree(0), m_CtrlPoints(vector<OpenMesh::Vec3d>(1)), m_Weights(vector<double>{0.0}), m_Rational(false) {}
	BezierCurve(const vector<OpenMesh::Vec3d>& p_CtrlPoints, bool rational);


public:
	const vector<OpenMesh::Vec3d>& GetCtrlPoints() const { return m_CtrlPoints; }
	vector<OpenMesh::Vec3d>& GetCtrlPoints() { return m_CtrlPoints; }

	const vector<double>& GetWeights() const { return m_Weights; }
	vector<double>& GetWeights() { return m_Weights; }

	const int& GetDegrees() const { return m_Degree; }

public:
	OpenMesh::Vec3d Evaluate(const double& para) const;
	void deCasteljau(BezierCurve& r_Part1, BezierCurve& r_Part2, OpenMesh::Vec3d& r_Value, double t)const;
	double BezierCurve::calculateControlPolygonLength() const;
	double BezierCurve::approximateLength() const;
};



