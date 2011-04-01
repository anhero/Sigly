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

#ifndef SIGLY_SIGLY_CONNECTION_BASE_N_H_
#define SIGLY_SIGLY_CONNECTION_BASE_N_H_

namespace sigly {
template<class mt_policy>
class HasSlots;

template<class mt_policy>
class _connection_base0 {
public:
	virtual HasSlots<mt_policy>* getDest() const = 0;
	virtual void emit() = 0;
	virtual _connection_base0* clone() = 0;
	virtual _connection_base0* duplicate(HasSlots<mt_policy>* pnewdest) = 0;
};

template<class arg1_type, class mt_policy>
class _connection_base1 {
public:
	virtual HasSlots<mt_policy>* getDest() const = 0;
	virtual void emit(arg1_type) = 0;
	virtual _connection_base1<arg1_type, mt_policy>* clone() = 0;
	virtual _connection_base1<arg1_type, mt_policy>* duplicate(HasSlots<mt_policy>* pnewdest) = 0;
};

template<class arg1_type, class arg2_type, class mt_policy>
class _connection_base2 {
public:
	virtual HasSlots<mt_policy>* getDest() const = 0;
	virtual void emit(arg1_type, arg2_type) = 0;
	virtual _connection_base2<arg1_type, arg2_type, mt_policy>* clone() = 0;
	virtual _connection_base2<arg1_type, arg2_type, mt_policy>* duplicate(HasSlots<mt_policy>* pnewdest) = 0;
};

template<class arg1_type, class arg2_type, class arg3_type, class mt_policy>
class _connection_base3 {
public:
	virtual HasSlots<mt_policy>* getDest() const = 0;
	virtual void emit(arg1_type, arg2_type, arg3_type) = 0;
	virtual _connection_base3<arg1_type, arg2_type, arg3_type, mt_policy>* clone() = 0;
	virtual _connection_base3<arg1_type, arg2_type, arg3_type, mt_policy>* duplicate(HasSlots<mt_policy>* pnewdest) = 0;
};

template<class arg1_type, class arg2_type, class arg3_type, class arg4_type, class mt_policy>
class _connection_base4 {
public:
	virtual HasSlots<mt_policy>* getDest() const = 0;
	virtual void emit(arg1_type, arg2_type, arg3_type, arg4_type) = 0;
	virtual _connection_base4<arg1_type, arg2_type, arg3_type, arg4_type, mt_policy>* clone() = 0;
	virtual _connection_base4<arg1_type, arg2_type, arg3_type, arg4_type, mt_policy>* duplicate(HasSlots<mt_policy>* pnewdest) = 0;
};

template < class arg1_type, class arg2_type, class arg3_type, class arg4_type,
         class arg5_type, class mt_policy >
class _connection_base5 {
public:
	virtual HasSlots<mt_policy>* getDest() const = 0;
	virtual void emit(arg1_type, arg2_type, arg3_type, arg4_type,
	                  arg5_type) = 0;
	virtual _connection_base5 < arg1_type, arg2_type, arg3_type, arg4_type,
	        arg5_type, mt_policy > * clone() = 0;
	virtual _connection_base5 < arg1_type, arg2_type, arg3_type, arg4_type,
	        arg5_type, mt_policy > * duplicate(HasSlots<mt_policy>* pnewdest) = 0;
};

template < class arg1_type, class arg2_type, class arg3_type, class arg4_type,
         class arg5_type, class arg6_type, class mt_policy >
class _connection_base6 {
public:
	virtual HasSlots<mt_policy>* getDest() const = 0;
	virtual void emit(arg1_type, arg2_type, arg3_type, arg4_type, arg5_type,
	                  arg6_type) = 0;
	virtual _connection_base6 < arg1_type, arg2_type, arg3_type, arg4_type,
	        arg5_type, arg6_type, mt_policy > * clone() = 0;
	virtual _connection_base6 < arg1_type, arg2_type, arg3_type, arg4_type,
	        arg5_type, arg6_type, mt_policy > * duplicate(HasSlots<mt_policy>* pnewdest) = 0;
};

template < class arg1_type, class arg2_type, class arg3_type, class arg4_type,
         class arg5_type, class arg6_type, class arg7_type, class mt_policy >
class _connection_base7 {
public:
	virtual HasSlots<mt_policy>* getDest() const = 0;
	virtual void emit(arg1_type, arg2_type, arg3_type, arg4_type, arg5_type,
	                  arg6_type, arg7_type) = 0;
	virtual _connection_base7 < arg1_type, arg2_type, arg3_type, arg4_type,
	        arg5_type, arg6_type, arg7_type, mt_policy > * clone() = 0;
	virtual _connection_base7 < arg1_type, arg2_type, arg3_type, arg4_type,
	        arg5_type, arg6_type, arg7_type, mt_policy > * duplicate(HasSlots<mt_policy>* pnewdest) = 0;
};

template < class arg1_type, class arg2_type, class arg3_type, class arg4_type,
         class arg5_type, class arg6_type, class arg7_type, class arg8_type, class mt_policy >
class _connection_base8 {
public:
	virtual HasSlots<mt_policy>* getDest() const = 0;
	virtual void emit(arg1_type, arg2_type, arg3_type, arg4_type, arg5_type,
	                  arg6_type, arg7_type, arg8_type) = 0;
	virtual _connection_base8 < arg1_type, arg2_type, arg3_type, arg4_type,
	        arg5_type, arg6_type, arg7_type, arg8_type, mt_policy > * clone() = 0;
	virtual _connection_base8 < arg1_type, arg2_type, arg3_type, arg4_type,
	        arg5_type, arg6_type, arg7_type, arg8_type, mt_policy > * duplicate(HasSlots<mt_policy>* pnewdest) = 0;
};
}

#endif // SIGLY_SIGLY_CONNECTION_BASE_N_H_