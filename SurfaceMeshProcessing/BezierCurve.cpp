#include "BezierCurve.h"

BezierCurve::BezierCurve(const vector<OpenMesh::Vec3d>& p_CtrlPoints, bool rational) {
	m_CtrlPoints.resize(p_CtrlPoints.size());
	for (size_t i = 0; i < p_CtrlPoints.size(); ++i) {
		m_CtrlPoints[i] = OpenMesh::Vec3d{ p_CtrlPoints[i][0],p_CtrlPoints[i][1], p_CtrlPoints[i][2] };
	}
	m_Degree = p_CtrlPoints.size() - 1;
	m_Weights.resize(p_CtrlPoints.size());
	m_Rational = rational;
	for (size_t i = 0; i < p_CtrlPoints.size(); ++i) {
		if (rational) {
			m_Weights[i] = p_CtrlPoints[i][2]; // 第三个分量，即 Z 坐标
		}
		else {
			m_Weights[i] = 1; // 非有理，权重都为1
		}
	}
}

OpenMesh::Vec3d BezierCurve::Evaluate(const double& para) const {
	double t = para;
	assert(t >= 0.0 && t <= 1.0);
	//de Casteljau 算法
	vector<OpenMesh::Vec3d> temp_points = m_CtrlPoints;
	vector<double> temp_weights = m_Weights;
	for (uint i = 1; i <= m_Degree; ++i) {
		for (uint j = 0; j <= m_Degree - i; ++j) {

			double temp_w = temp_weights[j] * (1.0 - t) + temp_weights[j + 1] * t;
			temp_points[j] = temp_points[j] * (1.0 - t) * temp_weights[j] / temp_w + temp_points[j + 1] * t * temp_weights[j + 1] / temp_w;
			temp_weights[j] = temp_weights[j] * (1.0 - t) + temp_weights[j + 1] * t;

		}
	}
	return temp_points[0];
}

//使用一次de Casteljau算法，计算得到细分点的值和两个子线段的控制点,(非)有理
void BezierCurve::deCasteljau(BezierCurve& r_Part1, BezierCurve& r_Part2, OpenMesh::Vec3d& r_Value, double t)const {
	assert(t >= 0.0 && t <= 1.0);
	r_Part1 = *this;
	r_Part2 = *this;
	vector<OpenMesh::Vec3d> temp_points = m_CtrlPoints;
	vector<double> temp_weights = m_Weights;
	r_Part2.m_CtrlPoints[0] = temp_points.back();
	r_Part2.m_Weights[0] = temp_weights.back();

	for (unsigned int i = 1; i <= m_Degree; ++i) {
		for (unsigned int j = 0; j <= m_Degree - i; ++j) {
			double temp_w = temp_weights[j] * (1.0 - t) + temp_weights[j + 1] * t;
			temp_points[j] = temp_points[j] * (1.0 - t) * temp_weights[j] / temp_w + temp_points[j + 1] * t * temp_weights[j + 1] / temp_w;
			temp_weights[j] = temp_weights[j] * (1.0 - t) + temp_weights[j + 1] * t;
		}
		//再投影到2D，除掉齐次坐标
		r_Part1.m_CtrlPoints[i] = (temp_points[0]);
		r_Part2.m_CtrlPoints[i] = temp_points[m_Degree - i];
		r_Part1.m_Weights[i] = temp_weights[0];
		r_Part2.m_Weights[i] = temp_weights[m_Degree - i];
	}

	//在这里求子曲线时，第二段子曲线的控制点序列与原曲线是相反的
	//为了程序的可读性，将反序列转换为按照参数不减方向，保持第二段子曲线与原曲线序列相同
	std::reverse(r_Part2.m_CtrlPoints.begin(), r_Part2.m_CtrlPoints.end());
	std::reverse(r_Part2.m_Weights.begin(), r_Part2.m_Weights.end());

	r_Value = temp_points[0];
}

double BezierCurve::calculateControlPolygonLength() const {
	double length = 0.0;
	OpenMesh::Vec3d current_euclidean_point;

	// Get the first Euclidean point
	if (m_Weights[0] == 0.0) return 0.0; // Cannot start if first weight is zero
	current_euclidean_point = m_CtrlPoints[0] / m_Weights[0];

	// Iterate through subsequent points, convert to Euclidean, and add segment length
	for (size_t i = 1; i < m_CtrlPoints.size(); ++i) {
		if (m_Weights[i] == 0.0) return 0.0; // Cannot calculate segment if weight is zero

		OpenMesh::Vec3d next_euclidean_point = m_CtrlPoints[i] / m_Weights[i];

		// Calculate the distance between the current and next Euclidean points
		length += (next_euclidean_point - current_euclidean_point).norm(); // Use .norm() or .length()

		// Move to the next point
		current_euclidean_point = next_euclidean_point;
	}

	return length;
}

double BezierCurve::approximateLength() const {
	if (m_Degree < 1) {
		return 0.0;
	}
	// If degree is 1 (linear), control polygon is the line segment itself.
	// We can directly calculate its length. Avoids subdivision overhead.
	if (m_Degree == 1) {
		return calculateControlPolygonLength();
	}


	// Perform the first level of subdivision at t=0.5
	BezierCurve curve1, curve2;
	OpenMesh::Vec3d point_at_0_5_weighted; // We don't need the point value itself for length approx

	// This call populates curve1 and curve2 with their respective weighted control points
	deCasteljau(curve1, curve2, point_at_0_5_weighted, 0.5);

	// Perform the second level of subdivision at t=0.5 for each resulting curve
	BezierCurve curve1_1, curve1_2;
	BezierCurve curve2_1, curve2_2;
	OpenMesh::Vec3d dummy_value_weighted; // Still don't need the point values

	curve1.deCasteljau(curve1_1, curve1_2, dummy_value_weighted, 0.5);
	curve2.deCasteljau(curve2_1, curve2_2, dummy_value_weighted, 0.5);

	// Calculate the control polygon length for each of the four final segments
	double length1_1 = curve1_1.calculateControlPolygonLength();
	double length1_2 = curve1_2.calculateControlPolygonLength();
	double length2_1 = curve2_1.calculateControlPolygonLength();
	double length2_2 = curve2_2.calculateControlPolygonLength();

	// The total approximate length is the sum of these four control polygon lengths
	return length1_1 + length1_2 + length2_1 + length2_2;
}
