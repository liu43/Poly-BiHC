# Poly-BiHC
论文<<2D Polynomial Biharmonic Coordinates for High-order Cages>>的代码和数据以及比较实验。框架来自于傅孝明老师，编译见傅孝明老师主页。编译成功后得到如下界面：

其中mesh存储了变形的网格，images存储了网格的纹理图片。

双调合坐标项目主体代码见viewerwidget.cpp，其中straight_Bih_Test函数为直边cage变形为曲边cage的函数，BiharmonicCurve_Test函数为曲边cage变形为曲边cage的函数。

其他函数为对比试验的代码：如CurveCage_Test计算了polynomial cauchy坐标和green坐标，CalculateCMVCWeight_Test计算了Cubic MVC坐标，CoonsWeight_Test计算了[Qin 2024]的坐标
。
