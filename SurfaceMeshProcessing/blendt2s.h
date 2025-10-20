#pragma once
#include <OpenMesh/Core/Geometry/VectorT.hh>
#include <vector>
#include <cmath>
#include <algorithm>

struct BezierCircleApproxParams {
    OpenMesh::Vec2d center = OpenMesh::Vec2d(0.0, 0.0); // 圆心
    double r_in = 6.0;                                  // 内半径
    double r_out = 12.0;                                 // 外半径
    double xmin = -60.0, xmax = 0.0;                    // 源矩形 x 范围
    double ymin = 0.0, ymax = 8.0;                     // 源矩形 y 范围
    double theta0 = 0.0;                                 // 起始角
    double theta_final = 2.0 * M_PI;                     // 最终角开口（整圈=2π）
    bool   ease_thickness = true;                        // 厚度缓动（只作用在半径）
};

// 厚度缓动：角度不开缓动，保证 t=0.25/0.5/0.75/1.0 精准对应 1/4/1/2/3/4/1 圆
inline double smoothstep01(double t) {
    t = std::clamp(t, 0.0, 1.0);
    return 3.0 * t * t - 2.0 * t * t * t;
}

// u3-u0 的“环形差”，保证在 [0,1] 范围内（用于跨 0 处的段）
inline double wrap01_diff(double u1, double u0) {
    double d = u1 - u0;
    if (d < 0.0) d += 1.0;
    return d;
}

// 构造：在“逐步扩展的圆弧”上，重建每段三次 Bézier 的四个控制点（端点+手柄）
// 使每条半径上的投影都是用标准 k=4/3*tan(φ/4) 的圆弧逼近。t=0 返回 source。
inline std::vector<OpenMesh::Vec3d>
rebuild_bezier_controls_on_growing_arc(
    const std::vector<OpenMesh::Vec3d>& CC_points_source,     // 形如 [0..3],[3..6],... 的控制点
    const BezierCircleApproxParams& P,
    double t,                                                 // 0..1：中心线角开口比例
    const std::vector<OpenMesh::Vec3d>* CC_points_target_opt = nullptr, // 可选：t=1 贴合
    bool snap_target_at_1 = true
) {
    const double tt = std::clamp(t, 0.0, 1.0);

    // t=0：保持直线，不改控制点（你的中心线要求）
    if (tt == 0.0) {
        return CC_points_source;
    }

    const double theta_span_t = P.theta_final * tt;                 // 本帧角开口
    const double k_thick = P.ease_thickness ? smoothstep01(tt) : tt;// 厚度缓动
    const double rm = 0.5 * (P.r_in + P.r_out);
    const double h = 0.5 * (P.r_out - P.r_in);

    auto u_of_x = [&](double x)->double {
        return (P.xmax != P.xmin) ? std::clamp((x - P.xmin) / (P.xmax - P.xmin), 0.0, 1.0) : 0.0;
    };
    auto v_of_y = [&](double y)->double {
        return (P.ymax != P.ymin) ? std::clamp((y - P.ymin) / (P.ymax - P.ymin), 0.0, 1.0) : 0.0;
    };
    auto radius_of_v = [&](double v)->double {
        return rm + ((v - 0.5) * 2.0) * h * k_thick; // 映到 [r_in, r_out]，带厚度缓动
    };
    auto base_on = [&](double theta, double r)->OpenMesh::Vec2d {
        return P.center + OpenMesh::Vec2d(r * std::cos(theta), r * std::sin(theta));
    };
    auto tangent = [&](double theta)->OpenMesh::Vec2d {
        return OpenMesh::Vec2d(-std::sin(theta), std::cos(theta));  // 圆的单位切向
    };

    const size_t N = CC_points_source.size();
    if (N < 4) return CC_points_source; // 不足一段

    std::vector<OpenMesh::Vec3d> out(N);

    // 逐段重建：段 [i,i+1,i+2,i+3]，i=0,3,6,...；最后一段 iwrap = (N-1)-2? 由你的数据结构决定
    for (size_t i = 0; i + 3 <= N; i += 3) {
        size_t i0 = i;
        size_t i1 = i + 1;
        size_t i2 = i + 2;
        size_t i3 = (i + 3 < N) ? (i + 3) : 0; // 闭合：最后一段用 [N-3, N-2, N-1, 0]

        // 源点
        const auto& P0s = CC_points_source[i0], & P1s = CC_points_source[i1];
        const auto& P2s = CC_points_source[i2], & P3s = CC_points_source[i3];

        // 参数 (u,v)
        const double u0 = u_of_x(P0s[0]), u3 = u_of_x(P3s[0]);
        const double v0 = v_of_y(P0s[1]), v1 = v_of_y(P1s[1]);
        const double v2 = v_of_y(P2s[1]), v3 = v_of_y(P3s[1]);

        // 角度（处理 wrap）
        const double th0 = P.theta0 + u0 * theta_span_t;
        const double th1 = P.theta0 + (u0 + wrap01_diff(u3, u0)) * theta_span_t;

        // 段角 φ & k
        const double phi = th1 - th0; // ∈ (0, 2π]，tt>0 保证 >0
        const double k = (std::abs(phi) < 1e-12) ? 0.0 : (4.0 / 3.0) * std::tan(0.25 * phi);

        // 各控制点所在半径
        const double r0 = radius_of_v(v0);
        const double r1 = radius_of_v(v1);
        const double r2 = radius_of_v(v2);
        const double r3 = radius_of_v(v3);

        // 端点
        OpenMesh::Vec2d A0 = base_on(th0, r0);
        OpenMesh::Vec2d A1 = base_on(th1, r3);

        // 切向方向（端点角）
        OpenMesh::Vec2d t0v = tangent(th0);
        OpenMesh::Vec2d t1v = tangent(th1);

        // 手柄：各自半径上、沿端点切向偏移 k*r
        OpenMesh::Vec2d H1 = base_on(th0, r1) + k * r1 * t0v;
        OpenMesh::Vec2d H2 = base_on(th1, r2) - k * r2 * t1v;

        // 写回（z 沿用源 z）
        out[i0] = OpenMesh::Vec3d(A0[0], A0[1], P0s[2]);
        out[i1] = OpenMesh::Vec3d(H1[0], H1[1], P1s[2]);
        out[i2] = OpenMesh::Vec3d(H2[0], H2[1], P2s[2]);
        out[i3] = OpenMesh::Vec3d(A1[0], A1[1], P3s[2]);
    }

    // 可选：t=1 时用你给的 target 覆盖（数值完全一致）
    if (CC_points_target_opt && snap_target_at_1 && std::abs(tt - 1.0) < 1e-12) {
        const auto& T = *CC_points_target_opt;
        if (T.size() == out.size()) out = T;
    }

    return out;
}
