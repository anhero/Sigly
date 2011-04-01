#ifndef SIGLY_SIGLY_SIGNAL_BASE_H_
#define SIGLY_SIGLY_SIGNAL_BASE_H_

namespace sigly {
template<class mt_policy>
class HasSlots;

template<class mt_policy>
class _signal_base : public mt_policy {
public:
	virtual void slotDisconnect(HasSlots<mt_policy>* pslot) = 0;
	virtual void slotDuplicate(const HasSlots<mt_policy>* poldslot, HasSlots<mt_policy>* pnewslot) = 0;
};
}

#endif // SIGLY_SIGLY_SIGNAL_BASE_H_