#pragma once
#include <iostream>
#include <vector>
#include <Eigen/Core>
#include <iostream>
#include <LBFGS.h>
#include <LBFGSB.h>

using Eigen::VectorXd;
using Eigen::MatrixXd;
using namespace LBFGSpp;

class Rosenbrock
{
private:
    int n;
    MatrixXd B;
    MatrixXd C; 
    MatrixXd f; 
    MatrixXd d;
public:
    Rosenbrock(int n_) : n(n_) {}
    Rosenbrock(int n_, const MatrixXd& B_init, const MatrixXd& C_init, const MatrixXd& f_init, const MatrixXd& d_init)
        : n(n_), B(B_init), C(C_init), f(f_init), d(d_init) {}
    double operator()(const VectorXd& x, VectorXd& grad)
    {
        /*double fx = 0.0;
        for (int i = 0; i < n; i += 2)
        {
            double t1 = 1.0 - x[i];
            double t2 = 10 * (x[i + 1] - x[i] * x[i]);
            grad[i + 1] = 20 * t2;
            grad[i] = -2.0 * (x[i] * grad[i + 1] + t1);
            fx += t1 * t1 + t2 * t2;
        }
        return fx;*/
        ///////////////my
        MatrixXd X = MatrixXd::Zero(n, n);
        for (int i = 0; i < n; i++)
            X(i, i) = x[i];
        //梯度
        MatrixXd term = B * f + C * X * d;
        MatrixXd gradX = 2 * (C.transpose() * term) * d.transpose();  // 简单的梯度计算
        for (int i = 0; i < n; i++)
        {
            grad[i] = gradX(i, i);
        }

        return term.squaredNorm();
    }
    
};

class Rosenbrock2
{
private:
    int n;
    MatrixXd B;
    MatrixXd C;
    MatrixXd f;
    MatrixXd d;
public:
    Rosenbrock2(int n_) : n(n_) {}
    Rosenbrock2(int n_, const MatrixXd& B_init, const MatrixXd& C_init, const MatrixXd& f_init, const MatrixXd& d_init)
        : n(n_), B(B_init), C(C_init), f(f_init), d(d_init) {}
    //2次的情况，x的大小是n/2
    double operator()(const VectorXd& x, VectorXd& grad)
    {
        /*double fx = 0.0;
        for (int i = 0; i < n; i += 2)
        {
            double t1 = 1.0 - x[i];
            double t2 = 10 * (x[i + 1] - x[i] * x[i]);
            grad[i + 1] = 20 * t2;
            grad[i] = -2.0 * (x[i] * grad[i + 1] + t1);
            fx += t1 * t1 + t2 * t2;
        }
        return fx;*/
        ///////////////my
        MatrixXd X = MatrixXd::Zero(n, n);
        int m = x.size();
        assert(n == 2 * m);
        for (int i = 0; i < m; i++)
        {
            X(i, i) = x[i];
            X(i + m, i + m) = x[i];
        }

        //梯度
        MatrixXd term = B * f + C * X * d;
        MatrixXd gradX = 2 * (C.transpose() * term) * d.transpose();  // 简单的梯度计算
        for (int i = 0; i < m; i++)
        {
            grad[i] = gradX(i, i) + gradX(i + m, i + m);
        }

        return term.squaredNorm();
    }
};

class Rosenbrock2AAAP
{
private:
    int n;
    MatrixXd B;
    MatrixXd C;
    MatrixXd f;
    MatrixXd d;
public:
    Rosenbrock2AAAP(int n_) : n(n_) {}
    Rosenbrock2AAAP(int n_, const MatrixXd& B_init, const MatrixXd& C_init, const MatrixXd& f_init, const MatrixXd& d_init)
        : n(n_), B(B_init), C(C_init), f(f_init), d(d_init) {}
    //2次的情况，x的大小是n/2
    double operator()(const VectorXd& x, VectorXd& grad)
    {
        ///////////////my
        MatrixXd X = MatrixXd::Zero(2*n, 2*n);
        int m = x.size();
        assert(n == 2 * m);
        for (int i = 0; i < m; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                X(i + j * m, i + j * m) = x[i];
            }
        }

        //梯度
        MatrixXd term = B * f + C * X * d;
        /*std::cout << "B" << B << std::endl;
        std::cout << "f" <<f << std::endl;
        std::cout << "C" << C << std::endl;
        std::cout << "X" << X << std::endl;
        std::cout << "d" << d << std::endl;
        std::cout <<"term"<< term << std::endl;*/
        //std::cout << "term" << term << std::endl;
        MatrixXd gradX = 2 * (C.transpose() * term) * d.transpose();  // 简单的梯度计算
        for (int i = 0; i < m; i++)
        {
            grad[i] = 0;
            for (int j = 0; j < 4; j++)
            {
                grad[i] += gradX(i+j*m, i+j*m) ;
                
            }
        }

        return term.squaredNorm();
    }
};
