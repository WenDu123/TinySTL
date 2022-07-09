#pragma once
#ifndef _RB_TREE_H_
#define _RB_TREE_H_


#include"Allocator.h"

#include"Uninitialized.h"



#include "functional.h"

namespace MyTinySTL {
	//常量
	typedef bool __rb_tree_color_type;
	const __rb_tree_color_type __rb_tree_black = true;  //黑为1
	const __rb_tree_color_type __rb_tree_red = false;  //红为0
	//节点
	//base_node
	struct __rb_tree_node_base {
		typedef __rb_tree_color_type color_type;
		typedef __rb_tree_node_base* base_ptr;

		color_type color;
		base_ptr parent;
		base_ptr left;
		base_ptr right;

		static base_ptr minimum(base_ptr x) {
			while (x->left != 0) {
				x = x->left;
			}
			return x;
		}

		static base_ptr maximum(base_ptr x) {
			while (x->right != 0) {
				x = x->right;
			}
			return x;
		}

	};

	__rb_tree_node_base* decrement_for_node(__rb_tree_node_base* y) {  //寻找前驱
		__rb_tree_node_base* node = y;
		if (node->color == __rb_tree_red && node->parent->parent == node) {
			//表示node为header,即node = end()
			//此时返回node为mostright
			node = node->right;
		}
		else if (node->left != 0) {
			__rb_tree_node_base* x = node;
			x = node->left;
			while (x->right != 0) {
				x = x->right;
			}
			node = x;
		}
		else {
			__rb_tree_node_base* x = node->parent;
			while (node == x->left) {
				node = x;
				x = x->parent;
			}
			node = x;
		}
		return node;
	}

	//node
	template<class Value>
	struct __rb_tree_node : public __rb_tree_node_base {
		typedef __rb_tree_node<Value>* link_type;
		Value value_field;
	};


	//迭代器

	//base_iterator
	struct __rb_tree_iterator_base {
		typedef __rb_tree_node_base* base_ptr;
		typedef bidirectional_iterator_tag iterator_category;
		typedef ptrdiff_t difference_type;

		base_ptr node;

		/*
		一个结点的后继，是大于x.key的最小关键字的结点，找右节点的左子树最小值
		一个结点的前驱，是小于x.key的最大关键字的结点，找左节点的右子树最大值

		前驱：给定任意节点，如果该节点是整颗树的最小值（绝对不存在左孩子的），它的前驱是null；
		否则如果该节点存在左孩子，它的前驱是以这个左孩子为根下的左边整颗子树中最大值的那个节点；
		如果不存在左孩子且不是最小值，它的前驱是根节点。

	    后继：给定任意节点，如果该节点是整颗树的最大值（绝对不存在右孩子的），它的后继是null；
		否则如果该节点存在右孩子，它的后继是以这个右孩子为根下的右边整颗子树中最小值的那个节点；
		如果不存在右孩子且不是最大值，它的后继是根节点
	
		*/

		//寻找该节点的后继,找找右节点的左子树最小值
		//后继：大于x.key的最小关键字的结点
		void increment() {
			if (node->right != 0) {   //状况1，存在右节点
				//后继是以这个右节点为根的，整颗子树中最小值的那个节点（在左子树最左）
				node = node->right;  //直接从右节点开始找
				while (node->left != 0)  //一直往左子树走，直到左子树为空，即找到最小值
					node = node->left;
			}
			else {   //状况2，不存在右节点,往上走
				base_ptr x = node->parent;
				/*
				x：父亲
				node：孩子
				情况2：node == x->right，node为x的右儿子，且node无右儿子，代表node>x，继续玩往上走
				   如果node != x->right，node为x的左儿子，且node无右儿子，代表node<x,此时x为node后继
				*/
				while (node == x->right) {  //情况2
					node = x;
					x = node->parent;
				}

				if (node->right != x) {  //情况3
					node = x;
					/*
					正常情况：node的右儿子不可能等于node的父节点，返回父节点即可
					特殊情况：
						1.要找最右节点的后继，应该返回root
						此时node = ，x = root
						但是根节点有右儿子且不可能等于header，调换node和x，返回node = root
					
					*/
				}
				//情况4
				/*
				特殊情况：node = root，且root无右节点
				node->right =nullptr = header = root->parent
				表示：只有根节点和左子节点，此时应该返回根节点
				即直接返回root
				*/

			}

		}
		//寻找该节点的前驱
		//前驱，是小于x.key的最大关键字的结点，找左节点的右子树最大值
		void decrement() {
			if (node->color == __rb_tree_red && node->parent->parent == node) {
				//表示node为header,即node = end()
				//此时返回node为mostright
				node = node->right;
			}
			else if (node->left != 0) {
				base_ptr x = node;
				x = node->left;
				while (x->right != 0) {
					x = x->right;
				}
				node = x;
			}
			else {
				base_ptr x = node->parent;
				while (node == x->left) {
					node = x;
					x = x->parent; 
				}
				node = x;
			}


		}

	};


	//iterator
	template<class Value,class Ref,class Ptr>
	struct __rb_tree_iterator : public __rb_tree_iterator_base {
		typedef Value value_type;
		typedef Ref reference;
		typedef Ptr pointer;
		typedef __rb_tree_iterator<Value, Value&, Value*>  iterator;
		typedef __rb_tree_iterator<Value,const Value&,const Value*>  const_iterator;
		typedef __rb_tree_iterator<Value, Ref, Ptr>  self;
		typedef __rb_tree_node<Value>* link_type;

		__rb_tree_iterator(){}
		__rb_tree_iterator(link_type x) { node = x; }
		__rb_tree_iterator(const iterator& iter) { node = iter.node; }

		reference operator* () { return link_type(node)->value_field; }
		pointer operator->() { return &(operator*()); }

		self& operator++() {
			increment();
			return *this;
		}

		self operator++(int) {
			self tmp = *this;
			increment();
			return tmp;
		}

		self& operator--() {
			decrement();
			return *this;
		}

		self operator--(int) {
			self tmp = *this;
			decrement();
			return tmp;
		}


	};

	inline bool operator==(const __rb_tree_iterator_base& x, const __rb_tree_iterator_base& y) {
		return x.node == y.node;
	}
	inline bool operator!=(const __rb_tree_iterator_base& x, const __rb_tree_iterator_base& y) {
		return !(x==y);
	}



	//class

	/* 首先我们应该注意的是模板的参数
	 * key代表红黑树上存储节点时依据的值
	 * value即节点的值
	 * 是仿函数,用于通过value获取到key
	 * Compare为比较key大小的标准
	 * Alloc是空间配置器
	 */
	template<class Key,class Value,class KeyOfValue,class Compare,class Alloc = alloc>
	class rb_tree {
	protected:
		typedef void* void_pointer;
		typedef __rb_tree_node_base* base_ptr;
		typedef __rb_tree_node<Value> rb_tree_node;
		typedef allocator<rb_tree_node, Alloc> rb_tree_allocator;
		typedef __rb_tree_color_type color_type;
		
	public:
		typedef Value value_type;
		typedef Key key_type;
		typedef value_type& reference;
		typedef const value_type& const_reference;
		typedef value_type* pointer;
		typedef const value_type* const_pointer;
		typedef rb_tree_node* link_type;
		typedef size_t size_type;
		typedef ptrdiff_t difference_type;

	protected: //节点的内存管理
		link_type get_node() {  //为节点申请一份内存空间
			return rb_tree_allocator::allocate(1);
		}
		void put_node(link_type p) {  //释放节点的内存空间
			rb_tree_allocator::deallocate(p,1);
		}
		link_type create_node(const value_type& val) {  //申请内存，并构造一个节点
			link_type tmp = get_node();
			construct(&tmp->value_field, val);
			return tmp;
		}
		link_type clone_node(link_type x) {  //克隆（复制）一个节点（值和颜色）
			link_type tmp = create_node(x->value_field);
			tmp->color = x->color;
			tmp->left = 0;
			tmp->right = 0;
			return tmp;
		}
		void destroy_node(link_type p) {  //析构这个节点，并释放内存
			destory(&(p->value_field));
			put_node(p);
		}

	protected://rb_tree数据结构操作
		//rb_tree 只用这三个数据表现
		size_type node_count; //记录树的大小（节点数量）
		link_type header; //头节点
		Compare key_compare; //仿函数对象，节点间键值比较大小准则

		//使用函数取得节点header成员
		link_type& root() const { return (link_type&) header->parent; }
		link_type& leftmost()const { return (link_type&) header->left; }
		link_type& rightmost()const { return (link_type&) header->right; }

		//使用函数取得节点x成员
		static link_type& left(link_type x) { return (link_type&)(x->left); }
		static link_type& right(link_type x) { return (link_type&)(x->right); }
		static link_type& parent(link_type x) { return (link_type&)(x->parent); }
		static reference value(link_type x) { return x->value_field; }
		static color_type& color(link_type x) { return (color_type&)(x->color); }
		static const key_type& key(link_type x) {
			return KeyOfValue()(value(x));
		}

		//使用函数取得节点x成员
		static link_type& left (base_ptr x) { return (link_type&)(x->left); }
		static link_type& right(base_ptr x) { return (link_type&)(x->right); }
		static link_type& parent(base_ptr x) { return (link_type&)(x->parent); }
		static reference value(base_ptr x) { return ((link_type)x)->value_field; }
		static color_type& color(base_ptr x) { return (color_type&)(link_type(x)->color); }
		static const key_type& key(base_ptr x) {
			return KeyOfValue()(value(link_type(x)));
		}

		//求最大值最小值
		static link_type minimum(link_type x) {
			return (link_type)__rb_tree_node_base::minimum(base_ptr(x));
		}

		static link_type maximum(link_type x) {
			return (link_type)__rb_tree_node_base::maximum(base_ptr(x));
		}

	public:
		typedef __rb_tree_iterator<value_type, reference, pointer> iterator;
		typedef __rb_tree_iterator<value_type, const reference, const pointer> const_iterator;

	private:
		iterator __insert(base_ptr _x, base_ptr _y, const value_type& val) {
			link_type x = (link_type)_x; //插入点
			link_type y = (link_type)_y; //插入点父节点
			link_type z; //插入点创建

			if (y == header || x != 0 || key_compare(KeyOfValue()(val), key(y))) {
				//树为空或者插入节点为父节点的左子节点进入if
				z = create_node(val);
				left(y) = z;
				//如果y == header，leftmost()=z,提前设置
				//如果y ！= header，z为y的左子节点
				if (y == header) {
					root() = z;
					rightmost() = z;
				}
				else if(y == leftmost()) {
					leftmost() = z;
				}
			}
			else {  //插入的新节点位于y的右子节点
				z = create_node(val);
				right(y) = z;
				if (y == rightmost()) {
					rightmost() = z;
				}
			}
			//设定新节点的父节点，左右节点
			parent(z) = y;
			left(z) = 0;
			right(z) = 0;

			__rb_tree_rebalance(z, header->parent);//插入新节点后调整红黑树使之平衡
			++node_count;

			return iterator(z);

		};
		link_type  rb_tree_copy(link_type x, link_type p) {
			link_type top = clone_node(x);
			top->parent = p;

			if (x->right)
				top->right = rb_tree_copy(right(x), top);
			p = top;
			x = left(x);
			while (x != 0) {
				link_type y = clone_node(x);
				p->left = y;
				y->parent = p;
				if (x->right)
					y->right = rb_tree_copy(right(x), y);
				p = y;
				x = left(x);
			}
			return top;
		}
		

		void init() {
			header = get_node();
			color(header) = __rb_tree_red;
			root() = 0;
			leftmost() = header;
			rightmost() = header;
		}
	public:
		rb_tree(const Compare& comp = Compare()) :node_count(0), key_compare(comp) {
			init();
		}
		rb_tree(const rb_tree& t) :node_count(0), key_compare(t.key_compare) {
			if(t.root()==0)
				init();
			else {
				header = get_node();
				color(header) = __rb_tree_red;
				root() = rb_tree_copy(t.root(),header);
				leftmost() = minimum(root());
				rightmost() = maximum(root());
			}
			node_count = t.node_count;
		}
		~rb_tree() {
			clear();
			put_node(header);
		}



	public:
		Compare key_comp() const { return key_compare; }
		iterator begin() const { return leftmost();}
		iterator end() const { return header; }
		bool empty() const { return node_count == 0; }
		size_type size() const { return node_count; }
		size_type max_size() const { return size_type(-1); }


		rb_tree& operator=(const rb_tree& x) {
			if (this != &x) { //防止自我赋值
				clear();
				node_count = 0;
				key_compare = x.key_compare;
				if (x.root() == 0) {
					root() = 0;
					leftmost() = header;
					rightmost() = header;
				}
				else {
					root() = rb_tree_copy(x.root(), header);
					leftmost() = minimum(root());
					rightmost() = maximum(root());
					node_count = x.node_count;
				}
			}
			return *this;
		}





		__rb_tree_node_base* __rb_tree_delete_node(__rb_tree_node_base* x, __rb_tree_node_base*& root);
		void erase(iterator x) {
			link_type y = (link_type)__rb_tree_delete_node(x.node, header->parent);
			//destory(y);
			destroy_node(y);
			--node_count;
		}
		void clear_erase(link_type x) {
			while (x != 0) {
				clear_erase(right(x));
				link_type y = left(x);
				destory(&y->value_field);
				x = y;
			}
		}
		void clear() {
			if (node_count != 0) {
				clear_erase(root()); //清除所有节点
				// 初始化Header节点
				leftmost() = header;
				root() = 0;
				rightmost() = header;
				node_count = 0;
			}
		}

		iterator find(const Key& k) {
			link_type y = header;
			link_type x = root();

			while (x != 0) {
				if (!key_compare(key(x), k)) {
					y = x;
					x = left(x);
				}
				else {
					//y = x;??
					x = right(x);
				}
			}

			iterator j = iterator(y);
			return (j == end() || key_compare(k, key(j.node)) ? end() : j);
		}


	public:
		iterator insert_equal(const value_type& val) { //节点键值允许重复
			link_type x = header;
			link_type y = root();

			while (y != 0) {
				x = y;
				y = key_compare(KeyOfValue()(val), key(y)) ? left(y) : right(y);
				//v<y,返回bool = 1，往左边走，反之往右边走
			}
			return __insert(y, x, val);
			//参数(新插入点，新插入点父节点，值)

		}


		template<class InputIterator>
		void insert_equal(InputIterator first, InputIterator last) {
			for (; first != last; ++first) {
				insert_equal(*first);
			}
		}


		pair<iterator, bool> insert_unique(const value_type& val) {
			link_type x = header;
			link_type y = root();
			bool comp = true;

			while (y != 0) {
				x = y;//x往下走
				comp = key_compare(KeyOfValue()(val), key(y));
				//comp = 1，val < y,往左，反之往右
				y = comp ? left(y) : right(y);
			}//找到位置

			iterator j = iterator(x);//x为插入节点父节点
			if (comp) { //往左
				if (j == begin()) {  //插入位置位于最左，字直接插入
					return pair<iterator,bool>(__insert(y, x, val),true); 
					//true表示不重复，插入成功
				}
				else {
					--j;//j = j的前驱，小于j最大的值，即为y
				}
			}
			//若往左，j=--j，此时j为x的前驱，即将插入的y为x的左子节点，代表y插入的是x前驱的位置，x>y>--j,否则y的父节点应该是--j才对
			//若往右，j = x，y的父节点，而且y是x的右子节点，所以 y > x

			if (key_compare(key(j.node), KeyOfValue()(val))) {
				//此时无论是往左还是往右，都进入if，如果相等则不会进入！
				return pair<iterator,bool>(__insert(y, x, val),true);

			}
			//代表新插入值和已有值重复，不插入
			return pair<iterator, bool>(j, false);
		}
		
		template<class InputIterator>
		void insert_unique(InputIterator first, InputIterator last) {
			for (; first != last; ++first) {
				insert_unique(*first);
			}
		}

		
		void insert_unique(const Value* first, const Value* last) {
			for (; first != last; ++first) {
				insert_unique(*first);
			}
		}


	};

	template <class Key,class Value, class KeyOfValue, class Compare, class Alloc>
	bool operator==(const rb_tree<Key,Value,KeyOfValue,Compare,Alloc>& x, const rb_tree<Key, Value, KeyOfValue, Compare, Alloc>& y) {

		return x.size() == y.size() && equal(x.begin(), x.end(), y.begin());

	}
	template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
	bool operator!=(const rb_tree<Key, Value, KeyOfValue, Compare, Alloc>& x, const rb_tree<Key, Value, KeyOfValue, Compare, Alloc>& y) {

		return !(x == y);

	}

	template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
	bool operator<(const rb_tree<Key, Value, KeyOfValue, Compare, Alloc>& x, const rb_tree<Key, Value, KeyOfValue, Compare, Alloc>& y) {

		return lexicographocal_compare(x.begin(), x.end(), y.begin(), y.end());

	}

	//左旋
	inline void __rb_tree_rotate_left(__rb_tree_node_base* x, __rb_tree_node_base*& root) {
		//x为原父节点，y为x的右节点
		//要注意，要设置节点A子节点等于节点B，还要把节点B的父亲设为节点A，双向设置
		__rb_tree_node_base* y = x->right;
		x->right = y->left;
		if (y->left != 0) {
			y->left->parent = x;
		}

		y->parent = x->parent;

		//y顶替x，全盘接受x的关系
		if (x == root) {
			x->parent->parent = y; //header -> parent
			root = y;
		}
		else if (x == x->parent->left) {
			x->parent->left = y;
		}
		else if (x == x->parent->right) {
			x->parent->right = y;
		}

		y->left = x;
		x->parent = y;

	}

	inline void __rb_tree_rotate_right(__rb_tree_node_base* x, __rb_tree_node_base*& root) {
		//x为原父节点，y为x的左节点
		//要注意，要设置节点A子节点等于节点B，还要把节点B的父亲设为节点A，双向设置
		__rb_tree_node_base* y = x->left;
		x->left = y->right;
		if (y->right != 0) {
			y->right->parent = x;
		}

		y->parent = x->parent;

		//y顶替x，全盘接受x的关系
		if (x == root) {
			x->parent->parent = y; //header -> parent
			root = y;
		}
		else if (x == x->parent->left) {
			x->parent->left = y;
		}
		else if (x == x->parent->right) {
			x->parent->right = y;
		}

		y->right = x;
		x->parent = y;

	}

	//全局函数，改变节点颜色或者旋转，使红黑树保持平衡
	/*平衡条件：
	（1）每个节点或者是黑色，或者是红色。
	（2）根节点是黑色。
	（3）每个叶子节点（NIL）是黑色。 [注意：这里叶子节点，是指为空(NIL或NULL)的叶子节点！]

	（4）如果一个节点是红色的，则它的子节点必须是黑色的。
	（5）从一个节点到该节点的子孙节点的所有路径上包含相同数目的黑节点。

	由规则4和5，可以推导红黑树平衡的关键——红黑树的最长路径最多是最短路径的两倍
	*/
	//root传引用，需要改变root

	/*
	平衡例子：父节点为祖父节点的左节点
	一、父节点为祖父节点的左节点
		1.父节点为黑
			直接插入不用改变
		2.父节点为红，伯父节点为红 
			新节点，父节点均为红，违反规则4
			父节点->黑
			伯父节点->黑
			祖父节点->红
			然后继续往上检查，新节点置为祖父节点
		3.父节点为红，无伯父节点或者伯父节点为黑
			新节点，父节点均为红，违反规则4
			使用第2点方法只改变颜色？不行，违反规则5

			原因：假色原本从根节点到父节点、伯父结点路径上黑色节点均为4个
			如果改变父节点->黑，伯父节点->黑，祖父节点->红
			父节点路径 4-1+1=4，伯父节点路径 4-1=3
			因为多了一个黑色节点而父节点伯父节点无法同时变色
			所以红黑树一边路径上永远多了一个黑色节点，另一边无法改变
			综上所述，父节点为红，伯父节点为黑的情况下，单纯改变颜色，违反规则5

			所以，应该使用旋转
			1.新节点位于父节点右子节点
				父节点左旋，把情况1变成插入节点为左子节点的情况2
			2.新节点位于父节点左子节点
			  此时，新节点为红，父节点为红，祖父节点为黑色
			  目的是把父节点变成黑色的，但直接变会让一边子树的黑色节点比另一边多1个
			  所以要把父节点变成祖父结点位置，变黑
			  这样子，祖父变红，移动另一边，双方-1黑色，而父节点代替祖父节点位置，双方+1，保持不变
			  
			  所以新节点位于父节点左子节点时，应该右旋

	
	*/
	inline void __rb_tree_rebalance(__rb_tree_node_base* x, __rb_tree_node_base*& root) {
		x->color = __rb_tree_red;
		while (x != root && x->parent->color == __rb_tree_red) {

			if (x->parent == x->parent->parent->left) { //父节点为左子节点
				__rb_tree_node_base* y = x->parent->parent->right;
				if (y && y->color == __rb_tree_red) { //伯父节点为红色
					//伯父节点必须检验是否存在，否者nullptr->color会出错
					x->parent->color = __rb_tree_black; //父节点为黑
					x->parent->parent->right->color = __rb_tree_black; //伯父节点为黑
					x->parent->parent->color = __rb_tree_red; //祖父节点为黑
					x = x->parent->parent;//往上继续检查
				}//if2
				else{ //伯父节点为黑或者为空
					if (x == x->parent->right) {  //新节点为父节点的右子节点，需要对父节点左旋
						x = x->parent;
						__rb_tree_rotate_left(x, root);
					}
					//此时新节点为父节点左子节点，要右旋，先变色，再对祖父节点右旋
					x->parent->color = __rb_tree_black;
					x->parent->parent->color = __rb_tree_red;
					__rb_tree_rotate_right(x->parent->parent, root);
				}//else2

			}//if1
			else {//父节点为右子节点
				__rb_tree_node_base* y = x->parent->parent->left;
				if (y && y->color == __rb_tree_red) { //伯父节点为红色
					x->parent->color = __rb_tree_black; //父节点为黑
					x->parent->parent->left->color = __rb_tree_black; //伯父节点为黑
					x->parent->parent->color = __rb_tree_red; //祖父节点为黑
					x = x->parent->parent;//往上继续检查
				}//if3
				else { //伯父节点为黑或者为空
					if (x == x->parent->left) {  //新节点为父节点的左子节点，需要对父节点右旋
						x = x->parent;
						__rb_tree_rotate_right(x, root);
					}
					//此时新节点为父节点右子节点，要左旋，先变色，再对祖父节点左旋
					x->parent->color = __rb_tree_black;
					x->parent->parent->color = __rb_tree_red;
					__rb_tree_rotate_left(x->parent->parent, root);
				}//else3
			}//else1
		}//while
		root->color = __rb_tree_black;//根节点永远黑色
	}//函数 


	//红黑树删除操作
	/*
	一.寻找删除的节点
	inline __rb_tree_node_base* __rb_tree_delete_node(__rb_tree_node_base* x, __rb_tree_node_base*& root)
	x为要删除的节点，y为实际删除的节点

		1.x无子节点或者只有一个子节点
			y指向x节点，然后用x的子节点替换x；

		2.x有左右两个子节点
			y指向x的前驱（小于x的最大的节点）
			用y的子节点继承y的关系，如果y是x的前驱，可以知道y必定无右子树
			然后把x的键值更换为y的键值

		判断y是否为黑色节点，y为黑色节点则进入第二步，对红黑树重新做平衡操作

	二.对红黑树重新平衡 
	inline __rb_tree_node_base* __rb_tree_rebalance_for_erase(__rb_tree_node_base* x, __rb_tree_node_base*& root)
		x 为上面y的子节点，已经替换y
		如果x为红色节点，直接变黑即可


		如果x为黑节点
		1.x为左节点
			（1）x的兄弟节点w为红色
					z(1)		   w1
				  /	 \     --》  /  \
				 y(1) w(0)	    z0
							   /  \
							  x1  
				现况：假设x和w两边路径原本为 5 个黑色节点，删除了一个黑色y，左边为4，右边为5，不平衡
				目标：把w变成黑色后变成父节点，原本父节点变红左移，此时左边为4不变，右边 5-1 = 4

			（2）x的兄弟节点w为黑色
			   （2.1）w的左右子节点均为黑色
			现况：假设x和w两边路径原本为 5 个黑色节点
					z			   z   
				  /	 \     --》  /  \   
				y	  w		x(node)  w
				实际删除的是y，如果y为黑色，zy路径-1=4，zw为5，w如果为黑色，直接把w变成红色，即可-1=4
				但是会导致z的父节点到y/w路径整体-1
				所以把node指向z，如果z为黑色，继续对上方的节点做循环，因为无法变黑+1，如果z为红色则自接变黑，-1+1，整体路径不变

				（2.2）w的右节点为黑色，左节点为红色
					z			   z		  	   z
				  /	 \     --》  /  \		     /	 \   
				y	  w		x(node)  w  ==》	 y	  a (black)
					/  \			/  \	  	       \
				   a    b		   a    b	            w (red)
														 \
														  b (balck)

				zy路径-1=4，zw为5，w如果为黑色，直接把w变成红色，即可-1=4,w的左节点a变黑，w右旋，使a，w，b在一条直线上
				zy为4，zw为5
				此时a为node的兄弟节点
				（2.3）a（w）的右节点为红色（左子节点为红色或者没有左子节点）
					z			   z		  	   z（b）
				  /	 \     --》  /  \		     /	 \
				y	  w		x(node)  w  ==》	 y	  a (black)
					/  \			/  \	  	       \         ==》
				   a    b		   a    b	            w (red)
														 \
														  b (balck)

				首先让兄弟节点a的颜色变成和父节点一致，这样子待会用a代替父节点z的时候整体无变化，
				然后让兄弟节点a的右子节点w变成黑色，这样的话，w代替a的位置时，右子树无变化
				父节点左旋，变成左子树一部分，此时zy为4+1，zw为5-1+1
				结束
	
	*/

	//
	inline void __rb_tree_rebalance_for_erase(__rb_tree_node_base* node,__rb_tree_node_base* node_parent,__rb_tree_node_base*& root) {

		//node为实际删除节点的子节点
		while (node != root && (node == 0 || node->color == __rb_tree_black)) {
			//node不为根节点，且node为黑色或者null
			if (node == node_parent->left) { //先讨论node为左节点的情况，右节点与左节点只需做相反动作
				__rb_tree_node_base* w = node_parent->right;

				if (w && w->color == __rb_tree_red) {  //情况1，node的兄弟节点w为红色
					w->color = __rb_tree_black;  
					node_parent->color = __rb_tree_red;
					//w 与 父节点互换颜色
					//然后把父节点左旋，w变成根节点，左支路不变，右支路-1，所以整体-1
					__rb_tree_rotate_left(node_parent, root);
					//此时node为黑色，未出循环，情况1转换为-》情况2、3、4
				}
				else {
					if (( w->left == 0 || w->left->color == __rb_tree_black) && ( w->right == 0 || w->right->color == __rb_tree_black)){  
						//情况2,w为黑/空，w儿子均为黑/空
						w->color = __rb_tree_red;//右支-1，整体-1
						node = node_parent;
						node_parent = node_parent->parent;//如果父节点为红色，即可出循环，然后红变黑，整体+1,否则继续往上探查
					}
					else {
						if (!(w->right) || w->right->color == __rb_tree_black) {
							//情况3，w为黑/空，w右儿子为黑/空，左儿子为红
							//目标：转换为情况4
							w->color = __rb_tree_red;
							w->left->color = __rb_tree_red;
							__rb_tree_rotate_right(w->left, root);//w的左儿子取代w位子，w为红色做w->left的左儿子
							w = node_parent->right;//w换成原w的左儿子，为黑色
							//且一系列操作后，右支路的黑色点数不变
						}
						//情况4，w为黑/空，w右儿子为红，左儿子为红
						//目标：用w代替父节点，父节点为黑色左移动，用w的右儿子（红变黑）代替w
						//左子路+1恢复正常，右支路-1+1=0不变，整体不变
						w->color = node_parent->color;//w和父节点颜色保持一致，待会替换父节点整体红黑不变
						node_parent->color = __rb_tree_black; //父节点为黑色左移动,左支路+1
						if(w->right) w->right->color = __rb_tree_black; //用w的右儿子（红变黑）代替w，右支路-1+1=0不变
						__rb_tree_rotate_left(node_parent, root);

						break; //while结束条件
					}
				}
			}//node为左子节点
			else {  //node为右子节点
				__rb_tree_node_base* w = node_parent->left;
				if (w->color == __rb_tree_red) {  //情况1，node的兄弟节点w为红色
					w->color = __rb_tree_black;
					node_parent->color = __rb_tree_red;
					__rb_tree_rotate_right(node_parent, root);
				}
				else {
					if ((w->left==0 || w->left->color == __rb_tree_black) && ( w->right == 0 || w->right->color == __rb_tree_black)) {
						//情况2,w为黑/空，w儿子均为黑/空
						w->color = __rb_tree_red;
						node = node_parent;
						node_parent = node_parent->parent;
					}
					else {
						if (!(w->left) || w->left->color == __rb_tree_black) {
							//情况3，w为黑/空，w左儿子为黑/空，右儿子为红
							//目标：转换为情况4
							w->color = __rb_tree_red;
							w->right->color = __rb_tree_red;
							__rb_tree_rotate_left(w, root);
							w = node_parent->left;
						}
						//情况4，w为黑/空，w右儿子为红，左儿子为红
						//目标：用w代替父节点，父节点为黑色y右移动，用w的左儿子（红变黑）代替w
						w->color = node_parent->color;//w和父节点颜色保持一致，待会替换父节点整体红黑不变
						node_parent->color = __rb_tree_black; //父节点为黑色右移动,右支路+1
						if(w->left) w->left->color = __rb_tree_black; //用w的左儿子（红变黑）代替w，左支路-1+1=0不变
						__rb_tree_rotate_right(node_parent, root);

						break; //while结束条件
					}
				}
			}

		}
		if(node) node->color = __rb_tree_black;//如果node为红色，直接变黑，即可补上空缺
		 
	}


	//找到删除节点，返回
	template<class Value, class Key, class KeyOfValue, class Compare, class Alloc>
	inline __rb_tree_node_base* rb_tree<Value,Key, KeyOfValue, Compare,Alloc>::__rb_tree_delete_node(__rb_tree_node_base* x, __rb_tree_node_base*& root) {
		__rb_tree_node_base* delete_node = x; //实际删除的节点
		__rb_tree_node_base* delete_node_child = 0; //实际删除的节点的孩子节点
		__rb_tree_node_base* delete_node_child_parent = 0; //实际删除的节点的孩子节点

		if (x->left == 0 || x->left == 0) {  //x只有一个子节点或者没有子节点
			delete_node = x;  //实际删除x
		}
		else {
			delete_node = decrement_for_node(x); //实际删除的节点为x的前驱
		}

		if (delete_node->right != 0) {
			delete_node_child = delete_node->right;
		}
		else if(delete_node->left != 0) {
			delete_node_child = delete_node->left;
		}
		else {
			delete_node_child = nullptr;
		}
		if (delete_node_child)
			delete_node_child->parent = delete_node->parent;
		else {
			delete_node_child_parent = delete_node->parent;
		}
		//注意delete_node_child为空则不行
		if (delete_node == root) {
			root = delete_node_child;
		}
		if (delete_node == delete_node->parent->right) {
			delete_node->parent->right = delete_node_child;
		}
		if (delete_node == delete_node->parent->left) {
			delete_node->parent->left = delete_node_child;
		}
	

		if (leftmost() == x) {
			if (x->right == 0) {
				leftmost() =(link_type)x->parent;
			}
			else {
				leftmost() = minimum((link_type)root);
			}
		}
		if (rightmost() == x) {
			if (x->left == 0) {
				rightmost() = (link_type)x->parent;
			}
			else {
				rightmost() = maximum((link_type)root);
			}
		}
	
		if (x != delete_node ) {
			((link_type)x)->value_field = value(delete_node);				
		}


		if (delete_node->color == __rb_tree_black) {
			__rb_tree_rebalance_for_erase(delete_node_child, delete_node_child_parent,root);
			//注意此处传第二参数delete_node_child_parent，是因为delete_node_child如果等于空，接下来在平衡函数中，如果使用delete_node_child->parent等都会报错，null无法访问parent/color等
		}
		return delete_node;

	}


	 

}








#endif
