#include <iostream>
#include <cstring >
using namespace std;

#define NUMBER 300


/*
哈夫曼树与编码模块说明

需要输入：
s[]：存放所有不重复字符数组
w[]：对应字符的出现频率数组
num：不重复字符的总数量

对外输出：
char_code[256][100] 全局数组	
char_code[ch]：代表字符ch对应的哈夫曼编码字符串（0、1组成）

调用顺序：
1.HuffTree<char>* tree=HuffmanBuild<char>(s,w,num);  构建哈夫曼树
2.char temp[100]={0};
  HuffmanCode<char>(tree->getRoot(),temp,0);    生成编码，写入char_code
3.任务完成后调用deleteHuffTree(tree->getRoot());
*/


//哈夫曼节点
template<typename T>
struct HuffNode
{
	T val;
	int weight;
	HuffNode<T>* left;
	HuffNode<T>* right;
	//构造函数
	HuffNode(T v=0,int w=0):val(v),weight(w),left(nullptr),right(nullptr){}
};

//哈夫曼树
template<typename T>
class HuffTree {
public:
	HuffNode<T>* root;
	//构造树
	HuffTree() { root = nullptr; }
	HuffTree(HuffNode<T>* r) { root = r; }
	//返回根节点
	HuffNode<T>* getRoot() {
		return root;
	}
};

//最小堆
template<typename T>
class MinHeap {
private:
	HuffTree<T>* heap[NUMBER];
	int size;
public:
	MinHeap() {
		size = 0;
	}
	//插入堆元素
	void insert(HuffTree<T>* t) {
		heap[++size] = t;
		int cur = size;
		//向上调整
		while (cur > 1) {
			int parent = cur / 2;
			if (heap[parent]->getRoot()->weight > heap[cur]->getRoot()->weight) {
				HuffTree<T>* tmp = heap[parent];
				heap[parent] = heap[cur];
				heap[cur] = tmp;
				cur = parent;
			}
			else {
				break;
			}
		}
	}
	//删除堆顶
	HuffTree<T>* removefirst() {
		if (size == 0)return nullptr;
		HuffTree<T>* res = heap[1];
		heap[1] = heap[size--];
		//向下调整
		int cur = 1;
		while (true) {
			int left = cur * 2;
			int right = cur * 2 + 1;
			int minPos = cur;
			if (left <= size && heap[left]->getRoot()->weight < heap[minPos]->getRoot()->weight) {
				minPos = left;
			}
			if (right <= size && heap[right]->getRoot()->weight < heap[minPos]->getRoot()->weight) {
				minPos = right;
			}
			if (minPos == cur)break;
			HuffTree<T>* tmp = heap[cur];
			heap[cur] = heap[minPos];
			heap[minPos] = tmp;
			cur = minPos;
		}
		return res;
	}
	int getSize() {
		return size;
	}
};


//构建哈夫曼树
template<typename T>
HuffTree<T>* HuffmanBuild(char* s, int* w, int num) {
	MinHeap<T>* forest = new MinHeap<T>();
    //初始化num棵单节点树，全部插入最小堆
	for (int i = 0; i < num; i++) {
		HuffNode<T>* leaf = new HuffNode<T>(s[i], w[i]);
		HuffTree<T>* tree = new HuffTree<T>(leaf);
		forest->insert(tree);
	}

	//循环合并
	while (forest->getSize() > 1) {
		//取出最小，次小两棵树
		HuffTree<T>* t1 = forest->removefirst();
		HuffTree<T>* t2 = forest->removefirst();
		//新建父节点，权值相加
		HuffNode<T>* newNode = new HuffNode<T>(0, t1->getRoot()->weight + t2->getRoot()->weight);
		newNode->left = t1->getRoot();
		newNode->right = t2->getRoot();
		HuffTree<T>* tempTree = new HuffTree<T>(newNode);
		forest->insert(tempTree);
	}
	//堆剩下最后一棵哈夫曼树
	return forest->removefirst();
 }

//哈夫曼编码
char char_code[256][100];

template<typename T>
void HuffmanCode(HuffNode<T>* ht1, char* Code, int length) {
	if (ht1 == nullptr)return;
    //递归出口，叶子节点
	if (ht1->left == nullptr && ht1->right == nullptr) {
		Code[length] = '\0';
		strcpy_s(char_code[ht1->val],100, Code);
		return;
	}
	//左分支为0
	Code[length] = '0';
	HuffmanCode(ht1->left, Code, length + 1);
	//右分支为1
	Code[length] = '1';
	HuffmanCode(ht1->right, Code, length + 1);
}

template<typename T>
void deleteHuffTree(HuffNode<T>* node) {
	if (node == nullptr)return;
	deleteHuffTree(node->left);
	deleteHuffTree(node->right);
	delete node;
}



//测试
int main() {
	char ch[] = { 'a','b','c','d','e' };
	int weight[] = { 2,3,5,7,8 };
	int num = 5;

	HuffTree<char>* huffTree = HuffmanBuild<char>(ch, weight, num);

	char tempBuf[100] = { 0 };
	HuffmanCode<char>(huffTree->getRoot(), tempBuf, 0);
	cout << "字符\t哈夫曼编码" << endl;
	for (int i = 0; i < num; i++) {
		cout << ch[i] << "\t" << char_code[ch[i]] << endl;
	}
	deleteHuffTree(huffTree->getRoot());
	delete huffTree;
	return 0;
}
