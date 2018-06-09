# List.h

List类封装了一个用于**储存KNN的特殊双向链表**（自动排序并至保存前K个数据）。

在客户代码中查询KNN时，将每个查询到的向量的id和distance都通过`push`方法插入到List中，但List仅会储存最近邻的K个向量，因此最后得到的即为KNN。

ListNode(List的节点)是一个结构体，主要包含向量的id和distance，以及下一个节点的指针。

其中List的最大容量为K，因为最后要返回的是K个最近邻向量的id。

List中的每个节点始终按照向量的distance从小到大排列。

List中仅储存最近邻的K个向量，主要算法在`push`方法里。

主要方法如下：

* 构造方法：`List(int temp_maxNum);`

  传入的参数为List的容量，即KNN中的K，即多少个最近邻。

* `bool push(int temp_id, float temp_distance);`

  * `temp_id`该向量的id
  * `temp_distance`该向量的欧氏距离

  在客户代码中，每查询到一个向量，都调用`push`方法插入到List中。

  该方法算法伪代码如下：

  ```c++
  // 当前节点为要插入的节点，K为KNN中的K，即多少个最近邻
  
  // 在尾部插入
  if (List为空 || temp_distance >= List尾节点的distance) {
      if (List已满) {
          return true;
      }
      将当前节点插入到List尾部;
      若List节点数大于K，删除尾部节点;
      return true;
  }
  // 在头部插入
  if (temp_distance <= List头节点的distance) {
      将当前节点插入到List头部;
      若List节点数大于K，删除尾部节点;
      return true;
  }
  // 遍历List，在符合条件的位置插入当前节点
  ListNode* current = head;
  ListNode* following = current->next;
  while (1) {
      if (current的distance <= 当前节点的distance && 
          当前节点的distance < following的distance) {
          在current和following之间插入当前节点;
          若List节点数大于K，删除尾部节点;
          return true;
      }
      current = current->next;
      following = current->next;
  }
  ```

* `bool print_in_file(ofstream& out) const;`

  将List的内容（KNN）输出到文件中。