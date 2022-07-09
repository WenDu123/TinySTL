#pragma once
#ifndef __PAIR_H__
#define __PAIR_H__

namespace MyTinySTL {
	template<class T1,class T2>
	struct pair {
		typedef T1 first_type;
		typedef T2 second_type;

		first_type first;
		second_type second;

		pair() : first(T1()),second(T2()){}
		pair(const T1& a,const T2& b) : first(a),second(b){}

		template<class U1,class U2>
		pair(const pair<U1,U2>& x) : first(x.first),second(x.second){}
	};

	template<class T1,class T2>
	inline bool operator==(const pair<T1, T2>& x, const pair<T1, T2>& y) {
		return x.first == y.first && x.second == y.second;
	}

	template<class T1, class T2>
	inline bool operator!=(const pair<T1, T2>& x, const pair<T1, T2>& y) {
		return !(x == y);
	}

	template<class T1, class T2>
	inline bool operator<(const pair<T1, T2>& x, const pair<T1, T2>& y) {
		return x.first < y.first || (!(x.first < y.first)&& x.second < y.second);
	}

	template<class T1, class T2>
	inline bool operator>(const pair<T1, T2>& x, const pair<T1, T2>& y) {
		return !(x < y);
	}




}




#endif // !__PAIR_H__

