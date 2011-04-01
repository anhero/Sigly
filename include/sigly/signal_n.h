#ifndef SIGLY_SIGLY_SIGNAL_N_H
#define SIGLY_SIGLY_SIGNAL_N_H

#include <sigly/defines.h>
#include <sigly/signal_base_n.h>
#include <sigly/lock_block.h>
#include <sigly/connection_n.h>

namespace sigly {
template<class mt_policy = SIGLY_DEFAULT_MT_POLICY>
class Signal0 : public _signal_base0<mt_policy> {
public:
	typedef typename _signal_base0<mt_policy>::connections_list::const_iterator const_iterator;
	Signal0() {
	}

	Signal0(const Signal0<mt_policy>& s)
		: _signal_base0<mt_policy>(s) {
	}

	template<class desttype>
	void connect(desttype* pclass, void (desttype::*pmemfun)()) {
		LockBlock<mt_policy> lock(this);
		_connection0<desttype, mt_policy>* conn =
		    new _connection0<desttype, mt_policy>(pclass, pmemfun);
		this->m_connected_slots.push_back(conn);
		pclass->signalConnect(this);
	}

	void emit() {
		LockBlock<mt_policy> lock(this);
		const_iterator itNext, it = this->m_connected_slots.begin();
		const_iterator itEnd = this->m_connected_slots.end();

		while(it != itEnd) {
			itNext = it;
			++itNext;

			(*it)->emit();

			it = itNext;
		}
	}

	void operator()() {
		LockBlock<mt_policy> lock(this);
		const_iterator itNext, it = this->m_connected_slots.begin();
		const_iterator itEnd = this->m_connected_slots.end();

		while(it != itEnd) {
			itNext = it;
			++itNext;

			(*it)->emit();

			it = itNext;
		}
	}
};

template<class arg1_type, class mt_policy = SIGLY_DEFAULT_MT_POLICY>
class Signal1 : public _signal_base1<arg1_type, mt_policy> {
public:
	typedef typename _signal_base1<arg1_type, mt_policy>::connections_list::const_iterator const_iterator;
	Signal1() {
	}

	Signal1(const Signal1<arg1_type, mt_policy>& s)
		: _signal_base1<arg1_type, mt_policy>(s) {
	}

	template<class desttype>
	void connect(desttype* pclass, void (desttype::*pmemfun)(arg1_type)) {
		LockBlock<mt_policy> lock(this);
		_connection1<desttype, arg1_type, mt_policy>* conn =
		    new _connection1<desttype, arg1_type, mt_policy>(pclass, pmemfun);
		this->m_connected_slots.push_back(conn);
		pclass->signalConnect(this);
	}

	void emit(arg1_type a1) {
		LockBlock<mt_policy> lock(this);
		const_iterator itNext, it = this->m_connected_slots.begin();
		const_iterator itEnd = this->m_connected_slots.end();

		while(it != itEnd) {
			itNext = it;
			++itNext;

			(*it)->emit(a1);

			it = itNext;
		}
	}

	void operator()(arg1_type a1) {
		LockBlock<mt_policy> lock(this);
		const_iterator itNext, it = this->m_connected_slots.begin();
		const_iterator itEnd = this->m_connected_slots.end();

		while(it != itEnd) {
			itNext = it;
			++itNext;

			(*it)->emit(a1);

			it = itNext;
		}
	}
};

template<class arg1_type, typename arg2_type, typename mt_policy = SIGLY_DEFAULT_MT_POLICY>
class Signal2 : public _signal_base2<arg1_type, arg2_type, mt_policy> {
public:
	typedef typename _signal_base2<arg1_type, arg2_type, mt_policy>::connections_list::const_iterator const_iterator;
	Signal2() {
	}

	Signal2(const Signal2<arg1_type, arg2_type, mt_policy>& s)
		: _signal_base2<arg1_type, arg2_type, mt_policy>(s) {
	}

	template<class desttype>
	void connect(desttype* pclass, void (desttype::*pmemfun)(arg1_type,
	             arg2_type)) {
		LockBlock<mt_policy> lock(this);
		_connection2<desttype, arg1_type, arg2_type, mt_policy>* conn = new
		_connection2<desttype, arg1_type, arg2_type, mt_policy>(pclass, pmemfun);
		this->m_connected_slots.push_back(conn);
		pclass->signalConnect(this);
	}

	void emit(arg1_type a1, arg2_type a2) {
		LockBlock<mt_policy> lock(this);
		const_iterator itNext, it = this->m_connected_slots.begin();
		const_iterator itEnd = this->m_connected_slots.end();

		while(it != itEnd) {
			itNext = it;
			++itNext;

			(*it)->emit(a1, a2);

			it = itNext;
		}
	}

	void operator()(arg1_type a1, arg2_type a2) {
		LockBlock<mt_policy> lock(this);
		const_iterator itNext, it = this->m_connected_slots.begin();
		const_iterator itEnd = this->m_connected_slots.end();

		while(it != itEnd) {
			itNext = it;
			++itNext;

			(*it)->emit(a1, a2);

			it = itNext;
		}
	}
};

template<class arg1_type, typename arg2_type, typename arg3_type, typename mt_policy = SIGLY_DEFAULT_MT_POLICY>
class Signal3 : public _signal_base3<arg1_type, arg2_type, arg3_type, mt_policy> {
public:
	typedef typename _signal_base3<arg1_type, arg2_type, arg3_type, mt_policy>::connections_list::const_iterator const_iterator;
	Signal3() {
	}

	Signal3(const Signal3<arg1_type, arg2_type, arg3_type, mt_policy>& s)
		: _signal_base3<arg1_type, arg2_type, arg3_type, mt_policy>(s) {
	}

	template<class desttype>
	void connect(desttype* pclass, void (desttype::*pmemfun)(arg1_type,
	             arg2_type, arg3_type)) {
		LockBlock<mt_policy> lock(this);
		_connection3<desttype, arg1_type, arg2_type, arg3_type, mt_policy>* conn =
		    new _connection3<desttype, arg1_type, arg2_type, arg3_type, mt_policy>(pclass,
		            pmemfun);
		this->m_connected_slots.push_back(conn);
		pclass->signalConnect(this);
	}

	void emit(arg1_type a1, arg2_type a2, arg3_type a3) {
		LockBlock<mt_policy> lock(this);
		const_iterator itNext, it = this->m_connected_slots.begin();
		const_iterator itEnd = this->m_connected_slots.end();

		while(it != itEnd) {
			itNext = it;
			++itNext;

			(*it)->emit(a1, a2, a3);

			it = itNext;
		}
	}

	void operator()(arg1_type a1, arg2_type a2, arg3_type a3) {
		LockBlock<mt_policy> lock(this);
		const_iterator itNext, it = this->m_connected_slots.begin();
		const_iterator itEnd = this->m_connected_slots.end();

		while(it != itEnd) {
			itNext = it;
			++itNext;

			(*it)->emit(a1, a2, a3);

			it = itNext;
		}
	}
};

template<class arg1_type, class arg2_type, class arg3_type, class arg4_type, class mt_policy = SIGLY_DEFAULT_MT_POLICY>
class Signal4 : public _signal_base4 < arg1_type, arg2_type, arg3_type,
		arg4_type, mt_policy > {
public:
	typedef typename _signal_base4<arg1_type, arg2_type, arg3_type, arg4_type, mt_policy>::connections_list::const_iterator const_iterator;
	Signal4() {
	}

	Signal4(const Signal4<arg1_type, arg2_type, arg3_type, arg4_type, mt_policy>& s)
		: _signal_base4<arg1_type, arg2_type, arg3_type, arg4_type, mt_policy>(s) {
	}

	template<class desttype>
	void connect(desttype* pclass, void (desttype::*pmemfun)(arg1_type,
	             arg2_type, arg3_type, arg4_type)) {
		LockBlock<mt_policy> lock(this);
		_connection4<desttype, arg1_type, arg2_type, arg3_type, arg4_type, mt_policy>*
		conn = new _connection4 < desttype, arg1_type, arg2_type, arg3_type,
		arg4_type, mt_policy > (pclass, pmemfun);
		this->m_connected_slots.push_back(conn);
		pclass->signalConnect(this);
	}

	void emit(arg1_type a1, arg2_type a2, arg3_type a3, arg4_type a4) {
		LockBlock<mt_policy> lock(this);
		const_iterator itNext, it = this->m_connected_slots.begin();
		const_iterator itEnd = this->m_connected_slots.end();

		while(it != itEnd) {
			itNext = it;
			++itNext;

			(*it)->emit(a1, a2, a3, a4);

			it = itNext;
		}
	}

	void operator()(arg1_type a1, arg2_type a2, arg3_type a3, arg4_type a4) {
		LockBlock<mt_policy> lock(this);
		const_iterator itNext, it = this->m_connected_slots.begin();
		const_iterator itEnd = this->m_connected_slots.end();

		while(it != itEnd) {
			itNext = it;
			++itNext;

			(*it)->emit(a1, a2, a3, a4);

			it = itNext;
		}
	}
};

template < class arg1_type, class arg2_type, class arg3_type, class arg4_type,
         class arg5_type, class mt_policy = SIGLY_DEFAULT_MT_POLICY >
class Signal5 : public _signal_base5 < arg1_type, arg2_type, arg3_type,
		arg4_type, arg5_type, mt_policy > {
public:
	typedef typename _signal_base5<arg1_type, arg2_type, arg3_type, arg4_type, arg5_type, mt_policy>::connections_list::const_iterator const_iterator;
	Signal5() {
	}

	Signal5(const Signal5 < arg1_type, arg2_type, arg3_type, arg4_type,
	        arg5_type, mt_policy > & s)
		: _signal_base5 < arg1_type, arg2_type, arg3_type, arg4_type,
		  arg5_type, mt_policy > (s) {
	}

	template<class desttype>
	void connect(desttype* pclass, void (desttype::*pmemfun)(arg1_type,
	             arg2_type, arg3_type, arg4_type, arg5_type)) {
		LockBlock<mt_policy> lock(this);
		_connection5 < desttype, arg1_type, arg2_type, arg3_type, arg4_type,
		             arg5_type, mt_policy > * conn = new _connection5 < desttype, arg1_type, arg2_type,
		arg3_type, arg4_type, arg5_type, mt_policy > (pclass, pmemfun);
		this->m_connected_slots.push_back(conn);
		pclass->signalConnect(this);
	}

	void emit(arg1_type a1, arg2_type a2, arg3_type a3, arg4_type a4,
	          arg5_type a5) {
		LockBlock<mt_policy> lock(this);
		const_iterator itNext, it = this->m_connected_slots.begin();
		const_iterator itEnd = this->m_connected_slots.end();

		while(it != itEnd) {
			itNext = it;
			++itNext;

			(*it)->emit(a1, a2, a3, a4, a5);

			it = itNext;
		}
	}

	void operator()(arg1_type a1, arg2_type a2, arg3_type a3, arg4_type a4,
	                arg5_type a5) {
		LockBlock<mt_policy> lock(this);
		const_iterator itNext, it = this->m_connected_slots.begin();
		const_iterator itEnd = this->m_connected_slots.end();

		while(it != itEnd) {
			itNext = it;
			++itNext;

			(*it)->emit(a1, a2, a3, a4, a5);

			it = itNext;
		}
	}
};


template < class arg1_type, class arg2_type, class arg3_type, class arg4_type,
         class arg5_type, class arg6_type, class mt_policy = SIGLY_DEFAULT_MT_POLICY >
class Signal6 : public _signal_base6 < arg1_type, arg2_type, arg3_type,
		arg4_type, arg5_type, arg6_type, mt_policy > {
public:
	typedef typename _signal_base6 < arg1_type, arg2_type, arg3_type,
	        arg4_type, arg5_type, arg6_type, mt_policy >::connections_list::const_iterator const_iterator;
	Signal6() {
	}

	Signal6(const Signal6 < arg1_type, arg2_type, arg3_type, arg4_type,
	        arg5_type, arg6_type, mt_policy > & s)
		: _signal_base6 < arg1_type, arg2_type, arg3_type, arg4_type,
		  arg5_type, arg6_type, mt_policy > (s) {
	}

	template<class desttype>
	void connect(desttype* pclass, void (desttype::*pmemfun)(arg1_type,
	             arg2_type, arg3_type, arg4_type, arg5_type, arg6_type)) {
		LockBlock<mt_policy> lock(this);
		_connection6 < desttype, arg1_type, arg2_type, arg3_type, arg4_type,
		             arg5_type, arg6_type, mt_policy > * conn =
		                 new _connection6 < desttype, arg1_type, arg2_type, arg3_type,
		arg4_type, arg5_type, arg6_type, mt_policy > (pclass, pmemfun);
		this->m_connected_slots.push_back(conn);
		pclass->signalConnect(this);
	}

	void emit(arg1_type a1, arg2_type a2, arg3_type a3, arg4_type a4,
	          arg5_type a5, arg6_type a6) {
		LockBlock<mt_policy> lock(this);
		const_iterator itNext, it = this->m_connected_slots.begin();
		const_iterator itEnd = this->m_connected_slots.end();

		while(it != itEnd) {
			itNext = it;
			++itNext;

			(*it)->emit(a1, a2, a3, a4, a5, a6);

			it = itNext;
		}
	}

	void operator()(arg1_type a1, arg2_type a2, arg3_type a3, arg4_type a4,
	                arg5_type a5, arg6_type a6) {
		LockBlock<mt_policy> lock(this);
		const_iterator itNext, it = this->m_connected_slots.begin();
		const_iterator itEnd = this->m_connected_slots.end();

		while(it != itEnd) {
			itNext = it;
			++itNext;

			(*it)->emit(a1, a2, a3, a4, a5, a6);

			it = itNext;
		}
	}
};

template < class arg1_type, class arg2_type, class arg3_type, class arg4_type,
         class arg5_type, class arg6_type, class arg7_type, class mt_policy = SIGLY_DEFAULT_MT_POLICY >
class Signal7 : public _signal_base7 < arg1_type, arg2_type, arg3_type,
		arg4_type, arg5_type, arg6_type, arg7_type, mt_policy > {
public:
	typedef typename _signal_base7 < arg1_type, arg2_type, arg3_type,
	        arg4_type, arg5_type, arg6_type, arg7_type, mt_policy >::connections_list::const_iterator const_iterator;
	Signal7() {
	}

	Signal7(const Signal7 < arg1_type, arg2_type, arg3_type, arg4_type,
	        arg5_type, arg6_type, arg7_type, mt_policy > & s)
		: _signal_base7 < arg1_type, arg2_type, arg3_type, arg4_type,
		  arg5_type, arg6_type, arg7_type, mt_policy > (s) {
	}

	template<class desttype>
	void connect(desttype* pclass, void (desttype::*pmemfun)(arg1_type,
	             arg2_type, arg3_type, arg4_type, arg5_type, arg6_type,
	             arg7_type)) {
		LockBlock<mt_policy> lock(this);
		_connection7 < desttype, arg1_type, arg2_type, arg3_type, arg4_type,
		             arg5_type, arg6_type, arg7_type, mt_policy > * conn =
		                 new _connection7 < desttype, arg1_type, arg2_type, arg3_type,
		arg4_type, arg5_type, arg6_type, arg7_type, mt_policy > (pclass, pmemfun);
		this->m_connected_slots.push_back(conn);
		pclass->signalConnect(this);
	}

	void emit(arg1_type a1, arg2_type a2, arg3_type a3, arg4_type a4,
	          arg5_type a5, arg6_type a6, arg7_type a7) {
		LockBlock<mt_policy> lock(this);
		const_iterator itNext, it = this->m_connected_slots.begin();
		const_iterator itEnd = this->m_connected_slots.end();

		while(it != itEnd) {
			itNext = it;
			++itNext;

			(*it)->emit(a1, a2, a3, a4, a5, a6, a7);

			it = itNext;
		}
	}

	void operator()(arg1_type a1, arg2_type a2, arg3_type a3, arg4_type a4,
	                arg5_type a5, arg6_type a6, arg7_type a7) {
		LockBlock<mt_policy> lock(this);
		const_iterator itNext, it = this->m_connected_slots.begin();
		const_iterator itEnd = this->m_connected_slots.end();

		while(it != itEnd) {
			itNext = it;
			++itNext;

			(*it)->emit(a1, a2, a3, a4, a5, a6, a7);

			it = itNext;
		}
	}
};

template < class arg1_type, class arg2_type, class arg3_type, class arg4_type,
         class arg5_type, class arg6_type, class arg7_type, class arg8_type, class mt_policy = SIGLY_DEFAULT_MT_POLICY >
class Signal8 : public _signal_base8 < arg1_type, arg2_type, arg3_type,
		arg4_type, arg5_type, arg6_type, arg7_type, arg8_type, mt_policy > {
public:
	typedef typename _signal_base8 < arg1_type, arg2_type, arg3_type,
	        arg4_type, arg5_type, arg6_type, arg7_type, arg8_type, mt_policy >::connections_list::const_iterator const_iterator;
	Signal8() {
	}

	Signal8(const Signal8 < arg1_type, arg2_type, arg3_type, arg4_type,
	        arg5_type, arg6_type, arg7_type, arg8_type, mt_policy > & s)
		: _signal_base8 < arg1_type, arg2_type, arg3_type, arg4_type,
		  arg5_type, arg6_type, arg7_type, arg8_type, mt_policy > (s) {
	}

	template<class desttype>
	void connect(desttype* pclass, void (desttype::*pmemfun)(arg1_type,
	             arg2_type, arg3_type, arg4_type, arg5_type, arg6_type,
	             arg7_type, arg8_type)) {
		LockBlock<mt_policy> lock(this);
		_connection8 < desttype, arg1_type, arg2_type, arg3_type, arg4_type,
		             arg5_type, arg6_type, arg7_type, arg8_type, mt_policy > * conn =
		                 new _connection8 < desttype, arg1_type, arg2_type, arg3_type,
		arg4_type, arg5_type, arg6_type, arg7_type,
		arg8_type, mt_policy > (pclass, pmemfun);
		this->m_connected_slots.push_back(conn);
		pclass->signalConnect(this);
	}

	void emit(arg1_type a1, arg2_type a2, arg3_type a3, arg4_type a4,
	          arg5_type a5, arg6_type a6, arg7_type a7, arg8_type a8) {
		LockBlock<mt_policy> lock(this);
		const_iterator itNext, it = this->m_connected_slots.begin();
		const_iterator itEnd = this->m_connected_slots.end();

		while(it != itEnd) {
			itNext = it;
			++itNext;

			(*it)->emit(a1, a2, a3, a4, a5, a6, a7, a8);

			it = itNext;
		}
	}

	void operator()(arg1_type a1, arg2_type a2, arg3_type a3, arg4_type a4,
	                arg5_type a5, arg6_type a6, arg7_type a7, arg8_type a8) {
		LockBlock<mt_policy> lock(this);
		const_iterator itNext, it = this->m_connected_slots.begin();
		const_iterator itEnd = this->m_connected_slots.end();

		while(it != itEnd) {
			itNext = it;
			++itNext;

			(*it)->emit(a1, a2, a3, a4, a5, a6, a7, a8);

			it = itNext;
		}
	}
};
}

#endif // SIGLY_SIGLY_SIGNAL_N_H