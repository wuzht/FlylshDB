### 概述  
> 主要有三个方法，readFileFromSource为从原始文件中读入数据，并做分页处理；oneKNN为寻找一个随机向量的K个最近邻，结果在窗口输出，主要用于测试；thousandKNN用于寻找多个随机向量的K个最近邻，最终结果输入到文件中。  

### readFileFromSource(const char* fileName)    

 - 参数：
 fileName:用于读取的源文件文件名。  

 - 过程：
 1、使用findDimension函数计算出向量的维度及目标每页最多能存储的向量数。
 2、利用fileIO方法读取源文件并存储为目标文件。  

### oneKNN(int k)和thousandKNN（int k, const char* fileName）  

 > oneKNN和thousandKNN类似，thousandKNN实际上只是多次使用oneKNN，所以，这里只是介绍oneKNN。  


 - 参数：
 k：寻找K个最近邻
 fileName：用于存储寻找结果的文件名  
 - 过程：  
 1、生成一个随机的向量ID，并通过findPage函数，找到该向量所在的页及在页中的位置。  
 2、用myRead函数获取该随机向量。  
 3、设置时钟开始计时。
 4、读取所有页中的向量，计算所有其他向量至该随机向量的欧式距离，并将该距离及其向量ID存入List（自动排序并至保存前200个数据）中。
 5、计时结束，打印所画时间及200个最近邻的ID。  

### 查询部分伪代码  

```
#in oneKNN
randomId = xrand()
find out the number of page which store the random vector
get the random_vector
clock_t start = clock()
for page in allFiles:
	Vector[] vector = read(page) #get all vector in page
	for v in vector:
		if v is not random_vector:
			dist = Distance(v, random_vector)
			List.push(v.id, dist)
	release the memory of vector
clock_t finish = clock()
cost_time = finish - start;
print cost_time
print List

#in List.push
def List.push(v.id, dist):
	if list is empty or the size of list is less than K:
		insert the {vid:dist} into list by order
	elif dist < list.tail.dist:
		delete the list.tail
		insert the {vid:dist} into list by order
	else
		do nothing
		
```

 