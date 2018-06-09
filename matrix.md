# create_Gaussian_matrix

生成高斯投影矩阵
n: 矩阵的行数（实验1中为32）
dimension: 矩阵的列数



# create_fly_matrix

生成果蝇矩阵，返回的矩阵每行记录的是1的位置
n: 矩阵的行数（实验中要求是10*d）
dimension: 矩阵的列数（实验中要求是0.1*d）



# product

计算两矩阵的点乘，通过传入type的值来确定是果蝇矩阵还是高斯矩阵，因为算法不同。
matrix1 投影矩阵
matrix2 输入矩阵
n 投影矩阵的行 
num 该页有多少个向量 ，即输入矩阵的行 
type： 0 高斯矩阵  1 果蝇矩阵 
生成的矩阵为num * n 



# typedef myVector* myMatrix;

把myVector*定义为myMatrix
矩阵的读写调用myFileIO.h里的myRead和myWrite就行（id不用）