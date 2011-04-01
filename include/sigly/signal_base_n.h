#ifndef SIGLY_SIGLY_SIGNAL_BASE_N_H_
#define SIGLY_SIGLY_SIGNAL_BASE_N_H

#include <list>

#include <sigly/lock_block.h>
#include <sigly/signal_base.h>
#include <sigly/connection_base_n.h>
#include <sigly/has_slots.h>

namespace sigly {
template<class mt_policy>
class _signal_base0 : public _signal_base<mt_policy> {
public:
	typedef typename std::list<_connection_base0<mt_policy> *>  connections_list;
	typedef typename connections_list::const_iterator const_iterator;
	typedef typename connections_list::iterator iterator;

	_signal_base0() {
	}

	_signal_base0(const _signal_base0& s)
		: _signal_base<mt_policy>(s) {
		LockBlock<mt_policy> lock(this);
		const_iterator  it = s.m_connected_slots.begin();
		const_iterator itEnd = s.m_connected_slots.end();

		while(it != itEnd) {
			(*it)->getDest()->signalConnect(this);
			m_connected_slots.push_back((*it)->clone());

			++it;
		}
	}

	~_signal_base0() {
		disconnectAll();
	}

	void disconnectAll() {
		LockBlock<mt_policy> lock(this);
		const_iterator it  = m_connected_slots.begin();
		const_iterator itEnd = m_connected_slots.end();

		while(it != itEnd) {
			(*it)->getDest()->signalDisconnect(this);
			delete *it;

			++it;
		}

		m_connected_slots.erase(m_connected_slots.begin(), m_connected_slots.end());
	}

	void disconnect(HasSlots<mt_policy>* pclass) {
		LockBlock<mt_policy> lock(this);
		iterator it = m_connected_slots.begin();
		iterator itEnd = m_connected_slots.end();

		while(it != itEnd) {
			if((*it)->getDest() == pclass) {
				delete *it;
				m_connected_slots.erase(iterator(it++));
				pclass->signalDisconnect(this);
				continue;
			}

			++it;
		}
	}

	void slotDisconnect(HasSlots<mt_policy>* pslot) {
		LockBlock<mt_policy> lock(this);
		iterator it = m_connected_slots.begin();
		iterator itEnd = m_connected_slots.end();

		while(it != itEnd) {
			iterator itNext = it;
			++itNext;

			if((*it)->getDest() == pslot) {
				delete *it;
				m_connected_slots.erase(it);
			}

			it = itNext;
		}
	}

	void slotDuplicate(const HasSlots<mt_policy>* oldtarget, HasSlots<mt_policy>* newtarget) {
		LockBlock<mt_policy> lock(this);
		iterator it = m_connected_slots.begin();
		iterator itEnd = m_connected_slots.end();

		while(it != itEnd) {
			if((*it)->getDest() == oldtarget) {
				m_connected_slots.push_back((*it)->duplicate(newtarget));
			}

			++it;
		}
	}

protected:
	connections_list m_connected_slots;
};

template<class arg1_type, class mt_policy>
class _signal_base1 : public _signal_base<mt_policy> {
public:
	typedef typename std::list<_connection_base1<arg1_type, mt_policy> *>  connections_list;
	typedef typename connections_list::const_iterator const_iterator;
	typedef typename connections_list::iterator iterator;

	_signal_base1() {
	}

	_signal_base1(const _signal_base1<arg1_type, mt_policy>& s)
		: _signal_base<mt_policy>(s) {
		LockBlock<mt_policy> lock(this);
		const_iterator it = s.m_connected_slots.begin();
		const_iterator itEnd = s.m_connected_slots.end();

		while(it != itEnd) {
			(*it)->getDest()->signalConnect(this);
			m_connected_slots.push_back((*it)->clone());

			++it;
		}
	}

	void slotDuplicate(const HasSlots<mt_policy>* oldtarget, HasSlots<mt_policy>* newtarget) {
		LockBlock<mt_policy> lock(this);
		iterator it = m_connected_slots.begin();
		iterator itEnd = m_connected_slots.end();

		while(it != itEnd) {
			if((*it)->getDest() == oldtarget) {
				m_connected_slots.push_back((*it)->duplicate(newtarget));
			}

			++it;
		}
	}

	~_signal_base1() {
		disconnectAll();
	}

	void disconnectAll() {
		LockBlock<mt_policy> lock(this);
		const_iterator it = m_connected_slots.begin();
		const_iterator itEnd = m_connected_slots.end();

		while(it != itEnd) {
			(*it)->getDest()->signalDisconnect(this);
			delete *it;

			++it;
		}

		m_connected_slots.erase(m_connected_slots.begin(), m_connected_slots.end());
	}

	void disconnect(HasSlots<mt_policy>* pclass) {
		LockBlock<mt_policy> lock(this);
		iterator it = m_connected_slots.begin();
		iterator itEnd = m_connected_slots.end();

		while(it != itEnd) {
			if((*it)->getDest() == pclass) {
				delete *it;
				m_connected_slots.erase(iterator(it++));
				pclass->signalDisconnect(this);
				continue;
			}

			++it;
		}
	}

	void slotDisconnect(HasSlots<mt_policy>* pslot) {
		LockBlock<mt_policy> lock(this);
		iterator it = m_connected_slots.begin();
		iterator itEnd = m_connected_slots.end();

		while(it != itEnd) {
			iterator itNext = it;
			++itNext;

			if((*it)->getDest() == pslot) {
				delete *it;
				m_connected_slots.erase(it);
			}

			it = itNext;
		}
	}


protected:
	connections_list m_connected_slots;
};

template<class arg1_type, class arg2_type, class mt_policy>
class _signal_base2 : public _signal_base<mt_policy> {
public:
	typedef typename std::list<_connection_base2<arg1_type, arg2_type, mt_policy> *>
	connections_list;
	typedef typename connections_list::const_iterator const_iterator;
	typedef typename connections_list::iterator iterator;

	_signal_base2() {
	}

	_signal_base2(const _signal_base2<arg1_type, arg2_type, mt_policy>& s)
		: _signal_base<mt_policy>(s) {
		LockBlock<mt_policy> lock(this);
		const_iterator it = s.m_connected_slots.begin();
		const_iterator itEnd = s.m_connected_slots.end();

		while(it != itEnd) {
			(*it)->getDest()->signalConnect(this);
			m_connected_slots.push_back((*it)->clone());

			++it;
		}
	}

	void slotDuplicate(const HasSlots<mt_policy>* oldtarget, HasSlots<mt_policy>* newtarget) {
		LockBlock<mt_policy> lock(this);
		iterator it = m_connected_slots.begin();
		iterator itEnd = m_connected_slots.end();

		while(it != itEnd) {
			if((*it)->getDest() == oldtarget) {
				m_connected_slots.push_back((*it)->duplicate(newtarget));
			}

			++it;
		}
	}

	~_signal_base2() {
		disconnectAll();
	}

	void disconnectAll() {
		LockBlock<mt_policy> lock(this);
		const_iterator it = m_connected_slots.begin();
		const_iterator itEnd = m_connected_slots.end();

		while(it != itEnd) {
			(*it)->getDest()->signalDisconnect(this);
			delete *it;

			++it;
		}

		m_connected_slots.erase(m_connected_slots.begin(), m_connected_slots.end());
	}

	void disconnect(HasSlots<mt_policy>* pclass) {
		LockBlock<mt_policy> lock(this);
		iterator it = m_connected_slots.begin();
		iterator itEnd = m_connected_slots.end();

		while(it != itEnd) {
			if((*it)->getDest() == pclass) {
				delete *it;
				m_connected_slots.erase(iterator(it++));
				pclass->signalDisconnect(this);
				continue;
			}

			++it;
		}
	}

	void slotDisconnect(HasSlots<mt_policy>* pslot) {
		LockBlock<mt_policy> lock(this);
		iterator it = m_connected_slots.begin();
		iterator itEnd = m_connected_slots.end();

		while(it != itEnd) {
			iterator itNext = it;
			++itNext;

			if((*it)->getDest() == pslot) {
				delete *it;
				m_connected_slots.erase(it);
			}

			it = itNext;
		}
	}

protected:
	connections_list m_connected_slots;
};

template<class arg1_type, class arg2_type, class arg3_type, class mt_policy>
class _signal_base3 : public _signal_base<mt_policy> {
public:
	typedef std::list<_connection_base3<arg1_type, arg2_type, arg3_type, mt_policy> *>
	connections_list;

	typedef typename connections_list::const_iterator const_iterator;
	typedef typename connections_list::iterator iterator;
	_signal_base3() {
	}

	_signal_base3(const _signal_base3<arg1_type, arg2_type, arg3_type, mt_policy>& s)
		: _signal_base<mt_policy>(s) {
		LockBlock<mt_policy> lock(this);
		const_iterator it = s.m_connected_slots.begin();
		const_iterator itEnd = s.m_connected_slots.end();

		while(it != itEnd) {
			(*it)->getDest()->signalConnect(this);
			m_connected_slots.push_back((*it)->clone());

			++it;
		}
	}

	void slotDuplicate(const HasSlots<mt_policy>* oldtarget, HasSlots<mt_policy>* newtarget) {
		LockBlock<mt_policy> lock(this);
		iterator it = m_connected_slots.begin();
		iterator itEnd = m_connected_slots.end();

		while(it != itEnd) {
			if((*it)->getDest() == oldtarget) {
				m_connected_slots.push_back((*it)->duplicate(newtarget));
			}

			++it;
		}
	}

	~_signal_base3() {
		disconnectAll();
	}

	void disconnectAll() {
		LockBlock<mt_policy> lock(this);
		const_iterator it = m_connected_slots.begin();
		const_iterator itEnd = m_connected_slots.end();

		while(it != itEnd) {
			(*it)->getDest()->signalDisconnect(this);
			delete *it;

			++it;
		}

		m_connected_slots.erase(m_connected_slots.begin(), m_connected_slots.end());
	}

	void disconnect(HasSlots<mt_policy>* pclass) {
		LockBlock<mt_policy> lock(this);
		iterator it = m_connected_slots.begin();
		iterator itEnd = m_connected_slots.end();

		while(it != itEnd) {
			if((*it)->getDest() == pclass) {
				delete *it;
				m_connected_slots.erase(iterator(it++));
				pclass->signalDisconnect(this);
				continue;
			}

			++it;
		}
	}

	void slotDisconnect(HasSlots<mt_policy>* pslot) {
		LockBlock<mt_policy> lock(this);
		iterator it = m_connected_slots.begin();
		iterator itEnd = m_connected_slots.end();

		while(it != itEnd) {
			iterator itNext = it;
			++itNext;

			if((*it)->getDest() == pslot) {
				delete *it;
				m_connected_slots.erase(it);
			}

			it = itNext;
		}
	}

protected:
	connections_list m_connected_slots;
};

template<class arg1_type, class arg2_type, class arg3_type, class arg4_type, class mt_policy>
class _signal_base4 : public _signal_base<mt_policy> {
public:
	typedef std::list < _connection_base4 < arg1_type, arg2_type, arg3_type,
	        arg4_type, mt_policy > * >  connections_list;
	typedef typename connections_list::const_iterator const_iterator;
	typedef typename connections_list::iterator iterator;

	_signal_base4() {
	}

	_signal_base4(const _signal_base4<arg1_type, arg2_type, arg3_type, arg4_type, mt_policy>& s)
		: _signal_base<mt_policy>(s) {
		LockBlock<mt_policy> lock(this);
		const_iterator it = s.m_connected_slots.begin();
		const_iterator itEnd = s.m_connected_slots.end();

		while(it != itEnd) {
			(*it)->getDest()->signalConnect(this);
			m_connected_slots.push_back((*it)->clone());

			++it;
		}
	}

	void slotDuplicate(const HasSlots<mt_policy>* oldtarget, HasSlots<mt_policy>* newtarget) {
		LockBlock<mt_policy> lock(this);
		iterator it = m_connected_slots.begin();
		iterator itEnd = m_connected_slots.end();

		while(it != itEnd) {
			if((*it)->getDest() == oldtarget) {
				m_connected_slots.push_back((*it)->duplicate(newtarget));
			}

			++it;
		}
	}

	~_signal_base4() {
		disconnectAll();
	}

	void disconnectAll() {
		LockBlock<mt_policy> lock(this);
		const_iterator it = m_connected_slots.begin();
		const_iterator itEnd = m_connected_slots.end();

		while(it != itEnd) {
			(*it)->getDest()->signalDisconnect(this);
			delete *it;

			++it;
		}

		m_connected_slots.erase(m_connected_slots.begin(), m_connected_slots.end());
	}

	void disconnect(HasSlots<mt_policy>* pclass) {
		LockBlock<mt_policy> lock(this);
		iterator it = m_connected_slots.begin();
		iterator itEnd = m_connected_slots.end();

		while(it != itEnd) {
			if((*it)->getDest() == pclass) {
				delete *it;
				this->m_connected_slots.erase(iterator(it++));
				pclass->signalDisconnect(this);
				continue;
			}

			++it;
		}
	}

	void slotDisconnect(HasSlots<mt_policy>* pslot) {
		LockBlock<mt_policy> lock(this);
		iterator it = m_connected_slots.begin();
		iterator itEnd = m_connected_slots.end();

		while(it != itEnd) {
			iterator itNext = it;
			++itNext;

			if((*it)->getDest() == pslot) {
				delete *it;
				m_connected_slots.erase(it);
			}

			it = itNext;
		}
	}

protected:
	connections_list m_connected_slots;
};

template < class arg1_type, class arg2_type, class arg3_type, class arg4_type,
         class arg5_type, class mt_policy >
class _signal_base5 : public _signal_base<mt_policy> {
public:
	typedef std::list < _connection_base5 < arg1_type, arg2_type, arg3_type,
	        arg4_type, arg5_type, mt_policy > * >  connections_list;
	typedef typename connections_list::const_iterator const_iterator;
	typedef typename connections_list::iterator iterator;

	_signal_base5() {
	}

	_signal_base5(const _signal_base5 < arg1_type, arg2_type, arg3_type, arg4_type,
	              arg5_type, mt_policy > & s)
		: _signal_base<mt_policy>(s) {
		LockBlock<mt_policy> lock(this);
		const_iterator it = s.m_connected_slots.begin();
		const_iterator itEnd = s.m_connected_slots.end();

		while(it != itEnd) {
			(*it)->getDest()->signalConnect(this);
			m_connected_slots.push_back((*it)->clone());

			++it;
		}
	}

	void slotDuplicate(const HasSlots<mt_policy>* oldtarget, HasSlots<mt_policy>* newtarget) {
		LockBlock<mt_policy> lock(this);
		iterator it = m_connected_slots.begin();
		iterator itEnd = m_connected_slots.end();

		while(it != itEnd) {
			if((*it)->getDest() == oldtarget) {
				m_connected_slots.push_back((*it)->duplicate(newtarget));
			}

			++it;
		}
	}

	~_signal_base5() {
		disconnectAll();
	}

	void disconnectAll() {
		LockBlock<mt_policy> lock(this);
		const_iterator it = m_connected_slots.begin();
		const_iterator itEnd = m_connected_slots.end();

		while(it != itEnd) {
			(*it)->getDest()->signalDisconnect(this);
			delete *it;

			++it;
		}

		m_connected_slots.erase(m_connected_slots.begin(), m_connected_slots.end());
	}

	void disconnect(HasSlots<mt_policy>* pclass) {
		LockBlock<mt_policy> lock(this);
		iterator it = m_connected_slots.begin();
		iterator itEnd = m_connected_slots.end();

		while(it != itEnd) {
			if((*it)->getDest() == pclass) {
				delete *it;
				m_connected_slots.erase(iterator(it++));
				pclass->signalDisconnect(this);
				continue;
			}

			++it;
		}
	}

	void slotDisconnect(HasSlots<mt_policy>* pslot) {
		LockBlock<mt_policy> lock(this);
		iterator it = m_connected_slots.begin();
		iterator itEnd = m_connected_slots.end();

		while(it != itEnd) {
			iterator itNext = it;
			++itNext;

			if((*it)->getDest() == pslot) {
				delete *it;
				m_connected_slots.erase(it);
			}

			it = itNext;
		}
	}

protected:
	connections_list m_connected_slots;
};

template < class arg1_type, class arg2_type, class arg3_type, class arg4_type,
         class arg5_type, class arg6_type, class mt_policy >
class _signal_base6 : public _signal_base<mt_policy> {
public:
	typedef std::list < _connection_base6 < arg1_type, arg2_type, arg3_type,
	        arg4_type, arg5_type, arg6_type, mt_policy > * >  connections_list;
	typedef typename connections_list::const_iterator const_iterator;
	typedef typename connections_list::iterator iterator;

	_signal_base6() {
	}

	_signal_base6(const _signal_base6 < arg1_type, arg2_type, arg3_type, arg4_type,
	              arg5_type, arg6_type, mt_policy > & s)
		: _signal_base<mt_policy>(s) {
		LockBlock<mt_policy> lock(this);
		const_iterator it = s.m_connected_slots.begin();
		const_iterator itEnd = s.m_connected_slots.end();

		while(it != itEnd) {
			(*it)->getDest()->signalConnect(this);
			m_connected_slots.push_back((*it)->clone());

			++it;
		}
	}

	void slotDuplicate(const HasSlots<mt_policy>* oldtarget, HasSlots<mt_policy>* newtarget) {
		LockBlock<mt_policy> lock(this);
		iterator it = m_connected_slots.begin();
		iterator itEnd = m_connected_slots.end();

		while(it != itEnd) {
			if((*it)->getDest() == oldtarget) {
				m_connected_slots.push_back((*it)->duplicate(newtarget));
			}

			++it;
		}
	}

	~_signal_base6() {
		disconnectAll();
	}

	void disconnectAll() {
		LockBlock<mt_policy> lock(this);
		const_iterator it = m_connected_slots.begin();
		const_iterator itEnd = m_connected_slots.end();

		while(it != itEnd) {
			(*it)->getDest()->signalDisconnect(this);
			delete *it;

			++it;
		}

		m_connected_slots.erase(m_connected_slots.begin(), m_connected_slots.end());
	}

	void disconnect(HasSlots<mt_policy>* pclass) {
		LockBlock<mt_policy> lock(this);
		iterator it = m_connected_slots.begin();
		iterator itEnd = m_connected_slots.end();

		while(it != itEnd) {
			if((*it)->getDest() == pclass) {
				delete *it;
				m_connected_slots.erase(iterator(it++));
				pclass->signalDisconnect(this);
				continue;
			}

			++it;
		}
	}

	void slotDisconnect(HasSlots<mt_policy>* pslot) {
		LockBlock<mt_policy> lock(this);
		iterator it = m_connected_slots.begin();
		iterator itEnd = m_connected_slots.end();

		while(it != itEnd) {
			iterator itNext = it;
			++itNext;

			if((*it)->getDest() == pslot) {
				delete *it;
				m_connected_slots.erase(it);
			}

			it = itNext;
		}
	}

protected:
	connections_list m_connected_slots;
};

template < class arg1_type, class arg2_type, class arg3_type, class arg4_type,
         class arg5_type, class arg6_type, class arg7_type, class mt_policy >
class _signal_base7 : public _signal_base<mt_policy> {
public:
	typedef std::list < _connection_base7 < arg1_type, arg2_type, arg3_type,
	        arg4_type, arg5_type, arg6_type, arg7_type, mt_policy > * >  connections_list;
	typedef typename connections_list::const_iterator const_iterator;
	typedef typename connections_list::iterator iterator;

	_signal_base7() {
	}

	_signal_base7(const _signal_base7 < arg1_type, arg2_type, arg3_type, arg4_type,
	              arg5_type, arg6_type, arg7_type, mt_policy > & s)
		: _signal_base<mt_policy>(s) {
		LockBlock<mt_policy> lock(this);
		const_iterator it = s.m_connected_slots.begin();
		const_iterator itEnd = s.m_connected_slots.end();

		while(it != itEnd) {
			(*it)->getDest()->signalConnect(this);
			m_connected_slots.push_back((*it)->clone());

			++it;
		}
	}

	void slotDuplicate(const HasSlots<mt_policy>* oldtarget, HasSlots<mt_policy>* newtarget) {
		LockBlock<mt_policy> lock(this);
		iterator it = m_connected_slots.begin();
		iterator itEnd = m_connected_slots.end();

		while(it != itEnd) {
			if((*it)->getDest() == oldtarget) {
				m_connected_slots.push_back((*it)->duplicate(newtarget));
			}

			++it;
		}
	}

	~_signal_base7() {
		disconnectAll();
	}

	void disconnectAll() {
		LockBlock<mt_policy> lock(this);
		const_iterator it = m_connected_slots.begin();
		const_iterator itEnd = m_connected_slots.end();

		while(it != itEnd) {
			(*it)->getDest()->signalDisconnect(this);
			delete *it;

			++it;
		}

		m_connected_slots.erase(m_connected_slots.begin(), m_connected_slots.end());
	}

	void disconnect(HasSlots<mt_policy>* pclass) {
		LockBlock<mt_policy> lock(this);
		iterator it = m_connected_slots.begin();
		iterator itEnd = m_connected_slots.end();

		while(it != itEnd) {
			if((*it)->getDest() == pclass) {
				delete *it;
				m_connected_slots.erase(iterator(it++));
				pclass->signalDisconnect(this);
				continue;
			}

			++it;
		}
	}

	void slotDisconnect(HasSlots<mt_policy>* pslot) {
		LockBlock<mt_policy> lock(this);
		iterator it = m_connected_slots.begin();
		iterator itEnd = m_connected_slots.end();

		while(it != itEnd) {
			iterator itNext = it;
			++itNext;

			if((*it)->getDest() == pslot) {
				delete *it;
				m_connected_slots.erase(it);
			}

			it = itNext;
		}
	}

protected:
	connections_list m_connected_slots;
};

template < class arg1_type, class arg2_type, class arg3_type, class arg4_type,
         class arg5_type, class arg6_type, class arg7_type, class arg8_type, class mt_policy >
class _signal_base8 : public _signal_base<mt_policy> {
public:
	typedef std::list < _connection_base8 < arg1_type, arg2_type, arg3_type,
	        arg4_type, arg5_type, arg6_type, arg7_type, arg8_type, mt_policy > * >
	        connections_list;
	typedef typename connections_list::const_iterator const_iterator;
	typedef typename connections_list::iterator iterator;

	_signal_base8() {
	}

	_signal_base8(const _signal_base8 < arg1_type, arg2_type, arg3_type, arg4_type,
	              arg5_type, arg6_type, arg7_type, arg8_type, mt_policy > & s)
		: _signal_base<mt_policy>(s) {
		LockBlock<mt_policy> lock(this);
		const_iterator it = s.m_connected_slots.begin();
		const_iterator itEnd = s.m_connected_slots.end();

		while(it != itEnd) {
			(*it)->getDest()->signalConnect(this);
			m_connected_slots.push_back((*it)->clone());

			++it;
		}
	}

	void slotDuplicate(const HasSlots<mt_policy>* oldtarget, HasSlots<mt_policy>* newtarget) {
		LockBlock<mt_policy> lock(this);
		iterator it = m_connected_slots.begin();
		iterator itEnd = m_connected_slots.end();

		while(it != itEnd) {
			if((*it)->getDest() == oldtarget) {
				m_connected_slots.push_back((*it)->duplicate(newtarget));
			}

			++it;
		}
	}

	~_signal_base8() {
		disconnectAll();
	}

	void disconnectAll() {
		LockBlock<mt_policy> lock(this);
		const_iterator it = m_connected_slots.begin();
		const_iterator itEnd = m_connected_slots.end();

		while(it != itEnd) {
			(*it)->getDest()->signalDisconnect(this);
			delete *it;

			++it;
		}

		m_connected_slots.erase(m_connected_slots.begin(), m_connected_slots.end());
	}

	void disconnect(HasSlots<mt_policy>* pclass) {
		LockBlock<mt_policy> lock(this);
		iterator it = m_connected_slots.begin();
		iterator itEnd = m_connected_slots.end();

		while(it != itEnd) {
			if((*it)->getDest() == pclass) {
				delete *it;
				m_connected_slots.erase(iterator(it++));
				pclass->signalDisconnect(this);
				continue;
			}

			++it;
		}
	}

	void slotDisconnect(HasSlots<mt_policy>* pslot) {
		LockBlock<mt_policy> lock(this);
		iterator it = m_connected_slots.begin();
		iterator itEnd = m_connected_slots.end();

		while(it != itEnd) {
			iterator itNext = it;
			++itNext;

			if((*it)->getDest() == pslot) {
				delete *it;
				m_connected_slots.erase(it);
			}

			it = itNext;
		}
	}

protected:
	connections_list m_connected_slots;
};
}

#endif // SIGLY_SIGLY_SIGNAL_BASE_N_H_