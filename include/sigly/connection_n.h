// Copyright (c) 2011 Anhero Inc.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
#ifndef SIGLY_SIGLY_CONNECTION_N_H_
#define SIGLY_SIGLY_CONNECTION_N_H_

#include <sigly/connection_base_n.h>
#include <sigly/has_slots.h>

namespace sigly {
template<class dest_type, class mt_policy>
class _connection0 : public _connection_base0<mt_policy> {
public:
	_connection0() {
		this->pobject = NULL;
		this->pmemfun = NULL;
	}

	_connection0(dest_type* pobject, void (dest_type::*pmemfun)()) {
		m_pobject = pobject;
		m_pmemfun = pmemfun;
	}

	virtual _connection_base0<mt_policy>* clone() {
		return new _connection0<dest_type, mt_policy>(*this);
	}

	virtual _connection_base0<mt_policy>* duplicate(HasSlots<mt_policy>* pnewdest) {
		return new _connection0<dest_type, mt_policy>((dest_type*)pnewdest, m_pmemfun);
	}

	virtual void emit() {
		(m_pobject->*m_pmemfun)();
	}

	virtual HasSlots<mt_policy>* getDest() const {
		return m_pobject;
	}

private:
	dest_type* m_pobject;
	void (dest_type::* m_pmemfun)();
};

template<class dest_type, class arg1_type, class mt_policy>
class _connection1 : public _connection_base1<arg1_type, mt_policy> {
public:
	_connection1() {
		this->pobject = NULL;
		this->pmemfun = NULL;
	}

	_connection1(dest_type* pobject, void (dest_type::*pmemfun)(arg1_type)) {
		m_pobject = pobject;
		m_pmemfun = pmemfun;
	}

	virtual _connection_base1<arg1_type, mt_policy>* clone() {
		return new _connection1<dest_type, arg1_type, mt_policy>(*this);
	}

	virtual _connection_base1<arg1_type, mt_policy>* duplicate(HasSlots<mt_policy>* pnewdest) {
		return new _connection1<dest_type, arg1_type, mt_policy>((dest_type*)pnewdest, m_pmemfun);
	}

	virtual void emit(arg1_type a1) {
		(m_pobject->*m_pmemfun)(a1);
	}

	virtual HasSlots<mt_policy>* getDest() const {
		return m_pobject;
	}

private:
	dest_type* m_pobject;
	void (dest_type::* m_pmemfun)(arg1_type);
};

template<class dest_type, class arg1_type, class arg2_type, class mt_policy>
class _connection2 : public _connection_base2<arg1_type, arg2_type, mt_policy> {
public:
	_connection2() {
		this->pobject = NULL;
		this->pmemfun = NULL;
	}

	_connection2(dest_type* pobject, void (dest_type::*pmemfun)(arg1_type,
	             arg2_type)) {
		m_pobject = pobject;
		m_pmemfun = pmemfun;
	}

	virtual _connection_base2<arg1_type, arg2_type, mt_policy>* clone() {
		return new _connection2<dest_type, arg1_type, arg2_type, mt_policy>(*this);
	}

	virtual _connection_base2<arg1_type, arg2_type, mt_policy>* duplicate(HasSlots<mt_policy>* pnewdest) {
		return new _connection2<dest_type, arg1_type, arg2_type, mt_policy>((dest_type*)pnewdest, m_pmemfun);
	}

	virtual void emit(arg1_type a1, arg2_type a2) {
		(m_pobject->*m_pmemfun)(a1, a2);
	}

	virtual HasSlots<mt_policy>* getDest() const {
		return m_pobject;
	}

private:
	dest_type* m_pobject;
	void (dest_type::* m_pmemfun)(arg1_type, arg2_type);
};

template<class dest_type, class arg1_type, class arg2_type, class arg3_type, class mt_policy>
class _connection3 : public _connection_base3<arg1_type, arg2_type, arg3_type, mt_policy> {
public:
	_connection3() {
		this->pobject = NULL;
		this->pmemfun = NULL;
	}

	_connection3(dest_type* pobject, void (dest_type::*pmemfun)(arg1_type,
	             arg2_type, arg3_type)) {
		m_pobject = pobject;
		m_pmemfun = pmemfun;
	}

	virtual _connection_base3<arg1_type, arg2_type, arg3_type, mt_policy>* clone() {
		return new _connection3<dest_type, arg1_type, arg2_type, arg3_type, mt_policy>(*this);
	}

	virtual _connection_base3<arg1_type, arg2_type, arg3_type, mt_policy>* duplicate(HasSlots<mt_policy>* pnewdest) {
		return new _connection3<dest_type, arg1_type, arg2_type, arg3_type, mt_policy>((dest_type*)pnewdest, m_pmemfun);
	}

	virtual void emit(arg1_type a1, arg2_type a2, arg3_type a3) {
		(m_pobject->*m_pmemfun)(a1, a2, a3);
	}

	virtual HasSlots<mt_policy>* getDest() const {
		return m_pobject;
	}

private:
	dest_type* m_pobject;
	void (dest_type::* m_pmemfun)(arg1_type, arg2_type, arg3_type);
};

template < class dest_type, class arg1_type, class arg2_type, class arg3_type,
         class arg4_type, class mt_policy >
class _connection4 : public _connection_base4 < arg1_type, arg2_type,
		arg3_type, arg4_type, mt_policy > {
public:
	_connection4() {
		this->pobject = NULL;
		this->pmemfun = NULL;
	}

	_connection4(dest_type* pobject, void (dest_type::*pmemfun)(arg1_type,
	             arg2_type, arg3_type, arg4_type)) {
		m_pobject = pobject;
		m_pmemfun = pmemfun;
	}

	virtual _connection_base4<arg1_type, arg2_type, arg3_type, arg4_type, mt_policy>* clone() {
		return new _connection4<dest_type, arg1_type, arg2_type, arg3_type, arg4_type, mt_policy>(*this);
	}

	virtual _connection_base4<arg1_type, arg2_type, arg3_type, arg4_type, mt_policy>* duplicate(HasSlots<mt_policy>* pnewdest) {
		return new _connection4<dest_type, arg1_type, arg2_type, arg3_type, arg4_type, mt_policy>((dest_type*)pnewdest, m_pmemfun);
	}

	virtual void emit(arg1_type a1, arg2_type a2, arg3_type a3,
	                  arg4_type a4) {
		(m_pobject->*m_pmemfun)(a1, a2, a3, a4);
	}

	virtual HasSlots<mt_policy>* getDest() const {
		return m_pobject;
	}

private:
	dest_type* m_pobject;
	void (dest_type::* m_pmemfun)(arg1_type, arg2_type, arg3_type,
	                              arg4_type);
};

template < class dest_type, class arg1_type, class arg2_type, class arg3_type,
         class arg4_type, class arg5_type, class mt_policy >
class _connection5 : public _connection_base5 < arg1_type, arg2_type,
		arg3_type, arg4_type, arg5_type, mt_policy > {
public:
	_connection5() {
		this->pobject = NULL;
		this->pmemfun = NULL;
	}

	_connection5(dest_type* pobject, void (dest_type::*pmemfun)(arg1_type,
	             arg2_type, arg3_type, arg4_type, arg5_type)) {
		m_pobject = pobject;
		m_pmemfun = pmemfun;
	}

	virtual _connection_base5 < arg1_type, arg2_type, arg3_type, arg4_type,
	arg5_type, mt_policy > * clone() {
		return new _connection5 < dest_type, arg1_type, arg2_type, arg3_type, arg4_type,
		       arg5_type, mt_policy > (*this);
	}

	virtual _connection_base5 < arg1_type, arg2_type, arg3_type, arg4_type,
	arg5_type, mt_policy > * duplicate(HasSlots<mt_policy>* pnewdest) {
		return new _connection5 < dest_type, arg1_type, arg2_type, arg3_type, arg4_type,
		       arg5_type, mt_policy > ((dest_type*)pnewdest, m_pmemfun);
	}

	virtual void emit(arg1_type a1, arg2_type a2, arg3_type a3, arg4_type a4,
	arg5_type a5) {
		(m_pobject->*m_pmemfun)(a1, a2, a3, a4, a5);
	}

	virtual HasSlots<mt_policy>* getDest() const {
		return m_pobject;
	}

private:
	dest_type* m_pobject;
	void (dest_type::* m_pmemfun)(arg1_type, arg2_type, arg3_type, arg4_type,
	                              arg5_type);
};

template < class dest_type, class arg1_type, class arg2_type, class arg3_type,
         class arg4_type, class arg5_type, class arg6_type, class mt_policy >
class _connection6 : public _connection_base6 < arg1_type, arg2_type,
		arg3_type, arg4_type, arg5_type, arg6_type, mt_policy > {
public:
	_connection6() {
		this->pobject = NULL;
		this->pmemfun = NULL;
	}

	_connection6(dest_type* pobject, void (dest_type::*pmemfun)(arg1_type,
	             arg2_type, arg3_type, arg4_type, arg5_type, arg6_type)) {
		m_pobject = pobject;
		m_pmemfun = pmemfun;
	}

	virtual _connection_base6 < arg1_type, arg2_type, arg3_type, arg4_type,
	arg5_type, arg6_type, mt_policy > * clone() {
		return new _connection6 < dest_type, arg1_type, arg2_type, arg3_type, arg4_type,
		       arg5_type, arg6_type, mt_policy > (*this);
	}

	virtual _connection_base6 < arg1_type, arg2_type, arg3_type, arg4_type,
	arg5_type, arg6_type, mt_policy > * duplicate(HasSlots<mt_policy>* pnewdest) {
		return new _connection6 < dest_type, arg1_type, arg2_type, arg3_type, arg4_type,
		       arg5_type, arg6_type, mt_policy > ((dest_type*)pnewdest, m_pmemfun);
	}

	virtual void emit(arg1_type a1, arg2_type a2, arg3_type a3, arg4_type a4,
	arg5_type a5, arg6_type a6) {
		(m_pobject->*m_pmemfun)(a1, a2, a3, a4, a5, a6);
	}

	virtual HasSlots<mt_policy>* getDest() const {
		return m_pobject;
	}

private:
	dest_type* m_pobject;
	void (dest_type::* m_pmemfun)(arg1_type, arg2_type, arg3_type, arg4_type,
	                              arg5_type, arg6_type);
};

template < class dest_type, class arg1_type, class arg2_type, class arg3_type,
         class arg4_type, class arg5_type, class arg6_type, class arg7_type, class mt_policy >
class _connection7 : public _connection_base7 < arg1_type, arg2_type,
		arg3_type, arg4_type, arg5_type, arg6_type, arg7_type, mt_policy > {
public:
	_connection7() {
		this->pobject = NULL;
		this->pmemfun = NULL;
	}

	_connection7(dest_type* pobject, void (dest_type::*pmemfun)(arg1_type,
	             arg2_type, arg3_type, arg4_type, arg5_type, arg6_type, arg7_type)) {
		m_pobject = pobject;
		m_pmemfun = pmemfun;
	}

	virtual _connection_base7 < arg1_type, arg2_type, arg3_type, arg4_type,
	arg5_type, arg6_type, arg7_type, mt_policy > * clone() {
		return new _connection7 < dest_type, arg1_type, arg2_type, arg3_type, arg4_type,
		       arg5_type, arg6_type, arg7_type, mt_policy > (*this);
	}

	virtual _connection_base7 < arg1_type, arg2_type, arg3_type, arg4_type,
	arg5_type, arg6_type, arg7_type, mt_policy > * duplicate(HasSlots<mt_policy>* pnewdest) {
		return new _connection7 < dest_type, arg1_type, arg2_type, arg3_type, arg4_type,
		       arg5_type, arg6_type, arg7_type, mt_policy > ((dest_type*)pnewdest, m_pmemfun);
	}

	virtual void emit(arg1_type a1, arg2_type a2, arg3_type a3, arg4_type a4,
	arg5_type a5, arg6_type a6, arg7_type a7) {
		(m_pobject->*m_pmemfun)(a1, a2, a3, a4, a5, a6, a7);
	}

	virtual HasSlots<mt_policy>* getDest() const {
		return m_pobject;
	}

private:
	dest_type* m_pobject;
	void (dest_type::* m_pmemfun)(arg1_type, arg2_type, arg3_type, arg4_type,
	                              arg5_type, arg6_type, arg7_type);
};

template < class dest_type, class arg1_type, class arg2_type, class arg3_type,
         class arg4_type, class arg5_type, class arg6_type, class arg7_type,
         class arg8_type, class mt_policy >
class _connection8 : public _connection_base8 < arg1_type, arg2_type,
		arg3_type, arg4_type, arg5_type, arg6_type, arg7_type, arg8_type, mt_policy > {
public:
	_connection8() {
		this->pobject = NULL;
		this->pmemfun = NULL;
	}

	_connection8(dest_type* pobject, void (dest_type::*pmemfun)(arg1_type,
	             arg2_type, arg3_type, arg4_type, arg5_type, arg6_type,
	             arg7_type, arg8_type)) {
		m_pobject = pobject;
		m_pmemfun = pmemfun;
	}

	virtual _connection_base8 < arg1_type, arg2_type, arg3_type, arg4_type,
	arg5_type, arg6_type, arg7_type, arg8_type, mt_policy > * clone() {
		return new _connection8 < dest_type, arg1_type, arg2_type, arg3_type, arg4_type,
		       arg5_type, arg6_type, arg7_type, arg8_type, mt_policy > (*this);
	}

	virtual _connection_base8 < arg1_type, arg2_type, arg3_type, arg4_type,
	arg5_type, arg6_type, arg7_type, arg8_type, mt_policy > * duplicate(HasSlots<mt_policy>* pnewdest) {
		return new _connection8 < dest_type, arg1_type, arg2_type, arg3_type, arg4_type,
		       arg5_type, arg6_type, arg7_type, arg8_type, mt_policy > ((dest_type*)pnewdest, m_pmemfun);
	}

	virtual void emit(arg1_type a1, arg2_type a2, arg3_type a3, arg4_type a4,
	arg5_type a5, arg6_type a6, arg7_type a7, arg8_type a8) {
		(m_pobject->*m_pmemfun)(a1, a2, a3, a4, a5, a6, a7, a8);
	}

	virtual HasSlots<mt_policy>* getDest() const {
		return m_pobject;
	}

private:
	dest_type* m_pobject;
	void (dest_type::* m_pmemfun)(arg1_type, arg2_type, arg3_type, arg4_type,
	                              arg5_type, arg6_type, arg7_type, arg8_type);
};
}

#endif // SIGLY_SIGLY_CONNECTION_N_H_