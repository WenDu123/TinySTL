#pragma once
#ifndef _RB_TREE_H_
#define _RB_TREE_H_


#include"Allocator.h"

#include"Uninitialized.h"



#include "functional.h"

namespace MyTinySTL {
	//����
	typedef bool __rb_tree_color_type;
	const __rb_tree_color_type __rb_tree_black = true;  //��Ϊ1
	const __rb_tree_color_type __rb_tree_red = false;  //��Ϊ0
	//�ڵ�
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

	__rb_tree_node_base* decrement_for_node(__rb_tree_node_base* y) {  //Ѱ��ǰ��
		__rb_tree_node_base* node = y;
		if (node->color == __rb_tree_red && node->parent->parent == node) {
			//��ʾnodeΪheader,��node = end()
			//��ʱ����nodeΪmostright
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


	//������

	//base_iterator
	struct __rb_tree_iterator_base {
		typedef __rb_tree_node_base* base_ptr;
		typedef bidirectional_iterator_tag iterator_category;
		typedef ptrdiff_t difference_type;

		base_ptr node;

		/*
		һ�����ĺ�̣��Ǵ���x.key����С�ؼ��ֵĽ�㣬���ҽڵ����������Сֵ
		һ������ǰ������С��x.key�����ؼ��ֵĽ�㣬����ڵ�����������ֵ

		ǰ������������ڵ㣬����ýڵ�������������Сֵ�����Բ��������ӵģ�������ǰ����null��
		��������ýڵ�������ӣ�����ǰ�������������Ϊ���µ�����������������ֵ���Ǹ��ڵ㣻
		��������������Ҳ�����Сֵ������ǰ���Ǹ��ڵ㡣

	    ��̣���������ڵ㣬����ýڵ��������������ֵ�����Բ������Һ��ӵģ������ĺ����null��
		��������ýڵ�����Һ��ӣ����ĺ����������Һ���Ϊ���µ��ұ�������������Сֵ���Ǹ��ڵ㣻
		����������Һ����Ҳ������ֵ�����ĺ���Ǹ��ڵ�
	
		*/

		//Ѱ�Ҹýڵ�ĺ��,�����ҽڵ����������Сֵ
		//��̣�����x.key����С�ؼ��ֵĽ��
		void increment() {
			if (node->right != 0) {   //״��1�������ҽڵ�
				//�����������ҽڵ�Ϊ���ģ�������������Сֵ���Ǹ��ڵ㣨������������
				node = node->right;  //ֱ�Ӵ��ҽڵ㿪ʼ��
				while (node->left != 0)  //һֱ���������ߣ�ֱ��������Ϊ�գ����ҵ���Сֵ
					node = node->left;
			}
			else {   //״��2���������ҽڵ�,������
				base_ptr x = node->parent;
				/*
				x������
				node������
				���2��node == x->right��nodeΪx���Ҷ��ӣ���node���Ҷ��ӣ�����node>x��������������
				   ���node != x->right��nodeΪx������ӣ���node���Ҷ��ӣ�����node<x,��ʱxΪnode���
				*/
				while (node == x->right) {  //���2
					node = x;
					x = node->parent;
				}

				if (node->right != x) {  //���3
					node = x;
					/*
					���������node���Ҷ��Ӳ����ܵ���node�ĸ��ڵ㣬���ظ��ڵ㼴��
					���������
						1.Ҫ�����ҽڵ�ĺ�̣�Ӧ�÷���root
						��ʱnode = ��x = root
						���Ǹ��ڵ����Ҷ����Ҳ����ܵ���header������node��x������node = root
					
					*/
				}
				//���4
				/*
				���������node = root����root���ҽڵ�
				node->right =nullptr = header = root->parent
				��ʾ��ֻ�и��ڵ�����ӽڵ㣬��ʱӦ�÷��ظ��ڵ�
				��ֱ�ӷ���root
				*/

			}

		}
		//Ѱ�Ҹýڵ��ǰ��
		//ǰ������С��x.key�����ؼ��ֵĽ�㣬����ڵ�����������ֵ
		void decrement() {
			if (node->color == __rb_tree_red && node->parent->parent == node) {
				//��ʾnodeΪheader,��node = end()
				//��ʱ����nodeΪmostright
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

	/* ��������Ӧ��ע�����ģ��Ĳ���
	 * key���������ϴ洢�ڵ�ʱ���ݵ�ֵ
	 * value���ڵ��ֵ
	 * �Ƿº���,����ͨ��value��ȡ��key
	 * CompareΪ�Ƚ�key��С�ı�׼
	 * Alloc�ǿռ�������
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

	protected: //�ڵ���ڴ����
		link_type get_node() {  //Ϊ�ڵ�����һ���ڴ�ռ�
			return rb_tree_allocator::allocate(1);
		}
		void put_node(link_type p) {  //�ͷŽڵ���ڴ�ռ�
			rb_tree_allocator::deallocate(p,1);
		}
		link_type create_node(const value_type& val) {  //�����ڴ棬������һ���ڵ�
			link_type tmp = get_node();
			construct(&tmp->value_field, val);
			return tmp;
		}
		link_type clone_node(link_type x) {  //��¡�����ƣ�һ���ڵ㣨ֵ����ɫ��
			link_type tmp = create_node(x->value_field);
			tmp->color = x->color;
			tmp->left = 0;
			tmp->right = 0;
			return tmp;
		}
		void destroy_node(link_type p) {  //��������ڵ㣬���ͷ��ڴ�
			destory(&(p->value_field));
			put_node(p);
		}

	protected://rb_tree���ݽṹ����
		//rb_tree ֻ�����������ݱ���
		size_type node_count; //��¼���Ĵ�С���ڵ�������
		link_type header; //ͷ�ڵ�
		Compare key_compare; //�º������󣬽ڵ���ֵ�Ƚϴ�С׼��

		//ʹ�ú���ȡ�ýڵ�header��Ա
		link_type& root() const { return (link_type&) header->parent; }
		link_type& leftmost()const { return (link_type&) header->left; }
		link_type& rightmost()const { return (link_type&) header->right; }

		//ʹ�ú���ȡ�ýڵ�x��Ա
		static link_type& left(link_type x) { return (link_type&)(x->left); }
		static link_type& right(link_type x) { return (link_type&)(x->right); }
		static link_type& parent(link_type x) { return (link_type&)(x->parent); }
		static reference value(link_type x) { return x->value_field; }
		static color_type& color(link_type x) { return (color_type&)(x->color); }
		static const key_type& key(link_type x) {
			return KeyOfValue()(value(x));
		}

		//ʹ�ú���ȡ�ýڵ�x��Ա
		static link_type& left (base_ptr x) { return (link_type&)(x->left); }
		static link_type& right(base_ptr x) { return (link_type&)(x->right); }
		static link_type& parent(base_ptr x) { return (link_type&)(x->parent); }
		static reference value(base_ptr x) { return ((link_type)x)->value_field; }
		static color_type& color(base_ptr x) { return (color_type&)(link_type(x)->color); }
		static const key_type& key(base_ptr x) {
			return KeyOfValue()(value(link_type(x)));
		}

		//�����ֵ��Сֵ
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
			link_type x = (link_type)_x; //�����
			link_type y = (link_type)_y; //����㸸�ڵ�
			link_type z; //����㴴��

			if (y == header || x != 0 || key_compare(KeyOfValue()(val), key(y))) {
				//��Ϊ�ջ��߲���ڵ�Ϊ���ڵ�����ӽڵ����if
				z = create_node(val);
				left(y) = z;
				//���y == header��leftmost()=z,��ǰ����
				//���y ��= header��zΪy�����ӽڵ�
				if (y == header) {
					root() = z;
					rightmost() = z;
				}
				else if(y == leftmost()) {
					leftmost() = z;
				}
			}
			else {  //������½ڵ�λ��y�����ӽڵ�
				z = create_node(val);
				right(y) = z;
				if (y == rightmost()) {
					rightmost() = z;
				}
			}
			//�趨�½ڵ�ĸ��ڵ㣬���ҽڵ�
			parent(z) = y;
			left(z) = 0;
			right(z) = 0;

			__rb_tree_rebalance(z, header->parent);//�����½ڵ����������ʹ֮ƽ��
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
			if (this != &x) { //��ֹ���Ҹ�ֵ
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
				clear_erase(root()); //������нڵ�
				// ��ʼ��Header�ڵ�
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
		iterator insert_equal(const value_type& val) { //�ڵ��ֵ�����ظ�
			link_type x = header;
			link_type y = root();

			while (y != 0) {
				x = y;
				y = key_compare(KeyOfValue()(val), key(y)) ? left(y) : right(y);
				//v<y,����bool = 1��������ߣ���֮���ұ���
			}
			return __insert(y, x, val);
			//����(�²���㣬�²���㸸�ڵ㣬ֵ)

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
				x = y;//x������
				comp = key_compare(KeyOfValue()(val), key(y));
				//comp = 1��val < y,���󣬷�֮����
				y = comp ? left(y) : right(y);
			}//�ҵ�λ��

			iterator j = iterator(x);//xΪ����ڵ㸸�ڵ�
			if (comp) { //����
				if (j == begin()) {  //����λ��λ��������ֱ�Ӳ���
					return pair<iterator,bool>(__insert(y, x, val),true); 
					//true��ʾ���ظ�������ɹ�
				}
				else {
					--j;//j = j��ǰ����С��j����ֵ����Ϊy
				}
			}
			//������j=--j����ʱjΪx��ǰ�������������yΪx�����ӽڵ㣬����y�������xǰ����λ�ã�x>y>--j,����y�ĸ��ڵ�Ӧ����--j�Ŷ�
			//�����ң�j = x��y�ĸ��ڵ㣬����y��x�����ӽڵ㣬���� y > x

			if (key_compare(key(j.node), KeyOfValue()(val))) {
				//��ʱ���������������ң�������if���������򲻻���룡
				return pair<iterator,bool>(__insert(y, x, val),true);

			}
			//�����²���ֵ������ֵ�ظ���������
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

	//����
	inline void __rb_tree_rotate_left(__rb_tree_node_base* x, __rb_tree_node_base*& root) {
		//xΪԭ���ڵ㣬yΪx���ҽڵ�
		//Ҫע�⣬Ҫ���ýڵ�A�ӽڵ���ڽڵ�B����Ҫ�ѽڵ�B�ĸ�����Ϊ�ڵ�A��˫������
		__rb_tree_node_base* y = x->right;
		x->right = y->left;
		if (y->left != 0) {
			y->left->parent = x;
		}

		y->parent = x->parent;

		//y����x��ȫ�̽���x�Ĺ�ϵ
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
		//xΪԭ���ڵ㣬yΪx����ڵ�
		//Ҫע�⣬Ҫ���ýڵ�A�ӽڵ���ڽڵ�B����Ҫ�ѽڵ�B�ĸ�����Ϊ�ڵ�A��˫������
		__rb_tree_node_base* y = x->left;
		x->left = y->right;
		if (y->right != 0) {
			y->right->parent = x;
		}

		y->parent = x->parent;

		//y����x��ȫ�̽���x�Ĺ�ϵ
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

	//ȫ�ֺ������ı�ڵ���ɫ������ת��ʹ���������ƽ��
	/*ƽ��������
	��1��ÿ���ڵ�����Ǻ�ɫ�������Ǻ�ɫ��
	��2�����ڵ��Ǻ�ɫ��
	��3��ÿ��Ҷ�ӽڵ㣨NIL���Ǻ�ɫ�� [ע�⣺����Ҷ�ӽڵ㣬��ָΪ��(NIL��NULL)��Ҷ�ӽڵ㣡]

	��4�����һ���ڵ��Ǻ�ɫ�ģ��������ӽڵ�����Ǻ�ɫ�ġ�
	��5����һ���ڵ㵽�ýڵ������ڵ������·���ϰ�����ͬ��Ŀ�ĺڽڵ㡣

	�ɹ���4��5�������Ƶ������ƽ��Ĺؼ�������������·����������·��������
	*/
	//root�����ã���Ҫ�ı�root

	/*
	ƽ�����ӣ����ڵ�Ϊ�游�ڵ����ڵ�
	һ�����ڵ�Ϊ�游�ڵ����ڵ�
		1.���ڵ�Ϊ��
			ֱ�Ӳ��벻�øı�
		2.���ڵ�Ϊ�죬�����ڵ�Ϊ�� 
			�½ڵ㣬���ڵ��Ϊ�죬Υ������4
			���ڵ�->��
			�����ڵ�->��
			�游�ڵ�->��
			Ȼ��������ϼ�飬�½ڵ���Ϊ�游�ڵ�
		3.���ڵ�Ϊ�죬�޲����ڵ���߲����ڵ�Ϊ��
			�½ڵ㣬���ڵ��Ϊ�죬Υ������4
			ʹ�õ�2�㷽��ֻ�ı���ɫ�����У�Υ������5

			ԭ�򣺼�ɫԭ���Ӹ��ڵ㵽���ڵ㡢�������·���Ϻ�ɫ�ڵ��Ϊ4��
			����ı丸�ڵ�->�ڣ������ڵ�->�ڣ��游�ڵ�->��
			���ڵ�·�� 4-1+1=4�������ڵ�·�� 4-1=3
			��Ϊ����һ����ɫ�ڵ�����ڵ㲮���ڵ��޷�ͬʱ��ɫ
			���Ժ����һ��·������Զ����һ����ɫ�ڵ㣬��һ���޷��ı�
			�������������ڵ�Ϊ�죬�����ڵ�Ϊ�ڵ�����£������ı���ɫ��Υ������5

			���ԣ�Ӧ��ʹ����ת
			1.�½ڵ�λ�ڸ��ڵ����ӽڵ�
				���ڵ������������1��ɲ���ڵ�Ϊ���ӽڵ�����2
			2.�½ڵ�λ�ڸ��ڵ����ӽڵ�
			  ��ʱ���½ڵ�Ϊ�죬���ڵ�Ϊ�죬�游�ڵ�Ϊ��ɫ
			  Ŀ���ǰѸ��ڵ��ɺ�ɫ�ģ���ֱ�ӱ����һ�������ĺ�ɫ�ڵ����һ�߶�1��
			  ����Ҫ�Ѹ��ڵ����游���λ�ã����
			  �����ӣ��游��죬�ƶ���һ�ߣ�˫��-1��ɫ�������ڵ�����游�ڵ�λ�ã�˫��+1�����ֲ���
			  
			  �����½ڵ�λ�ڸ��ڵ����ӽڵ�ʱ��Ӧ������

	
	*/
	inline void __rb_tree_rebalance(__rb_tree_node_base* x, __rb_tree_node_base*& root) {
		x->color = __rb_tree_red;
		while (x != root && x->parent->color == __rb_tree_red) {

			if (x->parent == x->parent->parent->left) { //���ڵ�Ϊ���ӽڵ�
				__rb_tree_node_base* y = x->parent->parent->right;
				if (y && y->color == __rb_tree_red) { //�����ڵ�Ϊ��ɫ
					//�����ڵ��������Ƿ���ڣ�����nullptr->color�����
					x->parent->color = __rb_tree_black; //���ڵ�Ϊ��
					x->parent->parent->right->color = __rb_tree_black; //�����ڵ�Ϊ��
					x->parent->parent->color = __rb_tree_red; //�游�ڵ�Ϊ��
					x = x->parent->parent;//���ϼ������
				}//if2
				else{ //�����ڵ�Ϊ�ڻ���Ϊ��
					if (x == x->parent->right) {  //�½ڵ�Ϊ���ڵ�����ӽڵ㣬��Ҫ�Ը��ڵ�����
						x = x->parent;
						__rb_tree_rotate_left(x, root);
					}
					//��ʱ�½ڵ�Ϊ���ڵ����ӽڵ㣬Ҫ�������ȱ�ɫ���ٶ��游�ڵ�����
					x->parent->color = __rb_tree_black;
					x->parent->parent->color = __rb_tree_red;
					__rb_tree_rotate_right(x->parent->parent, root);
				}//else2

			}//if1
			else {//���ڵ�Ϊ���ӽڵ�
				__rb_tree_node_base* y = x->parent->parent->left;
				if (y && y->color == __rb_tree_red) { //�����ڵ�Ϊ��ɫ
					x->parent->color = __rb_tree_black; //���ڵ�Ϊ��
					x->parent->parent->left->color = __rb_tree_black; //�����ڵ�Ϊ��
					x->parent->parent->color = __rb_tree_red; //�游�ڵ�Ϊ��
					x = x->parent->parent;//���ϼ������
				}//if3
				else { //�����ڵ�Ϊ�ڻ���Ϊ��
					if (x == x->parent->left) {  //�½ڵ�Ϊ���ڵ�����ӽڵ㣬��Ҫ�Ը��ڵ�����
						x = x->parent;
						__rb_tree_rotate_right(x, root);
					}
					//��ʱ�½ڵ�Ϊ���ڵ����ӽڵ㣬Ҫ�������ȱ�ɫ���ٶ��游�ڵ�����
					x->parent->color = __rb_tree_black;
					x->parent->parent->color = __rb_tree_red;
					__rb_tree_rotate_left(x->parent->parent, root);
				}//else3
			}//else1
		}//while
		root->color = __rb_tree_black;//���ڵ���Զ��ɫ
	}//���� 


	//�����ɾ������
	/*
	һ.Ѱ��ɾ���Ľڵ�
	inline __rb_tree_node_base* __rb_tree_delete_node(__rb_tree_node_base* x, __rb_tree_node_base*& root)
	xΪҪɾ���Ľڵ㣬yΪʵ��ɾ���Ľڵ�

		1.x���ӽڵ����ֻ��һ���ӽڵ�
			yָ��x�ڵ㣬Ȼ����x���ӽڵ��滻x��

		2.x�����������ӽڵ�
			yָ��x��ǰ����С��x�����Ľڵ㣩
			��y���ӽڵ�̳�y�Ĺ�ϵ�����y��x��ǰ��������֪��y�ض���������
			Ȼ���x�ļ�ֵ����Ϊy�ļ�ֵ

		�ж�y�Ƿ�Ϊ��ɫ�ڵ㣬yΪ��ɫ�ڵ������ڶ������Ժ����������ƽ�����

	��.�Ժ��������ƽ�� 
	inline __rb_tree_node_base* __rb_tree_rebalance_for_erase(__rb_tree_node_base* x, __rb_tree_node_base*& root)
		x Ϊ����y���ӽڵ㣬�Ѿ��滻y
		���xΪ��ɫ�ڵ㣬ֱ�ӱ�ڼ���


		���xΪ�ڽڵ�
		1.xΪ��ڵ�
			��1��x���ֵܽڵ�wΪ��ɫ
					z(1)		   w1
				  /	 \     --��  /  \
				 y(1) w(0)	    z0
							   /  \
							  x1  
				�ֿ�������x��w����·��ԭ��Ϊ 5 ����ɫ�ڵ㣬ɾ����һ����ɫy�����Ϊ4���ұ�Ϊ5����ƽ��
				Ŀ�꣺��w��ɺ�ɫ���ɸ��ڵ㣬ԭ�����ڵ������ƣ���ʱ���Ϊ4���䣬�ұ� 5-1 = 4

			��2��x���ֵܽڵ�wΪ��ɫ
			   ��2.1��w�������ӽڵ��Ϊ��ɫ
			�ֿ�������x��w����·��ԭ��Ϊ 5 ����ɫ�ڵ�
					z			   z   
				  /	 \     --��  /  \   
				y	  w		x(node)  w
				ʵ��ɾ������y�����yΪ��ɫ��zy·��-1=4��zwΪ5��w���Ϊ��ɫ��ֱ�Ӱ�w��ɺ�ɫ������-1=4
				���ǻᵼ��z�ĸ��ڵ㵽y/w·������-1
				���԰�nodeָ��z�����zΪ��ɫ���������Ϸ��Ľڵ���ѭ������Ϊ�޷����+1�����zΪ��ɫ���Խӱ�ڣ�-1+1������·������

				��2.2��w���ҽڵ�Ϊ��ɫ����ڵ�Ϊ��ɫ
					z			   z		  	   z
				  /	 \     --��  /  \		     /	 \   
				y	  w		x(node)  w  ==��	 y	  a (black)
					/  \			/  \	  	       \
				   a    b		   a    b	            w (red)
														 \
														  b (balck)

				zy·��-1=4��zwΪ5��w���Ϊ��ɫ��ֱ�Ӱ�w��ɺ�ɫ������-1=4,w����ڵ�a��ڣ�w������ʹa��w��b��һ��ֱ����
				zyΪ4��zwΪ5
				��ʱaΪnode���ֵܽڵ�
				��2.3��a��w�����ҽڵ�Ϊ��ɫ�����ӽڵ�Ϊ��ɫ����û�����ӽڵ㣩
					z			   z		  	   z��b��
				  /	 \     --��  /  \		     /	 \
				y	  w		x(node)  w  ==��	 y	  a (black)
					/  \			/  \	  	       \         ==��
				   a    b		   a    b	            w (red)
														 \
														  b (balck)

				�������ֵܽڵ�a����ɫ��ɺ͸��ڵ�һ�£������Ӵ�����a���游�ڵ�z��ʱ�������ޱ仯��
				Ȼ�����ֵܽڵ�a�����ӽڵ�w��ɺ�ɫ�������Ļ���w����a��λ��ʱ���������ޱ仯
				���ڵ����������������һ���֣���ʱzyΪ4+1��zwΪ5-1+1
				����
	
	*/

	//
	inline void __rb_tree_rebalance_for_erase(__rb_tree_node_base* node,__rb_tree_node_base* node_parent,__rb_tree_node_base*& root) {

		//nodeΪʵ��ɾ���ڵ���ӽڵ�
		while (node != root && (node == 0 || node->color == __rb_tree_black)) {
			//node��Ϊ���ڵ㣬��nodeΪ��ɫ����null
			if (node == node_parent->left) { //������nodeΪ��ڵ��������ҽڵ�����ڵ�ֻ�����෴����
				__rb_tree_node_base* w = node_parent->right;

				if (w && w->color == __rb_tree_red) {  //���1��node���ֵܽڵ�wΪ��ɫ
					w->color = __rb_tree_black;  
					node_parent->color = __rb_tree_red;
					//w �� ���ڵ㻥����ɫ
					//Ȼ��Ѹ��ڵ�������w��ɸ��ڵ㣬��֧·���䣬��֧·-1����������-1
					__rb_tree_rotate_left(node_parent, root);
					//��ʱnodeΪ��ɫ��δ��ѭ�������1ת��Ϊ-�����2��3��4
				}
				else {
					if (( w->left == 0 || w->left->color == __rb_tree_black) && ( w->right == 0 || w->right->color == __rb_tree_black)){  
						//���2,wΪ��/�գ�w���Ӿ�Ϊ��/��
						w->color = __rb_tree_red;//��֧-1������-1
						node = node_parent;
						node_parent = node_parent->parent;//������ڵ�Ϊ��ɫ�����ɳ�ѭ����Ȼ����ڣ�����+1,�����������̽��
					}
					else {
						if (!(w->right) || w->right->color == __rb_tree_black) {
							//���3��wΪ��/�գ�w�Ҷ���Ϊ��/�գ������Ϊ��
							//Ŀ�꣺ת��Ϊ���4
							w->color = __rb_tree_red;
							w->left->color = __rb_tree_red;
							__rb_tree_rotate_right(w->left, root);//w�������ȡ��wλ�ӣ�wΪ��ɫ��w->left�������
							w = node_parent->right;//w����ԭw������ӣ�Ϊ��ɫ
							//��һϵ�в�������֧·�ĺ�ɫ��������
						}
						//���4��wΪ��/�գ�w�Ҷ���Ϊ�죬�����Ϊ��
						//Ŀ�꣺��w���游�ڵ㣬���ڵ�Ϊ��ɫ���ƶ�����w���Ҷ��ӣ����ڣ�����w
						//����·+1�ָ���������֧·-1+1=0���䣬���岻��
						w->color = node_parent->color;//w�͸��ڵ���ɫ����һ�£������滻���ڵ������ڲ���
						node_parent->color = __rb_tree_black; //���ڵ�Ϊ��ɫ���ƶ�,��֧·+1
						if(w->right) w->right->color = __rb_tree_black; //��w���Ҷ��ӣ����ڣ�����w����֧·-1+1=0����
						__rb_tree_rotate_left(node_parent, root);

						break; //while��������
					}
				}
			}//nodeΪ���ӽڵ�
			else {  //nodeΪ���ӽڵ�
				__rb_tree_node_base* w = node_parent->left;
				if (w->color == __rb_tree_red) {  //���1��node���ֵܽڵ�wΪ��ɫ
					w->color = __rb_tree_black;
					node_parent->color = __rb_tree_red;
					__rb_tree_rotate_right(node_parent, root);
				}
				else {
					if ((w->left==0 || w->left->color == __rb_tree_black) && ( w->right == 0 || w->right->color == __rb_tree_black)) {
						//���2,wΪ��/�գ�w���Ӿ�Ϊ��/��
						w->color = __rb_tree_red;
						node = node_parent;
						node_parent = node_parent->parent;
					}
					else {
						if (!(w->left) || w->left->color == __rb_tree_black) {
							//���3��wΪ��/�գ�w�����Ϊ��/�գ��Ҷ���Ϊ��
							//Ŀ�꣺ת��Ϊ���4
							w->color = __rb_tree_red;
							w->right->color = __rb_tree_red;
							__rb_tree_rotate_left(w, root);
							w = node_parent->left;
						}
						//���4��wΪ��/�գ�w�Ҷ���Ϊ�죬�����Ϊ��
						//Ŀ�꣺��w���游�ڵ㣬���ڵ�Ϊ��ɫy���ƶ�����w������ӣ����ڣ�����w
						w->color = node_parent->color;//w�͸��ڵ���ɫ����һ�£������滻���ڵ������ڲ���
						node_parent->color = __rb_tree_black; //���ڵ�Ϊ��ɫ���ƶ�,��֧·+1
						if(w->left) w->left->color = __rb_tree_black; //��w������ӣ����ڣ�����w����֧·-1+1=0����
						__rb_tree_rotate_right(node_parent, root);

						break; //while��������
					}
				}
			}

		}
		if(node) node->color = __rb_tree_black;//���nodeΪ��ɫ��ֱ�ӱ�ڣ����ɲ��Ͽ�ȱ
		 
	}


	//�ҵ�ɾ���ڵ㣬����
	template<class Value, class Key, class KeyOfValue, class Compare, class Alloc>
	inline __rb_tree_node_base* rb_tree<Value,Key, KeyOfValue, Compare,Alloc>::__rb_tree_delete_node(__rb_tree_node_base* x, __rb_tree_node_base*& root) {
		__rb_tree_node_base* delete_node = x; //ʵ��ɾ���Ľڵ�
		__rb_tree_node_base* delete_node_child = 0; //ʵ��ɾ���Ľڵ�ĺ��ӽڵ�
		__rb_tree_node_base* delete_node_child_parent = 0; //ʵ��ɾ���Ľڵ�ĺ��ӽڵ�

		if (x->left == 0 || x->left == 0) {  //xֻ��һ���ӽڵ����û���ӽڵ�
			delete_node = x;  //ʵ��ɾ��x
		}
		else {
			delete_node = decrement_for_node(x); //ʵ��ɾ���Ľڵ�Ϊx��ǰ��
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
		//ע��delete_node_childΪ������
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
			//ע��˴����ڶ�����delete_node_child_parent������Ϊdelete_node_child������ڿգ���������ƽ�⺯���У����ʹ��delete_node_child->parent�ȶ��ᱨ��null�޷�����parent/color��
		}
		return delete_node;

	}


	 

}








#endif
