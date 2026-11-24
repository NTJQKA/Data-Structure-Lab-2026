#include <iostream>
#include <cstring>
#include <array>
#include <string>
#include "text_statistics.h"
using namespace std;

#define NUMBER 300


/*
哈夫曼树与编码模块说明

需要输入：
来自A模块 std::array<std::uint64_t,256> frequency
frequency[i]：字节i对应的出现频次

对外输出：
char_code[256][100] 全局数组
char_code[ch]：代表字节ch对应的哈夫曼编码字符串（0、1组成）

调用顺序：
1.HuffTree<char>* tree=HuffmanBuild<char>(stats.frequency);  构建哈夫曼树
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
	HuffNode(T v = 0, int w = 0) :val(v), weight(w), left(nullptr), right(nullptr) {}
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
HuffTree<T>* HuffmanBuild(const std::array<std::uint64_t, 256>& freq) {
	MinHeap<T>* forest = new MinHeap<T>();
	//遍历0~255，只把频次>0的字节生成叶子节点
	for (int i = 0; i < 256; i++)
	{
		if (freq[i] > 0)
		{
			T ch = static_cast<T>(i);
			int weight = static_cast<int>(freq[i]);
			HuffNode<T>* leaf = new HuffNode<T>(ch, weight);
			HuffTree<T>* tree = new HuffTree<T>(leaf);
			forest->insert(tree);
		}
	}

	//循环合并最小两棵树，逻辑完全不变
	while (forest->getSize() > 1) {
		HuffTree<T>* t1 = forest->removefirst();
		HuffTree<T>* t2 = forest->removefirst();
		HuffNode<T>* newNode = new HuffNode<T>(0, t1->getRoot()->weight + t2->getRoot()->weight);
		newNode->left = t1->getRoot();
		newNode->right = t2->getRoot();
		HuffTree<T>* tempTree = new HuffTree<T>(newNode);
		forest->insert(tempTree);
	}
	HuffTree<T>* resTree = forest->removefirst();
	delete forest;
	return resTree;
}

//哈夫曼编码结果全局数组
char char_code[256][100];

template<typename T>
void HuffmanCode(HuffNode<T>* ht1, char* Code, int length) {
	if (ht1 == nullptr)return;
	//递归出口，叶子节点
	if (ht1->left == nullptr && ht1->right == nullptr) {
		Code[length] = '\0';
		//强转为unsigned char，防止char负数造成数组下标越界
		strcpy_s(char_code[static_cast<unsigned char>(ht1->val)], 100, Code);
		return;
	}
	//左分支0
	Code[length] = '0';
	HuffmanCode(ht1->left, Code, length + 1);
	//右分支1
	Code[length] = '1';
	HuffmanCode(ht1->right, Code, length + 1);
}

//释放哈夫曼树内存
template<typename T>
void deleteHuffTree(HuffNode<T>* node) {
	if (node == nullptr)return;
	deleteHuffTree(node->left);
	deleteHuffTree(node->right);
	delete node;
}


//测试入口
int main() {
	//A模块读取文件，获取统计信息
	huffman::TextStats stats;
	string errMsg;
	bool readOk = huffman::read_and_count("test.txt", stats, errMsg);
	if (!readOk)
	{
		cout << errMsg << endl;
		return -1;
	}

	//B模块：直接使用A的frequency建树
	HuffTree<char>* huffTree = HuffmanBuild<char>(stats.frequency);

	char tempBuf[100] = { 0 };
	HuffmanCode<char>(huffTree->getRoot(), tempBuf, 0);

	//打印字符、频度、编码
	cout << "字符\t频度\t哈夫曼编码" << endl;
	for (int i = 0; i < 256; i++)
	{
		if (stats.frequency[i] > 0)
		{
			unsigned char ch = static_cast<unsigned char>(i);
			cout << ch << "\t" << stats.frequency[i] << "\t" << char_code[ch] << endl;
		}
	}

	//释放内存
	deleteHuffTree(huffTree->getRoot());
	delete huffTree;
	return 0;
}
