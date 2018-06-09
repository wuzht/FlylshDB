(实验报告请放除了"代码修改"的部分)

# hash.cpp算法描述

在hash.cpp中，主要任务是生成以下数据

1.高斯投影矩阵和果蝇投影矩阵。

2.使用高斯投影矩阵哈希后的数据。

3.使用果蝇投影矩阵哈希后的数据。

4.基于果蝇投影矩阵哈希后，应用了random机制的数据。

5.基于果蝇投影矩阵哈希后，应用了WTA机制的数据。

6.基于果蝇投影矩阵哈希后，应用了binary机制的数据。

* 函数`void saveMatrix(const myMatrix matrix, int dim, const int type)`

  分页保存生成的高斯投影矩阵或果蝇投影矩阵到对应的文件夹中

  注：其中果蝇矩阵为01稀疏矩阵，仅保存每行值为1的数据下标

* 函数`myMatrix readMatrix(int dim, const int type)`

  将高斯投影矩阵或果蝇投影矩阵从对应的文件夹中读取出来，并将其返回

* 函数`void generateMatrixAndSaveHashedVectorsAfterProduct(int type)`

  生成投影矩阵，并保存哈希后的数据，步骤如下：

  1. 生成高斯/果蝇矩阵，将其保存
  2. 读取高斯/果蝇矩阵
  3. 然后对原始数据进行处理（做内积）
  4. 分页保存使用高斯/果蝇投影矩阵哈希后的数据到对应的文件夹中

* 函数`void processWTAandBinary(int k)`

  对果蝇投影矩阵哈希后的数据应用random机制，并将其保存

  1. 读取果蝇投影矩阵哈希后数据
  2. 对于每个向量，随机选取相同的k个下标，仅保留这些下标对应的数据，向量维度变为k
  3. 将应用了random机制的数据分页保存到对应的文件夹中

* 函数`void processWTAandBinary(int k)`

  对果蝇投影矩阵哈希后的数据分别应用WTA和Binary机制的数据，并将其分别保存

  1. 读取果蝇投影矩阵哈希后数据
  2. 对于每个向量，保留哈希后的向量中最大的k个分量，其余分量置0，算法如下：
     * 创建了类WTAList，是一个长度为k的双向链表，实现方法与List.h同理。
     * 每读到一个数据都将其push到WTAList中，而WTAList自动排序并保存最大的k个数据。该算法时间复杂度为O(k * m)，其中m是果蝇投影矩阵哈希后向量的维度(m = 10d)。
     * 最后访问WTAList的数据，可得到它们相应的下标和值
  3. 将应用了WTA和Binary机制的数据分页保存到对应的文件夹中

  注：WTA和Binary的向量都是稀疏的。对于应用Binary后的向量，仅保存每行值为1的数据下标（k个）；对于应用WTA后的向量，仅保存每行值为1的数据下标（k个），以及对应的值。

#hash.cpp文件结构

原始文件的文件夹为files，数据集文件夹为mnist。

运行前要先新建以下文件夹，然后逐个运行main函数里的4个函数即可得到所有数据，对其进行读操作请参考以下数据结构：

* GaussMatrix

  存放高斯投影矩阵。行数k，列数d

* FlyMatrix

  存放果蝇投影矩阵。行数10d，列数0.1d

  稀疏矩阵，每行存放的是1的下标

* GaussHashedVectors

  存放使用高斯投影矩阵哈希后的向量。行数n，列数（向量维度）k

* FlyHashedVectors

  存放使用果蝇投影矩阵哈希后的向量。行数n，列数（向量维度）10d

* RamdomVectors

  存放基于果蝇投影矩阵哈希后，应用了random机制的向量。行数n，列数（向量维度）k 

* WTAVectors

  存放基于果蝇投影矩阵哈希后，应用了WTA机制的向量。行数n，列数（向量维度）**2 * k**

  稀疏矩阵，每行为一个向量。每个向量中，前k个数字（下标0:k-1）是应用WTA后剩余的数据的下标，后k个数字（下标k:2k-1）是前k个数字对应的数据的值。（求KNN时可设计算法）

* BinaryVectors

  存放基于果蝇投影矩阵哈希后，应用了binary机制的向量。行数n，列数（向量维度）k

  稀疏矩阵，每行为一个向量。每个向量中，前k个数字是应用WTA后剩余的数据的下标，其对应的值都为1，其余的都是0

# 修改k

若要修改k，修改宏#define k_GAUSS_OUTPUT_VEC_DIM 32

# 代码修改

### WTAList.h

仅用于WTA，目的是获取前k个最大值，可以不用管它

### myFileIO.h

* 因内存泄漏严重，在myWrite函数和myRead函数后增加以下代码

  ```c++
  if (slots != nullptr) {
      delete []slots;
      slots = nullptr;
  }
  ```

* myRead函数增加了一个可选参数`string fileName = "files/file"`，可改变文件路径，默认为files/file

### myVector.h

增加了printVec()函数，用于打印向量检查是否有错。