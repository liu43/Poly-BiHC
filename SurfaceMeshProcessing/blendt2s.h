#pragma once
#include <OpenMesh/Core/Geometry/VectorT.hh>
#include <vector>
#include <cmath>
#include <algorithm>

struct BezierCircleApproxParams {
    OpenMesh::Vec2d center = OpenMesh::Vec2d(0.0, 0.0); // Բ��
    double r_in = 6.0;                                  // �ڰ뾶
    double r_out = 12.0;                                 // ��뾶
    double xmin = -60.0, xmax = 0.0;                    // Դ���� x ��Χ
    double ymin = 0.0, ymax = 8.0;                     // Դ���� y ��Χ
    double theta0 = 0.0;                                 // ��ʼ��
    double theta_final = 2.0 * M_PI;                     // ���սǿ��ڣ���Ȧ=2�У�
    bool   ease_thickness = true;                        // ��Ȼ�����ֻ�����ڰ뾶��
};

// ��Ȼ������ǶȲ�����������֤ t=0.25/0.5/0.75/1.0 ��׼��Ӧ 1/4/1/2/3/4/1 Բ
inline double smoothstep01(double t) {
    t = std::clamp(t, 0.0, 1.0);
    return 3.0 * t * t - 2.0 * t * t * t;
}

// u3-u0 �ġ����β����֤�� [0,1] ��Χ�ڣ����ڿ� 0 ���ĶΣ�
inline double wrap01_diff(double u1, double u0) {
    double d = u1 - u0;
    if (d < 0.0) d += 1.0;
    return d;
}

// ���죺�ڡ�����չ��Բ�����ϣ��ؽ�ÿ������ B��zier ���ĸ����Ƶ㣨�˵�+�ֱ���
// ʹÿ���뾶�ϵ�ͶӰ�����ñ�׼ k=4/3*tan(��/4) ��Բ���ƽ���t=0 ���� source��
inline std::vector<OpenMesh::Vec3d>
rebuild_bezier_controls_on_growing_arc(
    const std::vector<OpenMesh::Vec3d>& CC_points_source,     // ���� [0..3],[3..6],... �Ŀ��Ƶ�
    const BezierCircleApproxParams& P,
    double t,                                                 // 0..1�������߽ǿ��ڱ���
    const std::vector<OpenMesh::Vec3d>* CC_points_target_opt = nullptr, // ��ѡ��t=1 ����
    bool snap_target_at_1 = true
) {
    const double tt = std::clamp(t, 0.0, 1.0);

    // t=0������ֱ�ߣ����Ŀ��Ƶ㣨���������Ҫ��
    if (tt == 0.0) {
        return CC_points_source;
    }

    const double theta_span_t = P.theta_final * tt;                 // ��֡�ǿ���
    const double k_thick = P.ease_thickness ? smoothstep01(tt) : tt;// ��Ȼ���
    const double rm = 0.5 * (P.r_in + P.r_out);
    const double h = 0.5 * (P.r_out - P.r_in);

    auto u_of_x = [&](double x)->double {
        return (P.xmax != P.xmin) ? std::clamp((x - P.xmin) / (P.xmax - P.xmin), 0.0, 1.0) : 0.0;
    };
    auto v_of_y = [&](double y)->double {
        return (P.ymax != P.ymin) ? std::clamp((y - P.ymin) / (P.ymax - P.ymin), 0.0, 1.0) : 0.0;
    };
    auto radius_of_v = [&](double v)->double {
        return rm + ((v - 0.5) * 2.0) * h * k_thick; // ӳ�� [r_in, r_out]������Ȼ���
    };
    auto base_on = [&](double theta, double r)->OpenMesh::Vec2d {
        return P.center + OpenMesh::Vec2d(r * std::cos(theta), r * std::sin(theta));
    };
    auto tangent = [&](double theta)->OpenMesh::Vec2d {
        return OpenMesh::Vec2d(-std::sin(theta), std::cos(theta));  // Բ�ĵ�λ����
    };

    const size_t N = CC_points_source.size();
    if (N < 4) return CC_points_source; // ����һ��

    std::vector<OpenMesh::Vec3d> out(N);

    // ����ؽ����� [i,i+1,i+2,i+3]��i=0,3,6,...�����һ�� iwrap = (N-1)-2? ��������ݽṹ����
    for (size_t i = 0; i + 3 <= N; i += 3) {
        size_t i0 = i;
        size_t i1 = i + 1;
        size_t i2 = i + 2;
        size_t i3 = (i + 3 < N) ? (i + 3) : 0; // �պϣ����һ���� [N-3, N-2, N-1, 0]

        // Դ��
        const auto& P0s = CC_points_source[i0], & P1s = CC_points_source[i1];
        const auto& P2s = CC_points_source[i2], & P3s = CC_points_source[i3];

        // ���� (u,v)
        const double u0 = u_of_x(P0s[0]), u3 = u_of_x(P3s[0]);
        const double v0 = v_of_y(P0s[1]), v1 = v_of_y(P1s[1]);
        const double v2 = v_of_y(P2s[1]), v3 = v_of_y(P3s[1]);

        // �Ƕȣ����� wrap��
        const double th0 = P.theta0 + u0 * theta_span_t;
        const double th1 = P.theta0 + (u0 + wrap01_diff(u3, u0)) * theta_span_t;

        // �ν� �� & k
        const double phi = th1 - th0; // �� (0, 2��]��tt>0 ��֤ >0
        const double k = (std::abs(phi) < 1e-12) ? 0.0 : (4.0 / 3.0) * std::tan(0.25 * phi);

        // �����Ƶ����ڰ뾶
        const double r0 = radius_of_v(v0);
        const double r1 = radius_of_v(v1);
        const double r2 = radius_of_v(v2);
        const double r3 = radius_of_v(v3);

        // �˵�
        OpenMesh::Vec2d A0 = base_on(th0, r0);
        OpenMesh::Vec2d A1 = base_on(th1, r3);

        // �����򣨶˵�ǣ�
        OpenMesh::Vec2d t0v = tangent(th0);
        OpenMesh::Vec2d t1v = tangent(th1);

        // �ֱ������԰뾶�ϡ��ض˵�����ƫ�� k*r
        OpenMesh::Vec2d H1 = base_on(th0, r1) + k * r1 * t0v;
        OpenMesh::Vec2d H2 = base_on(th1, r2) - k * r2 * t1v;

        // д�أ�z ����Դ z��
        out[i0] = OpenMesh::Vec3d(A0[0], A0[1], P0s[2]);
        out[i1] = OpenMesh::Vec3d(H1[0], H1[1], P1s[2]);
        out[i2] = OpenMesh::Vec3d(H2[0], H2[1], P2s[2]);
        out[i3] = OpenMesh::Vec3d(A1[0], A1[1], P3s[2]);
    }

    // ��ѡ��t=1 ʱ������� target ���ǣ���ֵ��ȫһ�£�
    if (CC_points_target_opt && snap_target_at_1 && std::abs(tt - 1.0) < 1e-12) {
        const auto& T = *CC_points_target_opt;
        if (T.size() == out.size()) out = T;
    }

    return out;
}
