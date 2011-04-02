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
#ifndef SIGLY_SIGLY_HAS_SLOTS_H_
#define SIGLY_SIGLY_HAS_SLOTS_H_

#include <set>

#include <sigly/defines.h>
#include <sigly/single_threaded.h>
#include <sigly/multi_threaded_global.h>
#include <sigly/multi_threaded_local.h>
#include <sigly/lock_block.h>
#include <sigly/signal_base.h>

namespace sigly {
template<class  mt_policy = SIGLY_DEFAULT_MT_POLICY>
class HasSlots : public mt_policy {
private:
	typedef typename std::set<_signal_base<mt_policy> *> sender_set;
	typedef typename sender_set::const_iterator const_iterator;

public:
	HasSlots(): mt_policy() {
	}

	HasSlots(const HasSlots& hs): mt_policy(hs) {
		LockBlock<mt_policy> lock(this);
		const_iterator it = hs.m_senders.begin();
		const_iterator itEnd = hs.m_senders.end();

		while(it != itEnd) {
			(*it)->slotDuplicate(&hs, this);
			m_senders.insert(*it);
			++it;
		}
	}

	void signalConnect(_signal_base<mt_policy>* sender) {
		LockBlock<mt_policy> lock(this);
		m_senders.insert(sender);
	}

	void signalDisconnect(_signal_base<mt_policy>* sender) {
		LockBlock<mt_policy> lock(this);
		m_senders.erase(sender);
	}

	virtual ~HasSlots() {
		disconnectAll();
	}

	void disconnectAll() {
		LockBlock<mt_policy> lock(this);
		const_iterator it = m_senders.begin();
		const_iterator itEnd = m_senders.end();

		while(it != itEnd) {
			(*it)->slotDisconnect(this);
			++it;
		}

		m_senders.erase(m_senders.begin(), m_senders.end());
	}

private:
	sender_set m_senders;
};
}

#endif // SIGLY_SIGLY_HAS_SLOTS_H_